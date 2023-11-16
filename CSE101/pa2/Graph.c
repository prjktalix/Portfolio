/************************
*  Graph.c
*  Brian Camilo
*  2023 Winter CSE101 pa1
*************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Graph.h"
#include "List.h"

typedef struct GraphObj{
	List *neighbors;
	int *colors;
	int *parents;
	int *distances;
	int source;
	int order;
	int size;
}GraphObj;

/*** Constructors-Destructors ***/
Graph newGraph(int n){
	Graph G = malloc(sizeof(GraphObj));
	G->neighbors =  malloc(sizeof(List) * (n+1));
	G->parents =  malloc(sizeof(int) * (n+1));
	G->distances =  malloc(sizeof(int) * (n+1));
	G->colors =  malloc(sizeof(int) * (n+1));
	//COLORS REFERENCE: 0 = white, 1 = gray, 2 = black
	
	for(int i = 0; i < n + 1; i++){
		G->neighbors[i] = newList();
		G->parents[i] = NIL;
		G->distances[i] = INF;
		G->colors[i] = 1;
	}
	
	G->source =  NIL;
	G->order =  n;
	G->size =  0;
	
	return G;
}

void freeGraph(Graph* pG){
	if(pG && *pG){
		for(int i = 1; i <= (*pG)->order; i++){
			free(&(*pG)->neighbors[i]);
		}		
		free((*pG)->colors);
		free((*pG)->parents);
		free((*pG)->distances);
		free((*pG)->neighbors);
		free(*pG);
		*pG = NULL;
	}
}


/*** Access functions ***/
int getOrder(Graph G){
	if(!G){
		fprintf(stderr,"Graph error: calling getOrder() on NULL list reference\n");
		exit(EXIT_FAILURE);
	} else {
		return G->order;
	}
}
int getSize(Graph G){
	if(!G){
		fprintf(stderr,"Graph error: calling getSize() on NULL list reference\n");
		exit(EXIT_FAILURE);
	} else {
		return G->size;
	}
}

int getSource(Graph G){
	if(!G){
		fprintf(stderr,"Graph error: calling getSource() on NULL list reference\n");
		exit(EXIT_FAILURE);
	} else {
		return G->source;
	}
}

int getParent(Graph G, int u){
	if(!G){
		fprintf(stderr,"Graph error: calling getParent() on NULL list reference\n");
		exit(EXIT_FAILURE);
	} else {
		if (u >= 1 && u <= G->order){
			return G->parents[u];
		}
	}
	return 0;
}

int getDist(Graph G, int u){
	if(!G){
		fprintf(stderr,"Graph error: calling getDist() on NULL list reference\n");
		exit(EXIT_FAILURE);
	}
	
	if(u >= 1 && u <= G->order){
		return G->distances[u];
	} else {
		fprintf(stderr,"Graph error: calling getDist() with invalid vertex\n");
		exit(EXIT_FAILURE);
	}
}

void getPath(List L, Graph G, int u){
	if(!G){
		fprintf(stderr,"Graph error: calling getPath() on NULL list reference\n");
		exit(EXIT_FAILURE);
	}   
	
	//based on PrintPath pseudocode
	if(u == getSource(G)){
		append(L, u);
	} else if(G->parents[u] == NIL){
		append(L, NIL);
	} else {
		getPath(L, G, G->parents[u]);
		if(back(L) != NIL){
			append(L, u);
		}
	}
}

/*** Manipulation procedures ***/
void makeNull(Graph G){
	if(!G){
		fprintf(stderr, "List error: calling makeNull() on a NULL reference.\n");
		exit(EXIT_FAILURE);
    	} 
    	
    	for (int i = 1; i < G->order; i++){
    		clear(G->neighbors[i]);
    		G->parents[i] = NIL;
    		G->distances[i] = INF;
    	}
    	G->size = 0;
    	G->source = NIL;
}

void addEdge(Graph G, int u, int v){
	if(!G){
		fprintf(stderr, "Error: calling addEdge() on a NULL reference.\n");
		exit(EXIT_FAILURE);
    } 
    	
	if(u >= 1 && u <= G->order && v >= 1 && v <= G->order){
		//add v to u
		if(length(G->neighbors[u]) == 0) {
            append(G->neighbors[u], v);
        } else { //ensure sorted list; similar code to pa1 sorting
			moveFront(G->neighbors[u]);
            bool insert_encountered = false;
            while(index(G->neighbors[u]) != -1){
				if(get(G->neighbors[u]) > v){
                    insertBefore(G->neighbors[u], v);
                    insert_encountered = true;
                    break;
                }
                moveNext(G->neighbors[u]);
            }
            if(!insert_encountered){
                append(G->neighbors[u], v);
            }
		}

		//add u to v
		if(length(G->neighbors[v]) == 0) {
            append(G->neighbors[v], u);
        } else { //ensure sorted list; similar code to pa1 sorting
			moveFront(G->neighbors[v]);
            bool insert_encountered = false;
            while(index(G->neighbors[v]) != -1){
				if(get(G->neighbors[v]) > u){
                    insertBefore(G->neighbors[v], u);
                    insert_encountered = true;
                    break;
                }
                moveNext(G->neighbors[v]);
            }
            if(!insert_encountered){
                append(G->neighbors[v], u);
            }
		}

		G->size++;
	}
}

void addArc(Graph G, int u, int v){
	if(!G){
		fprintf(stderr, "List error: calling addArc() on a NULL reference.\n");
		exit(EXIT_FAILURE);
    	} 
    	
    	if(u >= 1 && u <= G->order && v >= 1 && v <= G->order){
		//add v to u
		if(length(G->neighbors[u]) == 0) {
            append(G->neighbors[u], v);
        } else { //ensure sorted list; similar code to pa1 sorting
			moveFront(G->neighbors[u]);
            bool insert_encountered = false;
            while(index(G->neighbors[u]) != -1){
				if(get(G->neighbors[u]) > v){
                    insertBefore(G->neighbors[u], v);
                    insert_encountered = true;
                    break;
                }
                moveNext(G->neighbors[u]);
            }
            if(!insert_encountered){
                append(G->neighbors[u], v);
            }
		}
    	G->size++;
	}
}

//BFS based on provided pseudocode algorithm
void BFS(Graph G, int s){
	if(!G){
		fprintf(stderr, "Graph error: calling BFS() on a NULL reference.\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 1; i < getOrder(G) + 1; i++){
		G->colors[i] = 0;
		G->distances[i] = INF;
    	G->parents[i] = NIL;
    }

	G->colors[s] = 1;
    G->distances[s] = 0;
    G->parents[s] = NIL;

	List Q = newList();
    append(Q, s);

	while(length(Q) > 0){
        moveFront(Q);
        int x = get(Q);
        delete(Q);
        moveFront(G->neighbors[x]);
        while(index(G->neighbors[x]) != -1){
            int y = get(G->neighbors[x]);
            if(G->colors[y] == 0){
                G->colors[y] = 1;
                G->distances[y] = G->distances[x] + 1;
                G->parents[y] = x;
                append(Q, y);
            }
            moveNext(G->neighbors[x]);
        }
        G->colors[x] = 2;
    }

	G->source = s;
}

/*** Other operations ***/
void printGraph(FILE* out, Graph G){
	if(!G){
		fprintf(stderr, "Error: calling printGraph() on a NULL reference.\n");
		exit(EXIT_FAILURE);
    }

	for(int i = 1; i < getOrder(G) + 1; i++){
		fprintf(out, "%d: ", i);
		printList(out, G->neighbors[i]);
		fprintf(out, "\n");
	} 
}

