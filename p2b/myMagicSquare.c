///////////////////////////////////////////////////////////////////////////////
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2022, Deb Deppeler
////////////////////////////////////////////////////////////////////////////////
   
////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:      N/A
// Semester:         CS 354 Lecture 001 Fall 2022
// Instructor:       deppeler
// 
// Author:           Kayley Seow
// Email:            kseow@wisc.edu
// CS Login:         kayley
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Regarding the idea of the modulus, I had to look through my
//                   old code in order to check the concepts and fenceposts in a
//                   nested for loop.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;           // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 *
 * You need input from the user
 * returns the size of the square
 */
int getSize() {
    int squareSize;
    // prompt
    if (fscanf(stdin, "%d", &squareSize) == EOF){
       printf("Did not read size.\n")
        exit(1);
    }
    // checks size entered number, not even
    if (squareSize % 2 != 1 ){
        printf("Magic square size must be odd.\n")
        exit(1); 
    }

    if (squareSize < 3){
        printf("Magic square size must be >= 3.\n")
        exit(1);
    }
    return squareSize;
} 

   
/* TODO:
 * Makes a magic square of size n using the alternate 
 * Siamese magic square algorithm from assignment and 
 * returns a pointer to the completed MagicSquare struct.
 *
 * n the number of rows and columns
 * 
 * returns a MagicSquare that is filled out and all sums of rows and columns are the same
 */
MagicSquare *generateMagicSquare(int n) {
    // Error Checking
    MagicSquare *square = malloc(sizeof(MagicSquare));
    if (square == NULL) {
        printf("Error allocating memority for struct.\n");
        exit(1);
    }

    // set the size to n
    square -> size = n;
    square -> magic_square = malloc(sizeof(int *) * n);

    if (square -> magic_square == NULL) {
        printf("Error allocating memory for magic_square.\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        *(square -> magic_square + i) = malloc(sizeof(int) * n);
        if (*(square -> magic_square + i) == NULL) {
            printf("Error allocating memory for magic_square.\n");
            exit(1);
        }
    }

    // Siamese Method implementation
    int row = 0;
    int col = 0;
    int currcount = 1;
    int totcount = n*n;
    while (currcount <= totcount){
        // access row, column by adding in the row and then column
        *(*(square -> magic_square + row) + col) = currcount;
        count = count + 1;
        if (row - 1 < 0){
            row = n-1;
        } else {
            row = row - 1;
        }

        col = col + 1;
        col = col % n;

        // Wrap around dependent on the column value
        if (*(*(square -> magic_square + row) + col) != 0){
            row = row + 1;
            row = row % n;
            col = col-1;
            if(col < 0){
                col = n - 1;
            }
        } else {
            row = row + 1;
        }
    }
    return square;    
} 

/* TODO:  
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 * 
 * magic_square is a pointer to the MagicSquare that you want to print.
 * filename is a pointer to the filename you want to output to.
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {
    File *outMS = fopen(filename, "w"); // creates an empty file for writing
    if (outMS == NULL){
        fprintf(stderr, "Error opening output file %s.\n", filename);
        exit(1);
    }
    fprintf(outMS, "%d\n", magic_square ->size);

    for(int i = 0; i < magic_square -> size; i++){
        for(int j = 0; j < magic_square -> size; j++){
            fprintf(outMS, "%i", *(*(magic_square -> magic_square + i) + j));
            if (j < magic_square -> size -1){
                fprintf(outMS, ",");
            }
        }
        // Prints a new line at the end of every row
        fprintf(outMS, "\n");
    }

    if(fclose(outMS) != 0){
        printf("Error closing the output file.\n");
        exit(1);
    }
}

/* Generates a magic square of the user specified size and
 * output the quare to the output filename
 *
 * argc is an int checking for the right parameters
 * argv is a pointer to a pointer to an int which gives us the output file
 * Usage: ./myMagicSquare <output_filename>
 */
int main(int argc, char **argv) {
    // TODO: Check input arguments to get output filename
    if (argc != 2){
        printf("Wrong number of CL Arguments.\n");
        exit(1);
    }

    // TODO: Get magic square's size from user
    int mSquareSize = getSize();

    // TODO: Generate the magic square
    MagicSquare *sq = generateMagicSquare(mSquareSize);

    // TODO: Output the magic square
    fileOutMSquare(sq, *(argv + 1));

    // Free memory, from the inside out
    for(int i = 0; i < sq -> size; i++){
        free(*(sq->magic_square + i));
        *(sq->magic_square + i) = NULL;
    }

    // Free the magic_square
    free(sq->magic_square);
    sq->magic_square = NULL;
    free(sq);
    sq = NULL;

    return 0;
} 


//     F22 deppeler myMagicSquare.c      
