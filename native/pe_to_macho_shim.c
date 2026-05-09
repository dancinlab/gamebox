// native/pe_to_macho_shim.c — gamebox PE→Mach-O bootstrap shim
//
// Phase E1 (Track DL): scaffold + PE mmap. No i386 execution yet.
// Spec ref: docs/PE_TO_MACHO_SHIM_PLAN.ai.md §4
//
// own1: written from public Apple SDK + Microsoft PE-COFF spec. No
// Wine / CrossOver / Whisky / GPTK source. No ReactOS source.
//
// Honest emit per own2: __SHIM__ <PASS|PARTIAL|FAIL> pe_to_macho_shim
//   phase=<name> [reason=<text>] [k=v ...]
//
// E1 behavior:
//   1. Parse PE header (mirrors hexa pe_parser).
//   2. Map .text section into MAP_JIT memory (RX-eligible, but we
//      keep it RW for now; future phases set RX via
//      pthread_jit_write_protect_np for execution).
//   3. Map other sections into plain anonymous memory with section
//      perms (R, RW, R).
//   4. Emit __SHIM__ PARTIAL pe_to_macho_shim phase=mmap_done.
//   5. Exit 0 (no execution attempted).
//
// E2..E5 are not implemented here; this shim deliberately stops at
// mmap_done so that the orchestrator can record an honest E1
// boundary. validated_manjeom remains 0.

#include "pe_parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>

// MAP_JIT is Apple-specific; declare for portability if missing.
#ifndef MAP_JIT
#define MAP_JIT 0x800
#endif

static const char *SHIM_VERSION = "0.1.0-e1-harness-scaffold";
static const char *PHASE_NONE = "init";

#define SHIM_FAIL(phase, reason) do { \
    fprintf(stdout, "__SHIM__ FAIL pe_to_macho_shim phase=%s reason=%s\n", phase, reason); \
    return 1; \
} while (0)

#define SHIM_PARTIAL(phase, fmt, ...) do { \
    fprintf(stdout, "__SHIM__ PARTIAL pe_to_macho_shim phase=%s " fmt "\n", phase, ##__VA_ARGS__); \
} while (0)

#define SHIM_PASS(phase, fmt, ...) do { \
    fprintf(stdout, "__SHIM__ PASS pe_to_macho_shim phase=%s " fmt "\n", phase, ##__VA_ARGS__); \
} while (0)

// Section permission bits per PE-COFF spec (IMAGE_SCN_*).
#define IMG_SCN_MEM_EXECUTE 0x20000000U
#define IMG_SCN_MEM_READ    0x40000000U
#define IMG_SCN_MEM_WRITE   0x80000000U

// Try MAP_JIT mmap; fall back to plain anonymous if denied (e.g.,
// hardened-runtime entitlement issue at runtime).
static void *try_mmap_jit(size_t sz) {
    void *p = mmap(NULL, sz, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANON | MAP_JIT, -1, 0);
    if (p != MAP_FAILED) return p;
    return mmap(NULL, sz, PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANON, -1, 0);
}

static size_t round_up_page(size_t n) {
    long ps = sysconf(_SC_PAGESIZE);
    if (ps <= 0) ps = 16384; // arm64 macOS default
    size_t pps = (size_t)ps;
    return (n + pps - 1) & ~(pps - 1);
}

// Map all sections per layout. Returns 0 success, 1 fail.
static int map_pe_sections(const pe_image_t *img, const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        SHIM_FAIL("section_map", "open_failed");
    }
    int mapped = 0, jit_mapped = 0, plain_mapped = 0;
    for (uint16_t i = 0; i < img->num_sections; i++) {
        const pe_section_t *s = &img->sections[i];
        size_t vsz = (size_t)s->virtual_size;
        if (vsz == 0) vsz = (size_t)s->size_of_raw_data;
        if (vsz == 0) continue;
        size_t rsz = round_up_page(vsz);

        int wants_exec = (s->characteristics & IMG_SCN_MEM_EXECUTE) ? 1 : 0;
        void *region = NULL;
        if (wants_exec) {
            region = try_mmap_jit(rsz);
            if (region == MAP_FAILED) {
                close(fd);
                SHIM_FAIL("section_map", "mmap_jit_failed");
            }
            jit_mapped++;
        } else {
            region = mmap(NULL, rsz, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANON, -1, 0);
            if (region == MAP_FAILED) {
                close(fd);
                SHIM_FAIL("section_map", "mmap_anon_failed");
            }
            plain_mapped++;
        }

        // Copy raw bytes from file.
        size_t copy_sz = (size_t)s->size_of_raw_data;
        if (copy_sz > vsz) copy_sz = vsz;
        if (copy_sz > 0 && s->pointer_to_raw_data > 0) {
            ssize_t got = pread(fd, region, copy_sz, (off_t)s->pointer_to_raw_data);
            if (got < 0 || (size_t)got != copy_sz) {
                close(fd);
                SHIM_FAIL("section_map", "pread_short");
            }
        }
        // Apply final permissions (best-effort; JIT region stays RW for E1).
        if (!wants_exec) {
            int prot = 0;
            if (s->characteristics & IMG_SCN_MEM_READ)  prot |= PROT_READ;
            if (s->characteristics & IMG_SCN_MEM_WRITE) prot |= PROT_WRITE;
            if (prot == 0) prot = PROT_READ;
            if (mprotect(region, rsz, prot) < 0) {
                close(fd);
                SHIM_FAIL("section_map", "mprotect_failed");
            }
        }
        mapped++;
        SHIM_PARTIAL("section_map",
            "idx=%u name=%s vaddr=0x%X vsize=%u perms=%c%c%c jit=%d",
            i, s->name,
            s->virtual_address, s->virtual_size,
            (s->characteristics & IMG_SCN_MEM_READ) ? 'R' : '-',
            (s->characteristics & IMG_SCN_MEM_WRITE) ? 'W' : '-',
            (s->characteristics & IMG_SCN_MEM_EXECUTE) ? 'X' : '-',
            wants_exec);
    }
    close(fd);
    SHIM_PARTIAL("mmap_done",
        "sections_mapped=%d jit_mapped=%d plain_mapped=%d",
        mapped, jit_mapped, plain_mapped);
    return 0;
}

int main(int argc, char **argv) {
    fprintf(stdout, "[shim] gamebox pe_to_macho_shim version=%s\n", SHIM_VERSION);
    fprintf(stdout, "[shim] phase=E1 (harness scaffold + PE mmap; no i386 execution)\n");
    fprintf(stdout, "[shim] own1: Wine 0 / hexa+C-from-spec only\n");

    if (argc < 2) {
        SHIM_FAIL(PHASE_NONE, "usage:_pe_to_macho_shim_<pe_path>");
    }
    const char *pe_path = argv[1];

    pe_image_t img;
    if (pe_parse_file(pe_path, &img) != 0 || !img.ok) {
        SHIM_FAIL("parse", "pe_parse_failed");
    }
    pe_print_summary(&img, pe_path);
    SHIM_PARTIAL("parse",
        "machine=0x%X opt_magic=0x%X sections=%u entry_va=0x%llX",
        img.machine, img.opt_magic, img.num_sections,
        (unsigned long long)(img.image_base + img.entry_point_rva));

    if (map_pe_sections(&img, pe_path) != 0) {
        return 1;
    }

    // Honest emit: we got here, but never executed any i386 code.
    SHIM_PARTIAL("e1_done",
        "validated_manjeom=0 next_phase=e2_i386_decoder gate=docs/PE_TO_MACHO_SHIM_PLAN.ai.md");
    return 0;
}
