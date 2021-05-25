/*
 * %CopyrightBegin%
 *
 * Copyright Ericsson AB 2014-2020. All Rights Reserved.
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
 *
 */

#include <stdio.h>

#include "utils.h"

typedef int ErtsHeapFactory;
typedef int ErtsAlcType_t;

typedef struct {
    Uint32 hx;
    Uint32 skip;
    Uint i;
    Eterm  val;
} hxnode_t;

static Eterm hashmap_from_unsorted_array(ErtsHeapFactory*, hxnode_t *hxns, Uint n, int reject_dupkeys, ErtsAlcType_t temp_memory_allocator);
static int hxnodecmp(hxnode_t* a, hxnode_t* b);

static Eterm hashmap_from_unsorted_array(ErtsHeapFactory* factory,
                                         hxnode_t *hxns, Uint n,
                                         int reject_dupkeys,
                                         ErtsAlcType_t temp_memory_allocator) {
    Uint ix = 0;

    fprintf(stderr, "hxnode_t hxns[] = {\n");
    for (ix=0; ix<n; ix++) {
        fprintf(stderr, "{.hx=%u, .skip=%u, .i=%lu, .val=%lu},\n",
                hxns[ix].hx, hxns[ix].skip, hxns[ix].i, hxns[ix].val);
    }
    fprintf(stderr, "}\n");

    erts_qsort(hxns, n, sizeof(hxnode_t), (erts_void_ptr_cmp_t) hxnodecmp);

    return hxns[0].val;
}

void lto_bug(int x);

void lto_bug(int x)
{
    hxnode_t hxns[] = {
{.hx=10, .skip=1, .i=0},
{.hx=5, .skip=1, .i=4},
{.hx=14, .skip=1, .i=5},
};
    size_t n = sizeof(hxns) / sizeof(*hxns);
    size_t i;

    hashmap_from_unsorted_array(NULL, hxns, n, 0, 0);

    erts_qsort(hxns, n, sizeof(hxnode_t), (erts_void_ptr_cmp_t) hxnodecmp);

    for (i=0; i < n; i++) {
        fprintf(stderr, "hxns[%ld].hx = %u\t val=%lu\n", i, hxns[i].hx, hxns[i].val);
    }
}

static int hxnodecmp(hxnode_t *a, hxnode_t *b) {
    if (a->hx < b->hx)
	return 1;
    else if (a->hx == b->hx)
	return 0;
    else
	return -1;
}


