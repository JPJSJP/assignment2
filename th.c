#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <wait.h>


#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif




int main(){
  uid_t user_id;
  struct passwd *user_pw;

  time_t _time;
  struct tm *ptm;

  FILE *fp;


  char *token = NULL;
  char *token2 = NULL;

    char data[3000];

while(1){

    user_id = getuid();
    user_pw = getpwuid(user_id);

    ptm = localtime(&_time);

    char strBuffer[_MAX_PATH];
    char input[100];
    getcwd(strBuffer, _MAX_PATH);
    printf("[%d:%d:%d]%s@%s$", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, user_pw->pw_name ,strBuffer);

    scanf("%[^\n]s", input);
    getchar();
    bool backgOn = false;

while(1){

    token = strtok(input, " ");

    printf("test : %sasdf\n", token);


    pid_t pid;

    pid = fork();

    switch(pid){

      case -1:
         
      printf("cant fork, err\n");
      break;

       case 0:
      
      printf("child process %d\n", pid);


      if (!strcmp(token,"ls")){

       token2 = strtok(NULL, " ");
       execl("/bin/ls", "/bin/ls", token2, NULL);
       }
      
      else if (!strcmp(token, "cd")){
      token2 = strtok(NULL, " ");
      chdir(token2);

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

    fp = fopen(token, " ");
    int countHead = 10;

    for(int i = 0; i < countHead; i++) {
    fgets(data, sizeof(data), fp);
    printf("%s", data);

    }
    fclose(fp);


    }

    exit(0);



  default:

    printf("parent process %d\n", pid);
    wait(NULL);
    printf("child process end\n");
    

  }
  

  if(!backgOn) {
   break;
  }

}

}
    


}

