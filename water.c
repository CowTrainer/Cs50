#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int m;
    do
    {
        printf("How many minutes did you shower?\n");
        m = GetInt();
    }
    while(m < 0);
    {
        int b = m*12;
        printf("You used %i bottles of water\n", b);
    }
}



