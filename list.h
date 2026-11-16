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

typedef struct DotConfig{
    int font_size;          // размер шрифта узлов
    double nodesep;         // отступ между узлами
    double ranksep;         // отступ между рангами
    const char* node_color; // цвет рамки узла
    const char* edge_color; // цвет стрелок next
    const char* prev_color; // цвет стрелок prev
} DotConfig;

typedef struct HtmlConfig{
    const char* title;      // заголовок вкладки браузера
    const char* header_table;// заголовок над таблицей
    const char* header_graph;// заголовок над графом
    const char* graph_file; // имя файла картинки (например "graph.svg")

    const char* css_body;
    const char* css_table;
    const char* css_cell;
    const char* css_th;
    const char* css_free_td;
    const char* css_graph_div;
    const char* css_graph_img;
} HtmlConfig;

typedef struct DOT_FILE
{
    const char* file_name;
    DotConfig cfg;
    FILE* file;
} DOT_FILE;

typedef struct HTML_FILE
{
    const char* file_name;
    HtmlConfig cfg;
    FILE* file;
    int count_dump;
} HTML_FILE;

void fill_empty(int value, int* array, int size);
void print_list(List list);

void List_Init(List* list);
void List_push(List* list, int value, int prev_ip);
void List_pop(List* list, int ip);
void List_destroy(List* list);

void List_export_html_begin(HTML_FILE* html_file);
void List_export_html_end(HTML_FILE* html_file);

void print_html_list(List list, DOT_FILE* dot_file, HTML_FILE* html_file);

void List_export_dot(List list, DOT_FILE* dot_file);
void List_export_html(List list, HTML_FILE* html_file, const char* svg_name);

void DUMP_begin(DOT_FILE* dot_file, HTML_FILE* html_file);
void DUMP_end(DOT_FILE* dot_file, HTML_FILE* html_file);

#endif
