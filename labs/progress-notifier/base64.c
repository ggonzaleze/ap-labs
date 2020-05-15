// C program to encode an ASCII
// string in Base64 format
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include "logger.h"
#define SIZE 1000000

/*******************************************************************************************
Encoder and Decoder functions reference
--Encoder
*    Title: Encode an ASCII string into Base-64 Format
*    Author: GeeksforGeeks
*    Availability: https://www.geeksforgeeks.org/encode-ascii-string-base-64-format/
--Decoder
*    Title: Decode an Encoded Base 64 String to ASCII String
*    Author: GeeksforGeeks
*    Availability: https://www.geeksforgeeks.org/decode-encoded-base-64-string-ascii-string/
Delay function reference
*    Title: Time delay in C
*    Author: GeeksforGeeks
*    Availability: https://www.geeksforgeeks.org/time-delay-c/
*******************************************************************************************/

int size;
int c = 0;

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

// Takes string to be encoded as input
// and its length and returns encoded string
char* base64Encoder(char input_str[], int len_str)
{
    // Character set of base64 encoding scheme
    char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    // Resultant string
    char *res_str = (char *) malloc(SIZE * sizeof(char));

    int index, no_of_bits = 0, padding = 0, val = 0, count = 0, temp;
    int i, j, k = 0;

    // Loop takes 3 characters at a time from
    // input_str and stores it in val
    for (i = 0; i < len_str; i += 3)
        {
	    c = c + 1;
	    //delay(1);
            val = 0, count = 0, no_of_bits = 0;

            for (j = i; j < len_str && j <= i + 2; j++)
            {
                // binary data of input_str is stored in val
                val = val << 8;

                // (A + 0 = A) stores character in val
                val = val | input_str[j];

                // calculates how many time loop
                // ran if "MEN" -> 3 otherwise "ON" -> 2
                count++;

            }

            no_of_bits = count * 8;

            // calculates how many "=" to append after res_str.
            padding = no_of_bits % 3;

            // extracts all bits from val (6 at a time)
            // and find the value of each block
            while (no_of_bits != 0)
            {
                // retrieve the value of each block
                if (no_of_bits >= 6)
                {
                    temp = no_of_bits - 6;

                    // binary of 63 is (111111) f
                    index = (val >> temp) & 63;
                    no_of_bits -= 6;
                }
                else
                {
                    temp = 6 - no_of_bits;

                    // append zeros to right if bits are less than 6
                    index = (val << temp) & 63;
                    no_of_bits = 0;
                }
                res_str[k++] = char_set[index];
            }
    }

    // padding is done here
    for (i = 1; i <= padding; i++)
    {
        res_str[k++] = '=';
    }

    res_str[k] = '\0;';
    return res_str;

}

// C Program to decode a base64
// Encoded string back to ASCII string

/* char_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz0123456789+/" */

char* base64Decoder(char encoded[], int len_str)
{
    char* decoded_string;

    decoded_string = (char*)malloc(sizeof(char) * SIZE);

    int i, j, k = 0;

    // stores the bitstream.
    int num = 0;

    // count_bits stores current
    // number of bits in num.
    int count_bits = 0;

    // selects 4 characters from
    // encoded string at a time.
    // find the position of each encoded
    // character in char_set and stores in num.
    for (i = 0; i < len_str; i += 4) {
	c = c + 1;
        //delay(1);
        num = 0, count_bits = 0;
        for (j = 0; j < 4; j++) {
            // make space for 6 bits.
            if (encoded[i + j] != '=') {
                num = num << 6;
                count_bits += 6;
            }

            /* Finding the position of each encoded
            character in char_set
            and storing in "num", use OR
            '|' operator to store bits.*/

            // encoded[i + j] = 'E', 'E' - 'A' = 5
            // 'E' has 5th position in char_set.
            if (encoded[i + j] >= 'A' && encoded[i + j] <= 'Z')
                num = num | (encoded[i + j] - 'A');

            // encoded[i + j] = 'e', 'e' - 'a' = 5,
            // 5 + 26 = 31, 'e' has 31st position in char_set.
            else if (encoded[i + j] >= 'a' && encoded[i + j] <= 'z')
                num = num | (encoded[i + j] - 'a' + 26);

            // encoded[i + j] = '8', '8' - '0' = 8
            // 8 + 52 = 60, '8' has 60th position in char_set.
            else if (encoded[i + j] >= '0' && encoded[i + j] <= '9')
                num = num | (encoded[i + j] - '0' + 52);

            // '+' occurs in 62nd position in char_set.
            else if (encoded[i + j] == '+')
                num = num | 62;

            // '/' occurs in 63rd position in char_set.
            else if (encoded[i + j] == '/')
                num = num | 63;

            // ( str[i + j] == '=' ) remove 2 bits
            // to delete appended bits during encoding.
            else {
                num = num >> 2;
                count_bits -= 2;
            }
        }

        while (count_bits != 0) {
            count_bits -= 8;

            // 255 in binary is 11111111
            decoded_string[k++] = (num >> count_bits) & 255;
        }
    }

    // place NULL character to mark end of string.
    decoded_string[k] = '\0';

    return decoded_string;
}

void sigHand(){
	infof("%d/%d completed\n",c,size);
}


// Driver code
int main(int argc, char *argv[])
{
	int fp;
	if (argc < 3){
		errorf("Usage: ./base64 --encode/decode file.txt \n");
		exit(1);
	}
	if ((fp = open(argv[2], O_RDONLY)) < 0){
		errorf("Error opening file %s \n",argv[2]);
		exit(1);
	}
	size = lseek(fp, 0, SEEK_END);
	lseek(fp,0,SEEK_SET);
	char data[size];
	if(read(fp,data,size) < 0)
		errorf("An error ocurred in the read of file \n");
	close(fp);
	signal(SIGINT,sigHand);
	signal(SIGUSR1,sigHand);
	FILE * f;
	if(strcmp(argv[1],"--encode") == 0){
		char *encoded = base64Encoder(data, size);
		f = fopen("encoded.txt","w");
		fprintf(f,"%s", encoded);
	}
	else if(strcmp(argv[1],"--decode") == 0){
		char *decoded = base64Decoder(data, size-1);
		f = fopen("decoded.txt","w");
		fprintf(f,"%s", decoded);
	}
	else{
                errorf("Usage: ./base64 --encode/decode file.txt \n");
		exit(1);
	}
	fclose(f);
	return 0;
}
