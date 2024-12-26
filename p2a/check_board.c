///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2021 Deb Deppeler
// Posting or sharing this file is prohibited, including any changes/additions.
//
// We have provided comments and structure for this program to help you get 
// started.  Later programs will not provide the same level of commenting,
// rather you will be expected to add same level of comments to your work.
// 09/20/2021 Revised to free memory allocated in get_board_size function.
// 01/24/2022 Revised to use pointers for CLAs
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        check_board.c
// This File:        check_board.c
// Other Files:      board01.txt board0.txt board10.txt board1solution.txt board1.txt board2solution.txt board2.txt board3.txt board4.txt board5.txt board6.txt
// Semester:         CS 354 Fall 2022
// Instructor:       deppeler
//
// Author:           Kayley Seow
// Email:            kseow@wisc.edu
// CS Login:         kayley
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   including Peer Mentors, Instructors, and TAs.
//
// Persons:          N/A
//                   
//
// Online sources:   N/A
//                   
//                   
////////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
     
char *DELIM = ",";  // commas ',' are a common delimiter character for data strings
     
/* COMPLETED (DO NOT EDIT):       
 * Read the first line of input file to get the size of that board.
 * 
 * PRE-CONDITION #1: file exists
 * PRE-CONDITION #2: first line of file contains valid non-zero integer value
 *
 * fptr: file pointer for the board's input file
 * size: a pointer to an int to store the size
 */
void get_board_size(FILE *fptr, int *size) {      
    char *line1 = NULL;
    size_t len = 0;

    if ( getline(&line1, &len, fptr) == -1 ) {
        printf("Error reading the input file.\n");
		free(line1);
        exit(1);
    }

    char *size_chars = NULL;
    size_chars = strtok(line1, DELIM);
    *size = atoi(size_chars);

	// free memory allocated for reading first link of file
	free(line1);
	line1 = NULL;
}



/* TODO:
 * Returns 1 if and only if the board is in a valid Sudoku board state.
 * Otherwise returns 0.
 * 
 * A valid row or column contains only blanks or the digits 1-size, 
 * with no duplicate digits, where size is the value 1 to 9.
 * 
 * Note: p2A requires only that each row and each column are valid.
 * 
 * board: heap allocated 2D array of integers 
 * size:  number of rows and columns in the board
 */
int valid_board(int **board, int size) {
	// Check to see if the size of the sudoku board is valid
	if(size < 0 || size > 9){
		return 0;
	}

	// Check if the value at index is valid, or if it is a 0
	for(int row = 0; row < size; row++){
		for(int column = 0; column < size; column++){
			// Checks to see if the value is less than 0
			if(*(*(board + row ) + column) < 0){
				return 0;
			}

			// Checks to see if the value is greater than the inputted size
			if(*(*(board + row) + column) > size){
				return 0;
			}
		}
    	}

	// Iterates through the row to see if there are any duplicates
	for(int row = 0; row < size; row++){
		for(int col = 0; col < size; col++){
			int num = *(*(board + row ) + col);
       			if(num != 0){
				for(int num2 = 0; num2 < size; num2++){
					if(num2 != col){
						if(num == *(*(board + row ) + num2)){
							return 0;
						}
					}
				}
       			}
		}
	}

	// Iterates through the column to see if there are any duplicates
	for(int row = 0; row < size; row++){
		for(int col = 0; col < size; col++){
        		int num = *(*(board + row ) + col);
			if(num != 0){
        			for(int num2 = 0; num2 < size; num2++){
                			if(num2 != row){
                        			if(num == *(*(board + num2 ) + col)){
                                			return 0;
                        			}
                			}
        			}
			}
		}
	}
    return 1;
}

/* TODO: COMPLETE THE MAIN FUNCTION
 * This program prints "valid" (without quotes) if the input file contains
 * a valid state of a Sudoku puzzle board wrt to rows and columns only.
 *
 * A single CLA which is the name of the file that contains board data 
 * is required.
 *
 * argc: the number of command line args (CLAs)
 * argv: the CLA strings, includes the program name
 */
int main( int argc, char **argv ) {              
     
    // TODO: Check if number of command-line arguments is correct.
    if (argc > 2 || argc < 2){
	printf("Usage: ./check_board <input_filename>\n");
	exit(1);
    }

    // Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Can't open file for reading.\n");
        exit(1);
    }

    // Declare local variables.
    int size;

    // TODO: Call get_board_size to read first line of file as the board size.
	get_board_size(fp, &size);
	if (size < 1 || size > 9) {
		printf("invalid\n");
		exit(0);
	}

    // TODO: Dynamically allocate a 2D array for given board size.
	int **tempArray = (int**)malloc(sizeof(int*) * size);
	if (tempArray == NULL){
		printf("invalid\n");
		exit(0);
	}

	for(int i = 0; i < size; i++){
		*(tempArray + i) = malloc(sizeof(int) * size);
		if (*(tempArray + i) == NULL){
			printf("invalid\n");
			exit(0);
		}
	}

    // Read the rest of the file line by line.
    // Tokenize each line wrt the delimiter character 
    // and store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {

        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading line %i of the file.\n", i+2);
            exit(1);
        }

        token = strtok(line, DELIM);
        for (int j = 0; j < size; j++) {
            // TODO: Complete the line of code below
            // to initialize your 2D array.
            *(*(tempArray + i) + j) = atoi(token);
            token = strtok(NULL, DELIM);
        }
    }

    // TODO: Call the function valid_board and print the appropriate
    //       output depending on the function's return value.
	if(valid_board(tempArray, size)){
		printf("valid\n");
	}else{
		printf("invalid\n");
	}



	//int result = valid_board(tempArray, size);
	//if (result == 1){
	//	printf("valid\n");
	//} else {
	//	printf("invalid\n");
	//}

    // TODO: Free all dynamically allocated memory.
	for (int i = 0; i < size; i ++){
		free(*(tempArray + i));
		*(tempArray + i) = NULL;
	}
	free(tempArray);
	tempArray = NULL;

    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
     
    return 0;       
}       






// s22

