#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <pthread.h>

#define PUZZLE_DIM 9
#define NUM_THREADS 11

//Global Variables

//2D array to hold a sudoku puzzle
int sudoku_solution [PUZZLE_DIM][PUZZLE_DIM] = {0};
//Array to be used by threads to ensure numbers 1 through 9 are present
int check_nums[PUZZLE_DIM] = {0};
//Array to store the result of each thread
int thread_results [NUM_THREADS] = {0};
char * filename;

typedef struct{
  int row;
  int column;
  int threadID;
} parameters;

//Function Declarations
void readFile();
void printPuzzle();
void * validateRows(void * param);
void * validateColumns(void * param);
void * validateSubSquare(void * param);
void refreshCheckNums();

int main(int argc, char * argv[]){
  filename = argv[1];
  readFile();
  //TODO: Remove! For Error Checking only!
  printPuzzle();

  //Create data pointers
  parameters *data0 = (parameters *) malloc(sizeof(parameters));
  data0->row = 0;
  data0->column = 0;
  data0->threadID = 0;
  //Initialize threads
  pthread_t threads[NUM_THREADS];
  pthread_create(&threads[0], NULL, validateRows, data0);

  //Wait for threads to finish
  //TODO: Loop over all threads
  pthread_join(threads[0], NULL);

  //Check result array to ensure all all threads returned success
  //TODO: Change to for loop once all threads are implemented
  if(thread_results[0] == 1){
    printf("All Rows Valid\n");
  }
  if(thread_results[1] == 1){
    printf("All Columns Valid\n");
  }	

}

void readFile(){
  int i, j;
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
  int i, j;
  for(i = 0; i<PUZZLE_DIM; i++){
    for(j = 0; j< PUZZLE_DIM; j++){
      printf("%d ", sudoku_solution[i][j]);
    }	
    printf("\n");
  }
}
void * validateRows(void * param){
  parameters * rowData = (parameters *) param;
  int i, j, k, value;
  int numValidRows = 0;
  for(i = rowData->row; i < PUZZLE_DIM; i++){
    int nums = 0;
    refreshCheckNums();
    for(j = rowData->column; j < PUZZLE_DIM; j++){
      value = sudoku_solution[i][j];
      if(value >= 1 || value <=9){
        check_nums[value-1] = 1;
      }
    }
    for(k = 0; k < PUZZLE_DIM; k++){
      if(check_nums[k] == 1){
        nums++;
      }
    }
    if(nums == PUZZLE_DIM){
      numValidRows++;
    }
  }
  if(numValidRows == PUZZLE_DIM){
    thread_results[rowData->threadID] = 1;
  }
}
void * validateColumns(void * param){
  refreshCheckNums();

}
void * validateSubSquare(void * param){
  refreshCheckNums();

}
void refreshCheckNums(){
  int a;
  for(a=0; a<PUZZLE_DIM;a++){
    check_nums[a] = 0;
  }
}
