/************************
*  Sparse.c
*  Brian Camilo
*  2023 Winter CSE101 pa4
*  Main program that perform matrix operations
*************************/

#include <stdlib.h>

#include "Matrix.h"
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
    
    int n, a, b;
    fscanf(input, "%d %d %d\n\n", &n, &a, &b);

    Matrix A = newMatrix(n), B = newMatrix(n);
    int row = 0, column = 0;
    double x = 0.0;
   
    for(int i = 1; i <= a; i++){
        fscanf(input, "%i %i %lf", &row, &column, &x);
        changeEntry(A, row, column, x);
    }

    for(int i = 1; i <= b; i++){
        fscanf(input, "%i %i %lf", &row, &column, &x);
        changeEntry(B, row, column, x);
    }

    // print statements for a and b for non zero entries
    fprintf(output, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(output, A);
    fprintf(output, "\nB has %d non-zero entries:\n", NNZ(B));
    printMatrix(output, B);
    
    // print statements for a and b scalar multiplication by (1.5)
    fprintf(output, "\n(1.5)*A =\n");
    printMatrix(output, scalarMult(1.5, A));
    
    // print statements for sum A and B
    fprintf(output, "\nA+B =\n");
    printMatrix(output, sum(A, B));
    
    // print statement for sum A itself
    fprintf(output, "\nA+A =\n");
    printMatrix(output, sum(A, A));
    
    // print statement for difference B and A
    fprintf(output, "\nB-A =\n");
    printMatrix(output, diff(B, A));
    
    // print statement for difference of A itself
    fprintf(output, "\nA-A =\n");
    printMatrix(output, diff(A, A));
    
    // print statement for difference of A itself
    fprintf(output, "\nTranspose(A) =\n");
    printMatrix(output, transpose(A));
    
    // print statement for product A and B
    fprintf(output, "\nA*B =\n");
    printMatrix(output, product(A, B));
    
    // print statement for product B itself
    fprintf(output, "\nB*B =\n");
    printMatrix(output, product(B, B));

    freeMatrix(&A);
    freeMatrix(&B);
    fclose(input);
    fclose(output);
    
    return 0;
}
