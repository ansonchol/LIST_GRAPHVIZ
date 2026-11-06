#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"


/*!SECTION
верификатор:
нельзя удалить в пустом списке
ограничение на поиск пустого элемента
существование prev_ip
ограничение на индексы элементов(выход за массив)
неотрицательный count_elem
ограничение крайних случаев на head и tail
*/
void fill_empty(int value, int* array, int size)
{
    assert(array);

    for (int i = 0; i < size; i++)
    {
        array[i] = value;
    }
}

void print_list(List list)
{
    printf("HEAD: %d    Tail: %d\n", list.head, list.tail);
    printf("DATA:");
    for(int i = 0; i < list.max_size; i++)
    {
        printf("|%d|", list.data[i]);
    }
    printf("\n");

    printf("NEXT:");
    for(int i = 0; i < list.max_size; i++)
    {
        printf("|%d|", list.next[i]);
    }
    printf("\n");

    printf("PREV:");
    for(int i = 0; i < list.max_size; i++)
    {
        printf("|%d|", list.prev[i]);
    }
    printf("\n\n\n");
}

void List_Init(List* list)
{
    assert(list);

    list->max_size = 30;

    list->data = (int*)calloc(list->max_size, sizeof(int));
    list->next = (int*)calloc(list->max_size, sizeof(int));
    list->prev = (int*)calloc(list->max_size, sizeof(int));

    list->head = 0;
    list->tail = 0;
    list->count_elem = 0;

    fill_empty(EMPTY,     list->data, list->max_size);
    fill_empty(EMPTY_PTR, list->next, list->max_size);
    fill_empty(EMPTY_PTR, list->prev, list->max_size);

    list->next[0] = 0;
    list->prev[0] = 0;
}

void List_push(List* list, int value, int prev_ip)
{
    assert(list);

    //поиск пустого элемента начиная с 1 элемента
    int ip_empty = 1;
    while (list->data[ip_empty] != EMPTY && ip_empty < list->max_size)
    {
        ip_empty++;
    }
    list->data[ip_empty] = value;

    //TODO изменение tail
    if (list->tail == prev_ip)
    {
    //    tail = ip_empty;
        list->next[ip_empty] = list->next[list->tail]; //передача head new tail
        list->next[list->tail] = ip_empty;             //бывший tail имеет другой next

        list->prev[ip_empty] = list->tail;
        list->prev[0]        = ip_empty;

        list->tail = ip_empty;
    }
    else
    {
        //изменение prev и next
        int next_ip = list->next[prev_ip]; //старый next prev_ip
        //изменение для вновь нового элемента
        list->next[ip_empty] = next_ip;
        list->prev[ip_empty] = prev_ip;

        //изменения для старого prev элемента
        list->next[prev_ip] = ip_empty;

        //изменения для нового элемента после старого
        list->prev[next_ip] = ip_empty;
    }

    list->count_elem++;

}

void List_pop(List* list, int ip)
{
    assert(list);

    int next_ip = list->next[ip];

    if (ip == list->head)
    {
        list->prev[0] = next_ip;
        list->next[0] = list->next[list->head];

        list->data[ip] = EMPTY;
        list->next[ip] = EMPTY_PTR;
        list->prev[ip] = EMPTY_PTR;

        list->head = next_ip;
    }

    else
    {
        int prev_ip = list->prev[ip];

        list->data[ip] = EMPTY;
        list->next[ip] = EMPTY_PTR;
        list->prev[ip] = EMPTY_PTR;

        //изменение для prev элемента
        list->next[prev_ip] = next_ip;

        //изменение для next элемента
        list->prev[next_ip] = prev_ip;
    }
    list->count_elem--;

}

void List_destroy(List* list)
{
    assert(list);

    free(list->data);
    free(list->next);
    free(list->prev);

    list->head = 0;
    list->tail = 0;
    list->count_elem = 0;
}
