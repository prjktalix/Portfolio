/************************
*  Graph.h
*  Brian Camilo
*  2023 Winter CSE101 pa1
*************************/
#include "List.h"

#pragma once

#define INF -1	// infinity
#define NIL 0	// undefined

typedef struct GraphObj *Graph; // pointer to GraphObj

/*** Constructors-Destructors ***/
Graph newGraph(int n);
void freeGraph(Graph* pG);

/*** Access functions ***/
int getOrder(Graph G);
int getSize(Graph G);
int getSource(Graph G);
int getParent(Graph G, int u);
int getDist(Graph G, int u);
void getPath(List L, Graph G, int u);

/*** Manipulation procedures ***/
void makeNull(Graph G);
void addEdge(Graph G, int u, int v);
void addArc(Graph G, int u, int v);
void BFS(Graph G, int s);
/*** Other operations ***/
void printGraph(FILE* out, Graph G);
