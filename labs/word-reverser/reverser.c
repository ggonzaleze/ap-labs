#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
	// Place your magic here
	char c;
	char reversed[500];
	char tmp[500];
	int notEOF = 1;

	while(notEOF){
		strcpy(reversed,"");
		while((c = getchar()) != '\n' && notEOF == 1){
			if(c == EOF){
				notEOF = 0;
			}
			else{
				tmp[0] = c;
				tmp[1] = '\0';
				strcpy(reversed,strcat(tmp,reversed));
			}
		}
		if (notEOF == 1)
			printf("%s \n",reversed);
	}
	return 0;
}
