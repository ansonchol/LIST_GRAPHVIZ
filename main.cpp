#include <stdio.h>

#include "list.h"

int main()
{
    List list = {};

    List_Init(&list);

    List_push(&list, 10, 0);
    List_push(&list, 20, 1);
    List_push(&list, 30, 2);
    print_list(list);

    // List_pop(&list, 0);
    // print_list(list);

    List_push(&list, 15, 1);
    print_list(list);

    List_destroy(&list);
    return 0;
}
