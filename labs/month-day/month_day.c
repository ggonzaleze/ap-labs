#include <stdio.h>
#include <stdlib.h>

/* month_name:  return name of n-th month */
char* month_name(int n) {

   static char *name[] = {
       "Illegal month",
       "January", "February", "March",
       "April", "May", "June",
       "July", "August", "September",
       "October", "November", "December"
   };

   return name[n];
}

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){
	static char daytab[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
	int isLeap = 0;
	int sumDays = 0;
	int month = 0;
	int day = 0;
	if(year % 4 == 0){isLeap=1;}
	for(int i = 0; i < 13; i = i + 1){
		sumDays = sumDays + daytab[isLeap][i];
		if(yearday <= sumDays){
			month = i;
			break;
		}
	}
	for(int j = 0; j < month; j = j + 1){
		yearday = yearday - daytab[isLeap][j];
	}
	day = yearday;
	*pmonth = month;
	*pday = day;
}

int main(int argc, char *argv[]) {
	int year = atoi(argv[1]);
	int yearday = atoi(argv[2]);
	int month;
	int day;
	if(year < 0 || yearday < 0 || year == 0){
		printf("Illegal arguments \n");
	}else{
		month_day(year,yearday,&month,&day);
		printf("%s %02d, %d \n",month_name(month),day,year);
	}
	return 0;
}
