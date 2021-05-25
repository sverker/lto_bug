/*
 * %CopyrightBegin%
 *
 * Copyright Ericsson AB 1998-2018. All Rights Reserved.
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

#define ASSERT(x, ERROR) ((x) ? (void)1 : (fprintf(stderr, ERROR "\n"), abort()))
 

void erts_qsort_check_part(byte *base,
                           size_t nr_of_items,
                           size_t item_size,
                           erts_void_ptr_cmp_t compare,
                           byte* pivot_part_start);

void erts_qsort_check_part(byte *base,
                           size_t nr_of_items,
                           size_t item_size,
                           erts_void_ptr_cmp_t compare,
                           byte* pivot)
{
    byte *p = base;
    byte *end = base + (nr_of_items * item_size);

    while (p < pivot) {
        ASSERT(compare(p, pivot) < 0, "NOT LESS THAN PIVOT");
        p += item_size;
    }

    p += item_size;

    while (p < end) {
        ASSERT(compare(p, pivot) > 0, "NOT HIGHER THAN PIVOT");
        p += item_size;
    }
}


