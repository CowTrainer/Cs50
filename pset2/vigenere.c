#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main(int argc, string argv[])
{
    if(2>argc||argc>2)
    {
        printf("Try again");
        return 1;
    }
    string k = argv[1];
    for(int count = 0 ;count < strlen(k);count++ )
    {
        if(!isalpha(argv[1][count]))
        {
            printf("Try again");
            return 1;
        }
        
    }
    
    string text= GetString();
    
    int higher = 0;
    int lower = 0;
    int textl = strlen(text);
    int count3 = 0;
    for(int count1 = 0 ;count1 < textl;count1++ )
    {
        k[count1]=toupper(k[count1]);
        
    }
    for (int count2 =0; count2 < textl;count2++)
    {
        if (isalpha(text[count2]))
        {
            higher = ((int)text[count2]+(k[count3]-65)-65)%26;
            lower =((int)text[count2]+(k[count3]-65)-97)%26;
            count3++;
            count3=(count3)%strlen(k);
            if(isupper(text[count2]))
                printf("%c",higher+65);
            if(islower(text[count2]))
                printf("%c",lower+97);
        }
        if (!isalpha(text[count2]))
        {
            printf("%c",text[count2]);
        }
    }
    printf("\n");
}