#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <wait.h>

#define MAX_STR_SIZE 200
#define KEY_NUM     9527
#define KEY_NUM2    9528
#define MEM_SIZE    1024


int main(int argc, char* argv[]) {

///////////////////////
  if(argc != 3) {
      exit(1);
  }


  if(strcmp(argv[1], "-n")) {
      exit(1);
  }

//checking argc, argv 


  int rc;
  int status;


  int   shm_id;
  double *shm_price;
  
  int shm_set_id;
  double *shm_set_price;

  shm_id = shmget( (key_t)KEY_NUM, MEM_SIZE, IPC_CREAT|0666);
  shm_set_id = shmget( (key_t)KEY_NUM2, sizeof(double) * 5, IPC_CREAT|0666);

  shm_price = (double*) shmat( shm_id, ( void *)0, 0);
  //save to b1

  shm_set_price = (double*) shmat( shm_set_id, ( void *)0, 0);

  shm_set_price[0] = 0; //counter
  shm_set_price[1] = 0; //avertagex
  shm_set_price[2] = 0; //averagey
  shm_set_price[3] = 0; //mutex
  shm_set_price[4] = 0; //lineNum

  FILE* fp;

  double tmp = 0;
  double averageX = 0;
  double averageY = 0;
  double betaTmp = 0;
  double beta1 = 0;
  double beta0 = 0;
  int lineNum = 0;
  double sumOfX = 0;
  double sumOfY = 0;
  char line[MAX_STR_SIZE];
  pid_t pid;
  pid_t wpid;
  double* x;
  double* y;

  int argvi = atoi(argv[2]);

  for(int i = 0; i < argvi; i++) {
    pid = fork();
  
    switch(pid){
      case -1:
             break;
      case 0:


     
        fp = fopen("input.txt", "r");

        for(int k = -1; k < i; k++) {
         fgets(line, MAX_STR_SIZE, fp);
        }
        while(!feof(fp)){
          tmp = atof(strtok(line, " "));
          sumOfX += tmp;
          tmp = atof(strtok(NULL, " "));
          sumOfY += tmp;
          lineNum++;

          for(int g = 0; g < argvi; g++) {
            fgets(line, MAX_STR_SIZE, fp);
          }
        }
//save values to sumOfX and sumOfY and lineNum
//total line  devided by process number
//example, process1's line is 20000000 and process`s line is 20000000


       fclose(fp);

        x = (double*) malloc(sizeof(double) * lineNum);
        y = (double*) malloc(sizeof(double) * lineNum);

        fp = fopen("input.txt", "r");

        int xyi = 0;


// save value to x[i] and y[i]
        for(int h = -1; h < i; h++) {
          fgets(line, MAX_STR_SIZE, fp);
        }
        while (!feof(fp)){
          x[xyi] = atof(strtok(line, " "));
          y[xyi] = atof(strtok(NULL, " "));
          xyi++;

          for(int q = 0; q < argvi ; q++) {
            fgets(line, MAX_STR_SIZE, fp);
          }
        }

        fclose(fp);
////////////////



 
        //self mutex     
        //IPC lines
        //merge value of process into shared memory
        while(shm_set_price[3] == 1) {};
        shm_set_price[3] = 1;
 
        shm_set_price[1] += sumOfX;
        shm_set_price[2] += sumOfY;
        shm_set_price[4] += lineNum;
        shm_set_price[3] = 0;

        if(shm_set_price[0] == (argvi-1)) {
            shm_set_price[1] = shm_set_price[1] / shm_set_price[4];
            shm_set_price[2] = shm_set_price[2] / shm_set_price[4];
        };
 
        shm_set_price[0] += 1;

        while(shm_set_price[0] != argvi){};
        ////////////////////////////

        for(int u = 0; u < lineNum; u += 1) {
          beta1 += (x[u] - shm_set_price[1]) * (y[u] - shm_set_price[2]);
          betaTmp += (x[u] -  shm_set_price[1]) * (x[u] - shm_set_price[1]);
        }

 
        
        //merge value of process into shared memory
        shm_price[2*i] = 0;
        shm_price[2*i+1] = 0;

        shm_price[2*i] += beta1;
        shm_price[2*i+1] += betaTmp;
             //////////////////////////
        exit(EXIT_SUCCESS);

      default:
         break;
    }
  }
 
    while ((wpid = wait(&status)) > 0){}

    for(int b = 0; b < argvi; b++) {
        beta1 += shm_price[2*b];
        betaTmp += shm_price[2*b+1];
    }

    beta1 = beta1 / betaTmp;
    beta0 = shm_set_price[2] - (beta1 * shm_set_price[1]);
    
 

    printf("Y = %.5lf + %.5lfX\n", beta0, beta1);
 


}

