////////////////////////////////////////////////////////////////////////////////
// Main File:        sendsig.c
// This File:        sendsig.c
// Other Files:      mySigHandler.c division.c
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

/* Main function running the program. Kills either SIGINT or SIGUSR1.
 *
 * No pre-conditions are required.
 * argc is an int which records how many arguments are passed to the program.
 * argv is a character pointer array which points to each argument passed to 
 * the program.
 * Returns 0 if the function runs smoothly.
 */
int main(int argc, char *argv[]){
    // Check to see the correct number of arguments
    if (argc != 3){
        printf("Usage: sendsig <signal type> <pid>\n");
        exit(1);
    }

    // Check to see that the commands are either -u or -i
    if (!((strcmp(argv[1], "-u") != 0)||(strcmp(argv[1], "-i") != 0))){
        printf("Usage: sendsig <signal type> <pid>\n");
        exit(1);
    }

    // Validate pid
    int pid_t = 0;
    if ((pid_t = atoi(argv[2])) == 0){
        printf("Usage: sendsig <signal type> <pid>\n");
        exit(1);
    }
    pid_t = atoi(argv[2]);
    
    // Handle the -u case, kill SIGUSR1
    if (strcmp(argv[1], "-u") == 0){
        if (kill(pid_t, SIGUSR1) != 0){
            printf("Error: kill invalid on -u\n");
            exit(0);
        }
    } 
    // Handle the -i case, kill SIGINT
    else if (strcmp(argv[1], "-i") == 0) {
        if (kill(pid_t, SIGINT) != 0){
            printf("Error: kill invalid on -i\n");
            exit(0);
        }
    }
    return 0;
}