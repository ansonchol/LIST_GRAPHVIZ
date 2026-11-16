#include <stdio.h>
#include <stdlib.h>
#include "list.h"

//TODO оптимизировать множественную вставку

int main()
{

    List list = {};
    List_Init(&list);

    DOT_FILE dot_file =
    {
        .file_name = "graph.dot",
        .cfg =
        {
            .font_size = 14,
            .nodesep = 0.4,
            .ranksep = 0.6,
            .node_color = "red",
            .edge_color = "darkgreen",
            .prev_color = "red"
        }
    };

    HTML_FILE html_file =
    {
        .file_name = "dump.html",
        .cfg =
        {
            .title = "Моя структура данных",
            .header_table = "Двусвязный список в виде массива",
            .header_graph = "Связи между элементами",
            .graph_file = "graph.svg",

            .css_body = "font-family: Arial, sans-serif; padding: 20px; color: #333; "
                "background-image: url('bj.jpg'); "
                "background-repeat: no-repeat; "
                "background-size: 400px auto; "
                "background-position: left bottom; "
                "background-attachment: fixed; "
                "background-color: #f9f9f9;",
            .css_table = "border-collapse: collapse; margin-bottom: 30px; text-align: center; font-size: 14px;",
            .css_cell = "border: 1px solid #333; padding: 8px 12px; min-width: 30px;",
            .css_th = "background-color: #e0e0e0; font-weight: bold;",
            .css_free_td = "background-color: #eeeeee; color: #888888;",
            .css_graph_div = "border: 1px solid #ccc; padding: 10px; background: #fff; overflow-x: auto;",
            .css_graph_img = "min-width: 500px; height: auto;"
        },
    };

    DUMP_begin(&dot_file, &html_file);

    List_push(&list, 10, 0);
    List_push(&list, 20, 1);
    List_push(&list, 30, 2);
    List_push(&list, 15, 3);

    print_html_list(list, &dot_file, &html_file);   // состояние до pop

    List_pop(&list, 2);

    print_html_list(list, &dot_file, &html_file);   // состояние после pop

    DUMP_end(&dot_file, &html_file);

    system("explorer.exe dump.html");

    List_destroy(&list);
    return 0;
}
