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
parameters * threadParameters(int row, int column, int id);
void * validateRows(void * param);
void * validateColumns(void * param);
void * validateSubSquare(void * param);
void refreshCheckNums();

int main(int argc, char * argv[]){
  int b ;
  int successfulThreads = 0;
  filename = argv[1];
  readFile();
  //TODO: Remove! For Error Checking only!
  //printPuzzle();

  //Initialize threads
  pthread_t threads[NUM_THREADS];
  pthread_create(&threads[0], NULL, validateRows, threadParameters(0,0,0));
  pthread_create(&threads[1], NULL, validateColumns, threadParameters(0,0,1));
  pthread_create(&threads[2], NULL, validateSubSquare, threadParameters(0,0,2));
  pthread_create(&threads[3], NULL, validateSubSquare, threadParameters(0,3,3));
  pthread_create(&threads[4], NULL, validateSubSquare, threadParameters(0,6,4));
  pthread_create(&threads[5], NULL, validateSubSquare, threadParameters(3,0,5));
  pthread_create(&threads[6], NULL, validateSubSquare, threadParameters(3,3,6));
  pthread_create(&threads[7], NULL, validateSubSquare, threadParameters(3,6,7));
  pthread_create(&threads[8], NULL, validateSubSquare, threadParameters(6,0,8));
  pthread_create(&threads[9], NULL, validateSubSquare, threadParameters(6,3,9));
  pthread_create(&threads[10], NULL, validateSubSquare, threadParameters(6,6,10));

  //Wait for threads to finish
  for(b = 0; b < NUM_THREADS; b++){
    pthread_join(threads[b], NULL);
  }


  //Check result array to ensure all all threads returned success
  for(b = 0; b < NUM_THREADS; b++){
    if(thread_results[b] == 1){
      successfulThreads++;
    }
  }  
  if(successfulThreads == NUM_THREADS)
    printf("Valid solution to a sudoku puzzle\n");
  else
    printf("Not a valid solution to a sudoku puzzle\n");
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
parameters * threadParameters(int row, int column, int id){
  parameters *data = (parameters *) malloc(sizeof(parameters));
  data->row = row;
  data->column = column;
  data->threadID = id;
  return data;
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
        check_nums[value-1]++;
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
  parameters * columnData = (parameters *) param;
  int i, j, k, value;
  int numValidColumns = 0;
  for(i = columnData->column; i < PUZZLE_DIM; i++){
    int nums = 0;
    refreshCheckNums();
    for(j = columnData->row; j < PUZZLE_DIM; j++){
      value = sudoku_solution[j][i];
      if(value >= 1 || value <=9){
        check_nums[value-1]++;
      }
    }
    for(k = 0; k < PUZZLE_DIM; k++){
      if(check_nums[k] == 1){
        nums++;
      }
    }
    if(nums == PUZZLE_DIM){
      numValidColumns++;
    }
  }
  if(numValidColumns == PUZZLE_DIM){
    thread_results[columnData->threadID] = 1;
  }

}
void * validateSubSquare(void * param){
  refreshCheckNums();
  parameters * subSquareData = (parameters *) param;
  int i, j, k, value;
  int nums = 0;
  for(i = subSquareData->row; i < subSquareData->row + 3; i++){
    for(j = subSquareData->column; j < subSquareData->column + 3; j++){
       value = sudoku_solution[i][j];
       if(value >= 1 || value <=9){
        check_nums[value-1]++;
       }
    } 
  }
  for(k = 0; k < PUZZLE_DIM; k++){
    if(check_nums[k] == 1){
        nums++;
    }
  }
  if(nums == PUZZLE_DIM){
    thread_results[subSquareData->threadID] = 1;
  }
}
void refreshCheckNums(){
  int a;
  for(a=0; a<PUZZLE_DIM;a++){
    check_nums[a] = 0;
  }
}
