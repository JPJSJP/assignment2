#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pwd.h>



#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif


/*

b1up = 0;
b1down = 0;


for(int i = 1; i <= n; i++) {

   b1up += (x[i] - avgX) * (y[i] - avgY);
   b1down += pow(x[i] - avgX)

}

b1 = b1up/b1down


b0 = avgY - b1*avgX


y = b0 + b1*x

*/




int main(){
    uid_t user_id;
    struct passwd *user_pw;

    time_t _time;
    struct tm *ptm;

    FILE *fp;


    /*
    pid_t pid;
    
    pid = fork();
    
    if (pid == -1){
        printf("cant fork, err\n");
        exit(0);
    }

    else if (pid == 0){
        printf("child process %d\n", pid);
    }
    
    else if (pid > 0) {

        printf("parent process %d\n", pid);
    }
    */

    
    while(1){

    char *token = NULL;
    
    user_id = getuid();
    user_pw = getpwuid(user_id);

    ptm = localtime(&_time);

    char strBuffer[_MAX_PATH];
    char input[100];
    getcwd(strBuffer, _MAX_PATH);
    printf("[%d:%d:%d]%s@%s$", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, user_pw->pw_name ,strBuffer);
    
    scanf("%[^\n]s", input);
    
    token = strtok(input, " ");

    printf("test : %sasdf\n", token);
   
    char data[3000];

    if (!strcmp(token,"ls")){
    
    
    DIR *dir = NULL;
    struct dirent *ent;
    dir = opendir("./");
    if (dir != NULL) {

        while ((ent = readdir(dir)) != NULL) {
            printf("%s\n", ent->d_name);

        }
    closedir(dir);
    } else {

        perror("");
        return EXIT_FAILURE;

    }
 

    }

    else if (!strcmp(token, "cd")){
    token = strtok(NULL, " ");
    printf("text2 : %sasdf\n", token);
    chdir(token);

    }

    else if (!strcmp(token, "echo")){
   
    token = strtok(NULL, " ");
    printf("%s", token);
   
    }   

    else if(!strcmp(token, "cat")){
    token = strtok(NULL, " ");
    

        fp = fopen(token, "r");
        while(!feof(fp)){
             fgets(data, sizeof(data), fp);
             printf("%s", data);
	}
     
        fclose(fp);

    }

    else if (!strcmp(token, "head")){

        token = strtok(NULL, " ");
        
        fp = fopen(token, "r");
        int countHead = 10;        

        for(int i = 0; i < countHead; i++) {
            fgets(data, sizeof(data), fp);
            printf("%s", data);

        }
        fclose(fp);


    }



    getchar();
   

}


  DIR *dir = NULL;
    struct dirent *ent;
    dir = opendir("./");
    if (dir != NULL) {

        while ((ent = readdir(dir)) != NULL) {
            printf("%s\n", ent->d_name);

        }
    closedir(dir);
    } else {

        perror("");
        return EXIT_FAILURE;

    }

}
