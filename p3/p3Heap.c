///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2022 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission Fall 2022, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//// Main File:        p3Heap.c
//// This File:        p3Heap.c
//// Other Files:      N/A
//// Semester:         CS 354 Lecture 01? Fall 2022
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
//// Persons:          Identify persons by name, relationship to you, and email.
////                   Describe in detail the the ideas and help they provided.
////
//// Online sources:   avoid web searches to solve your problems, but if you do
////                   search, be sure to include Web URLs and description of 
////                   of any information you find.
////////////////////////////// 80 columns wide ///////////////////////////////////

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "p3Heap.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           

    int size_status;

    /*
     * Size of the block is always a multiple of 8.
     * Size is stored in all block headers and in free block footers.
     *
     * Status is stored only in headers using the two least significant bits.
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit 
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     * 
     * End Mark: 
     *  The end of the available memory is indicated using a size_status of 1.
     * 
     * Examples:
     * 
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     * 
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heap_start = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int alloc_size;

/*
 * Additional global variables may be added as needed below
 */
const int HDRSize = 4;
 
/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 * - If a BEST-FIT block found is NOT found, return NULL
 *   Return NULL unable to find and allocate block for desired size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the 
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* balloc(int size) { 
    int paddedSize;
    int smallestDifference = -1;
    blockHeader* currentHeader = heap_start;
    blockHeader* bestFit = NULL;

    // Check Size
    if (size <= 0 || size > alloc_size){
        return NULL;
    }

    // Padding
    if ((size+HDRSize) % 8 != 0){ // Not a multiple of 8
        int withoutpad = size + HDRSize;
        paddedSize = withoutpad + (8-(withoutpad%8));
    } else {
        paddedSize = size + HDRSize;
    }

    // Update the size_status to the amount of space you will need
    // Check to see if padded size is larger than allocated size
    if (paddedSize > alloc_size){
        return NULL;
    }

    // Iterate through the heap, and immediately allocates best-exact fit on first contact
    while (currentHeader -> size_status != 1){
        int currentBlockSize = currentHeader -> size_status; // with the 4 of the header
        // Mask to compare, and then Subtract P and A bits
        currentBlockSize = (currentBlockSize & ~3); // header, size, and padding

        // Exact fit
        if ((((currentHeader -> size_status) & 1) == 0) && currentBlockSize == paddedSize){
            // Allocate the current a-bit
            currentHeader -> size_status += 1;
            // Set p-bit
            blockHeader* nextHeader = (blockHeader*) ((void*)currentHeader + currentBlockSize);
            //check mask on the next header, end block
            if ((nextHeader -> size_status != 1)){
            //if ((nextHeader -> size_status != 1) && ((nextHeader -> size_status & 2))){
                nextHeader -> size_status += 2;
            }
            // return a pointer to the fourth bit after the size, data payload
            return (void*)(currentHeader) + 4;
        }
            
        // Check for the (second) best fit
        if (((currentHeader -> size_status & 1) == 0) && (currentBlockSize > paddedSize)){
            // First iteration, where the best fit is still undefined
            if (bestFit == NULL){
                bestFit = currentHeader;
            }
            if (smallestDifference == -1){
                smallestDifference = currentBlockSize - paddedSize;
            }
            else { // fix this so that you keep track of smallest size properly
                int smallDiffTemp = currentBlockSize-paddedSize;
                if (smallDiffTemp < smallestDifference){
                    // Set the smallest difference to the current smallest difference
                    smallestDifference = smallDiffTemp;

                    bestFit = currentHeader;
                }
            }
        }

        // Set the header to the next header
        currentHeader = (blockHeader*) ((void*)currentHeader + currentBlockSize);
    }

    // second best fit + split
    if (bestFit != NULL){
        int bestFitBlockSize = bestFit -> size_status;

        // Save the pbit, either 0 or 2
        int pbit = (bestFitBlockSize & 2);

        // if you want to unmask a block just | 2

        // Change the size of the header
        bestFit -> size_status = paddedSize + 1 + pbit;
        // make sure, assume than the a bit is 1

        // Create the next header
        blockHeader* freeHeader = ((void*)bestFit + paddedSize);
        freeHeader -> size_status  = ((bestFitBlockSize & ~3) - paddedSize);
        freeHeader -> size_status += 2;
        return (void*)(bestFit) + 4;
    }
    return NULL;
}  
 
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */                    
int bfree(void *ptr) {
    int ptrAsInt = (int)(ptr - 4);
    // Check if the pointer is NULL    
    if (ptr == NULL){
        return -1;
    }
    // Check if the pointer is not a multiple of 8
    int ptrCalculate = ptrAsInt - (int)heap_start;
    if (ptrCalculate % 8 != 0){
        return -1;
    }
    // Check if ptr is outside of the heap space
    if (((ptrAsInt < (int)heap_start)) && ((int)ptr > ((int)heap_start + alloc_size))){
        return -1;
    }
    // Check if ptr block is already freed (check the a bit)
    blockHeader* currentBlock = (blockHeader*)(ptr) - 1;
    if (((currentBlock -> size_status) & 1) == 0) {
        return -1;
    }

    // On Success, run 0
    int size = ((currentBlock -> size_status) & ~3);

    currentBlock -> size_status -= 1;
        
    blockHeader* nextHeader = currentBlock + (size / sizeof(blockHeader));
    nextHeader -> size_status -= 2;

     return 0;
} 

/*
 * Function for traversing heap block list and coalescing all adjacent 
 * free blocks.
 *
 * This function is used for delayed coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce() {
    int returnCoalesce = 0;
    blockHeader* currentHeader = heap_start;
    blockHeader* nextHeader = (blockHeader*) ((void*)currentHeader + currentHeader -> size_status);

    while(nextHeader -> size_status != 1){
        int currentBlockSize = ((currentHeader -> size_status) & ~3);
        int aBit = (currentHeader -> size_status & 1);
        // Check to see if the current block is allocated
        if (aBit == 1){
            // Increment to the next block
            currentHeader = (blockHeader*) ((void*)currentHeader + currentBlockSize);
            continue;
        }
        // If the current block is not allocated, we can coalesce
        else { // aBit == 0, which means that it is unallocated
            nextHeader = (currentHeader + currentBlockSize/4);
            int nextABit = ((nextHeader -> size_status) & 1);
            // Check to see if the next block is allocated
            if (nextABit == 1){ 
                // Change current to be in place of the next header
                currentHeader = (currentHeader + currentBlockSize/4);
                int nextBlockSize = nextHeader -> size_status;
                // Change next to be in place of the next next header
                nextHeader = (nextHeader + nextBlockSize/4);
                continue;
            }
            else {
                int nextBlockSize = nextHeader -> size_status;
                int nextSize = (nextBlockSize & ~3);
                // Add the size of the next block to the current block
                currentHeader -> size_status += nextSize;
                nextHeader = (nextHeader + nextBlockSize/4);
                returnCoalesce = 1;
            }
        }
    }
    return returnCoalesce;
}

/* 
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int init_heap(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple myInit calls
 
    int pagesize;   // page size
    int padsize;    // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* end_mark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    alloc_size -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heap_start = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    end_mark = (blockHeader*)((void*)heap_start + alloc_size);
    end_mark->size_status = 1;

    // Set size in header
    heap_start->size_status = alloc_size;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heap_start->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heap_start + alloc_size - 4);
    footer->size_status = alloc_size;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void disp_heap() {     
 
    int counter;
    char status[6];
    char p_status[6];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heap_start;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, 
	"*********************************** Block List **********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
    fprintf(stdout, 
	"*********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout, 
	"*********************************************************************************\n");
    fflush(stdout);

    return;  
} 


// end of myHeap.c (Spring 2022)                                         


