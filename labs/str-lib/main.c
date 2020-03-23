#include <stdio.h>
#include <stdlib.h>

int mystrlen(char *);
char *mystradd(char *, char *);
int mystrfind(char *, char *);

int main(int argc, char *args[]) {
	if(argc < 4){
		printf("Too few arguments \n");
	}
	else{
		printf("Initial length      : %d\n",mystrlen(args[1]));
		char *newStr = mystradd(args[1],args[2]);
		printf("New String          : %s\n",newStr);
		if(mystrfind(newStr,args[3]) == 1){
			printf("SubString was found : yes\n");
		}else{
			printf("SubString was found : no\n");
		}
		free(newStr);
	}
	return 0;
}
