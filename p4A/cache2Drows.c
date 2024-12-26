////////////////////////////////////////////////////////////////////////////////
//// Main File:        cache2Drows.c
//// This File:        cache2Drows.c
//// Other Files:      cache1D.c, cache2Dcols.c, cache2Dclash.c
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

#define row 3000
#define col 500
static int arr2D[row][col];

int main(int argc, const char *argv[]){
    for (int i = 0; i < row; i ++){
        for (int j = 0; j < col; j ++){
            arr2D[i][j] = i + j;
        }
    }
    return 0;
}