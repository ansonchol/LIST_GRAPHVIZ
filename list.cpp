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
    int ip_empty = 1; //нулевой элемент всегда пуст
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

void print_html_list(List list, DOT_FILE* dot_file, HTML_FILE* html_file)
{
    assert(dot_file && html_file);

    // Уникальное имя svg для этого дампа
    char svg_name[64];
    snprintf(svg_name, sizeof(svg_name), "graph_%d.svg", html_file->count_dump);

    if (dot_file->file) fclose(dot_file->file);
    dot_file->file = fopen(dot_file->file_name, "w");
    assert(dot_file->file);

    List_export_dot(list, dot_file);
    fclose(dot_file->file);
    dot_file->file = NULL;

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "dot -Tsvg graph.dot -o %s", svg_name);
    system(cmd);

    // Передаём имя svg в экспорт html
    List_export_html(list, html_file, svg_name);   // <-- изменить сигнатуру
    fflush(html_file->file);

    html_file->count_dump++;
    print_list(list);
}

// -------------------------- GRAPHVIZ  -------------------------------------------
void List_export_dot(List list, DOT_FILE* dot_file)
{
    assert(dot_file);

    fprintf(dot_file->file,
    "digraph {\n"
    "   rankdir = LR;\n"
    "   splines = ortho;\n"
    "   nodesep = %f;\n"
    "   ranksep = %f;\n"
    "   node [color=\"%s\", fontsize=%d, shape=record];\n"
    "   edge [color=\"%s\", arrowhead=\"vee\", arrowsize = 1.0];\n",
    dot_file->cfg.nodesep, dot_file->cfg.ranksep, dot_file->cfg.node_color, dot_file->cfg.font_size, dot_file->cfg.edge_color);

    for (int i = 0; i < list.max_size; i++)
    {
        if (list.next[i] == EMPTY_PTR)
        {
            fprintf(dot_file->file,
            "   element%d [label=\"[%d]\\nfree\", style=filled, fillcolor=\"#eeeeee\", fontcolor=\"#888888\"];\n",
            i, i);
        }
        else
        {
            fprintf(dot_file->file,
            "   element%d [label=\"[%d]\\ndata: %d | next: %d | prev: %d\"];\n",
            i, i, list.data[i], list.next[i], list.prev[i]);
        }
    }

    // 2. НЕВИДИМАЯ цепочка для ВСЕХ узлов
    for (int i = 0; i < list.max_size - 1; i++)
    {
        fprintf(dot_file->file, "   element%d -> element%d [style=invis];\n", i, i + 1);
    }

    // 3. Рёбра next/prev
    for (int i = 0; i < list.max_size; i++)
    {
        if (list.next[i] == EMPTY_PTR) continue;

        fprintf(dot_file->file, "   element%d -> element%d [color=\"%s\", constraint=false];\n",
                i, list.next[i], dot_file->cfg.edge_color);
        fprintf(dot_file->file, "   element%d -> element%d [color=\"%s\", style=dashed, constraint=false, dir=forward];\n",
                i, list.prev[i], dot_file->cfg.prev_color);
    }

    fprintf(dot_file->file, "}\n");
}


// -------------------------- ГЕНЕРАЦИЯ HTML --------------------------
void List_export_html_begin(HTML_FILE* html_file)
{
    assert(html_file && html_file->file);

    fprintf(html_file->file,
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
    html_file->cfg.title,
    html_file->cfg.css_body,   html_file->cfg.css_table,
    html_file->cfg.css_cell,   html_file->cfg.css_th,
    html_file->cfg.css_free_td,
    html_file->cfg.css_graph_div, html_file->cfg.css_graph_img);
}

void List_export_html_end(HTML_FILE* html_file)
{
    assert(html_file && html_file->file);
    fprintf(html_file->file, "</body>\n</html>\n");
}

void List_export_html(List list, HTML_FILE* html_file, const char* svg_name)
{
    assert(html_file && html_file->file);

    // Разделитель между дампами (по желанию)
    fprintf(html_file->file, "<hr>\n");

    fprintf(html_file->file, "<p><b>HEAD:</b> %d &nbsp;&nbsp; <b>TAIL:</b> %d</p>\n",
            list.head, list.tail);

    // ---- Таблица ----
    fprintf(html_file->file, "<h2>%s</h2>\n", html_file->cfg.header_table);
    fprintf(html_file->file, "<table>\n");

    fprintf(html_file->file, "<tr><th>index</th>");
    for (int i = 0; i < list.max_size; i++)
        fprintf(html_file->file, "<th>%d</th>", i);
    fprintf(html_file->file, "</tr>\n");

    fprintf(html_file->file, "<tr><th>data</th>");
    for (int i = 0; i < list.max_size; i++) {
        if (list.data[i] == EMPTY)
            fprintf(html_file->file, "<td class=\"free\">-</td>");
        else
            fprintf(html_file->file, "<td>%d</td>", list.data[i]);
    }
    fprintf(html_file->file, "</tr>\n");

    fprintf(html_file->file, "<tr><th>next</th>");
    for (int i = 0; i < list.max_size; i++) {
        if (list.next[i] == EMPTY_PTR)
            fprintf(html_file->file, "<td class=\"free\">-</td>");
        else
            fprintf(html_file->file, "<td>%d</td>", list.next[i]);
    }
    fprintf(html_file->file, "</tr>\n");

    fprintf(html_file->file, "<tr><th>prev</th>");
    for (int i = 0; i < list.max_size; i++) {
        if (list.prev[i] == EMPTY_PTR)
            fprintf(html_file->file, "<td class=\"free\">-</td>");
        else
            fprintf(html_file->file, "<td>%d</td>", list.prev[i]);
    }
    fprintf(html_file->file, "</tr>\n");

    fprintf(html_file->file, "</table>\n");

    // ---- Граф ----
    fprintf(html_file->file, "<h2>%s</h2>\n", html_file->cfg.header_graph);
    fprintf(html_file->file,
        "<div class=\"graph-container\">\n"
        "   <img src=\"%s\" alt=\"Graphviz Graph\">\n"
        "</div>\n",
        svg_name);
}

//--------------------------------открытие и закрытие файлов---------------------
void DUMP_begin(DOT_FILE* dot_file, HTML_FILE* html_file)
{
    assert(dot_file && html_file);

    dot_file->file = fopen(dot_file->file_name, "w");
    if (!dot_file->file)
    {
        printf("ERROR: %s\n", dot_file->file_name);
        return;
    }

    html_file->file = fopen(html_file->file_name, "w");
    if (!html_file->file)
    {
        printf("ERROR: %s\n", html_file->file_name);
        fclose(dot_file->file);
        dot_file->file = NULL;
        return;
    }

    html_file->count_dump = 0;

    // Шапка HTML пишется один раз
    List_export_html_begin(html_file);
}

void DUMP_end(DOT_FILE* dot_file, HTML_FILE* html_file)
{
    assert(dot_file && html_file);

    if (html_file->file) {
        List_export_html_end(html_file);   // </body></html>
        fclose(html_file->file);
        html_file->file = NULL;
    }
    if (dot_file->file) {
        fclose(dot_file->file);
        dot_file->file = NULL;
    }
}
