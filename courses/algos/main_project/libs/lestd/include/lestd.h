#ifndef LESTD_H_
#define LESTD_H_

#include <stdint.h>

typedef int32_t int_t;

#define STD_FMEM_SIZE (16 * 1024 * 1024)

// Write one byte to the standard output
void std_putc(int_t byte_val);

// Read one byte from the standard input
int_t std_getc();

// Exit the program with return code ret_code
void std_exit(int_t ret_code);

// Read the flat memory 32b entry at index pos
int_t std_fmemget(int_t pos);

// Write the flat memory 32b entry at index pos
void std_fmemset(int_t pos, int_t val);

// Copy n entries starting at index src, to the n entries starting at index dst
// src and dst can overlap
void std_fmemcpy(int_t dst, int_t src, int_t n);

#endif //! LESTD_H_
