/************************
*  FindPath.c
*  Brian Camilo
*  2023 Winter CSE101 pa1
*************************/

#include <stdlib.h>

#include "Graph.h"

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
        addEdge(graph, vertex_a, vertex_b);
    }

    printGraph(output, graph);
    fprintf(output, "\n");

    //perform BFS on each given source vertex in the input file then 
    //get the distance and path to the destination vertex
    List path = newList();
    while(fscanf(input, "%d %d", &vertex_a, &vertex_b)){
        if(vertex_a == 0 && vertex_b == 0){
            break;
        }
        
        //do BFS
        BFS(graph, vertex_a);

        //get distance
        int distance = getDist(graph, vertex_b);
        if(distance == INF){
            fprintf(output, "The distance from %i to %i is infinity\n", getSource(graph), vertex_b);
        } else {
            fprintf(output, "The distance from %i to %i is %i\n", getSource(graph), vertex_b, distance);
        }
        
        //get path
        getPath(path, graph, vertex_b);
        moveFront(path);
        if(get(path) == NIL){
            fprintf(output, "No %i-%i path exists\n", getSource(graph), vertex_b);
        } else {
            fprintf(output, "A shortest %i-%i path is: ", getSource(graph), vertex_b);
            printList(output, path);
            fprintf(output, "\n");
        }
        
        fprintf(output, "\n");
        clear(path);
    }

    freeList(&path);
    freeGraph(&graph);
    fclose(input);
    fclose(output);

    return 0;
}