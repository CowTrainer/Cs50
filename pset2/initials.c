#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
int main(void)
{
    string name = GetString();
    if(isalpha(name[0]))
    {
        printf("%c",toupper(name[0]));
    }
    for (int count =0 ; count < strlen(name);count++)
    {
        if(isalpha(name[count]))
        {
           
        }
        else
        {
            printf("%c",toupper(name[count+1]));
        }
    }
    printf("\n");
}