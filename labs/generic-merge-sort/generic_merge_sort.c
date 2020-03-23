#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isNumerical = 0;

void merge(char *arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    char *L[n1], *R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
	if (isNumerical == 1){
		if ( atoi(L[i]) <= atoi(R[j]) )
        	{
            		arr[k] = L[i];
            		i++;
        	}
        	else
        	{
            		arr[k] = R[j];
            		j++;
        	}
	}else{
        	if (strcmp(L[i],R[j]) <= 0)
        	{
           		 arr[k] = L[i];
            		i++;
        	}
        	else
        	{
            		arr[k] = R[j];
            		j++;
        	}
	}
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(char *arr[], int l, int r)
{
    if (l < r)
    {
        int m = l+(r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

void printArray(char *A[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%s ", A[i]);
    printf("\n");
}

int main(int argc,char *args[])
{
    if(strcmp(args[1],"-n") == 0){
        char *arr[argc-2];
        isNumerical = 1;
	for(int i = 0; i < argc-2; i++)
        	arr[i] = args[i+2];
        int arr_size = sizeof(arr)/sizeof(arr[0]);
        mergeSort(arr, 0, arr_size - 1);
        printArray(arr, arr_size);
    }else{
        char *arr[argc-1];
        for(int i = 0; i < argc-1; i++)
		arr[i] = args[i+1];
        int arr_size = sizeof(arr)/sizeof(arr[0]);
        mergeSort(arr, 0, arr_size - 1);
        printArray(arr, arr_size);
    }
    return 0;
}
