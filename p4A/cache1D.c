////////////////////////////////////////////////////////////////////////////////
//// Main File:        cache1D.c
//// This File:        cache1D.c
//// Other Files:      cache2Drows.c, cache2Dcols.c, cache2Dclash.c
//// Semester:         CS 354 Lecture 01 Fall 2022
//// Instructor:       deppeler
//// 
//// Author:           Kayley Seow
//// Email:            kseow@wisc.edu
//// CS Login:         kayley
////
///////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
////                   fully acknowledge and credit all sources of help,
////                   other than Instructors and TAs.
////
//// Persons:          N/A
////
//// Online sources:   N/A
////////////////////////////// 80 columns wide ///////////////////////////////////

#define GLOBAL_N 100000
static int arr[GLOBAL_N];

int main(int argc, const char *argv[]){
    for (int i = 0; i < GLOBAL_N; i++){
        arr[i] = i;
    }
    return 0;
}