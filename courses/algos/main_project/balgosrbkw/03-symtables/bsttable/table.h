#ifndef TABLE_H_
#define TABLE_H_

#include "lestd.h"

// Symbols table
// Associate every unique key identifier with a value.
// Can insert / update / remove entries
// Can query present: present ? what's the value
// Can iterate through all the key/value pairs

// Allocate memory for a new empty symbols table
int_t table_new();

// Clear all the memory allocated for the symbols table
void table_free(int_t st);

// Add an entry to the symbols table
// If there was already an entry, value is updated
// returns 1 if it was an insertion, 0 if it was an update
int_t table_put(int_t st, int_t key, int_t val);

// Remove the entry associated with the key
// Returns 1 if the key was found and deleted, 0 otherwhise
int_t table_delete(int_t st, int_t key);

// Returns the value associated with a key
// Panic if the key is not found
int_t table_get(int_t st, int_t key);

// Returns 1 if key is found, 0 otherwhise
int_t table_contains(int_t st, int_t key);

// Returns the number of elements in the symbols table
int_t table_size(int_t st);

// Allocate memory for a table iterator, pointing to begining of symbols table
int_t table_it_new(int_t st);

// Free memory of table iterator
void table_it_free(int_t it);

// Returns 1 is the iterator is at the end, 0 otherwhise
int_t table_it_is_end(int_t it);

// Get the actual key the iterator points to
// Panic if the iterator is at the end
int_t table_it_get_key(int_t it);

// Get the actual value the iterator points to
// Panic if the iterator is at the end
int_t table_it_get_val(int_t it);

// Move the iterator to the next element
// If it is end, does nothing
void table_it_next(int_t it);

#endif //! TABLE_H_
