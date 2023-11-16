/************************
*  FindComponents.c
*  Brian Camilo
*  2023 Winter CSE101 pa3
*************************/

#include <stdlib.h>

#include "Graph.h"
#include "List.h"

int main(int argc, char *argv[]){
    FILE *input = NULL, *output = NULL;

    //there must be exactly two arguments the user will provide
    if(argc != 3){
        if(argc > 3){
            fprintf(stderr, "Error: More than two command line arguments passed.\n");
        } else if(argc < 3){
            fprintf(stderr, "Error: Less than two command line arguments passed.\n");
        }
        exit(EXIT_FAILURE);
    }

    //open input file for reading
    input = fopen(argv[1], "r");
    if(!input){
        fprintf(stderr, "File error: Cant open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    //open output file for writing
    output = fopen(argv[2], "w");
    if(!output){
        fprintf(stderr, "File error: Cant open file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    //get the size of graph from input file and initialize a graph
    int graph_size = 0;
    fscanf(input, "%d\n", &graph_size);
    Graph graph = newGraph(graph_size);

    //in the input file, every 1st number is vertex A while the 2nd is vertex B
    int vertex_a = 0, vertex_b = 0;
    
    //read each line from input and then add an edge using the two vertices
    while(fscanf(input, "%d %d", &vertex_a, &vertex_b) == 2){
        if(vertex_a == 0 && vertex_b == 0){
            break;
        }
        addArc(graph, vertex_a, vertex_b);
    }

    //print values
    fprintf(output, "Adjacency list representation of G:\n");
    printGraph(output, graph);
    fprintf(output, "\n");
    
    //create list containing 1 to <graph size>
    List list = newList();
    for(int i = 1; i <= graph_size; i++){
        append(list, i);
    }

    //create transpose of graph
    Graph graph_trans = transpose(graph);
        
    //do DFS and transpose DFS
    DFS(graph, list);
    DFS(graph_trans, list);

    //count components
    int component_count = 0;
    moveFront(list);
    while(index(list) != -1){
        if(getParent(graph_trans, get(list)) == NIL){
            component_count++;
        }
        moveNext(list);
    }
    fprintf(output, "G contains %i strongly connected components:\n", component_count);

    //determine strong components
    component_count = 0;
    List list_strong = newList();
    moveBack(list);
    for(int i = 1; i <= getOrder(graph); i++){
        prepend(list_strong, get(list));
        if(getParent(graph_trans, get(list)) == NIL){
            fprintf(output, "Component %d: ", ++component_count);
		    printList(output, list_strong);
		    fprintf(output, "\n");
		    clear(list_strong);
        }
        movePrev(list);
    }

    freeList(&list);
    freeList(&list_strong);
    freeGraph(&graph);
    freeGraph(&graph_trans);
    fclose(input);
    fclose(output);
    
    return 0;
}
