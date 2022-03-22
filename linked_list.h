//
// Created by Herimi on 2022/2/8.
// 链表
//

#ifndef DATASTRUCTURE_LINKED_LIST_H
#define DATASTRUCTURE_LINKED_LIST_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

typedef struct LinkedListNode {
    void *element;
    // 上一个（一定不为空，循环双向链表）
    struct LinkedListNode *previous;
    // 下一个（一定不为空，循环双向链表）
    struct LinkedListNode *next;
} LinkedListNode;

/**
 * 循环双向链表
 */
typedef struct LinkedList {
    // 头节点，什么都不存，单纯的为了好操作
    LinkedListNode *head;
    // 逻辑大小
    size_t size;
} LinkedList;

/**
 * 初始化
 * @return
 */
LinkedList *linked_list_init() {
    LinkedList *list = (LinkedList *) calloc(1, sizeof(LinkedList));
    LinkedListNode *head = (LinkedListNode *) calloc(1, sizeof(LinkedListNode));
    head->previous = head;
    head->next = head;
    list->head = head;
    return list;
}

/**
 * 遍历
 * @param list 列表本身
 * @param consumer 方法指针，每次遍历到数据都会调用这个
 */
void linked_list_for_each(LinkedList *list, void (*consumer)(void *)) {
    LinkedListNode *head = list->head;
    LinkedListNode *node = head->next;
    // 循环到头指针的时候需要结束
    while (node != head) {
        consumer(node->element);
        node = node->next;
    }
}

/**
 * 添加元素
 * @param list
 * @param element
 * @return
 */
bool linked_list_add(LinkedList *list, void *element) {
    LinkedListNode *node = (LinkedListNode *) calloc(1, sizeof(LinkedListNode));
    LinkedListNode *head = list->head;
    LinkedListNode *last = head->previous;
    head->previous = node;
    // 最后一个元素的下一个指针指向当前节点
    last->next = node;
    node->previous = last;
    node->next = head;
    node->element = element;
    list->size++;
    return true;
}

/**
 * 指定位置插入一个元素
 * @param list
 * @param index
 * @param element
 * @return
 */
bool linked_list_insert(LinkedList *list, size_t index, void *element) {
    LinkedListNode *head = list->head;
    LinkedListNode *current = head->next;
    for (int i = 0; i < index; ++i) {
        // 到底了就退出
        if (current == head) {
            break;
        }
        current = current->next;
    }
    LinkedListNode *node = (LinkedListNode *) calloc(1, sizeof(LinkedListNode));

    node->element = element;
    node->previous = current->previous;
    node->next = current;

    current->previous->next = node;
    current->previous = node;
    list->size++;
    return true;
}

/**
 * 删除元素
 * @param list
 * @param element
 * @return
 */
bool linked_list_remove(LinkedList *list, void *element) {
    LinkedListNode *head = list->head;
    LinkedListNode *node = head->next;
    // 循环到头指针的时候需要结束
    while (node != head) {
        if (node->element == element) {
            break;
        }
        node = node->next;
    }
    LinkedListNode *previous = node->previous;
    LinkedListNode *next = node->next;

    previous->next = next;
    next->previous = previous;
    free(node);
    list->size--;
    return true;
}

#endif //DATASTRUCTURE_LINKED_LIST_H
