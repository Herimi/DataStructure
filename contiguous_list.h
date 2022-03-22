//
// Created by Herimi on 2022/2/8.
// 顺序表
//

#ifndef DATASTRUCTURE_CONTIGUOUS_LIST_H
#define DATASTRUCTURE_CONTIGUOUS_LIST_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/**
 * 顺序表使用数组实现
 */

typedef struct ContiguousList {
    // 数组本身（是一个指针数组）
    void **elements;
    // 逻辑大小
    size_t size;
    // 真实大小
    size_t real_size;
} ContiguousList;

/**
 * 初始化
 * @return
 */
ContiguousList *contiguous_list_init() {
    size_t real_size = 1;
    void **elements = (void **) calloc(real_size, sizeof(void *));
    ContiguousList *list = (ContiguousList *) calloc(1, sizeof(ContiguousList));
    list->elements = elements;
    list->size = 0;
    list->real_size = real_size;
    return list;
}

/**
 * 遍历
 * @param list 列表本身
 * @param consumer 方法指针，每次遍历到数据都会调用这个
 */
void contiguous_list_for_each(ContiguousList *list, void (*consumer)(void *)) {
    for (int i = 0; i < list->size; ++i) {
        consumer(*(list->elements + i));
    }
}

/**
 * 扩容，该方法不一定会扩容，只有容量不足了才会扩容
 * @param list
 * @return
 */
bool contiguous_list_expand_capacity(ContiguousList *list) {
    // 空间满了就开始扩容
    if (list->size >= list->real_size) {
        void **elements = list->elements;
        size_t old_real_size = list->real_size;
        // 字节长度
        size_t old_length = (sizeof(void *) * old_real_size);
        // 每次扩大一倍
        size_t real_size = old_real_size << 1;
        // 字节长度
        size_t length = (sizeof(void *) * real_size);
        elements = realloc(elements, length);
        // 从新的部分开始初始化
        memset(elements + old_real_size, 0, length - old_length);
        list->elements = elements;
        list->real_size = real_size;
        return true;
    }
    return false;
}

/**
 * 添加元素
 * @param list
 * @param element
 * @return
 */
bool contiguous_list_add(ContiguousList *list, void *element) {
    contiguous_list_expand_capacity(list);
    // 添加数据
    *(list->elements + list->size) = element;
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
bool contiguous_list_insert(ContiguousList *list, size_t index, void *element) {
    if (index < 0) {
        return false;
    }
    // 下标超出list的部分就加入到最后一个位置
    if (index >= list->size) {
        return contiguous_list_add(list, element);
    }
    contiguous_list_expand_capacity(list);
    // 从最后一个开始，一直到当前位置结束
    for (size_t i = list->size; i > index; --i) {
        // 后面的等于前面的
        *(list->elements + i) = *(list->elements + i - 1);
    }
    *(list->elements + index) = element;
    list->size++;
    return true;
}

/**
 * 删除元素
 * @param list
 * @param element
 * @return
 */
bool contiguous_list_remove(ContiguousList *list, void *element) {
    for (int i = 0; i < list->size; ++i) {
        if (*(list->elements + i) == element) {
            // 找到了就用后面的覆盖前面的，不可以越界
            for (int j = i + 1; j < list->size; ++j) {
                *(list->elements + j - 1) = *(list->elements + j);
            }
            list->size--;
            break;
        }
    }
    return true;
}

#endif //DATASTRUCTURE_CONTIGUOUS_LIST_H
