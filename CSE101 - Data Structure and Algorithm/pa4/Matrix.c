/************************
*  Matrix.c
*  Brian Camilo
*  2023 Winter CSE101 pa4
*  Implementation file for Matrix ADT
*************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "List.h"
#include "Matrix.h"

typedef struct EntryObj *Entry;

typedef struct EntryObj{ 
	int col;
	double val;
} EntryObj;

typedef struct MatrixObj{
	List *rows;
	int size;
	int nonzero;
} MatrixObj;

// Constructor-Destructor

// newEntry()
// create new entry
Entry newEntry(int col, double val){
	Entry E = malloc(sizeof(EntryObj));
	E->col = col;
	E->val = val;
	
	return E;
}

// newMatrix()
// create new matrix
Matrix newMatrix(int n){
	Matrix M = malloc(sizeof(MatrixObj));
	M->rows = malloc(sizeof(List) *(n+1));
	
	for(int i = 1; i <= n; i++){
		M->rows[i] = newList();
	}

	M->size = n;
	M->nonzero = 0;

	return M;
}

// freeEntry()
// frees the entry
void freeEntry(Entry *pE){
	if(pE && *pE){
		free(*pE);
		*pE = NULL;
	}
}

// freeMatrix()
// frees the entry
void freeMatrix(Matrix* pM){
	if(pM && *pM){
		makeZero(*pM);
		for(int i = 1; i <= (*pM)->size; i++){
			freeList(&(*pM)->rows[i]);
		}		
		free((*pM)->rows);
		free(*pM);
		*pM = NULL;
	}
}

// Access functions

// size() return the size of square Matrix M.
int size(Matrix M){
 	if(!M){
        fprintf(stderr, "Matrix error: calling size() on NULL list reference\n");
        exit(EXIT_FAILURE);
    } else {
        return M->size;
    }
}

// NNZ() return the number of non-zero elements in M.
int NNZ(Matrix M){
	if(!M){
        fprintf(stderr, "Matrix error: calling NNZ() on NULL list reference\n");
        exit(EXIT_FAILURE);
    } else {
        return M->nonzero;
    }
}

// equals() return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B){
	if(A == NULL || B == NULL){
        fprintf(stderr, "Matrix error: calling equals() on NULL list reference\n");
        exit(EXIT_FAILURE);
	}

	if(size(A) != size(B)){
		return 0;
	}

	Matrix A_copy = copy(A);
	Matrix B_copy = copy(B);
	for(int i = 1; i <= size(A); i++){
		if(length(A_copy->rows[i]) != length(B_copy->rows[i])){
			return 0;
		}
		moveFront(A_copy->rows[i]);
		moveFront(B_copy->rows[i]);
		while(index((A_copy->rows)[i]) >= 0) {
			// if column and value aren't the same
            if(((Entry)get((A_copy->rows)[i]))->val != ((Entry)get((B_copy->rows)[i]))->val){
                return 0;
            }
			moveNext(A_copy->rows[i]);
			moveNext(B_copy->rows[i]);
		}
	}

	return 1;
}

// Manipulation procedures

// makeZero() re-sets M to the zero Matrix state.
void makeZero(Matrix M){
	if(!M){
        fprintf(stderr, "Matrix error: calling makeZero() on NULL list reference\n");
        exit(EXIT_FAILURE);
    } 
	
	M->nonzero = 0;
	for(int i = 1; i <= size(M); i++){
		while(length((M->rows)[i]) > 0){
			moveFront((M->rows)[i]);
			Entry entry = (Entry)get((M->rows)[i]);
			freeEntry(&entry);
			deleteFront((M->rows)[i]);
		}
	}
}

// changeEntry() changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
	if(!M){
        fprintf(stderr, "Matrix error: calling changeEntry() on NULL list reference\n");
        exit(EXIT_FAILURE);
    } 

	if(i < 1 || i > size(M) || j < 1 || j >size(M)){ 
      printf("Matrix error: row and column are invalid\n");
      exit(EXIT_FAILURE);
	}

	bool found = false;
	moveFront((M->rows)[i]);
	while(index((M->rows)[i]) >= 0){
		Entry entry = (Entry)get((M->rows)[i]);
		if(entry->col == j){
			found = true;
			if(x == 0){
				freeEntry(&entry);
				delete((M->rows)[i]);
				M->nonzero--;
			} else {
				entry->val = x;
			}
			break;
		}
		moveNext((M->rows)[i]);
	}
	
	if(!found && x != 0){
		Entry new_entry = newEntry(j, x);
		if(length(M->rows[i]) == 0){
			append(M->rows[i], new_entry);
		} else {
			moveFront((M->rows)[i]);
			while(index((M->rows)[i]) >= 0){
				Entry list_entry = (Entry)get((M->rows)[i]);
				if(list_entry->col > new_entry->col){
					found = true;
					insertBefore((M->rows)[i], new_entry);
					break;
				}
				moveNext((M->rows)[i]);
			}
			if(!found){
				append(M->rows[i], new_entry);
			}
		}
		M->nonzero++;
	}
}

// Matrix Arithmetic operations

// copy() returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A){
	if(!A){
        fprintf(stderr, "Matrix error: calling copy() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

	Matrix matrix = newMatrix(size(A));
	for(int i = 1; i <= size(A); i++){
		moveFront((A->rows)[i]);
		while(index((A->rows)[i]) >= 0){
			Entry A_entry = (Entry)get((A->rows)[i]);
            append((matrix->rows)[i], newEntry(A_entry->col, A_entry->val));
            moveNext((A->rows)[i]);
		}
	}
	matrix->nonzero = A->nonzero;

    return matrix;
}

// transpose() returns a reference to a new Matrix object representing the transpose of A.
Matrix transpose(Matrix A){
	if(!A){
        fprintf(stderr, "Matrix error: calling transpose() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

	Matrix matrix = newMatrix(size(A));
	for(int i = 1; i <= size(A); i++){
		moveFront((A->rows)[i]);
		while(index((A->rows)[i]) >= 0){
			Entry A_entry = (Entry)get((A->rows)[i]);
            append((matrix->rows)[A_entry->col], newEntry(i, A_entry->val));
            moveNext((A->rows)[i]);
		}
	}
	matrix->nonzero = A->nonzero;

    return matrix;
}

// scalarMult() returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A){
	if(!A){
        fprintf(stderr, "Matrix error: calling scalarMult() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }
	
	Matrix matrix = newMatrix(size(A));
	for(int i = 1; i <= size(A); i++){
		moveFront((A->rows)[i]);
		while(index((A->rows)[i]) >= 0){
			Entry A_entry = (Entry)get((A->rows)[i]);
            append((matrix->rows)[i], newEntry(A_entry->col, x * A_entry->val));
            matrix->nonzero++;
            moveNext((A->rows)[i]);
		}
	}
	
   	return matrix;
}

// sum() returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B){
	if(A == NULL || B == NULL){
        fprintf(stderr, "Matrix error: calling sum() on NULL list reference\n");
        exit(EXIT_FAILURE);
    } 
	
	if(size(A) != size(B)){
        fprintf(stderr, "Matrix error: Matrices are not equal\n");
        exit(EXIT_FAILURE);
    }

	Matrix matrix = newMatrix(size(A));
	Matrix A_copy = copy(A);
	Matrix B_copy = copy(B);
	for(int i = 1; i <= size(A); i++){
		moveFront(A_copy->rows[i]);
    	moveFront(B_copy->rows[i]);
		while(index(A_copy->rows[i]) >= 0 || index(B_copy->rows[i]) >= 0){
			Entry A_entry = NULL, B_entry = NULL;
			if(index(A_copy->rows[i]) >= 0){
				A_entry = (Entry)get(A_copy->rows[i]);
			}
			if(index(B_copy->rows[i]) >= 0){
				B_entry = (Entry)get(B_copy->rows[i]);
			}
			
			if(A_entry && B_entry && A_entry->col == B_entry->col){
				double sum = A_entry->val + B_entry->val;
				if(sum != 0){
					append(matrix->rows[i], newEntry(A_entry->col, sum));
					matrix->nonzero++;
				}
				moveNext(A_copy->rows[i]);
				moveNext(B_copy->rows[i]);
			} else {
				if((A_entry && B_entry && A_entry->col < B_entry->col) || (A_entry && !B_entry)){
					append(matrix->rows[i], newEntry(A_entry->col, A_entry->val));
					moveNext(A_copy->rows[i]);
				} else if((A_entry && B_entry && B_entry->col < A_entry->col) || (B_entry && !A_entry)){
					append(matrix->rows[i], newEntry(B_entry->col, B_entry->val));
					moveNext(B_copy->rows[i]);
				}
				matrix->nonzero++;
			}
		}
	}

	return matrix;
}

// diff() returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
	if(!A){
        fprintf(stderr, "Matrix error: calling diff() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

	if(size(A) != size(B)){
        fprintf(stderr, "Matrix error: Matrices are not equal\n");
        exit(EXIT_FAILURE);
    }

	Matrix matrix = newMatrix(size(A));
	Matrix A_copy = copy(A);
	Matrix B_copy = copy(B);
	for(int i = 1; i <= size(A); i++){
		moveFront(A_copy->rows[i]);
    	moveFront(B_copy->rows[i]);
		while(index(A_copy->rows[i]) >= 0 || index(B_copy->rows[i]) >= 0){
			Entry A_entry = NULL, B_entry = NULL;
			if(index(A_copy->rows[i]) >= 0){
				A_entry = (Entry)get(A_copy->rows[i]);
			}
			if(index(B_copy->rows[i]) >= 0){
				B_entry = (Entry)get(B_copy->rows[i]);
			}

			if(A_entry && B_entry && A_entry->col == B_entry->col){
				double diff = A_entry->val - B_entry->val;
				if(diff != 0){
					append(matrix->rows[i], newEntry(A_entry->col, diff));
					matrix->nonzero++;
				}
				moveNext(A_copy->rows[i]);
				moveNext(B_copy->rows[i]);
			} else {
				if((A_entry && B_entry && A_entry->col < B_entry->col) || (A_entry && !B_entry)){
					append(matrix->rows[i], newEntry(A_entry->col, A_entry->val));
					moveNext(A_copy->rows[i]);
				} else if((A_entry && B_entry && B_entry->col < A_entry->col) || (B_entry && !A_entry)){
					append(matrix->rows[i], newEntry(B_entry->col, B_entry->val * (double)-1.0));
					moveNext(B_copy->rows[i]);
				}
				matrix->nonzero++;
			} 
		}
	}

	return matrix;
}

// computes the vector dot product of two matrix rows
double vectorDot(List P, List Q) {
    if(!P || !Q){
        fprintf(stderr, "Matrix error: calling vectorDot() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

    moveFront(P);
    moveFront(Q);
    double total = 0.0;
    while(index(P) >= 0 && index(Q) >= 0){
        Entry P_entry = (Entry)get(P);
        Entry Q_entry = (Entry)get(Q);
        if(P_entry->col == Q_entry->col){
            total += (P_entry->val * Q_entry->val);
        }
        if(P_entry->col <= Q_entry->col){
            moveNext(P);
        }
        if(Q_entry->col <= P_entry->col){
            moveNext(Q);
        }
    }

    return total;
}

// product() returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
	if(!A){
        fprintf(stderr, "Matrix error: calling product() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }
	
	if(size(A) != size(B)){
        fprintf(stderr, "Matrix error: Matrices are not equal\n");
        exit(EXIT_FAILURE);
    }

	Matrix trans_matrix = transpose(B);
    Matrix prod_matrix = newMatrix(size(A));
    for(int i = 1; i <= size(prod_matrix); i++){
        if(length(A->rows[i]) == 0){
            continue;
        }
        for(int j = 1; j <= size(prod_matrix); j++){
            if(length(trans_matrix->rows[j]) == 0){
                continue;
            }
            double v_dot = vectorDot(A->rows[i], trans_matrix->rows[j]);
            if(v_dot == 0){
                continue;
            } else {
                append(prod_matrix->rows[i], newEntry(j, v_dot));
                prod_matrix->nonzero++;
            }
        }
    }
    
	//freeMatrix(&trans_matrix);
    return prod_matrix;
}

// printMatrix() prints a string representation of Matrix M to filestream out.
void printMatrix(FILE* out, Matrix M){
	if(!M){
        fprintf(stderr, "Matrix error: calling printMatrix() on NULL list reference\n");
        exit(EXIT_FAILURE);
    }

	for(int i = 1; i <= size(M); i++){
		if(length(M->rows[i]) > 0){
			fprintf(out, "%d:", i);
			moveFront(M->rows[i]);
			while(index(M->rows[i]) >= 0){
				Entry E = (Entry)get((M->rows)[i]);
				fprintf(out, " (%i, %.1lf)", E->col, E->val);
				moveNext((M->rows)[i]);
			}
			fprintf(out, "\n");
		}
	}
}
