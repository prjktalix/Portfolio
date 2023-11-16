/****************************************************************************************
*  Brian Camilo, bcamilo
*  2023 Winter CSE101 pa1
*  Lex.c
*  Build function that a performs sorting algorithm
*****************************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "List.h"

#define LINE_INPUT_LENGTH 500

int main(int argc, char *argv[]){
    FILE *input, *output;
    int n_lines = 0;
    char line_input[LINE_INPUT_LENGTH];

    if(argc != 3){
        if(argc > 3){
            fprintf(stderr, "Error: More than two command line arguments passed.\n");
        } else if(argc < 3){
            fprintf(stderr, "Error: Less than two command line arguments passed.\n");
        }
        exit(EXIT_FAILURE);
    }

    input = fopen(argv[1], "r");
    if(!input){
        fprintf(stderr, "Error: Unable to open input file %s.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    output = fopen(argv[2], "w");
    if(!output){
        fprintf(stderr, "Error: Unable to open output file %s.\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    //count lines in file
    while(fgets(line_input, LINE_INPUT_LENGTH, input) != NULL){
        n_lines++;
    }
    
    //read input file lines
    rewind(input);
    char **lines_read = (char**)calloc(n_lines, sizeof(char*));
    for(int i = 0; i < n_lines; i++){
        fgets(line_input, LINE_INPUT_LENGTH, input);
        if(line_input[strlen(line_input) - 1] != '\n'){ 
            strcat(line_input, "\n"); //if last line in the file doesnt have \n, append \n
        }
        lines_read[i] = malloc(strlen(line_input));
        strcpy(lines_read[i], line_input);
    }
    
    //indirect sorting
    List list = newList();
    append(list, 0);    
    for(int i = 1; i < n_lines; i++){
        moveFront(list);
        bool insert_encountered = false;
        char *current_line = lines_read[i];
        while(index(list) != -1){
            if(strcmp(lines_read[get(list)], current_line) >= 0){
                insertBefore(list, i);
                insert_encountered = true;
                break;
            }
            moveNext(list);
        }
        if(!insert_encountered){
            append(list, i);
        }
    }

    //printing to output file
    if(length(list) > 0){
        moveFront(list);
        while(index(list) != -1){
            fprintf(output, "%s", lines_read[get(list)]);
            moveNext(list);
        }
    }

    freeList(&list);
    for(int i = 0; i < n_lines; i++){
        free(lines_read[i]);
    }
    free(lines_read);
    fclose(input);
    fclose(output);
    
    return 0;
}
