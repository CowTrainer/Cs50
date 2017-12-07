#include <stdio.h>
#include <cs50.h>
#include <math.h>
int main(void)
{
    int quartercount = 0;
    int dimecount = 0;
    int nickelcount = 0;
    int pennycount = 0;
    int total = 0;
    printf("How much change is owed?\n");
    float amount = GetFloat();
        
    while(amount<=0)
    {
        printf("Sorry that is an invalid number.Please try again\n");
        amount = GetFloat(); 
    }

    int amount2= (int) round(amount* 100.0);
    
    while(amount2 >= 25)
    {
        amount2 = amount2 - 25;
        quartercount++;
        
        
    }
    while(amount2 >= 10)
    {
        amount2 = amount2 - 10;
        dimecount++;
    }
    while(amount2 >= 5)
    {
        amount2 = amount2 - 5;
        nickelcount++;
    }
    while(amount2 >= 1)
    {
        amount2--;
        pennycount++;
    }
    total = quartercount+dimecount+nickelcount+pennycount;
    printf( "%d\n", total);
}