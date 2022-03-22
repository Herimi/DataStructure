#include <stdio.h>

#include "contiguous_list.h"
#include "linked_list.h"
#include "binary_search_tree.h"
#include "red_black_tree.h"

void foreach_consumer(void *element) {
    printf("%d\n", *((int *) element));
}

void contiguous_list_test() {
    printf("-------contiguous_list_test start -------\n");
    ContiguousList *list = contiguous_list_init();
    printf("debug\n");
    int arr[] = {1, 2, 3, 4};
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) {
        int x = 100;
        contiguous_list_add(list, &arr[i]);
        contiguous_list_insert(list, 100, &x);
    }
    int x = 10086;
    contiguous_list_insert(list, 1, &x);
    printf("debug\n");
    contiguous_list_for_each(list, foreach_consumer);
    printf("debug\n");
    contiguous_list_remove(list, &arr[2]);
    printf("debug\n");
    contiguous_list_for_each(list, foreach_consumer);
    printf("-------contiguous_list_test end -------\n\n");
}

void linked_list_test() {
    printf("-------linked_list_test start -------\n");
    LinkedList *list = linked_list_init();
    printf("debug\n");
    int arr[] = {1, 2, 3, 4};
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) {
        int x = 100;
        linked_list_add(list, &arr[i]);
        linked_list_insert(list, 1, &x);
    }
    int x = 10086;
    linked_list_insert(list, 1, &x);
    linked_list_insert(list, 100, &x);
    printf("debug\n");
    linked_list_for_each(list, foreach_consumer);
    printf("debug\n");
    linked_list_remove(list, &arr[2]);
    printf("debug\n");
    linked_list_for_each(list, foreach_consumer);
    printf("-------linked_list_test end -------\n\n");
}

int int_compare(void *element1, void *element2) {
    return *((int *) element1) - *((int *) element2);
}

void binary_search_tree_test() {
    BinarySearchTree *tree = (BinarySearchTree *) binary_search_tree_init();
    int arr[] = {5, 4, 9, 8, 6, 7, 3, 1, 2, 0};
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) {
        binary_search_tree_add(tree, &arr[i], int_compare);
    }
    binary_search_tree_remove(tree, &arr[0], int_compare);


    binary_search_tree_foreach(tree, foreach_consumer);
    printf("-----------------------\n");
    binary_search_tree_check(tree, int_compare);
}

void red_black_tree_test() {
    RedBlackTree *tree = (RedBlackTree *) red_black_tree_init();
    int arr[] = {5, 4, 20, 8, 6, 7, 3, 1, 2, 0, 9, 21};
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) {
        red_black_tree_add(tree, &arr[i], int_compare);
    }
    for (int i = 0; i < 6; ++i) {
        red_black_tree_remove(tree, &arr[i], int_compare);
        printf("======================\n");
        red_black_tree_check(tree, int_compare);
    }
//    red_black_tree_foreach(tree, foreach_consumer);
//    printf("-----------------------\n");
}

int main() {
//    contiguous_list_test();
//    linked_list_test();
//    binary_search_tree_test();
    red_black_tree_test();
    return 0;
}




