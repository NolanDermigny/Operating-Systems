//Nolan Dermingny Recieved Code for p1s1 from Noah Hays
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include "util.h"
#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

int main(int argc, char *argv[]) {
	char command [MAX_COMMAND_LENGTH];
	char *args[MAX_ARGUMENTS]= {""};
	FILE *file;
	int i=0;
	int batch=0;
	int result=0;
	char path[100]="/bin";
	char pathTest[100];
	int builtIn=0;
	char CWD[100];
	int correctLocation;
	if(argc==1) {
		file=stdin;
	} else {
		file=fopen(argv[1], "r");
		batch++;
	}
	if(batch==0) {
			printf("wish> ");
	}	
	while(fgets(command,sizeof(command),file)!= NULL) {
	int isFile=0;
		int builtIn=0;
		command[strcspn(command, "\n")] = NULL;
		char *token=strtok(command, " ");
		int i=0;
		while(token) {
			args[i++]=token;
			token = strtok(NULL," ");
		}
		
		args[i]=NULL;
		if(strcmp(args[0], "exit")==0) {
			builtIn++;
			if(args[1] != NULL) {
				builtInError();
				exit(0);
			}
			exit(0);
		}
		
		int length=10;
		for(int j=0; j<10; j++) {
			if(args[j] == NULL) {
				length--;
			}
		}
		
		if(strcmp(args[0], "cd")==0 || strcmp(args[0], "cd\n")==0) {
			builtIn++;
			if(length>=3) {
				builtInError();
				continue;
			}
			int result=chdir(args[1]);
			if(result==-1) {
				builtInError();
				continue;
			}
			strcat(path, " ");
			strcat(path, args[1]);
		}
		
		if(strcmp(args[0], "path")==0) {
			builtIn++;
			if(length<2) {
				strcpy(path,"");
			} else {
				int i=1;
				while(args[i] != NULL) {
					strcat(path, " ");
					strcat(path, "/");
					strcat(path, args[i]);
					i++;
				}
			}
			
		}
		
		char *checker=strstr(args[0], ".sh");
		if(checker != NULL) {
			//splitString();
			int noPath=0;
			char *finalPaths[100]= {""};
			strcpy(pathTest, path);
			pathTest[strcspn(pathTest, "\n")]='\0';
			char *token=strtok(pathTest, " ");
			int i=0;
			while(token) {
				finalPaths[i++]=token;
				token = strtok(NULL," ");
			}
			finalPaths[i]=NULL;
			i=0;
			char checker[100];
			while(finalPaths[i] != NULL) {
				memset(checker, '\0', sizeof(checker));
				strcat(checker, getcwd(CWD, sizeof(CWD)));
				strcat(checker, finalPaths[i]);
				strcat(checker, "/");
				strcat(checker, args[0]);
				int result=access(checker, X_OK);
				if(result==0) {
					noPath++;
					correctLocation=i;
				}
				i++;
			}
			if(noPath==0) {
				builtInError();
				continue;
			} else {
				isFile++;
			}
			
		}
		if(builtIn==0) {
		//builtInCheck(*args);
		pid_t pid=fork();
		if(pid <0) {
			perror("Fork failed");
			exit(1);
		} else if (pid==0) {
			char *finalPaths[100]= {""};
			strcpy(pathTest, path);
			pathTest[strcspn(pathTest, "\n")]='\0';
			char *token=strtok(pathTest, " ");
			int i=0;
			while(token) {
				finalPaths[i++]=token;
				token = strtok(NULL," ");
			}
			finalPaths[i]=NULL;
			i=0;
			int error=0;
			char checker[100];
		while(finalPaths[i] != NULL) {
			strcpy(checker, finalPaths[i]);
			printf("%s\n", checker);
			if(isFile==1) {
				strcat(checker, "/sh");
			} else {
				strcat(checker, "/");
				strcat(checker, args[0]);
			}
			int result=access(checker, X_OK);
			if(result==0) {
				if(isFile==1) {
					char directory[100];
					getcwd(directory, sizeof(directory));
					strcat(directory,finalPaths[correctLocation]);
					strcat(directory, "/");
					strcat(directory,args[0]);
					strcpy(args[0], "sh ");
					args[1]=directory;
					strcat(finalPaths[i],"/sh");
					execv(finalPaths[i],args);
				} else {
					execv(checker,args);
				}
			} else {
				error++;
			}
			i++;
		}
		if(error==i) {
			builtInError();
			continue;
		}
		} else {
			int status;
			waitpid(pid, &status, 0);
		}
		}
	}
	fclose(file);

	return 0;
}
