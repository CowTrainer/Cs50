#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    printf("How many minutes in the shower?\n");
    int minutes = GetInt();
    int bottles = minutes * 12;
    printf("Minute(s):%d \nBottles: %d\n", minutes, bottles);
    
}