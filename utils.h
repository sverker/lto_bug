
typedef int (*erts_void_ptr_cmp_t)(const char *, const char *);
void erts_qsort(char *base,
                unsigned nr_of_items,
                unsigned item_size,
                erts_void_ptr_cmp_t compare);

