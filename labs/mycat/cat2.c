#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* filecopy:  copy file ifp to file ofp */
void filecopy(int ifp, int ofp)
{
    if(ifp == 0){
        char data[1];
	while(read(ifp,data,1) != '\0'){
		write(ofp,data,1);
	}
    }else{
	int size = lseek(ifp, 0, SEEK_END);
	lseek(ifp,0,SEEK_SET);
	char data[size+1];
	if(read(ifp,data,size) < 0)
		write(1, "An error occurred in the read.\n", 31);
	data[size+1] = '\0';
	if(write(ofp,data,size) != size)
		write(1, "There was an error writing to standard out\n", 44);
    }
}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{
    int fp;
    void filecopy(int, int);
    char *prog = argv[0];   /* program name for errors */

    if (argc == 1)  /* no args; copy standard input */
        filecopy(0, 1);
    else
        while (--argc > 0)
            if ((fp = open(*++argv, O_RDONLY)) < 0) {
                printf("%s: can′t open %s\n",
			prog, *argv);
                return 1;
            } else {
                filecopy(fp, 1);
                close(fp);
            }

    return 0;
}
