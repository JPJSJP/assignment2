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

 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


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

typedef struct xypack{
double x;
double y;
int number;
}xypack;



int main(int argc, char* argv[]) {

int rc;
int status;

int argvi = atoi(argv[2]);
pthread_t threads[argvi];

FILE *fp = fopen("input.txt", "r");


while(!feof(fp)){
        fgets(line, MAX_STR_SIZE , fp);
        tmp = atof(strtok(line, " "));
        sumOfX += tmp;
        tmp = atof(strtok(NULL, " "));
        sumOfY += tmp;
        lineNum++;
}

fclose(fp);

xypack xy[lineNum];

fp = fopen("input.txt", "r");

int xyi = 0;

while (!feof(fp)){
        fgets(line, MAX_STR_SIZE , fp);
        xy[xyi].x = atof(strtok(line, " "));
        xy[xyi].y = atof(strtok(line, " "));
        xy[xyi].number = argvi;
        xyi++;
}

fclose(fp);

averageX = sumOfX / lineNum;
averageY = sumOfY / lineNum;

for(int i1 = 0; i1 < argvi; i1++) {
pthread_create(&threads[i1], NULL, &thread_cb, (void *)xy);
}

for(int i2 = 0; i2 < argvi; i2++) {
pthread_join(threads[i2], (void **)&status);
//pthread_create(&threads[i], NULL, &thread_cb, (void *)i);
}

beta1 = averageY - beta1 * averageX;
beta0 = beta1/betaTmp;
printf("%f %f", beta0, beta1);

}

void *thread_cb(void *arg) {
    double result=0.0;
    xypack* xy;
    xy = ((xypack*)arg); 
    //printf("therad: %d, %d\n", (int)arg, getpid());
    double beta1TMP = 0;
    double betaTMP2 = 0;
    for(int i = xy[0].number; i < lineNum; i += xy[0].number) {
        beta1TMP += (xy[i].x - averageX) * (xy[i].y - averageY);
        betaTMP2 += pow(xy[i].x - averageX, 2);
    }
    pthread_mutex_lock(&mutex);

    beta1 += beta1TMP;
    betaTmp += betaTMP2;
  
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
