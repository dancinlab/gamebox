// native/pe_parse.h — PE header read primitives (own1: written from
// Microsoft PE-COFF spec, no Wine source).
//
// Spec reference: Microsoft PE Format
//   https://learn.microsoft.com/en-us/windows/win32/debug/pe-format
//
// This is a *read-only* parser. We extract the PE header + section
// table + entry-point RVA. Mapping into our process is done by the
// caller (pe_to_macho_shim.c).

#ifndef GAMEBOX_PE_PARSE_H
#define GAMEBOX_PE_PARSE_H

#include <stdint.h>
#include <stddef.h>

#define PE_MAX_SECTIONS 32

// Per Microsoft PE-COFF spec.
typedef struct {
    uint32_t virtual_size;
    uint32_t virtual_address;
    uint32_t size_of_raw_data;
    uint32_t pointer_to_raw_data;
    uint32_t characteristics;
    char     name[9];   // 8-byte name + null-term
} pe_section_t;

typedef struct {
    int      ok;                 // 0 = parse failed
    uint16_t machine;            // 0x14C = i386, 0x8664 = AMD64
    uint16_t opt_magic;          // 267 = PE32, 523 = PE32+
    uint16_t num_sections;
    uint32_t e_lfanew;           // offset to PE header from file start
    uint64_t image_base;         // ImageBase (low 32 if PE32)
    uint32_t size_of_image;
    uint32_t entry_point_rva;    // AddressOfEntryPoint
    // ── Import Directory (Data Directory entry 1, r9 IAT name-based autobind) ──
    uint32_t import_dir_rva;     // RVA of Import Directory (0 if absent)
    uint32_t import_dir_size;    // size of Import Directory in bytes
    pe_section_t sections[PE_MAX_SECTIONS];
} pe_image_t;

// Read PE headers from `path`. Returns 0 on success, -1 on error.
// `out` is fully populated on success; `out->ok` is 1 on success.
// Failure modes set `out->ok = 0` and the function returns -1.
int pe_parse_file(const char *path, pe_image_t *out);

// Pretty-print PE summary to stdout (for debugging / E1 PARTIAL emit).
void pe_print_summary(const pe_image_t *img, const char *path);

#endif // GAMEBOX_PE_PARSE_H
