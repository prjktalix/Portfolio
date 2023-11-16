/************************
*  Graph.c
*  Brian Camilo
*  2023 Winter CSE101 pa3
*************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "List.h"
#include "Graph.h"

typedef struct GraphObj{
	List *neighbors;
	int *color;
	int *discover;
	int *parent;
	int *finish;
	int order;
	int size;
	int time;
}GraphObj;

// Constructors-Destructors
Graph newGraph(int n){
  Graph G = malloc(sizeof(GraphObj));
  G->neighbors =  malloc(sizeof(List) * (n+1));
  G->parent =  malloc(sizeof(int) * (n+1));
  G->discover =  malloc(sizeof(int) * (n+1));
  G->finish =  malloc(sizeof(int) * (n+1));
  
  G->color =  malloc(sizeof(int) * (n+1));
  //COLORS REFERENCE: 0 = white, 1 = gray, 2 = black

  for(int i = 0; i < n + 1; i++){
    G->neighbors[i] = newList();
    G->discover[i] = UNDEF;
    G->finish[i] = UNDEF;
    G->parent[i] = NIL;
    G->color[i] = 1;
  }
  
  G->order = n;
  G->size = 0;
  G->time = 0;

  return G;
}

void freeGraph(Graph* pG){
  if(pG && *pG){
    for(int i = 1; i <= (*pG)->order; i++){
      freeList(&(*pG)->neighbors[i]);
    }		
    free((*pG)->color);
    free((*pG)->parent);
    free((*pG)->discover);
    free((*pG)->finish);
    free(*pG);
    *pG = NULL;
	}
}

// Access functions
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

/* Pre: 1<=u<=n=getOrder(G) */
int getParent(Graph G, int u){
  if(!G){
    fprintf(stderr,"Graph error: calling getParent() on NULL list reference\n");
    exit(EXIT_FAILURE);
  } else if(u >=1 &&  u <= G->order){
    return G->parent[u];
  }
  
  return 0;
} 

/* Pre: 1<=u<=n=getOrder(G) */
int getDiscover(Graph G, int u){
  if(!G){
    fprintf(stderr,"Graph error: calling getDiscover() on NULL list reference\n");
    exit(EXIT_FAILURE);
  }

  if(u >=1 &&  u <= G->order){
    return G->discover[u];
  } else {
    fprintf(stderr,"Graph error: calling getDiscover() with invalid vertex\n");
    exit(EXIT_FAILURE);
	}
} 

/* Pre: 1<=u<=n=getOrder(G) */
int getFinish(Graph G, int u){
  if(!G){
    fprintf(stderr,"Graph error: calling getFinish() on NULL list reference\n");
    exit(EXIT_FAILURE);
  }  
  
  if (u >=1 &&  u <= G->order){
    return G->finish[u];
  } else {
    fprintf(stderr,"Graph error: calling getFinish() with invalid vertex\n");
    exit(EXIT_FAILURE);
	}
} 

// Manipulation procedures
void addEdge(Graph G, int u, int v){
	if(!G){
		fprintf(stderr, "Error: calling addEdge() on a NULL reference.\n");
		exit(EXIT_FAILURE);
  } 
    	
	if(u >= 1 && u <= G->order && v >= 1 && v <= G->order){
		//add v to u
		if(length(G->neighbors[u]) == 0){
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
		if(length(G->neighbors[v]) == 0){
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
 
  //check if v already exists in list u
  moveFront(G->neighbors[u]);
  while(index(G->neighbors[u]) != -1){
    if(get(G->neighbors[u]) == v){
      return;
    }
    moveNext(G->neighbors[u]);
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

/*private helper*/
/*pseudocode from GraphAlgorithm Visit*/
void Visit(Graph G, List S, int x){
  if(!G){
    fprintf(stderr, "List error: calling Visit() on a NULL reference.\n");
    exit(EXIT_FAILURE);
  }

  G->discover[x] = ++(G->time); // discover x
  G->color[x] = 1;
  moveFront(G->neighbors[x]);
  for(int y = 0; y< length(G->neighbors[x]); y++){
    if(G->color[ get(G->neighbors[x])] == 0){
      G->parent[get(G->neighbors[x])] = x;
      Visit(G, S, get(G->neighbors[x]));
    }
    moveNext(G->neighbors[x]);
  }
  G->color[x] = 2;
  prepend(S,x);
  G->finish[x] = ++(G->time);     // finish x
}

/* Pre: length(S)==getOrder(G) */
void DFS(Graph G, List S){
  if(!G){
		fprintf(stderr, "List error: calling DFS() on a NULL reference.\n");
		exit(EXIT_FAILURE);
  } 

  if(length(S) == getOrder(G)){
    for(int i = 1; i < getOrder(G) + 1; i++){
      G->color[i] = 0;
      G->parent[i] = NIL;
    }

    G->time = 0;
    List list = copyList(S); // get order of vertices 5-7
    clear(S);
    moveFront(list);
    for(int i = 1; i < getOrder(G) + 1; i++){
      if(G->color[get(list)] == 0){
        Visit(G, S, get(list));
      }
      moveNext(list);
    }
    freeList(&list);
  }
}

// Other Functions
Graph transpose(Graph G){
  if(!G){
    fprintf(stderr, "List error: calling transpose() on a NULL reference.\n");
    exit(EXIT_FAILURE);
  } 

  Graph graph = newGraph(getOrder(G));
  for(int i = 1; i <= getOrder(G); i++){
    moveFront(G->neighbors[i]);
    while(index(G->neighbors[i]) != -1) {
      addArc(graph,(get(G->neighbors[i])), i);
      moveNext(G->neighbors[i]);
    }
  }
  graph->size = getSize(G);

  return graph;
}

Graph copyGraph(Graph G){
  if(!G){
    fprintf(stderr, "Error: calling copyGraph() on a NULL reference.\n");
    exit(EXIT_FAILURE);
  }

  Graph graph = newGraph(getOrder(G));
  for(int i = 1; i <= getOrder(G); i++){
    for(moveFront(G->neighbors[i]); index(G->neighbors[i]) >= 0; moveNext(G->neighbors[i])){
      addArc(graph, i, (get(G->neighbors[i])));
    }

  }
  graph->size = getSize(G);

  return graph;
}

void printGraph(FILE* out , Graph G){
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
