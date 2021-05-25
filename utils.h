#include <stddef.h>

typedef unsigned char byte;
typedef unsigned Uint32;
typedef unsigned long Uint;
typedef unsigned long Eterm;
typedef unsigned long UWord;

typedef int (*erts_void_ptr_cmp_t)(const byte *, const byte *);
void erts_qsort(void *base,
                size_t nr_of_items,
                size_t item_size,
                erts_void_ptr_cmp_t compare);

