/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/******************************************************************************/
/*** NOME:                                                                  ***/
/*** COGNOME:                                                               ***/
/*** MATRICOLA:                                                             ***/
/******************************************************************************/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <upo/bst.h>
#include <upo/sort.h>

/**** BEGIN of EXERCISE #1 ****/

const void *upo_bst_predecessor(const upo_bst_t bst, const void *key)
{
    const void *pred = NULL;
    if (!upo_bst_is_empty(bst))
    {
        pred = upo_bst_predecessor_impl(bst->root, bst->key_cmp, key);
    }
    return pred;
}

const void *upo_bst_predecessor_impl(upo_bst_node_t *node, upo_bst_comparator_t key_cmp, const void *key)
{
    if (node != NULL)
    {
        int cmp = key_cmp(key, node->key);
        if (cmp <= 0)
        {
            // Case: key <= node->key
            return upo_bst_predecessor_impl(node->left, key_cmp, key);
        }
        // Case: key > node->key
        const void *pred_key = upo_bst_predecessor_impl(node->right, key_cmp, key);
        if (pred_key != NULL)
        {
            return pred_key;
        }
        return node->key;
    }
    return NULL;
}

/**** END of EXERCISE #1 ****/

/**** BEGIN of EXERCISE #2 ****/

void upo_bidi_bubble_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    assert(base != NULL);

    unsigned char *pc = base;
    int swapped = 1;
    for (size_t k = 0; k < n && swapped; ++k)
    {
        swapped = 0;
        for (size_t i = 0; i < (n - 1); ++i)
        {
            if (cmp(pc + i * size, pc + (i + 1) * size) > 0)
            {
                // base[i] and base[i+1] are out of order -> swap
                upo_swap(pc + i * size, pc + (i + 1) * size, size);
                swapped = 1;
            }
        }
        if (swapped)
        {
            swapped = 0;
            for (size_t i = n - 2; i > 0; --i)
            {
                if (cmp(pc + i * size, pc + (i - 1) * size) < 0)
                {
                    // base[i] and base[i-1] are out of order -> swap
                    upo_swap(pc + i * size, pc + (i - 1) * size, size);
                    swapped = 1;
                }
            }
        }
    }
}

static void upo_swap(void *a, void *b, size_t size)
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

/**** END of EXERCISE #2 ****/
