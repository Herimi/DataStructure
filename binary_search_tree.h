//
// Created by Herimi on 2022/3/20.
//

#ifndef DATASTRUCTURE_BINARY_SEARCH_TREE_H
#define DATASTRUCTURE_BINARY_SEARCH_TREE_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "contiguous_list.h"

typedef struct BinarySearchTreeNode {
    struct BinarySearchTreeNode *parent;
    /**
     * 左子结点
     */
    struct BinarySearchTreeNode *left;
    /**
     * 右子节点
     */
    struct BinarySearchTreeNode *right;
    /**
     * 结点的内容
     */
    void *element;
} BinarySearchTreeNode;

typedef struct BinarySearchTree {
    BinarySearchTreeNode *root;
    size_t size;
} BinarySearchTree;

BinarySearchTree *binary_search_tree_init() {
    BinarySearchTree *tree = (BinarySearchTree *) calloc(1, sizeof(BinarySearchTree));
    return tree;
}

void binary_search_tree_foreach_child(BinarySearchTreeNode *node, void (*consumer)(void *)) {
    if (node == NULL) {
        return;
    }
    printf("进入左节点\n");
    binary_search_tree_foreach_child(node->left, consumer);
    printf("回溯\n");
    consumer(node->element);
    printf("进入右节点\n");
    binary_search_tree_foreach_child(node->right, consumer);
    printf("回溯\n");
}

void binary_search_tree_foreach(BinarySearchTree *tree, void (*consumer)(void *)) {
    BinarySearchTreeNode *node = tree->root;
    binary_search_tree_foreach_child(node, consumer);
}

void binary_search_tree_add(BinarySearchTree *tree, void *element, int (*compare)(void *, void *)) {
    BinarySearchTreeNode *parent = NULL;
    BinarySearchTreeNode *node = tree->root;
    int i = 0;
    while (node != NULL) {
        parent = node;
        i = compare(element, node->element);
        if (i < 0) {
            node = node->left;
        } else if (i > 0) {
            node = node->right;
        } else {
            return;
        }
    }
    node = (BinarySearchTreeNode *) calloc(1, sizeof(BinarySearchTreeNode));
    if (parent != NULL) {
        if (i < 0) {
            parent->left = node;
        } else if (i > 0) {
            parent->right = node;
        }
    } else {
        tree->root = node;
    }
    node->parent = parent;
    node->element = element;
    tree->size++;
}

BinarySearchTreeNode *binary_search_tree_find_min(BinarySearchTreeNode *node) {
    if (node == NULL) {
        return NULL;
    }
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

/**
 * 右边的移植到左边的上面
 * @param transplanted 被移植的
 * @param transplanting 需要移植过去的
 */
void binary_search_tree_transplant(BinarySearchTree *tree, BinarySearchTreeNode *transplanted,
                                   BinarySearchTreeNode *transplanting) {
    BinarySearchTreeNode *parent = transplanted->parent;
    if (parent == NULL) {
        tree->root = transplanting;
    } else {
        if (transplanted == parent->left) {
            parent->left = transplanting;
        } else {
            parent->right = transplanting;
        }
    }
    if (transplanting != NULL) {
        transplanting->parent = parent;
    }

}

/**
 * @param tree
 * @param node 需要删除的
 */
void binary_search_tree_remove_node(BinarySearchTree *tree, BinarySearchTreeNode *node) {
    BinarySearchTreeNode *left = node->left;
    BinarySearchTreeNode *right = node->right;
    // 没有子树或者只有一个子树的情况，直接用另外一颗子树移植过去即可
    if (left == NULL) {
        binary_search_tree_transplant(tree, node, right);
    } else if (right == NULL) {
        binary_search_tree_transplant(tree, node, left);
    } else {
        // 查找后继节点
        BinarySearchTreeNode *after = binary_search_tree_find_min(right);
        // 后继节点的右节点移植到后继节点的位置，此时的后继变成了一个游离状态的节点
        binary_search_tree_transplant(tree, after, after->right);
        // 将需要删除的节点的左右子树赋给后继节点来替代他
        after->left = node->left;
        node->left->parent = after;
        after->right = node->right;
        node->right->parent = after;
        // 然后将后继移植过去，即可完成后继和删除节点的位置互换
        binary_search_tree_transplant(tree, node, after);
    }
    free(node);
    tree->size--;
}

void binary_search_tree_remove(BinarySearchTree *tree, void *element, int (*compare)(void *, void *)) {
    BinarySearchTreeNode *node = tree->root;
    while (node != NULL) {
        int i = compare(element, node->element);
        if (i < 0) {
            node = node->left;
        } else if (i > 0) {
            node = node->right;
        } else {
            // 执行删除
            binary_search_tree_remove_node(tree, node);
            break;
        }
    }
}

ContiguousList *binary_search_tree_check_list = NULL;

void binary_search_tree_check_add_list(void *element) {
    contiguous_list_add(binary_search_tree_check_list, element);
}

void *binary_search_tree_check(BinarySearchTree *tree, int (*compare)(void *, void *)) {
    binary_search_tree_check_list = (ContiguousList *) contiguous_list_init();
    binary_search_tree_foreach(tree, binary_search_tree_check_add_list);

    for (int i = 1; i < binary_search_tree_check_list->size; ++i) {
        void *before = *(binary_search_tree_check_list->elements + (i - 1));
        void *current = *(binary_search_tree_check_list->elements + i);
        if (compare(before, current) >= 0) {
            printf("出现错误");
            return current;
        }
    }
    free(binary_search_tree_check_list);
    binary_search_tree_check_list = NULL;
    return NULL;
}


#endif //DATASTRUCTURE_BINARY_SEARCH_TREE_H
