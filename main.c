#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <pthread.h>

/* Macro Definitions */
#define PUZZLE_DIM 9
#define NUM_THREADS 11

/* Global Variables */

//Name of file provided by user as command line argument
char * filename;
//2D array to hold a sudoku puzzle
int sudoku_solution [PUZZLE_DIM][PUZZLE_DIM] = {0};
//Array to be used by threads to ensure numbers 1 through 9 are present
int check_nums[PUZZLE_DIM] = {0};
//Array to store the result of each thread
int thread_results [NUM_THREADS] = {0};

/* Structure to hold the data required by each thread */
typedef struct{
  int row;
  int column;
  int threadID;
} parameters;

/* Function Declarations */
void readFile();
void printPuzzle();
parameters * threadParameters(int row, int column, int id);
void * validateRows(void * param);
void * validateColumns(void * param);
void * validateSubSquare(void * param);
void refreshCheckNums();

int main(int argc, char * argv[]){
  int b ;
  //Number of threads that found their area of the puzzle valid
  int successfulThreads = 0;
  //Obtain file name from user
  filename = argv[1];
  //Read contents of file to place in 2D array
  readFile();

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
  pthread_create(&threads[10],NULL, validateSubSquare, threadParameters(6,6,10));

  //Wait for all threads to finish
  for(b = 0; b < NUM_THREADS; b++){
    pthread_join(threads[b], NULL);
  }

  //Check result array to ensure all all threads found their area of the puzzle to be valid
  for(b = 0; b < NUM_THREADS; b++){
    if(thread_results[b] == 1){
      successfulThreads++;
    }
  } 
  //Inform user if their solution to a sudoku puzzle in valid by printing a message 
  if(successfulThreads == NUM_THREADS)
    printf("Valid solution to a sudoku puzzle\n");
  else
    printf("Not a valid solution to a sudoku puzzle\n");
}

/* Function to read a file from user containing a possible solution to a sudoku puzzle */
void readFile(){
  int i, j;
  FILE * file = fopen(filename, "r");
  //Print error message if file failed to open
  if(file == NULL){
    fprintf(stderr, "ERROR: File Failed To Open!\n");
    exit(1);
  }
  //Store the puzzle values in a 2D array
  for(i = 0; i<PUZZLE_DIM; i++){
    for(j = 0; j< PUZZLE_DIM; j++){
      fscanf(file, "%d", &sudoku_solution[i][j]);
    }
  }
  //Close the file
  fclose(file);
  
}
/* Function used to print sudoku puzzle */
void printPuzzle(){
  int i, j;
  for(i = 0; i<PUZZLE_DIM; i++){
    for(j = 0; j< PUZZLE_DIM; j++){
      printf("%d ", sudoku_solution[i][j]);
    }	
    printf("\n");
  }
}
/* Function to create data pointer for a worker thread */
parameters * threadParameters(int row, int column, int id){
  parameters *data = (parameters *) malloc(sizeof(parameters));
  //assign a row, column and id to the worker thread
  data->row = row;
  data->column = column;
  data->threadID = id;
  //return the data pointer
  return data;
}

/* Function to validate all of the rows of a sudoku puzzle */
void * validateRows(void * param){
  parameters * rowData = (parameters *) param;
  int i, j, k, value;
  int numValidRows = 0;
  //Loop over all rows
  for(i = rowData->row; i < PUZZLE_DIM; i++){
    int nums = 0;
    //Refresh the temporary array used to check if a row contains all values between 1-9
    refreshCheckNums();
    //Loop over all columns
    for(j = rowData->column; j < PUZZLE_DIM; j++){
      value = sudoku_solution[i][j];
      if(value >= 1 || value <=9){
        check_nums[value-1]++;
      }
    }
    //Loop to check all values 1-9 are present
    for(k = 0; k < PUZZLE_DIM; k++){
      if(check_nums[k] == 1){
        nums++;
      }
    }
    //If a row contains all numbers between 1-9 increase the number of valid rows
    if(nums == PUZZLE_DIM){
      numValidRows++;
    }
  }
  //If all rows contain numbers between 1-9 update the status of the thread
  if(numValidRows == PUZZLE_DIM){
    thread_results[rowData->threadID] = 1;
  }
}
/* Function to validate all of the columns of a sudoku puzzle */
void * validateColumns(void * param){
  parameters * columnData = (parameters *) param;
  int i, j, k, value;
  int numValidColumns = 0;
  //Loop over all columns
  for(i = columnData->column; i < PUZZLE_DIM; i++){
    int nums = 0;
    //Refresh the temporary array
    refreshCheckNums();
    //Loop over all rows
    for(j = columnData->row; j < PUZZLE_DIM; j++){
      value = sudoku_solution[j][i];
      if(value >= 1 || value <=9){
        check_nums[value-1]++;
      }
    }
    //Check the column has all values between 1-9
    for(k = 0; k < PUZZLE_DIM; k++){
      if(check_nums[k] == 1){
        nums++;
      }
    }
    //Increment the number of valid columns if column is valid
    if(nums == PUZZLE_DIM){
      numValidColumns++;
    }
  }
  //If all columns contain numbers between 1-9 update the status of the thread
  if(numValidColumns == PUZZLE_DIM){
    thread_results[columnData->threadID] = 1;
  }

}
/* Function to validate a single 3x3 subsquare of a sudoku puzzle */
void * validateSubSquare(void * param){
  //Refresh the temporary array
  refreshCheckNums();
  parameters * subSquareData = (parameters *) param;
  int i, j, k, value;
  int nums = 0;
  //Loop over rows of subsquare
  for(i = subSquareData->row; i < subSquareData->row + 3; i++){
    //Loop over columns of subsquare
    for(j = subSquareData->column; j < subSquareData->column + 3; j++){
       value = sudoku_solution[i][j];
       if(value >= 1 || value <=9){
        check_nums[value-1]++;
       }
    } 
  }
  //Ensure the subsquare contains all numbers between 1-9
  for(k = 0; k < PUZZLE_DIM; k++){
    if(check_nums[k] == 1){
        nums++;
    }
  }
  //If the subsquare is valid update the status of the worker thread
  if(nums == PUZZLE_DIM){
    thread_results[subSquareData->threadID] = 1;
  }
}
/* Refresh the temporary array used to ensure all values between 1-9 are present */
void refreshCheckNums(){
  int a;
  for(a=0; a<PUZZLE_DIM;a++){
    check_nums[a] = 0;
  }
}
