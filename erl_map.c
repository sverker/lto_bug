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
 *
 */

#include "utils.h"

typedef struct {
    unsigned hx;
    unsigned skip;
} hxnode_t;

static int hxnodecmp(hxnode_t *a, hxnode_t *b)
{
    if (a->hx > b->hx)
	return 1;
    else if (a->hx == b->hx)
	return 0;
    else
	return -1;
}

int main(int argc, char **argv)
{
    hxnode_t hxns[] = { {1}, {2}, {0}, };
    unsigned n = sizeof(hxns) / sizeof(*hxns);

    erts_qsort((char*)hxns, n, sizeof(hxnode_t), (erts_void_ptr_cmp_t) hxnodecmp);

    /* Read and return from sort result to make sure the entire erts_qsort()
     * is not optimized away.
     */
    return hxns[0].hx; /* Should be 0 if OK */
}



