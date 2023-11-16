/************************************
*  Shuffle.cpp
*  Brian Camilo
*  2023 Winter CSE101 pa5
*  Main program
************************************/

#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include "List.h"

using namespace std;

void shuffle(List& D){
        List left, right, result;
	int n = D.length();
        
        D.moveFront();
        for(int i = 0; i < n; i++){
                if(i < (n / 2)){
                        left.insertBefore(D.moveNext());
                } else {
                        right.insertBefore(D.moveNext());
                }
        }

        left.moveFront();
        right.moveFront();

        // merge approach is to insert elements from the right list to the left list
        for(int i = 0; i < (n / 2); i++){
                left.insertBefore(right.moveNext());
                left.moveNext();
        }
        if(n % 2 != 0 || n == 1){
                left.insertBefore(right.moveNext());
        }
        D = left;
}

int main(int argc, char * argv[]){
	// check command line for correct number of arguments
	if(argc != 2){
		cerr << "Error: One (1) argument is expected." << endl;
		return(EXIT_FAILURE);
	}

        int num_max_cards = atoi(argv[1]);
        if(num_max_cards <= 0){
                cerr << "Error: Argument must be a positive integer." << endl;
		return(EXIT_FAILURE);
        }

	cout << "deck size       shuffle count" << endl;
        cout << string(30, '-') << endl;	

	for(int i = 1; i <= num_max_cards; i++){
                int counter = 1;
                List list, list_copy;
                
                for(int j = 1; j <= i; j++){
		        list.insertBefore(j);
                }
                list_copy = list;

                shuffle(list);
                while(!list.equals(list_copy)){
                        shuffle(list);
                        counter++;
                }

                cout << " " << setw(16) << left << i << setw(16) << left << counter << endl;
	}
	
	return(EXIT_SUCCESS);
}
