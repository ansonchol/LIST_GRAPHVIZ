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

    list->max_size = 20;

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

void print_html_list(List list, const char* file_dot, DotConfig dot_cfg,
                     const char* file_html, HtmlConfig html_cfg)
{
    List_export_dot(list, file_dot, dot_cfg);
    system("dot -Tsvg graph.dot -o graph.svg");

    List_export_html(list, file_html, html_cfg);
}

// -------------------------- GRAPHVIZ  -------------------------------------------
void List_export_dot(List list, const char* file_dot, DotConfig cfg)
{
    FILE* graph = fopen(file_dot, "w");
    if (graph == NULL) { printf("ERROR: cannot open %s\n", file_dot); return; }

    fprintf(graph,
    "digraph {\n"
    "   rankdir = LR;\n"
    "   splines = ortho;\n"
    "   nodesep = %f;\n"
    "   ranksep = %f;\n"
    "   node [color=\"%s\", fontsize=%d, shape=record];\n"
    "   edge [color=\"%s\", arrowhead=\"vee\", arrowsize = 1.0];\n",
    cfg.nodesep, cfg.ranksep, cfg.node_color, cfg.font_size, cfg.edge_color);

    for (int i = 0; i < list.max_size; i++)
    {
        if (list.next[i] == EMPTY)
        {
            fprintf(graph,
            "   element%d [label=\"[%d]\\nfree\", style=filled, fillcolor=\"#eeeeee\", fontcolor=\"#888888\"];\n",
            i, i);
        }
        else
        {
            fprintf(graph,
            "   element%d [label=\"[%d]\\ndata: %d | next: %d | prev: %d\"];\n",
            i, i, list.data[i], list.next[i], list.prev[i]);
        }
    }

    // 2. НЕВИДИМАЯ цепочка для ВСЕХ узлов
    for (int i = 0; i < list.max_size - 1; i++)
    {
        fprintf(graph, "   element%d -> element%d [style=invis];\n", i, i + 1);
    }

    // 3. Рёбра next/prev
    for (int i = 0; i < list.max_size; i++)
    {
        if (list.next[i] == EMPTY_PTR) continue;

        fprintf(graph, "   element%d -> element%d [color=\"%s\", constraint=false];\n", i, list.next[i], cfg.edge_color);
        fprintf(graph, "   element%d -> element%d [color=\"%s\", style=dashed, constraint=false, dir=forward];\n", i, list.prev[i], cfg.prev_color);
    }

    fprintf(graph, "}\n");
    fclose(graph);
}


// -------------------------- ГЕНЕРАЦИЯ HTML --------------------------
void List_export_html(List list, const char* file_html, HtmlConfig cfg)
{
    FILE* html = fopen(file_html, "w");
    if (html == NULL) { printf("ERROR: cannot open %s\n", file_html); return; }

    fprintf(html,
    "<!DOCTYPE html>\n"
    "<html>\n<head>\n"
    "   <meta charset=\"utf-8\">\n"
    "   <title>%s</title>\n"
    "   <style>\n"
    "       body { %s }\n"
    "       table { %s }\n"
    "       th, td { %s }\n"
    "       th { %s }\n"
    "       .free { %s }\n"
    "       .graph-container { %s }\n"
    "       .graph-container img { %s }\n"
    "   </style>\n"
    "</head>\n<body>\n",
    cfg.title, cfg.css_body, cfg.css_table, cfg.css_cell,
    cfg.css_th, cfg.css_free_td, cfg.css_graph_div, cfg.css_graph_img);

    fprintf(html, "HEAD: %d     TAIL: %d\n", list.head, list.tail);
    // 2. Таблица
    fprintf(html, "   <h2>%s</h2>\n", cfg.header_table);
    fprintf(html, "   <table>\n");

    // Строка index
    fprintf(html, "       <tr><th>index</th>");
    for (int i = 0; i < list.max_size; i++)
        fprintf(html, "<th>%d</th>", i);
    fprintf(html, "</tr>\n");

    // Строка data
    fprintf(html, "       <tr><th>data</th>");
    for (int i = 0; i < list.max_size; i++) {
        if (list.data[i] == EMPTY)
            fprintf(html, "<td class=\"free\">-</td>");
        else fprintf(html, "<td>%d</td>", list.data[i]);
    }
    fprintf(html, "</tr>\n");

    // Строка next
    fprintf(html, "       <tr><th>next</th>");
    for (int i = 0; i < list.max_size; i++) {
        if (list.next[i] == EMPTY_PTR)
            fprintf(html, "<td class=\"free\">-</td>");
        else fprintf(html, "<td>%d</td>", list.next[i]);
    }
    fprintf(html, "</tr>\n");

    // Строка prev
    fprintf(html, "       <tr><th>prev</th>");
    for (int i = 0; i < list.max_size; i++) {
        if (list.prev[i] == EMPTY_PTR)
            fprintf(html, "<td class=\"free\">-</td>");
        else fprintf(html, "<td>%d</td>", list.prev[i]);
    }
    fprintf(html, "</tr>\n");

    fprintf(html, "   </table>\n");

    // 3. Граф
    fprintf(html, "   <h2>%s</h2>\n", cfg.header_graph);
    fprintf(html,
    "   <div class=\"graph-container\">\n"
    "       <img src=\"%s\" alt=\"Graphviz Graph\">\n"
    "   </div>\n",
    cfg.graph_file);

    fprintf(html, "</body>\n</html>\n");
    fclose(html);
}

//--------------------------------открытие и закрытие файлов---------------------
void DUMP_begin(FILE_graph* graph)
{
    //очистка старого файла
    graph->dot = fopen(graph->file_dot, "w");
    if (graph->dot == NULL)
    {
        printf("ERROR: file %s doesn't exist", graph->file_dot);
        return;
    }

    graph->html = fopen(graph->file_html, "w");
    if (graph->html == NULL)
    {
        printf("ERROR: file %s doesn't exist", graph->file_html);
        return;
    }

    fclose(graph->dot);
    fclose(graph->html);

    //Заполнение нового файла
    graph->dot = fopen(graph->file_dot, "a");
    if (graph->dot == NULL)
    {
        printf("ERROR: file %s doesn't exist", graph->file_dot);
        return;
    }

    graph->html = fopen(graph->file_html, "a");
    if (graph->html == NULL)
    {
        printf("ERROR: file %s doesn't exist", graph->file_html);
        return;
    }
}

void DUMP_end(FILE_graph* graph)
{
    assert(graph);

    fclose(graph->dot);
    fclose(graph->html);
}
