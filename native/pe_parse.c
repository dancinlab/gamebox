// native/pe_parse.c — PE header reader.
//
// own1: written from Microsoft PE-COFF spec. No Wine source.
// Spec: https://learn.microsoft.com/en-us/windows/win32/debug/pe-format

#include "pe_parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

static uint16_t rd_u16_le(const uint8_t *p) {
    return (uint16_t)(p[0]) | ((uint16_t)(p[1]) << 8);
}

static uint32_t rd_u32_le(const uint8_t *p) {
    return  (uint32_t)(p[0])
          | ((uint32_t)(p[1]) << 8)
          | ((uint32_t)(p[2]) << 16)
          | ((uint32_t)(p[3]) << 24);
}

static uint64_t rd_u64_le(const uint8_t *p) {
    uint64_t lo = rd_u32_le(p);
    uint64_t hi = rd_u32_le(p + 4);
    return lo | (hi << 32);
}

int pe_parse_file(const char *path, pe_image_t *out) {
    memset(out, 0, sizeof(*out));

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return -1;
    }
    struct stat st;
    if (fstat(fd, &st) < 0 || st.st_size < 0x80) {
        close(fd);
        return -1;
    }
    size_t sz = (size_t)st.st_size;
    void *map = mmap(NULL, sz, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (map == MAP_FAILED) {
        return -1;
    }
    const uint8_t *buf = (const uint8_t *)map;

    // DOS header — magic at 0x00 = "MZ" (0x5A4D).
    if (rd_u16_le(buf) != 0x5A4D) {
        munmap(map, sz);
        return -1;
    }
    uint32_t e_lfanew = rd_u32_le(buf + 0x3C);
    if (e_lfanew + 0x18 > sz) {
        munmap(map, sz);
        return -1;
    }
    out->e_lfanew = e_lfanew;

    // PE signature at e_lfanew = "PE\0\0" (0x00004550).
    if (rd_u32_le(buf + e_lfanew) != 0x00004550) {
        munmap(map, sz);
        return -1;
    }

    // COFF File Header (20 bytes after PE signature).
    const uint8_t *coff = buf + e_lfanew + 4;
    out->machine = rd_u16_le(coff + 0);
    out->num_sections = rd_u16_le(coff + 2);
    uint16_t opt_size = rd_u16_le(coff + 16);

    // Optional header (PE32 or PE32+).
    const uint8_t *opt = coff + 20;
    out->opt_magic = rd_u16_le(opt + 0);
    if (out->opt_magic == 0x10B) {
        // PE32 — ImageBase at offset 28 (32-bit), AddressOfEntryPoint at 16, SizeOfImage at 56.
        // DataDirectory[0] = Export (offset 96); DataDirectory[1] = Import (offset 104).
        out->entry_point_rva = rd_u32_le(opt + 16);
        out->image_base = (uint64_t)rd_u32_le(opt + 28);
        out->size_of_image = rd_u32_le(opt + 56);
        // Import Directory (DataDirectory entry 1, PE32 offset 104 / 0x68).
        if (opt_size >= 112 && (size_t)(opt + 112 - buf) <= sz) {
            out->import_dir_rva  = rd_u32_le(opt + 104);
            out->import_dir_size = rd_u32_le(opt + 108);
        }
    } else if (out->opt_magic == 0x20B) {
        // PE32+ — ImageBase at offset 24 (64-bit), AddressOfEntryPoint at 16, SizeOfImage at 56.
        // DataDirectory[0] = Export (offset 112); DataDirectory[1] = Import (offset 120).
        out->entry_point_rva = rd_u32_le(opt + 16);
        out->image_base = rd_u64_le(opt + 24);
        out->size_of_image = rd_u32_le(opt + 56);
        // Import Directory (DataDirectory entry 1, PE32+ offset 120 / 0x78).
        if (opt_size >= 128 && (size_t)(opt + 128 - buf) <= sz) {
            out->import_dir_rva  = rd_u32_le(opt + 120);
            out->import_dir_size = rd_u32_le(opt + 124);
        }
    } else {
        munmap(map, sz);
        return -1;
    }

    // Section table — immediately after optional header.
    const uint8_t *sec = opt + opt_size;
    if (out->num_sections > PE_MAX_SECTIONS) {
        munmap(map, sz);
        return -1;
    }
    for (uint16_t i = 0; i < out->num_sections; i++) {
        const uint8_t *s = sec + (size_t)i * 40;
        if ((size_t)(s + 40 - buf) > sz) {
            munmap(map, sz);
            return -1;
        }
        memcpy(out->sections[i].name, s, 8);
        out->sections[i].name[8] = 0;
        out->sections[i].virtual_size       = rd_u32_le(s + 8);
        out->sections[i].virtual_address    = rd_u32_le(s + 12);
        out->sections[i].size_of_raw_data   = rd_u32_le(s + 16);
        out->sections[i].pointer_to_raw_data= rd_u32_le(s + 20);
        out->sections[i].characteristics    = rd_u32_le(s + 36);
    }

    munmap(map, sz);
    out->ok = 1;
    return 0;
}

void pe_print_summary(const pe_image_t *img, const char *path) {
    fprintf(stdout, "[shim.parse] path=%s\n", path);
    fprintf(stdout, "[shim.parse] machine=0x%04X (332=i386, 8664=AMD64)\n", img->machine);
    fprintf(stdout, "[shim.parse] opt_magic=0x%X (10B=PE32, 20B=PE32+)\n", img->opt_magic);
    fprintf(stdout, "[shim.parse] sections=%u\n", img->num_sections);
    fprintf(stdout, "[shim.parse] entry_rva=0x%X\n", img->entry_point_rva);
    fprintf(stdout, "[shim.parse] image_base=0x%llX\n", (unsigned long long)img->image_base);
    fprintf(stdout, "[shim.parse] size_of_image=0x%X (%u bytes)\n",
            img->size_of_image, img->size_of_image);
    for (uint16_t i = 0; i < img->num_sections; i++) {
        fprintf(stdout, "[shim.parse] section[%u] %-9s vaddr=0x%X vsize=%u raw_off=0x%X raw_sz=%u char=0x%X\n",
                i,
                img->sections[i].name,
                img->sections[i].virtual_address,
                img->sections[i].virtual_size,
                img->sections[i].pointer_to_raw_data,
                img->sections[i].size_of_raw_data,
                img->sections[i].characteristics);
    }
}
