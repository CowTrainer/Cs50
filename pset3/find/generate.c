/**
 * generate.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// constant
#define LIMIT 65536

int main(int argc, string argv[])
{
    // Checks that command meets the programs requirements, if not it asks for correct command.
    if (argc != 2 && argc != 3)
    {
        printf("Usage: generate n [s]\n");
        return 1;
    }

    // Converts second commnd argument(first number) to an intager so the program knows how many random numbers to output
    int n = atoi(argv[1]);

    // Checks for third argument (second number) in command. If there is no number provided the number will be produced by time(NULL) which outputs the number of seconds between the current time and 12:00 am UTC Jan 1st 1970.
    // This number is put through srand48() to produce a seed used in drand48()to produce random numbers 
    if (argc == 3)
    {
        srand48((long int) atoi(argv[2]));
    }
    else
    {
        srand48((long int) time(NULL));
    }

    // drand48() is used to produce an pseudo random number between 0 and 1, this is multiplied by the 16 bit intager limit(65536) to produce a number 1 or above, up to 65536.
    // This number is then type casted into an int and outputted with printf(). The for loop runs this sequence for the amount of times that the user inputted via the second command line argument.
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}