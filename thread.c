#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>


#define MAX_STR_SIZE 1000


void *thread_cb(void *);

//int rc;
//int status;

//int argvi = atoi(argv[2]);
//pthread_t threads[argvi];
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


  if(argc != 3) {
      exit(1);
  }


if(strcmp(argv[1],"-n" )) {
 exit(1);
}


int rc;
int status;

printf("%s %s %s \n", argv[0], argv[1], argv[2]);

int argvi = atoi(argv[2]);
//threads = malloc()
pthread_t threads[argvi];  //(pthread_t*) malloc( );

//printf("type = %s", typeof(argvi));
printf("price = %d\n", argvi);


FILE *fp = fopen("input.txt", "r");


    fgets(line, MAX_STR_SIZE, fp);
while(!feof(fp)){
        tmp = atof(strtok(line, " "));
        sumOfX += tmp;
        tmp = atof(strtok(NULL, " "));
        sumOfY += tmp;
        lineNum++;
    
    fgets(line, MAX_STR_SIZE, fp);
}

printf("%d\n", lineNum);


fclose(fp);

fp = fopen("input.txt", "r");
x = (double*)malloc((lineNum * sizeof(double)));
y = (double*)malloc(lineNum * sizeof(double));

int xyi = 0;


    fgets(line, MAX_STR_SIZE, fp);
while (!feof(fp)){
        x[xyi] = atof(strtok(line, " "));
        y[xyi] = atof(strtok(line, " "));
        xyi++;
    
    fgets(line, MAX_STR_SIZE, fp);
}

fclose(fp);

averageX = sumOfX / lineNum;
averageY = sumOfY / lineNum;

printf("%lf %lf\n", averageX, averageY);

argvii = (int*)malloc(sizeof(int));
argvii[0] = argvi;
printf("argvii = %d\n", argvii[0]);

int passNum[argvi];

for(int gg = 0; gg < argvi; gg++) {
    passNum[gg] = gg;
}


for(int i1 = 0; i1 < argvi; i1++) {
    int error = pthread_create(&threads[i1], NULL, &thread_cb, (void *)&passNum[i1]);
    //printf("%d\n", error);
}

printf("create end \n");

for(int i2 = 0; i2 < argvi; i2++) {
    printf("join\n");
    pthread_join(threads[i2], NULL);
//pthread_create(&threads[i], NULL, &thread_cb, (void *)i);
}

free(x);
free(y);
beta1 = beta1 / betaTmp;
beta0 = averageY - (beta1 * averageX);
printf("beta1 = %lf beta2 = %lf\n", beta1, beta0);
free(argvii);
}

void *thread_cb(void *arg) {

    int number = *((int*)arg);
    //printf("number : %d\n", number);
    //printf("therad: %d, %d\n", (int)arg, getpid());
    double beta1TMP = 0;
    double beta1TMP2 = 0;
    

//    pthread_mutex_lock(&mutex);

    for(int i = number; i < lineNum; i += argvii[0]) { 
        /*if(i%1000000 == 1 || i% 1000000 == 0) {
            printf("%d\n", i);
        }*/

        beta1TMP += (x[i] - averageX) * (y[i] - averageY);
        beta1TMP2 += (x[i] - averageX) * (x[i] - averageX);
    }
    //pthread_mutex_lock(&mutex);

    pthread_mutex_lock(&mutex);

    beta1 += beta1TMP;
    betaTmp += beta1TMP2;
  
    pthread_mutex_unlock(&mutex);

   /*
    while (!done[(int)arg])
    {
       for (i=0; i < 1000000; i++)
       {
              result = result + (double)random()
       }
       printf("thread: %d, result = %e\n", (int)arg, result);
    }
    */
    pthread_exit((void *) 0);
}
