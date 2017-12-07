#include <stdio.h>
#include <cs50.h>

int main (void)
{

    int counter = 0;
    int height = 0;
    do
    {
        printf("What is the height of the pyramid?\n");
        height = get_int();
    }
    while (height > 23 || height < 0);

    for (int row = 1; row <= height; row++)
    {
        for (counter = 0; counter < height - row; counter++)
            printf(" ");
        for(counter = -1;counter<row;counter++)
            printf("#");
        
            printf("\n");
    }

}