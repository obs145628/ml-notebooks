#include "lealloc.h"

// Dynamic size block
// - metada
// - data (ptr)
//
// - metada:
// - 31bits len
// - 1 bit used
//
// List of dynamic blocs
// Metadata of first block start at 0, so pointer at 1
// 0 can be used as nullptr
// End of list: medata past last element has len of 0
//
// Alloc:
// Go through the list
// Take first free block big enough
// Allocate only the required size
// Combine multiple free blocks in one while looking for free space
//
// Free:
// SImply set metadata used to 0

static int_t find_block(int_t len) {
  int_t mtd_ptr = 0;
  int_t found = 0;
  while (found == 0) {
    int_t mtd = std_fmemget(mtd_ptr);
    int_t block_used = mtd % 2;
    int_t block_len = mtd / 2;
    if (block_len == 0) { // end of list
      found = 1;
    } else if (block_used == 0 && block_len >= len) {
      found = 1;
    } else {
      mtd_ptr = mtd_ptr + len + 1;
    }
  }

  return mtd_ptr;
}

static void push_back(int_t mtd_ptr, int_t len) {
  int_t next_mtd = mtd_ptr + len + 1;
  std_fmemset(mtd_ptr, 2 * len + 1);
  std_fmemset(next_mtd, 0);
}

static void cut_block(int_t mtd_ptr, int_t len) {
  int_t mtd = std_fmemget(mtd_ptr);
  int_t old_len = mtd / 2;

  int_t split_mtd_ptr = mtd_ptr + len + 1;
  int_t split_mtd = 2 * (len - old_len - 1);
  std_fmemset(split_mtd_ptr, split_mtd);

  mtd = 2 * len + 1;
  std_fmemset(mtd_ptr, mtd);
}

int_t fm_alloc(int_t len) {
  int_t mtd_ptr = find_block(len);
  int_t mtd = std_fmemget(mtd_ptr);
  int_t mtd_len = mtd / 2;
  if (mtd_len == 0) {
    push_back(mtd_ptr, len);
  } else if (len > mtd_len + 2) { // avoid too small fragmentation
    cut_block(mtd_ptr, len);
  } else {
    std_fmemset(mtd_ptr, mtd + 1); // keep same block size, but set used bit
  }

  return mtd_ptr + 1;
}

void fm_free(int_t addr) {
  if (addr) {
    int_t mdt = std_fmemget(addr - 1);
    mdt = mdt - 1; // used bit must be 1, we set it to 0
    std_fmemset(addr - 1, mdt);
  }
}
