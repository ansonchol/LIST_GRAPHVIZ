#ifndef LIST_H
#define LIST_H

#define EMPTY -1
#define EMPTY_PTR -1


typedef struct list
{
    int head;
    int tail;

    int* data;
    int* next;
    int* prev;

    int count_elem;
    int max_size;
} List;

void fill_empty(int value, int* array, int size);
void print_list(List list);

void List_Init(List* list);
void List_push(List* list, int value, int prev_ip);
void List_pop(List* list, int ip);
void List_destroy(List* list);

#endif
