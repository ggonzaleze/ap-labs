#include <stdio.h>

//#define   LOWER  0       /* lower limit of table */
//#define   UPPER  300     /* upper limit */
//#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc,char **args)
{
   int fahr,lower,upper,step;
   lower = strtol(args[1],NULL,10);
   if(argc == 2){
	printf("Fahrenheit: %3d, Celcius: %6.1f\n", lower, (5.0/9.0)*(lower-32));
   }
   else{
    	upper = strtol(args[2],NULL,10);
    	step = strtol(args[3],NULL,10);
        for (fahr = lower; fahr <= upper; fahr = fahr + step)
		printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
   }
   return 0;
}
