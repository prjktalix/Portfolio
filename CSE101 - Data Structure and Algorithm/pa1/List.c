/********************************************
*  Brian Camilo, bcamilo
*  2023 Winter CSE101 pa1
*  List.c
*  Implementation file for Integer List ADT
********************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "List.h"

// private node struct with pseudocode from queue.c
typedef struct NodeObj* Node;

typedef struct NodeObj{
    int data;
    Node next;
    Node prev;
} NodeObj;

// private list struct with pseudocode from queue.c
typedef struct ListObj{
    Node front;
    Node back;
    Node cursor;
    int length;
    int index;
} ListObj;

// Create nodes
// pseudocode from queue.c
Node newNode(int data){
    Node N;
    N = malloc(sizeof(NodeObj));
    N->data = data;
    N->next = NULL;
    N->prev = NULL;
    return N;
}

// pseudocode from lecture
void freeNode(Node* node){    
    if(node && *node){
        free(*node);
        node = NULL;
    }
}

// Constructors-Destructors ---------------------------------------------------

// returns reference to new empty list
// pseudocode from queue.c
List newList(void){     
    List L = malloc(sizeof(ListObj));
    L->front = L->back = NULL;
    L->cursor = NULL;
    L->length = 0;
    L->index = -1;
    return L;
}  
             
// pseudocode from queue.c
void freeList(List* pL){    
    if(pL !=NULL && *pL != NULL){
        free(*pL);
        *pL = NULL;
    }
}

// Access functions -----------------------------------------------------------

// returns numbers of element in L
int length(List L){
    if(!L){
        fprintf(stderr, "List error: calling length() on NULL list reference\n");
        exit(EXIT_FAILURE);
    } else {
        return L->length;
    }
}

// returns index of cursor element
int index(List L){  
    if(!L){
        fprintf(stderr, "List error: calling index() on NULL list reference\n");
        exit(EXIT_FAILURE);
    } else{
        return L->index;
    }
}

// returns front element of L. 
// pre: length()>0
int front(List L){
    if(!L){
        fprintf(stderr, "List error: calling front() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    if(L->length <= 0){
        fprintf(stderr, "List error: calling front() length <= 0 \n");
        exit(EXIT_FAILURE);
    } else {
        return L->front->data;
    }
}

// returns back element of L. 
// pre: length()>0
int back(List L){
    if(!L){
        fprintf(stderr, "List error: calling back() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    if (L->length <= 0){
        fprintf(stderr, "List error: calling back() length <= 0 \n");
        exit(EXIT_FAILURE);
    } else {
        return L->back->data;
    }
}

// returns cursor element of L. 
// pre: length()>0, index()>=0
int get(List L){
    if(!L){
        fprintf(stderr, "List error: calling get() on NULL list reference\n");
        exit(EXIT_FAILURE);
    } 
    if(L->length <= 0){
        fprintf(stderr, "List error: calling get() on length <= 0\n");
        exit(EXIT_FAILURE);
    } else if(index(L) < 0){
        fprintf(stderr, "List error: calling get() on index < 0\n");
        exit(EXIT_FAILURE);
    } else {
        return L->cursor->data;
    }
}

// Returns true iff Lists A and B are in same 
// state, and returns false otherwise.
bool equals(List A, List B){
    if(A == NULL || B == NULL){
        fprintf(stderr, "List error: calling equals() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    bool is_equal = true;
    if(A->length == B->length){ //checks first if A & B have same lengths. If false, automatically not equal
        Node node_A = A->front;
        Node node_B = B->front;
        while(is_equal && node_A){
            is_equal = (node_A->data == node_B->data);
            node_A = node_A->next;
            node_B = node_B->next;
        }
    } else {
        is_equal = false;
    }

    return is_equal;
}           

// Manipulation procedures ----------------------------------------------------

// Resets L to its original empty state.
void clear(List L){
    if(!L){
        fprintf(stderr, "List error: calling clear() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    Node current = L->front;
    Node next = NULL;
    while(current){
        next = current->next;
        freeNode(&current);
        current = next;
    }
    L->front = L->back = L->cursor = NULL;
    L->index = -1;
    L->length = 0;
}

// Overwrites the cursor element’s data with x.         
// Pre: length()>0, index()>=0                 
void set(List L, int x){
    if(!L){
        fprintf(stderr, "List error: calling set() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    if(L->length >0 && L->index >= 0){
        L->cursor->data = x;
    }   
}

// If L is non-empty, sets cursor under the front element
// otherwise does nothing.
void moveFront(List L){
    if(!L){
        fprintf(stderr, "List error: calling movefront() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }
    
    if(L->length > 0){
        L->cursor = L->front;
        L->index = 0;
    }
}

// If L is non-empty, sets cursor under the back element,
// otherwise does nothing. 
void moveBack(List L){
    if(!L){
        fprintf(stderr, "List error: calling moveBack() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    if(L->length > 0){
        L->cursor = L->back;
        L->index = L->length - 1;
    }
}             

// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing
void movePrev(List L){
    if(!L){
        fprintf(stderr, "List error: calling movePrev() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    if(L->cursor && L->index > 0 && L->index <= length(L) - 1){
        L->cursor = L->cursor->prev;
		L->index--;
    } else if(L->cursor && L->index == 0){
        L->cursor = NULL;
        L->index = -1;
    }
}                

// If cursor is defined and not at back, move cursor one
// step toward the back of L, if cursor is defined and at
// back, cursor becomes undefined, if cursor is undefined
// do nothing
void moveNext(List L){
    if(!L){
        fprintf(stderr, "List error: calling moveNext() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    if(L->cursor && L->index >= 0 && L->index < length(L) - 1){
        L->cursor = L->cursor->next;
        L->index++;
    } else if(L->cursor && L->index == length(L) - 1){
        L->cursor = NULL;
        L->index = -1;
    }
}           

// Insert new element into L. If L is non-empty,
// insertion takes place before front element.
void prepend(List L, int x){
    if(!L){
        fprintf(stderr, "List error: calling prepend() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(x);
    if(L->length == 0){
        L->front = L->back = N;
    } else {                        // not empty
        L->front->prev = N;
        N->next = L->front;
        L->front = N;
        if(L->cursor){
             L->index++;
        }
    }
    L->length++;
} 

// Insert new element into L. If L is non-empty,
// insertion takes place after back element.
void append(List L, int x){      
    if(!L){
        fprintf(stderr, "List error: calling append() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(x);
    if(L->length == 0){
        L->front = L->back = N;
    } else {
        N->prev = L->back;
        L->back->next = N;
        L->back = N;
    }
    L->length++;
}

// Insert new element before cursor.
// Pre: length()>0, index()>=0
void insertBefore(List L, int x){
    if(!L){
        fprintf(stderr, "List error: calling insertBefore() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    if(L->length <= 0 && L->index < 0 ){
        fprintf(stderr, "List error: calling insertBefore() witih empty list and undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    
    Node N = newNode(x);
    if(L->cursor != L->front){      // not front
        N->next = L->cursor;
        N->prev = L->cursor->prev;
        L->cursor->prev->next = N;
    } else {                         // front
        N->next = L->front;
        L->front = N;
    }
    L->cursor->prev = N;
    L->length++;
    L->index++;
} 

// Insert new element after cursor.
// Pre: length()>0, index()>=0
void insertAfter(List L, int x){
    if(!L){
        fprintf(stderr, "List error: calling insertAfter() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(x);
    if (L->length <= 0 && L->index < 0 ){
        fprintf(stderr, "List error: calling insertAfter() witih empty list and undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    if(L->cursor != L->back){       // not back
        N->prev = L->cursor;
        N->next = L->cursor->next;
        L->cursor->next->prev = N;
    } else {                        // back
        N->prev = L->back;
        L->back = N;    
    }
    L->cursor->next = N;
    L->length++;
}

// Delete the front element. 
// Pre: length()>0
void deleteFront(List L){
    if(!L){
        printf("List error: calling deleteFront() on NULL list reference.");
        exit(EXIT_FAILURE);
    }
    if(length(L) == 0){
        printf("List error: calling deleteFront() on empty list.");
        exit(EXIT_FAILURE);
    }
    
    //check first if cursor is at the front. If so, cursor shall be undefined
    if(L->cursor == L->front){
        L->cursor = NULL;
    }
    //two cases for deleteFront: (1) list has one element and (2) list has more than one element;
    if(length(L) == 1){
        freeNode(&(L->front));
        L->front = L->back = NULL;
    } else {
        L->front = L->front->next;
        freeNode(&(L->front->prev));
        L->front->prev = NULL;
    }

    L->index--;
    L->length--;
}  

// Delete the back element. 
//Pre: length()>0
void deleteBack(List L){
    if(!L){
        printf("List error: calling deleteFront() on NULL list reference.");
        exit(EXIT_FAILURE);
    }
    if(length(L) == 0){
        printf("List error: calling deleteFront() on empty list.");
        exit(EXIT_FAILURE);
    }
    
    //check first if cursor is at the back. If so, cursor shall be undefined
    if(L->cursor == L->back){
        L->cursor = NULL;
        L->index = -1;
    }
    //two cases for deleteBack: (1) list has one element and (2) list has more than one element
    if(length(L) == 1){
        freeNode(&(L->front));
        L->front = L->back = NULL;
    } else {
        L->back = L->back->prev;
        freeNode(&(L->back->next));
        L->back->next = NULL;
    }

    L->length--;
}          

// Delete cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L){
    if(!L){
        printf("List error: calling delete() on NULL list reference.");
        exit(EXIT_FAILURE);
    }
    if(length(L) == 0){
        printf("List error: calling delete() on empty list.");
        exit(EXIT_FAILURE);
    }
    if(index(L) < 0){
        printf("List error: calling delete() with invalid index.\n");
        exit(EXIT_FAILURE);
    }
    
    Node cursor = L->cursor;
    //if cursor is at the front
    if(cursor == L->front){
        deleteFront(L);
    } 
    //if cursor is at the back
    else if(cursor == L->back){
        deleteBack(L);
    }
    //if cursor is neither at front nor back
    else{
        cursor->prev->next = cursor->next;
        cursor->next->prev = cursor->prev;
        freeNode(&cursor);
        cursor = NULL;
        L->index = -1;
        L->length--;
    }
}

// Other operations -----------------------------------------------------------

// Prints to the file pointed to by out
void printList(FILE* out, List L){
   if(!L){
		fprintf(stderr, "List error: calling printList() on a NULL reference.\n");
		exit(EXIT_FAILURE);
    }

    Node node = NULL;
    for(node = L->front; node; node = node->next){
        printf("%d ", node->data);
    }
}

// returns a new List representing the same integer
// sequence as L.
List copyList(List L){             
    if(!L){
		fprintf(stderr, "List error: calling copyList() on a NULL reference.\n");
		exit(EXIT_FAILURE);
	}
    
    Node node = NULL;
    List copy = newList();

    for(node = L->front; node; node = node->next){
        append(copy, node->data);
    }
    
    return copy;
}

