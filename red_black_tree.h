//
// Created by Herimi on 2022/3/20.
//

#ifndef DATASTRUCTURE_RED_BLACK_TREE_H
#define DATASTRUCTURE_RED_BLACK_TREE_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "contiguous_list.h"

static bool RED = false;
static bool BLACK = true;

typedef struct RedBlackTreeNode {
    /**
     * 颜色
     * true表示黑色
     * false 表示红色
     */
    bool color;
    struct RedBlackTreeNode *parent;
    /**
     * 左子结点
     */
    struct RedBlackTreeNode *left;
    /**
     * 右子节点
     */
    struct RedBlackTreeNode *right;
    /**
     * 结点的内容
     */
    void *element;
} RedBlackTreeNode;

typedef struct RedBlackTree {
    RedBlackTreeNode *root;
    /**
     * 叶结点
     */
    RedBlackTreeNode *NIL;
    size_t size;
} RedBlackTree;


RedBlackTreeNode *red_black_tree_init_nil(RedBlackTreeNode *NIL) {
    if (NIL == NULL) {
        NIL = (RedBlackTreeNode *) calloc(1, sizeof(RedBlackTreeNode));
    }
    NIL->left = NIL;
    NIL->right = NIL;
    NIL->parent = NIL;
    NIL->color = BLACK;
    return NIL;
}

RedBlackTree *red_black_tree_init() {
    RedBlackTree *tree = (RedBlackTree *) calloc(1, sizeof(RedBlackTree));
    RedBlackTreeNode *NIL = red_black_tree_init_nil(NULL);
    tree->NIL = NIL;
    tree->root = NIL;
    return tree;
}

void red_black_tree_foreach_child(RedBlackTree *tree, RedBlackTreeNode *node, void (*consumer)(void *)) {
    if (node == tree->NIL) {
        return;
    }
    printf("进入左节点\n");
    red_black_tree_foreach_child(tree, node->left, consumer);
    printf("回溯\n");
    consumer(node->element);
    printf("颜色:%s", node->color ? "黑色" : "红色");
    printf("进入右节点\n");
    red_black_tree_foreach_child(tree, node->right, consumer);
    printf("回溯\n");
}

void red_black_tree_foreach(RedBlackTree *tree, void (*consumer)(void *)) {
    RedBlackTreeNode *node = tree->root;
    red_black_tree_foreach_child(tree, node, consumer);
}

/**
 * 左旋
 * @param tree
 * @param node
 */
void red_black_tree_locate_left(RedBlackTree *tree, RedBlackTreeNode *node) {
    RedBlackTreeNode *child = node->right;
    if (child == tree->NIL) {
        return;
    }
    RedBlackTreeNode *parent = node->parent;
    if (parent != tree->NIL) {
        if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    } else {
        tree->root = child;
    }
    child->parent = node->parent;

    node->right = child->left;
    if (child->left->parent != tree->NIL) {
        child->left->parent = node;
    }

    child->left = node;
    node->parent = child;
}

/**
 * 右旋
 * @param tree
 * @param node
 */
void red_black_tree_locate_right(RedBlackTree *tree, RedBlackTreeNode *node) {
    RedBlackTreeNode *child = node->left;
    if (child == tree->NIL) {
        return;
    }
    RedBlackTreeNode *parent = node->parent;
    if (parent != tree->NIL) {
        if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    } else {
        tree->root = child;
    }
    child->parent = node->parent;

    node->left = child->right;
    if (child->right->parent != tree->NIL) {
        child->right->parent = node;
    }

    child->right = node;
    node->parent = child;
}

/**
 * 添加的调整
 * @param tree
 * @param node
 */
void red_black_tree_add_fixup(RedBlackTree *tree, RedBlackTreeNode *node) {
    while (node->parent->color == RED) {
        if (tree->NIL->parent != tree->NIL) {
            printf("-------------------------\n");
        }
        RedBlackTreeNode *parent = node->parent;
        RedBlackTreeNode *grandparent = parent->parent;
        if (parent == grandparent->left) {
            RedBlackTreeNode *uncle = grandparent->right;
            if (uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
            } else {
                // LR型，需转成LL型
                if (parent == parent->right) {
                    red_black_tree_locate_left(tree, parent);
                    parent = node->parent;
                }
                parent->color = BLACK;
                grandparent->color = RED;

                red_black_tree_locate_right(tree, grandparent);
                grandparent = parent->parent;
            }
        } else {
            RedBlackTreeNode *uncle = grandparent->left;
            if (uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
            } else {
                // RL型，需转成RR型
                if (parent == parent->left) {
                    red_black_tree_locate_left(tree, parent);
                    parent = node->parent;
                }
                parent->color = BLACK;
                grandparent->color = RED;

                red_black_tree_locate_right(tree, grandparent);
                grandparent = parent->parent;
            }
        }
        node = grandparent;
    }
    tree->root->color = BLACK;
}

/**
 * 添加
 * @param tree
 * @param element
 * @param compare
 */
void red_black_tree_add(RedBlackTree *tree, void *element, int (*compare)(void *, void *)) {
    RedBlackTreeNode *parent = tree->NIL;
    RedBlackTreeNode *node = tree->root;
    int i = 0;
    while (node != tree->NIL) {
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
    node = (RedBlackTreeNode *) calloc(1, sizeof(RedBlackTreeNode));
    node->color = RED;
    node->left = tree->NIL;
    node->right = tree->NIL;
    if (parent != tree->NIL) {
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
    red_black_tree_add_fixup(tree, node);
}

RedBlackTreeNode *red_black_tree_find_min(RedBlackTree *tree, RedBlackTreeNode *node) {
    if (node == tree->NIL) {
        return tree->NIL;
    }
    while (node->left != tree->NIL) {
        node = node->left;
    }
    return node;
}

/**
 * 右边的移植到左边的上面
 * @param transplanted 被移植的
 * @param transplanting 需要移植过去的
 */
void red_black_tree_transplant(RedBlackTree *tree, RedBlackTreeNode *transplanted,
                               RedBlackTreeNode *transplanting) {
    RedBlackTreeNode *parent = transplanted->parent;
    if (parent == tree->NIL) {
        tree->root = transplanting;
    } else {
        if (transplanted == parent->left) {
            parent->left = transplanting;
        } else {
            parent->right = transplanting;
        }
    }
    // transplanting就算是nil也要修改，因为需要向上回溯
    transplanting->parent = parent;
}

/**
 * 删除时修复
 * @param tree
 * @param doubleColorNode 双色所在的节点，额外颜色必定为黑色
 */
void red_black_tree_remove_fixup(RedBlackTree *tree, RedBlackTreeNode *doubleColorNode) {
    // 双色节点回溯到根节点的时候可以退出，节点为红黑色时可以退出，因为只有节点双色这个性质被打破，所以只要删除红节点即可
    // 一定用判断doubleColorNode->parent != tree->NIL，因为回溯或者其他原因导致doubleColorNode是NIL但是父节点依然是自己
    while (doubleColorNode->parent != tree->NIL && doubleColorNode->color == BLACK) {
        RedBlackTreeNode *parent = doubleColorNode->parent;
        if (doubleColorNode == parent->left) {
            // 兄弟节点为红色
            RedBlackTreeNode *brother = parent->right;
            if (brother->color == RED) {
                brother->color = parent->color;
                parent->color = RED;
                red_black_tree_locate_left(tree, parent);
            } else {
                // 兄弟节点为黑色，单子为红色
                if (brother->left->color == RED) { //RL型，转成RR型
                    brother->color = RED;
                    brother->left->color = BLACK;
                    red_black_tree_locate_right(tree, brother);
                    brother = parent->right;
                }
                if (brother->right->color == RED) { //RR型
                    brother->color = parent->color;
                    parent->color = RED;
                    brother->right->color = BLACK;
                    red_black_tree_locate_left(tree, parent);
                } else {
                    // 兄弟节点为黑色，双子为黑色
                    brother->color = RED;
                    // 指针向上就代表把上面加上一层黑色
//                    parent->color = BLACK;
                }
            }
        } else {
            // 兄弟节点为红色
            RedBlackTreeNode *brother = parent->left;
            if (brother->color == RED) {
                brother->color = parent->color;
                parent->color = RED;
                red_black_tree_locate_right(tree, parent);
            } else {
                // 兄弟节点为黑色，单子为红色
                if (brother->right->color == RED) { //LR型，转成LL型
                    brother->color = RED;
                    brother->right->color = BLACK;
                    red_black_tree_locate_left(tree, brother);
                    brother = parent->left;
                }
                if (brother->left->color == RED) { //LL型
                    brother->color = parent->color;
                    parent->color = RED;
                    brother->left->color = BLACK;
                    red_black_tree_locate_right(tree, parent);
                } else {
                    // 兄弟节点为黑色，双子为黑色
                    brother->color = RED;
                    // 指针向上就代表把上面加上一层黑色
//                    parent->color = BLACK;
                }
            }
        }
        // 指针向上就代表把上面加上一层黑色
        doubleColorNode = doubleColorNode->parent;
    }
    doubleColorNode->color = BLACK;
    tree->root->color = BLACK;
}

/**
 * @param tree
 * @param node 需要删除的
 */
void red_black_tree_remove_node(RedBlackTree *tree, RedBlackTreeNode *node) {
    // 逻辑删除的节点颜色（额外的颜色）
    bool extraColor;
    // 包含双重颜色的节点，就算是nil也没关系，nil的parent发证改变时也能相应的向上回溯
    RedBlackTreeNode *doubleColorNode;

    RedBlackTreeNode *left = node->left;
    RedBlackTreeNode *right = node->right;
    // 没有子树或者只有一个子树的情况，直接用另外一颗子树移植过去即可
    if (left == tree->NIL) {
        red_black_tree_transplant(tree, node, right);
        extraColor = node->color;
        // doubleColorNode是nil也没关系，这种情况在修复时不会进行回溯
        doubleColorNode = right;
    } else if (right == tree->NIL) {
        red_black_tree_transplant(tree, node, left);
        extraColor = node->color;
        doubleColorNode = left;
    } else {
        // 查找后继节点
        RedBlackTreeNode *after = red_black_tree_find_min(tree, right);
        // 后继节点的右节点移植到后继节点的位置，此时的后继变成了一个游离状态的节点
        red_black_tree_transplant(tree, after, after->right);

        extraColor = after->color;
        doubleColorNode = after->right;

        // 将需要删除的节点的左右子树赋给后继节点来替代他
        after->left = node->left;
        node->left->parent = after;
        after->right = node->right;
        node->right->parent = after;
        after->color = node->color;

        // 然后将后继移植过去，即可完成后继和删除节点的位置互换
        red_black_tree_transplant(tree, node, after);
    }
    // 额外颜色为红色的不需要删除，因为逻辑删除的红色节点其中一个子节点一定为nil（删除特性决定），那么相应的另一个节点也一定是nil（黑高相等）
    if (extraColor == BLACK) {
        red_black_tree_remove_fixup(tree, doubleColorNode);
    }
    red_black_tree_init_nil(tree->NIL);
    // 这个与逻辑删除的不同，是真正删除的节点
    free(node);
    tree->size--;
}

void red_black_tree_remove(RedBlackTree *tree, void *element, int (*compare)(void *, void *)) {
    RedBlackTreeNode *node = tree->root;
    while (node != tree->NIL) {
        int i = compare(element, node->element);
        if (i < 0) {
            node = node->left;
        } else if (i > 0) {
            node = node->right;
        } else {
            // 执行删除
            red_black_tree_remove_node(tree, node);
            break;
        }
    }
}

ContiguousList *red_black_tree_check_list = NULL;

void red_black_tree_check_add_list(void *element) {
    printf("%d\n", *(int *) element);
    contiguous_list_add(red_black_tree_check_list, element);
}

int red_black_tree_foreach_child_check(RedBlackTree *tree, RedBlackTreeNode *node, void (*consumer)(void *)) {
    if (node == tree->NIL) {
        return 1;
    }
    printf("进入左节点\n");
    int left_black = red_black_tree_foreach_child_check(tree, node->left, consumer);
    printf("回溯\n");
    consumer(node->element);
    printf("颜色:%s\n", node->color ? "黑色" : "红色");
    if (node->parent->element != NULL) {
        printf("当前的父节点:%d\n\n", *(int *) node->parent->element);
    }
    if (node->color == RED && node->parent->color == RED) {
        printf("-----双红色-----\n");
    }
    printf("进入右节点\n");
    int right_black = red_black_tree_foreach_child_check(tree, node->right, consumer);
    if (left_black != right_black) {
        printf("-----左右黑高不一致-----\n");
    }
    printf("回溯\n");
    printf("左黑高%d,右黑高%d\n", left_black, right_black);
    return node->color == BLACK ? left_black + 1 : left_black;
}

void red_black_tree_foreach_check(RedBlackTree *tree, void (*consumer)(void *)) {
    RedBlackTreeNode *node = tree->root;
    red_black_tree_foreach_child_check(tree, node, consumer);
}


void *red_black_tree_check(RedBlackTree *tree, int (*compare)(void *, void *)) {
    red_black_tree_check_list = (ContiguousList *) contiguous_list_init();
    red_black_tree_foreach_check(tree, red_black_tree_check_add_list);

    for (int i = 1; i < red_black_tree_check_list->size; ++i) {
        void *before = *(red_black_tree_check_list->elements + (i - 1));
        void *current = *(red_black_tree_check_list->elements + i);
        if (compare(before, current) >= 0) {
            printf("---------------出现错误------------\n");
            return current;
        }
    }
    if (
            tree->NIL->parent != tree->NIL ||
            tree->NIL->left != tree->NIL ||
            tree->NIL->right != tree->NIL
            ) {
        printf("---------------出现错误，叶子节点不对------------\n");
    }
    if (tree->root->parent != tree->NIL) {
        printf("---------------出现错误，根节点的父节点不是NIL------------\n");
    }
    free(red_black_tree_check_list);
    red_black_tree_check_list = NULL;
    return NULL;
}


#endif //DATASTRUCTURE_RED_BLACK_TREE_H



