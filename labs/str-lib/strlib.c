#include <stdlib.h>
#include <stdio.h>

int mystrlen(char *str){
	int size = 0;
	int i = 0;
	while(str[i] != '\0'){
		size++;
		i++;
	}
	return size;
}

char *mystradd(char *origin, char *addition){
	int sOrigin = mystrlen(origin);
	int sAdd = mystrlen(addition);
	int size = sOrigin + sAdd;
	char *newStr = malloc(size + 1);
	int i = 0;
	int strI = 0;

	while(origin[i] != '\0'){
		newStr[strI] = origin[i];
		strI++;
		i++;
	}
	i = 0;
	while(addition[i] != '\0'){
		newStr[strI] = addition[i];
		strI++;
		i++;
	}
	newStr[strI] = '\0';
	return newStr;
}

int mystrfind(char *origin, char *substr){
	int sOrigin = mystrlen(origin);
	int sSubs = mystrlen(substr);
	int iO = 0;
	int iS = 0;
	while(iO < sOrigin){
		if(origin[iO] == substr[iS]){
			while(origin[iO] == substr[iS] && iS < sSubs){
				iO++;
				iS++;
			}
			if(substr[iS] == '\0'){return 1;}
			else{
				iS = 0;
				continue;
			}
		}
		iO++;
	}
	return 0;
}
