#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>


#define MAX_STR_SIZE 1000


void *thread_cb(void *);

pthread_t* threads;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

double* x;
double* y;
double tmp = 0;
double averageX = 0;
double averageY = 0;
double betaTmp = 0;
double beta1 = 0;
double beta0 = 0; 
int lineNum = 0;
double sumOfX = 0;
double sumOfY = 0;

int* argvii;

int count = 0;
char line[MAX_STR_SIZE];


int main(int argc, char* argv[]) {

    //argc, argv check///
    if(argc != 3) {
        exit(1);
    }


    if(strcmp(argv[1],"-n" )) {
        exit(1);
    }
   //////////////////////////

    int rc;
    int status;

    int argvi = atoi(argv[2]);
 
    pthread_t threads[argvi];  

    FILE* fp = fopen("input.txt", "r");

    //Until we get the average, we do no tuse pthread
    fgets(line, MAX_STR_SIZE, fp);
    while(!feof(fp)){
        tmp = atof(strtok(line, " "));
        sumOfX += tmp;
        tmp = atof(strtok(NULL, " "));
        sumOfY += tmp;
        lineNum++;
    
        fgets(line, MAX_STR_SIZE, fp);
    }

    fclose(fp);

      //ex x[0...39999999] y[0...39999999]
    x = (double*)malloc((lineNum * sizeof(double)));
    y = (double*)malloc(lineNum * sizeof(double));

    averageX = sumOfX / lineNum;
    averageY = sumOfY / lineNum;

    argvii = (int*)malloc(sizeof(int));
    argvii[0] = argvi;

    //pthread arg
    int passNum[argvi];

    for(int gg = 0; gg < argvi; gg++) {
      passNum[gg] = gg;
    }/////////////
   ///////////////////////////


    //
    //pthread starts
    for(int i1 = 0; i1 < argvi; i1++) {
        int error = pthread_create(&threads[i1], NULL, &thread_cb, (void *)&passNum[i1]);
   
    }


    //pthread join
    for(int i2 = 0; i2 < argvi; i2++) {
        pthread_join(threads[i2], NULL);
 
    }




    free(x);
    free(y);
    beta1 = beta1 / betaTmp;
    beta0 = averageY - (beta1 * averageX);
    printf("Y = %.5lf + %.5lfX\n", beta0, beta1);
    free(argvii);
}

void *thread_cb(void *arg) {

    int number = *((int*)arg);
   
    double beta1TMP = 0;
    double beta1TMP2 = 0;

    int xyi = 0;

    FILE *ft = fopen("input.txt", "r");

    char linee[MAX_STR_SIZE];

    for(int k = -1; k < number; k++) {
        fgets(linee, MAX_STR_SIZE, ft);
        xyi++;
    }

    while (!feof(ft)){        //

    //Write a mutex for mutual exclusion
   
    pthread_mutex_lock(&mutex);
   
        x[xyi] = atof(strtok(linee, " "));
        y[xyi] = atof(strtok(NULL, " "));

    pthread_mutex_unlock(&mutex);
        for(int t = 0; t < argvii[0]; t++) {
            fgets(linee, MAX_STR_SIZE, ft);
            xyi++;
        }
    }
   

    fclose(ft);
 
    for(int i = number; i < lineNum; i += argvii[0]) { 
        

        beta1TMP += (x[i] - averageX) * (y[i] - averageY);
        beta1TMP2 += (x[i] - averageX) * (x[i] - averageX);
    }
    


    //Write a mutex for mutual exclusion

    pthread_mutex_lock(&mutex);

    beta1 += beta1TMP;
    betaTmp += beta1TMP2;
  
    pthread_mutex_unlock(&mutex);

    pthread_exit((void *) 0);
}
