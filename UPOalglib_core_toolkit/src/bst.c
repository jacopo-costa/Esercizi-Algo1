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

#include "bst_private.h"
#include <stdio.h>
#include <stdlib.h>

/**** EXERCISE #1 - BEGIN of FUNDAMENTAL OPERATIONS ****/

upo_bst_t upo_bst_create(upo_bst_comparator_t key_cmp)
{
    upo_bst_t tree = malloc(sizeof(struct upo_bst_s));
    if (tree == NULL)
    {
        perror("Unable to create a binary search tree");
        abort();
    }

    tree->root = NULL;
    tree->key_cmp = key_cmp;

    return tree;
}

void upo_bst_destroy(upo_bst_t tree, int destroy_data)
{
    if (tree != NULL)
    {
        upo_bst_clear(tree, destroy_data);
        free(tree);
    }
}

void upo_bst_clear_impl(upo_bst_node_t *node, int destroy_data)
{
    if (node != NULL)
    {
        upo_bst_clear_impl(node->left, destroy_data);
        upo_bst_clear_impl(node->right, destroy_data);

        if (destroy_data)
        {
            free(node->key);
            free(node->value);
        }

        free(node);
    }
}

void upo_bst_clear(upo_bst_t tree, int destroy_data)
{
    if (tree != NULL)
    {
        upo_bst_clear_impl(tree->root, destroy_data);
        tree->root = NULL;
    }
}

void *upo_bst_put(upo_bst_t tree, void *key, void *value)
{
    void *valueOld = NULL;

    tree->root = upo_bst_put_impl(tree->root, tree->key_cmp, key, value, valueOld);
    return valueOld;
}

upo_bst_node_t *upo_bst_put_impl(upo_bst_node_t *node, upo_bst_comparator_t key_cmp, void *key, void *value, void *valueOld)
{
    if (node == NULL)
    {
        node = malloc(sizeof(upo_bst_node_t));
        node->key = key;
        node->value = value;
        node->right = NULL;
        node->left = NULL;
        return node;
    }

    int cmp = key_cmp(key, node->key);

    if (cmp < 0)
    {
        node->left = upo_bst_put_impl(node->left, key_cmp, key, value, valueOld);
    }
    else if (cmp > 0)
    {
        node->right = upo_bst_put_impl(node->right, key_cmp, key, value, valueOld);
    }
    else
    {
        valueOld = node->value;
        node->value = value;
    }

    return node;
}

void upo_bst_insert(upo_bst_t tree, void *key, void *value)
{
    tree->root = upo_bst_insert_impl(tree->root, tree->key_cmp, key, value);
}

upo_bst_node_t *upo_bst_insert_impl(upo_bst_node_t *node, upo_bst_comparator_t key_cmp, void *key, void *value)
{
    if (node == NULL)
    {
        node = malloc(sizeof(upo_bst_node_t));
        node->key = key;
        node->value = value;
        node->right = NULL;
        node->left = NULL;
        return node;
    }
    int cmp = key_cmp(key, node->key);

    if (cmp < 0)
    {
        node->left = upo_bst_insert_impl(node->left, key_cmp, key, value);
    }
    else if (cmp > 0)
    {
        node->right = upo_bst_insert_impl(node->right, key_cmp, key, value);
    }
    else
    {
    }

    return node;
}

void *upo_bst_get(const upo_bst_t tree, const void *key)
{
    upo_bst_node_t *node = upo_bst_get_impl(tree->root, tree->key_cmp, key);
    if (node != NULL)
    {
        return node->value;
    }
    else
    {
        return NULL;
    }
}

upo_bst_node_t *upo_bst_get_impl(upo_bst_node_t *node, upo_bst_comparator_t key_cmp, const void *key)
{
    if (node == NULL)
    {
        return NULL;
    }

    int cmp = key_cmp(key, node->key);
    if (cmp < 0)
    {
        return upo_bst_get_impl(node->left, key_cmp, key);
    }
    else if (cmp > 0)
    {
        return upo_bst_get_impl(node->right, key_cmp, key);
    }
    else
    {
        return node;
    }
}

int upo_bst_contains(const upo_bst_t tree, const void *key)
{
    if (upo_bst_get_impl(tree->root, tree->key_cmp, key) != NULL)
    {
        return 1;
    }

    return 0;
}

void upo_bst_delete(upo_bst_t tree, const void *key, int destroy_data)
{
    if (tree != NULL)
        tree->root = upo_bst_delete_impl(tree->root, tree->key_cmp, key, destroy_data);
}

upo_bst_node_t *upo_bst_delete_impl(upo_bst_node_t *node, upo_bst_comparator_t key_cmp, const void *key, int destroy_data)
{
    if (node == NULL)
    {
        return NULL;
    }

    int cmp = key_cmp(key, node->key);

    if (cmp < 0)
    {
        node->left = upo_bst_delete_impl(node->left, key_cmp, key, destroy_data);
    }
    else if (cmp > 0)
    {
        node->right = upo_bst_delete_impl(node->right, key_cmp, key, destroy_data);
    }
    else
    {
        if (node->left != NULL && node->right != NULL)
        {
            upo_bst_node_t *max = upo_bst_find_max(node->left);
            if (destroy_data)
            {
                free(node->key);
                free(node->value);
            }
            node->key = max->key;
            node->value = max->value;
            node->left = upo_bst_delete_impl(node->left, key_cmp, node->key, 0);
        }
        else
        {
            upo_bst_node_t *temp = node;
            if (node->left == NULL)
            {
                node = node->right;
            }
            else
            {
                node = node->left;
            }
            if (destroy_data)
            {
                free(temp->key);
                free(temp->value);
            }
            free(temp);
        }
    }

    return node;
}

upo_bst_node_t *upo_bst_find_max(upo_bst_node_t *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    else
    {
        if (node->right != NULL)
        {
            return upo_bst_find_max(node->right);
        }
        else
        {
            return node;
        }
    }
}

upo_bst_node_t *upo_bst_find_min(upo_bst_node_t *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    else
    {
        if (node->left != NULL)
        {
            return upo_bst_find_min(node->left);
        }
        else
        {
            return node;
        }
    }
}

size_t upo_bst_size(const upo_bst_t tree)
{
    if (!upo_bst_is_empty(tree))
    {
        return upo_bst_size_impl(tree->root);
    }
    return 0;
}

size_t upo_bst_size_impl(upo_bst_node_t *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return 1 + upo_bst_size_impl(node->left) + upo_bst_size_impl(node->right);
}

size_t upo_bst_height(const upo_bst_t tree)
{
    if (!upo_bst_is_empty(tree))
    {
        return upo_bst_height_impl(tree->root);
    }
    return 0;
}

size_t upo_bst_height_impl(upo_bst_node_t *node)
{
    if (node == NULL || upo_bst_is_leaf_impl(node))
    {
        return 0;
    }
    return 1 + upo_max_impl(upo_bst_height_impl(node->left), upo_bst_height_impl(node->right));
}

size_t upo_max_impl(size_t a, size_t b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

int upo_bst_is_leaf_impl(upo_bst_node_t *node)
{
    if (node->left == NULL && node->right == NULL)
    {
        return 1;
    }
    return 0;
}

void upo_bst_traverse_in_order(const upo_bst_t tree, upo_bst_visitor_t visit, void *visit_context)
{
    if (!upo_bst_is_empty(tree))
    {
        upo_bst_traverse_in_order_impl(tree->root, visit, visit_context);
    }
    return;
}

void upo_bst_traverse_in_order_impl(upo_bst_node_t *node, upo_bst_visitor_t visit, void *visit_context)
{
    if (node != NULL)
    {
        upo_bst_traverse_in_order_impl(node->left, visit, visit_context);
        visit(node->key, node->value, visit_context);
        upo_bst_traverse_in_order_impl(node->right, visit, visit_context);
    }
}

int upo_bst_is_empty(const upo_bst_t tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return 1;
    }
    return 0;
}

/**** EXERCISE #1 - END of FUNDAMENTAL OPERATIONS ****/

/**** EXERCISE #2 - BEGIN of EXTRA OPERATIONS ****/

void *upo_bst_min(const upo_bst_t tree)
{
    void *minKey = NULL;
    if (!upo_bst_is_empty(tree))
    {
        minKey = upo_bst_min_impl(tree->root);
    }
    return minKey;
}

void *upo_bst_min_impl(upo_bst_node_t *node)
{
    if (node->left != NULL)
    {
        return upo_bst_min_impl(node->left);
    }
    else
    {
        return node->key;
    }
}

void *upo_bst_max(const upo_bst_t tree)
{
    void *maxKey = NULL;
    if (!upo_bst_is_empty(tree))
    {
        maxKey = upo_bst_max_impl(tree->root);
    }
    return maxKey;
}

void *upo_bst_max_impl(upo_bst_node_t *node)
{
    if (node->right != NULL)
    {
        return upo_bst_max_impl(node->right);
    }
    else
    {
        return node->key;
    }
}

void upo_bst_delete_min(upo_bst_t tree, int destroy_data)
{
    if (!upo_bst_is_empty(tree))
    {
        tree->root = upo_bst_delete_min_impl(tree->root, destroy_data);
    }
}

upo_bst_node_t *upo_bst_delete_min_impl(upo_bst_node_t *node, int destroy_data)
{
    if (node->left != NULL)
    {
        node->left = upo_bst_delete_min_impl(node->left, destroy_data);
    }
    else
    {
        upo_bst_node_t *temp = node;
        node = node->right;
        if (destroy_data)
        {
            free(temp->value);
            free(temp->key);
        }
        free(temp);
    }

    return node;
}

void upo_bst_delete_max(upo_bst_t tree, int destroy_data)
{
    if (!upo_bst_is_empty(tree))
    {
        tree->root = upo_bst_delete_max_impl(tree->root, destroy_data);
    }
}

upo_bst_node_t *upo_bst_delete_max_impl(upo_bst_node_t *node, int destroy_data)
{
    if (node->right != NULL)
    {
        node->right = upo_bst_delete_max_impl(node->right, destroy_data);
    }
    else
    {
        upo_bst_node_t *temp = node;
        node = node->left;
        if (destroy_data)
        {
            free(temp->value);
            free(temp->key);
        }
        free(temp);
    }

    return node;
}

void *upo_bst_floor(const upo_bst_t tree, const void *key)
{
    if (!upo_bst_is_empty(tree))
    {
        const upo_bst_node_t *floor = NULL;
        floor = upo_bst_floor_impl(tree->root, tree->key_cmp, key);
        if (floor != NULL)
        {
            return floor->key;
        }
    }
    return NULL;
}

const upo_bst_node_t *upo_bst_floor_impl(upo_bst_node_t *node, upo_bst_comparator_t key_cmp, const void *key)
{
    if (node != NULL)
    {
        int cmp = key_cmp(key, node->key);
        if (cmp < 0)
        {
            /* The key at current node is greater than the given key.
             * Search in the left subtree if there is a smaller key that is
             * also less than the given key. */
            return upo_bst_floor_impl(node->left, key_cmp, key);
        }
        else if (cmp > 0)
        {
            /* The key at current node is lower than the given key.
             * Search in the right subtree if there is a larger key that is
             * still smaller than the given key. */
            const upo_bst_node_t *floor_node = NULL;
            floor_node = upo_bst_floor_impl(node->left, key_cmp, key);
            return (floor_node != NULL) ? floor_node : node;
        }
        else
        {
            return node;
        }
    }
    return NULL;
}

void *upo_bst_ceiling(const upo_bst_t tree, const void *key)
{
    if (!upo_bst_is_empty(tree))
    {
        const upo_bst_node_t *ceiling = NULL;
        ceiling = upo_bst_ceiling_impl(tree->root, tree->key_cmp, key);
        if (ceiling != NULL)
        {
            return ceiling->key;
        }
    }
    return NULL;
}

const upo_bst_node_t *upo_bst_ceiling_impl(upo_bst_node_t *node, upo_bst_comparator_t key_cmp, const void *key)
{
    if (node != NULL)
    {
        int cmp = key_cmp(key, node->key);
        if (cmp < 0)
        {
            const upo_bst_node_t *ceiling_node = NULL;
            ceiling_node = upo_bst_ceiling_impl(node->left, key_cmp, key);
            return (ceiling_node != NULL) ? ceiling_node : node;
        }
        else if (cmp > 0)
        {
            /* The key at current node is lower than the given key.
             * Search in the right subtree if there is a larger key that is
             * still smaller than the given key. */
            
            return upo_bst_ceiling_impl(node->right, key_cmp, key);
        }
        else
        {
            return node;
        }
    }
    return NULL;
}

upo_bst_key_list_t upo_bst_keys_range(const upo_bst_t tree, const void *low_key, const void *high_key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

upo_bst_key_list_t upo_bst_keys(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

int upo_bst_is_bst(const upo_bst_t tree, const void *min_key, const void *max_key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

/**** EXERCISE #2 - END of EXTRA OPERATIONS ****/

upo_bst_comparator_t upo_bst_get_comparator(const upo_bst_t tree)
{
    if (tree == NULL)
    {
        return NULL;
    }

    return tree->key_cmp;
}
