////////////////////////////////////////////////////////////////////////////////
//// Main File:        cache2Dclash.c
//// This File:        cache2Dclash.c
//// Other Files:      cache1D.c, cache2Drows.c, cache2Dcols.c
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

#define row 128
#define col 8
static int arr2D[row][col];

int main(int argc, char *argv[]){
	for(int i = 0; i < 100; i ++){
		for(int j = 0; j < row; j += 64){
			for(int k = 0; k < col; k++){
				arr2D[j][k] = i + j + k;
			}
		}
	}
}
