/*
 * %CopyrightBegin%
 *
 * Copyright Ericsson AB 1996-2020. All Rights Reserved.
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

typedef struct
{
    byte* pivot_part_start;
    byte* pivot_part_end;
} erts_qsort_partion_array_result;

static void erts_qsort_swap(size_t item_size, byte* ivp, byte* jvp);
void erts_qsort_check_part(byte *base,
                           size_t nr_of_items,
                           size_t item_size,
                           erts_void_ptr_cmp_t compare,
                           byte* pivot);


static void erts_qsort_helper(byte *base,
                              size_t nr_of_items,
                              size_t item_size,
                              erts_void_ptr_cmp_t compare);
static byte*
erts_qsort_partion_array(byte *base,
                         size_t nr_of_items,
                         size_t item_size,
                         erts_void_ptr_cmp_t compare);

static void erts_qsort_swap(size_t item_size,
                            byte* iptr,
                            byte* jptr)
{
    UWord* iwp = (UWord*) iptr;
    UWord* jwp = (UWord*) jptr;
    size_t cnt;
    for (cnt = item_size / sizeof(UWord); cnt; cnt--) {
        UWord tmp = *jwp;
        *jwp = *iwp;
        *iwp = tmp;
        jwp++;
        iwp++;
    }
}

static byte*
erts_qsort_partion_array(byte *base,
                         size_t nr_of_items,
                         size_t item_size,
                         erts_void_ptr_cmp_t compare)
{
    byte* second_part_start = base + (nr_of_items * item_size);
    byte* curr = base + item_size;
    byte* pivot;

    while (curr != second_part_start) {
        int compare_res = compare(curr, base);
        if (compare_res < 0) {
            /* Include in first part */
            curr += item_size;
        } else {
            /* Move to last part */
            second_part_start -= item_size;
            erts_qsort_swap(item_size, curr, second_part_start);
        }
    }
    pivot = second_part_start - item_size;
    erts_qsort_swap(item_size, base, pivot);
    erts_qsort_check_part(base,
                          nr_of_items,
                          item_size,
                          compare,
                          pivot);
    return pivot;
}

static void erts_qsort_helper(byte *base,
                              size_t nr_of_items,
                              size_t item_size,
                              erts_void_ptr_cmp_t compare)
{
    byte *pivot;
    size_t nr_of_items_in_first_partion;
    size_t nr_of_items_second_partion;
    if (nr_of_items <= 1){
        return;
    }
    pivot = erts_qsort_partion_array(base,
                                     nr_of_items,
                                     item_size,
                                     compare);
    nr_of_items_in_first_partion = (pivot - base) / item_size;
    nr_of_items_second_partion = nr_of_items - ((pivot - base) / item_size) - 1;
    erts_qsort_helper(base,
                      nr_of_items_in_first_partion,
                      item_size, compare);
    erts_qsort_helper(pivot + item_size,
                      nr_of_items_second_partion,
                      item_size,
                      compare);
}

/***************
 ** Quicksort **
 ***************
 *
 * A quicksort implementation with the same interface as the qsort
 * function in the C standard library.
 *
 */
void erts_qsort(void *base,
                size_t nr_of_items,
                size_t item_size,
                erts_void_ptr_cmp_t compare)
{
    erts_qsort_helper((byte*)base, nr_of_items, item_size, compare);
}

