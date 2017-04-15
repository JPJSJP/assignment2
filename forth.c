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
#include <fcntl.h>
#include <ctype.h>
#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif


void main(){
    uid_t user_id;
    struct passwd *user_pw;
    time_t _time;
    struct tm *ptm;
    
    FILE *fp;

    char *token = NULL;
    char *token2 = NULL;


    char data[300];

    while(1){

        int andFlag = 0;
        int redirect = 0;
        int backG = 0;
        char *filename;

        char *args[128];
        //args[0] = '\0';
        char **next = args;
	// input is -1, output is 1
        user_id = getuid();
        user_pw = getpwuid(user_id);
        time(&_time);
        ptm = localtime(&_time);

        char strBuffer[_MAX_PATH];
        char line[512];
        memset(line, 0, 512);


        getcwd(strBuffer, _MAX_PATH);
        printf("[%d:%d:%d]%s@%s$", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, user_pw->pw_name ,strBuffer);

        scanf("%[^\n]s", line);
        getchar();
        
        char *temp = strtok(line, " ");        
        if(temp == NULL) {continue;}

        while(andFlag == 0) {
            redirect = 0;
            backG = 0;
	    //char *args[128];
	    //char **next = args;            

	    while (temp != NULL) {
               *next++ = temp;
               temp = strtok(NULL, " ");
                
               if(temp != NULL) {
                 //printf("temp : %s\n", temp);                  

                 if (!strcmp(temp, ">")) { 
                      redirect = 1;
                      filename = strtok(NULL, " ");
                      temp = strtok(NULL, " ");
                   
                  }
                  else if(!strcmp(temp, "<")) {
                      redirect = -1;
                      filename = strtok(NULL, " ");
                      temp = strtok(NULL," ");  
                  }
                }
                if(temp != NULL) {
               if(!strcmp(temp,"&&")) {
                      andFlag = 1;
                      temp = strtok(NULL, " ");
                      break;
                  }
                 else if (!strcmp(temp,"&")) {
                      backG = 1;
                      break;
                 }
                }
            }
            
            *next = NULL;
	    
            puts("Checking:");
            for (next = args; *next != 0; next++)
                puts(*next);

            if(andFlag == 1) {andFlag = 0;}
            else if (andFlag == 0){andFlag = 1;}



            
            if (!strcmp(args[0], "cd")){
                chdir(args[1]);
            }

    


            else{
            pid_t pid;
            pid = fork();
                switch(pid){
                    case -1:
                        printf("cant fork, err\n");
                        break;

                    case 0:
            
              if(redirect == -1) {
                int fd = open(filename, O_RDONLY);
                dup2(fd, STDIN_FILENO);
                close(fd);
             }
            else if (redirect == 1) {
              int fd = open(filename, O_WRONLY|O_TRUNC|O_CREAT, 0644);
              dup2(fd, STDOUT_FILENO);
              close(fd);
              }         
                           execvp(args[0], args);
                          
                    default:
                        printf("parent process %d\n", pid);
                        if(backG == 0) {
                            wait(NULL);
                        }
                        printf("child process end\n");
                        break;


                }
            }
        }
    }
}
