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


#define MAX_STR_SIZE 1000
#define KEY_NUM     9527
#define MEM_SIZE    1024


int main(int argc, char* argv[]) {
  if(argc != 3) {
      exit(1);
  }


  if(strcmp(argv[1], "-n")) {
      exit(1);
  }

  int rc;
  int status;


  int   shm_id;
  double *shm_price;
  int   count;
  shm_id = shmget( (key_t)KEY_NUM, MEM_SIZE, IPC_CREAT|0666);

  shm_price = (double*) shmat( shm_id, ( void *)0, 0);
  

  shm_price[0] = 0;
  shm_price[1] = 0;

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
  printf("%s %s %s \n", argv[0], argv[1], argv[2]);

  int argvi = atoi(argv[2]);
  printf("price = %d\n", argvi);

  printf("parent pid = %d\n", getpid());
  for(int i = 0; i < argvi; i++) {
    pid = fork();
  
    switch(pid){
      case -1:
        printf("error\n");
        break;
      case 0:
        printf("in child process (pid = %d)\n", getpid());
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

        fclose(fp);

        x = (double*) malloc(sizeof(double) * lineNum);
        y = (double*) malloc(sizeof(double) * lineNum);

        fp = fopen("input.txt", "r");

        int xyi = 0;

        for(int h = -1; h < i; h++) {
          fgets(line, MAX_STR_SIZE, fp);
        }
        while (!feof(fp)){
          //if(xyi > 19999990)printf("%d\n",xyi);
          x[xyi] = atof(strtok(line, " "));
          y[xyi] = atof(strtok(line, " "));
          xyi++;
          for(int q = 0; q < argvi ; q++) {
            fgets(line, MAX_STR_SIZE, fp);
          }
        }

        fclose(fp);


        averageX = sumOfX / lineNum;
        averageY = sumOfY / lineNum;


        for(int u = i; u < lineNum; u += argvi) {
          beta1 += (x[i] - averageX) * (y[i] - averageY);
          betaTmp += (x[i] - averageX) * (x[i] - averageX);
        }


        beta1 = beta1 / betaTmp;
        beta0 = averageY - beta1 * averageX;

        //printf("%lf\n", beta0);
        //printf("%lf\n", beta1);
        

        shm_price[2*i] += beta0;
        shm_price[2*i+1] += beta1;

        //printf("%f %f\n", beta0, beta1);
        printf("hey!\n");
        exit(EXIT_SUCCESS);

      default:
        printf("parents\n");
        //wait(NULL);
        break;
    }
  }
 
    while ((wpid = wait(&status)) > 0)
    {
        printf("Exit status of %d was %d (%s)\n", (int)wpid, status,
               (status > 0) ? "accept" : "reject");
    }


    for(int b = 0; b < argvi; b++) {
        beta0 = shm_price[2*b];
        beta1 = shm_price[2*b+1];
    } 

    printf("Y = %lf + %lfX\n", beta0, beta1);
 


}

