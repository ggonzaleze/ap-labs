#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>

#define REPORT_FILE "packages_report.txt"
#define SIZE 1024

struct Package {
	char *package;//key
	char *installDate;
	char *updateDate;
	char *removeDate;
	int updates;
};

struct Package* packagesArray[SIZE];
struct Package* packageItem;

int hashCode(char *key){
	return strlen(key) % SIZE;
}

struct Package* search(char *key){
	int index = hashCode(key);
	while(packagesArray[index] != NULL){
		if(strcmp(packagesArray[index]->package,key) == 0)
			return packagesArray[index];
		index = index + 1;//next cell
		index = index % SIZE;//restart index if necessary
	}
	return NULL; //not found
}

void insert(char *key, char *installDate, char *updateDate, char *removeDate){
	struct Package* newPackage = (struct Package*) malloc(sizeof(struct Package));
	newPackage->package = strdup(key);
	newPackage->installDate = strdup(installDate);
	newPackage->updateDate = strdup(updateDate);
	newPackage->removeDate = strdup(removeDate);
	newPackage->updates = 0;

	int index = hashCode(key);
	while(packagesArray[index] != NULL){
		index = index + 1;//it is occupied, move to next
		index = index % SIZE;//restart index if necessary
	}
	packagesArray[index] = newPackage;//do the insertion
}

void displayPackages(){
	int i;
	for(i = 0; i < SIZE; i++){
		if(packagesArray[i] != NULL){
			printf("(%s,%s,%s,%s,%d)\n",
			packagesArray[i]->package,
			packagesArray[i]->installDate,
			packagesArray[i]->updateDate,
			packagesArray[i]->removeDate,
			packagesArray[i]->updates);
		}
	}
	printf("\n");
}

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {
	if (argc < 2) {
                printf("Usage:./pacman-analizer.o pacman.log\n");
                return 1;
        }

        analizeLog(argv[1], REPORT_FILE);
        return 0;
}

void analizeLog(char *logFile, char *report) {
	printf("Generating Report from: [%s] log file\n", logFile);

	// Implement your solution here.
	remove(report);
	int filedesc = open(logFile, O_RDONLY);
	if(filedesc < 0)
        	write(1, "An error occurred in the write.\n", 32);
	int size = lseek(filedesc, 0, SEEK_END);
	lseek(filedesc,0,SEEK_SET);
	char data[size+1];
	if(read(filedesc,data,size) < 0)
		write(1, "An error occurred in the read.\n", 31);
	data[size+1] = '\0';
	close(filedesc);

	int line = 0;
	char date[17];
	char operation[10];
	char p[200];
	int j;

	for(int i = 0; i < size+1; i++){
		line++;
		if(data[i] == '\n'){
			line = 0;
		}
		if(line > 0 && data[i] == '['){
			i = i + 1;
			j = 0;
			line++;
			while(data[i] != ']'){
				date[j] = data[i];
				j++;
				i = i + 1;
				line++;
			}
			date[16] = '\0';
			i = i + 1;
                        line++;
			while(data[i] != ']'){
                                i = i + 1;
                                line++;
			}
			j = 0;
                        i = i + 2;
                        line = line + 2;
			while(!isspace(data[i])){
                                operation[j] = data[i];
                                j++;
                                i = i + 1;
                                line++;
                        }
			operation[j] = '\0';
			if(strcmp(operation,"")==0)
				continue;
			j = 0;
                        i = i + 1;
			line++;
			while(!isspace(data[i])){
                                p[j] = data[i];
                                j++;
                                i = i + 1;
                                line++;
                        }
			p[j] = '\0';
		}
		if(strcmp(operation,"installed") == 0 || strcmp(operation,"removed") == 0 || strcmp(operation,"upgraded") == 0){
			FILE * fp;
			fp = fopen (report,"a");
			fprintf(fp, "%s,",date);
			fprintf(fp, "%s,",operation);
			fprintf(fp, "%s\n",p);
			fclose (fp);
			p[0] = '\0';
			operation[0] = '\0';
			date[0] = '\0';
		}  //cierra if de compares
	}//cierra for
	int tmp = open(report,O_RDONLY);
	int sz = lseek(tmp,0,SEEK_END);
	lseek(tmp,0,SEEK_SET);
	char formattedData[sz+1];
	read(tmp,formattedData,sz);
	formattedData[sz+1] = '\0';
	close(tmp);

	char l[250];
	int lIndex = 0;
	int k;
	for(k = 0; k < sz ; k++){
		while(formattedData[k] != '\n'){
			l[lIndex] = formattedData[k];
			lIndex++;
			k++;
		}
		l[lIndex] = '\0';
		char *d = strtok(l, ",");
		char *o = strtok(NULL, ",");
		char *n = strtok(NULL, ",");
		struct Package* packageItem = search(n);
		if(packageItem == NULL){
			insert(n,d,"--","--");
		}else{
			if(strcmp(o,"installed") == 0){
				packageItem->installDate = strdup(d);
			}
			if(strcmp(o,"removed") == 0){
				packageItem->removeDate = strdup(d);
			}
			if(strcmp(o,"upgraded") == 0){
				packageItem->updateDate = strdup(d);
				packageItem->updates = packageItem->updates+1;
			}
		}
		lIndex = 0;
		l[0] = '\0';
	} //cierro for

	int installedPackages = 0;
	int removedPackages = 0;
	int upgradedPackages = 0;
	int currentInstalled = 0;
	for(int x = 0; x < SIZE; x++){
		if(packagesArray[x] != NULL){
			installedPackages++;
			if(strcmp(packagesArray[x]->updateDate,"--") != 0){
				upgradedPackages++;
			}
			if(strcmp(packagesArray[x]->removeDate,"--") != 0){
                                removedPackages++;
                        }
		}
	}
	FILE * fp = fopen(report,"w");
	fprintf(fp,"Pacman Packages Report\n");
	fprintf(fp,"----------------------\n");
	fprintf(fp,"-Installed  : %d\n",installedPackages);
	fprintf(fp,"-Removed  : %d\n",removedPackages);
	fprintf(fp,"-Upgraded  : %d\n",upgradedPackages);
	fprintf(fp,"-Current installed  : %d\n",installedPackages-removedPackages);
	fprintf(fp,"\n");
	fprintf(fp,"List of packages\n");
        fprintf(fp,"----------------------\n");
	for(int y = 0; y < SIZE; y++){
		if(packagesArray[y] != NULL){
			fprintf(fp,"- Package Name              : %s\n",packagesArray[y]->package);
			fprintf(fp,"\t- Install date      : %s\n",packagesArray[y]->installDate);
			fprintf(fp,"\t- Last update date  : %s\n",packagesArray[y]->updateDate);
			fprintf(fp,"\t- How many updates  : %d\n",packagesArray[y]->updates);
			fprintf(fp,"\t- Removal date      : %s\n",packagesArray[y]->removeDate);
		}
	}
	fclose(fp);
	printf("Report is generated at: [%s]\n", report);
}
