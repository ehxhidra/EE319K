
// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 1/2/2023
// Student names: HyukJoo Chung and Yunjae Jo
// Last modification date: 4/2/2023
#include <stdint.h>

// Declare state variables for FiFo
//        size, buffer, put and get indexes
#define FIFO_SIZE 9
static uint8_t PutI, GetI;
static char Fifo[FIFO_SIZE];


//  FiFo_Init
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void Fifo_Init() {
//Complete this
  PutI = 0; //enqueue
    GetI = 0; //dequeue
}

//  FiFo_Put
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t Fifo_Put(char data){
  //Complete this routine
  if(((PutI+1)%FIFO_SIZE) == GetI) {
        return 0; // fifo full
    }
  Fifo[PutI] = data;         // save in Fifo
  PutI = (PutI+1)%FIFO_SIZE; // next place to put
  return 1;

}

// * Fifo_Get
// Gets an element from the FIFO
// Input: none
// Output: If the FIFO is empty return 0
//         If the FIFO has data, remove it, and return it
uint32_t Fifo_Get(void){
  //Complete this routine
    if(GetI == PutI) {
        return 0; // fail if empty
    }
  char temp = Fifo[GetI];
  GetI = (GetI+1)%FIFO_SIZE; // next place to get
  return temp;

}