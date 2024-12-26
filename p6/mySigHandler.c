////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        mySigHandler.c
// Other Files:      sendsig.c division.c
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

// Define the delay for the alarm() function
#define DELAY 4

// Global variable, both main() and alarm handler need to use it
int userCount = 0;

/* A handler for SIGALRM; it checks and prints current time and pid. This 
 * function must also re-arm or restart a new alarm to go off again 4 seconds 
 * later, and then return back to the main function, which continues its 
 * infinite loop.  Finally, the main function must provide some instruction to 
 * user before starting the infinite loop.
 *
 * SIGALRM is called with the alarm function in main().
 */
void handler_SIGALRM(){
    time_t currentTime;

    // Cast -1 to time_t, and check if the time is an error value
    time(&currentTime);
    if (time(&currentTime) == (time_t)-1){
        printf("Error: time() call returns an error.\n");
        exit(1);
    }

    // Checks to see if the current time is valid
    char *timePrint = ctime(&currentTime);
    if (timePrint == NULL){
        printf("Error: String after ctime() is NULL.\n");
        exit(1);
    }

    // Sets the pid
    int pid_t = getpid();

    // Prints the pid
    printf("PID: %d CURRENT TIME: %s", pid_t, timePrint);

    // Re-arm or restart a new alarm to go off again 4 seconds later
    alarm(DELAY);
    return;
}

/* A handler for SIGUSR1; it prints confirmation of SIGUSR1 being handled, and
 * increments the user count.
 *
 * SIGUSR1 is called with a kill() function to send user signals and interrupts.
 */
void handler_SIGUSR1(){
    printf("SIGUSR1 handled and counted!\n");
    userCount = userCount + 1;
    return;
}

/* A handler for SIGINT; it prints confirmation of SIGINT, and prints out how 
 * many times the SIGUSR1 was handled and exits the program.
 *
 * SIGINT is called with the keyboard input Ctrl+c.
 */
void handler_SIGINT(){
    printf("\nSIGINT handled.\n");
    printf("SIGUSR1 was handled %d times. Exiting now.\n", userCount);
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
    printf("Pid and time print every 4 seconds.\n");
    printf("Enter Ctrl-C to end the program.\n");

    // Sets up an alarm that will go off 4 seconds later, re-arm or restart a 
    // new alarm to go off again 4 seconds later
    alarm(DELAY);

    // A sigaction struct for the SIGALRM handler
    struct sigaction SigAlrmAction;
    memset(&SigAlrmAction, 0, sizeof(SigAlrmAction));
    SigAlrmAction.sa_handler = handler_SIGALRM;
    if (sigaction(SIGALRM, &SigAlrmAction, NULL) != 0){
        printf("Error: binding SIGALRM Handler.\n");
        exit(1);
    }

    // A sigaction struct for the SIGUSR1 handler
    struct sigaction SigUsrAction;
    memset(&SigUsrAction, 0, sizeof(SigUsrAction));
    SigUsrAction.sa_handler = handler_SIGUSR1;
    if (sigaction(SIGUSR1, &SigUsrAction, NULL) != 0){
        printf("Error: binding SIGUSR1 Handler.\n");
        exit(1);
    }

    // A sigaction struct for the SIGINT handler
    struct sigaction KeyInterruptAction;
    memset(&KeyInterruptAction, 0, sizeof(KeyInterruptAction));
    KeyInterruptAction.sa_handler = handler_SIGINT;
    if (sigaction(SIGINT, &KeyInterruptAction, NULL) != 0){
        printf("Error: binding SIGINT Handler.\n");
        exit(0);
    }

    // While loop to continously run
    while (1){
    }

    return 0;
}