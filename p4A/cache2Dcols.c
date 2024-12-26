////////////////////////////////////////////////////////////////////////////////
//// Main File:        cache2Dcols.c
//// This File:        cache2Dcols.c
//// Other Files:      cache1D.c, cache2Drows.c, cache2Dclash.c
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

int main(int argc, char *argv[]){
	for(int i = 0; i < col; i++){
		for(int j = 0; j < row; j++){
			arr2D[j][i] = i + j;
		}
	}
}
