/*
 * %CopyrightBegin%
 *
 * Copyright Ericsson AB 2021. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * %CopyrightEnd%
 */

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/* Failed assert do exit(1) */
#define ASSERT(x, ERROR) ((x) ? (void)1 : (printf("ASSERT: "ERROR "\n"), exit(1)))

static void erts_qsort_swap(unsigned item_size,
                            char* iptr,
                            char* jptr)
{
    unsigned long* iwp = (unsigned long*) iptr;
    unsigned long* jwp = (unsigned long*) jptr;
    unsigned cnt;
    for (cnt = item_size / sizeof(unsigned long); cnt; cnt--) {
        unsigned long tmp = *jwp;
        *jwp = *iwp;
        *iwp = tmp;
        jwp++;
        iwp++;
    }
}


static void check_partitions(char *base,
                             unsigned nr_of_items,
                             unsigned item_size,
                             erts_void_ptr_cmp_t compare,
                             char* pivot)
{
    char *p = base;
    char *end = base + (nr_of_items * item_size);

    while (p < pivot) {
        ASSERT(compare(p, pivot) <= 0, "HIGHER THAN PIVOT");
        p += item_size;
    }

    p += item_size;

    while (p < end) {
        ASSERT(compare(p, pivot) > 0, "NOT HIGHER THAN PIVOT");
        p += item_size;
    }
}

static char*
erts_qsort_partion_array(char *base,
                         unsigned nr_of_items,
                         unsigned item_size,
                         erts_void_ptr_cmp_t compare)
{
    char* second_part_start = base + (nr_of_items * item_size);
    char* curr = base + item_size;
    char* pivot;

    /* Use first element *base as pivot */

    while (curr != second_part_start) {
        int compare_res = compare(curr, base);
        if (compare_res <= 0) {
            /* Keep in first part */
            curr += item_size;
        } else {
            /* Move to second part */

            /* BUG: After this branch first is taken the loop will never call
             * BUG: compare() again. It seems it thinks *curr and *base are the
             * BUG: same as the pointers haven't changed.
             */
            second_part_start -= item_size;
            erts_qsort_swap(item_size, curr, second_part_start);
        }
    }
    /* Put pivot into place */
    pivot = second_part_start - item_size;
    erts_qsort_swap(item_size, base, pivot);

    /* Check if BUG has happened */
    check_partitions(base, nr_of_items, item_size, compare, pivot);
    return pivot;
}

void erts_qsort(char *base,
                unsigned nr_of_items,
                unsigned item_size,
                erts_void_ptr_cmp_t compare)
{
    char *pivot;
    unsigned items_in_1st_part;
    unsigned items_in_2nd_part;
    if (nr_of_items <= 1){
        return;
    }
    pivot = erts_qsort_partion_array(base,
                                     nr_of_items,
                                     item_size,
                                     compare);
    items_in_1st_part = (pivot - base) / item_size;
    items_in_2nd_part = nr_of_items - items_in_1st_part - 1;
    erts_qsort(base,
               items_in_1st_part,
               item_size,
               compare);
    erts_qsort(pivot + item_size,
               items_in_2nd_part,
               item_size,
               compare);
}


