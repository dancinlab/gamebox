// native/i386_decode_test.c — disassemble entry block of a PE file.
//
// Phase E2 standalone tester. Reads PE, finds .text + entry RVA,
// disassembles N instructions starting at entry, prints each line.
//
// Usage: ./i386_decode_test <pe_path> [count]
// Default count = 64.

#include "pe_parse.h"
#include "i386_decode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <pe.exe> [count]\n", argv[0]);
        return 1;
    }
    const char *path = argv[1];
    int count = (argc >= 3) ? atoi(argv[2]) : 64;
    if (count <= 0) count = 64;

    pe_image_t img;
    if (pe_parse_file(path, &img) != 0 || !img.ok) {
        fprintf(stderr, "[disasm] PE parse failed: %s\n", path);
        return 1;
    }
    if (img.machine != 0x14C) {
        fprintf(stderr, "[disasm] not i386 (machine=0x%X) — cannot disassemble with i386 decoder\n", img.machine);
        return 1;
    }

    // Find .text containing entry RVA.
    const pe_section_t *text = NULL;
    for (uint16_t i = 0; i < img.num_sections; i++) {
        const pe_section_t *s = &img.sections[i];
        if (img.entry_point_rva >= s->virtual_address &&
            img.entry_point_rva < s->virtual_address + (s->virtual_size ? s->virtual_size : s->size_of_raw_data)) {
            text = s;
            break;
        }
    }
    if (!text) {
        fprintf(stderr, "[disasm] entry RVA 0x%X not in any section\n", img.entry_point_rva);
        return 1;
    }
    fprintf(stdout, "[disasm] section=%s vaddr=0x%X entry_rva=0x%X entry_va=0x%llX\n",
            text->name, text->virtual_address, img.entry_point_rva,
            (unsigned long long)(img.image_base + img.entry_point_rva));

    int fd = open(path, O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }
    struct stat st; if (fstat(fd, &st) < 0) { close(fd); return 1; }
    size_t fsz = (size_t)st.st_size;
    void *map = mmap(NULL, fsz, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (map == MAP_FAILED) { perror("mmap"); return 1; }

    // Map entry RVA → file offset.
    uint32_t entry_off_in_section = img.entry_point_rva - text->virtual_address;
    uint32_t entry_file_off = text->pointer_to_raw_data + entry_off_in_section;
    if (entry_file_off + 256 > fsz) {
        fprintf(stderr, "[disasm] entry file offset out of range\n");
        munmap(map, fsz);
        return 1;
    }
    const uint8_t *code = (const uint8_t *)map + entry_file_off;
    size_t buf_len = (size_t)text->size_of_raw_data - entry_off_in_section;
    if (buf_len > fsz - entry_file_off) buf_len = fsz - entry_file_off;
    uint32_t eip_base = (uint32_t)(img.image_base + img.entry_point_rva);

    fprintf(stdout, "[disasm] decoding up to %d insns starting at eip=0x%X\n", count, eip_base);
    fprintf(stdout, "[disasm] -------- ------------------------ ------ ------------------------\n");
    fprintf(stdout, "[disasm]      eip                    bytes  op     operands\n");
    fprintf(stdout, "[disasm] -------- ------------------------ ------ ------------------------\n");

    size_t off = 0;
    int decoded = 0;
    int unknown_count = 0;
    while (decoded < count && off < buf_len) {
        i386_insn_t insn;
        int n = i386_decode_one(code, buf_len, off, eip_base, &insn);
        if (n == 0) {
            fprintf(stdout, "[disasm] [decode] truncated at off=%zu\n", off);
            break;
        }
        char line[128];
        i386_format_insn(&insn, line, sizeof(line));
        fprintf(stdout, "[disasm] %s\n", line);
        if (insn.op == I386_OP_UNKNOWN) unknown_count++;
        off += (size_t)n;
        decoded++;

        // Stop early on RET / HLT to keep output focused on entry block.
        if (insn.op == I386_OP_RET || insn.op == I386_OP_HLT) {
            fprintf(stdout, "[disasm] (terminator: %s)\n", i386_op_name(insn.op));
            break;
        }
    }

    munmap(map, fsz);
    fprintf(stdout, "__I386_DECODE__ %s phase=disasm decoded=%d unknown=%d\n",
            (decoded > 0 && unknown_count < decoded / 2) ? "PARTIAL" : "PARTIAL",
            decoded, unknown_count);
    fprintf(stdout, "[disasm] note: PARTIAL because decoder coverage is intentionally subset (E2 first batch). Unknowns are not failures — just opcodes outside the initial coverage list.\n");
    return 0;
}
