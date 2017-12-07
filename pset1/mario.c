#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    int counter = 0;
    int rownumber = 0;
    int height = 0;
    do
    {
    printf("What is the height of the pyramid?\n");
    height = get_int();
    }
    while (height > 23||height<0);
    for(rownumber = 1;rownumber<=height;rownumber++)
    {
        for(counter = 0; counter < height-rownumber;counter++)
        printf(" ");
    
        for(counter = -1;counter<rownumber;counter++)
        printf("#");
    
        printf("\n");
    }
}

