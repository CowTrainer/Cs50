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
    printf("plaintext:");
    string text= GetString();
    int length = strlen(text);
    int k = atoi (argv[1]);
    int higher = 0;
    int lower = 0;
    for (int count =0 ; count < length;count++)
    {
        higher = ((int)text[count]+k-65)%26;
        lower =((int)text[count]+k-97)%26;
        if(isalpha(text[count]))
            {
            if(islower(text[count]))
                text[count]= lower+97;
            if(isupper(text[count]))
                text[count]= higher+65;
            }
    }
    printf("ciphertext: %s\n",text);
}
