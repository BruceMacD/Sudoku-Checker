#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define PUZZLE_DIM 9
#define NUM_THREADS 11

//Global Variables
int i, j;
//2D array to hold a sudoku puzzle
int sudoku_solution [PUZZLE_DIM][PUZZLE_DIM] = {0};
//Array to be used by threads to ensure numbers 1 through 9 are present
int check_nums[PUZZLE_DIM] = {0};
//Array to store the result of each thread
int thread_results [NUM_THREADS] = {0};
char * filename;

//Function Declarations
void readFile();
void printPuzzle();
void * validateRows(void * param);
void * validateColumns(void * param);
void * validateSubSquare(void * param);

int main(int argc, char * argv[]){
  filename = argv[1];
  readFile();
  //For Error Checking
  printPuzzle();

  //Create data pointers

  //Initialize threads

  //Wait for threads to finish

  //Check result array to ensure all all threads returned success

}

void readFile(){
  FILE * file = fopen(filename, "r");
  if(file == NULL){
    fprintf(stderr, "ERROR: File Failed To Open!");
    exit(1);
  }
  for(i = 0; i<PUZZLE_DIM; i++){
    for(j = 0; j< PUZZLE_DIM; j++){
      fscanf(file, "%d", &sudoku_solution[i][j]);
    }
  }
  fclose(file);
  
}
void printPuzzle(){
  for(i = 0; i<PUZZLE_DIM; i++){
    for(j = 0; j< PUZZLE_DIM; j++){
      printf("%d ", sudoku_solution[i][j]);
    }	
    printf("\n");
  }
}
void * validateRows(void * param){

}
void * validateColumns(void * param){

}
void * validateSubSquare(void * param){

}

