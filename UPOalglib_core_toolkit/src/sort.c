/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/*
 * Copyright 2015 University of Piemonte Orientale, Computer Science Institute
 *
 * This file is part of UPOalglib.
 *
 * UPOalglib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UPOalglib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UPOalglib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include "sort_private.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void upo_insertion_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    unsigned char *ptr = base;
    size_t i, j;

    for (i = 1; i <= n - 1; i++)
    {
        j = i;
        while (j > 0 && (cmp(ptr + j * size, ptr + (j - 1) * size)) < 0)
        {
            upo_myswap(ptr + (j - 1) * size, ptr + j * size, size);
            j--;
        }
    }
}

void upo_merge_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    upo_merge_sort_rec(base, 0, n - 1, size, cmp);
}

static void upo_merge_sort_rec(void *base, size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    if (lo >= hi)
    {
        return;
    }

    size_t mid = (lo + hi) / 2;
    upo_merge_sort_rec(base, lo, mid, size, cmp);
    upo_merge_sort_rec(base, mid + 1, hi, size, cmp);
    upo_merge_sort_merge(base, lo, mid, hi, size, cmp);
}

static void upo_merge_sort_merge(void *base, size_t lo, size_t mid, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    size_t i = 0;
    size_t k = 0;
    size_t j = (mid + 1) - lo;
    size_t n = (hi - lo) + 1; // grandezza array

    unsigned char *ptr = base;
    unsigned char *aux = malloc(n * size);

    if (aux == NULL)
    {
        abort();
    }

    memcpy(aux, ptr + lo * size, n * size);

    for (k = lo; k <= hi; k++)
    {
        if (i > (mid - lo))
        {
            memcpy(ptr + k * size, aux + j * size, size);
            j++;
        }
        else if (j > (hi - lo))
        {
            memcpy(ptr + k * size, aux + i * size, size);
            i++;
        }
        else if ((cmp(aux + j * size, aux + i * size)) < 0)
        {
            memcpy(ptr + k * size, aux + j * size, size);
            j++;
        }
        else
        {
            memcpy(ptr + k * size, aux + i * size, size);
            i++;
        }
    }

    free(aux);
}

void upo_quick_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    upo_quick_sort_rec(base, 0, n - 1, size, cmp);
}

static void upo_quick_sort_rec(void *base, size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    if (lo >= hi)
    {
        return;
    }

    size_t j = upo_quick_sort_partition(base, lo, hi, size, cmp);

    if (j > 0)
    {
        upo_quick_sort_rec(base, lo, j - 1, size, cmp);
    }
    upo_quick_sort_rec(base, j + 1, hi, size, cmp);
}

static size_t upo_quick_sort_partition(void *base, size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    size_t i = lo;
    size_t j = hi + 1;

    unsigned char *ptr = base;
    unsigned char *pivot = ptr + lo * size;

    while (1)
    {
        do
        {
            i++;
        } while (i >= hi || (cmp(ptr + i * size, pivot)) >= 0);

        do
        {
            j--;
        } while (j <= lo || (cmp(ptr + j * size, pivot)) <= 0);

        if (i >= j)
        {
            break;
        }
        upo_myswap(ptr + i * size, ptr + j * size, size);
    }
    upo_myswap(pivot, ptr + j * size, size);
    return j;
}

static void upo_myswap(void *a, void *b, size_t size)
{

    assert(a);
    assert(b);

    if (a == b)
    {
        return;
    }

    void *tmp = malloc(size);
    if (tmp == NULL)
    {
        abort();
    }

    memmove(tmp, a, size);
    memmove(a, b, size);
    memmove(b, tmp, size);

    free(tmp);
}