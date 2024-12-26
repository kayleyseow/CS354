////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c
// This File:        division.c
// Other Files:      sendsig.c mySigHandler.c
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
// Persons:          N/A
//
// Online sources:   N/A
////////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Buffer defined from writeup
#define BUFFER 100

// Global variable because both handlers and main() use it
int completedDivisionOps = 0;

/* A handler for SIGFPE; it prints confirmation of SIGFPE, and prints out how 
 * many times the division operations were and exits the program due to division
 * by 0.
 *
 * SIGINT is called with the keyboard input Ctrl+c.
 */
void handler_SIGFPE(){
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", completedDivisionOps);
    printf("The program will be terminated.\n");
	exit(0);
}

/* A handler for SIGINT; it prints confirmation of SIGINT, and prints out how 
 * many times the division operations were and exits the program.
 *
 * SIGINT is called with the keyboard input Ctrl+c.
 */
void handler_SIGINT(){
    printf("\nTotal number of operations completed successfully: %d\n", completedDivisionOps);
    printf("The program will be terminated.\n");
	exit(0);
}

/* Main function running the program.
 *
 * No pre-conditions are required.
 * argc is an int which records how many arguments are passed to the program.
 * argv is a character pointer array which points to each argument passed to 
 * the program.
 * Returns 0 if the function runs smoothly.
 */
int main(int argc, char *argv[]){
    // Struct for division by 0
    struct sigaction divZeroAction;
    memset(&divZeroAction, 0, sizeof(divZeroAction));
    divZeroAction.sa_handler = handler_SIGFPE;
    if (sigaction(SIGFPE, &divZeroAction, NULL) != 0){
        printf("Error: binding SIGFPE Handler.\n");
        exit(0);
    }

    // Struct for a keyboard interrupt
    struct sigaction keyInterruptAction;
    memset(&keyInterruptAction, 0, sizeof(keyInterruptAction));
    keyInterruptAction.sa_handler = handler_SIGINT;
    if (sigaction(SIGINT, &keyInterruptAction, NULL) != 0){
        printf("Error: binding SIGINT Handler.\n");
        exit(0);
    }

    // While loop to continuously ask for integer input
    while(1){
        char int1[BUFFER];
        char int2[BUFFER];

        printf("Enter first integer: ");
        // if the fgets is NULL, return an error
        if (fgets(int1, BUFFER, stdin) == NULL){
            printf("Error: First Integer Input");
            exit(0);
        }
        printf("Enter second integer: ");
        // if the fgets is NULL, return an error
        if (fgets(int2, BUFFER, stdin) == NULL){
            printf("Error: Second Integer Input");
            exit(0);
        }

        // After checking, we can save it as the integer
        int number1 = atoi(int1);
        int number2 = atoi(int2);
        // Print out the integer, increment the global variable
        printf("%d / %d is %d with a remainder of %d\n", number1, number2, number1 / number2, number1 % number2);
        completedDivisionOps = completedDivisionOps + 1;
    }
    return 0;
}