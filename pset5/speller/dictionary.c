/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

int count = 0;
#include "dictionary.h"
#define SHASHTABLE 27
int hash(const char *word);
typedef struct node
    {
        char *word;
        struct node *next;
    }
node;
char word[LENGTH+1];
node *hashtable[SHASHTABLE];
int loaded=0;
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int place = hash(word);
    node* checker = hashtable[place];
    if (hashtable[place] == NULL) 
        return false;
    while(checker!=NULL)
    {
        if(strcasecmp(checker->word,word)==0)
        {
        return true;
        }
        checker=checker->next;
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{   
    FILE *dfile = fopen(dictionary, "r");
    if (dfile == NULL)
    {
        return 2;
    }
    while (fscanf(dfile,"%s\n",word) !=EOF)
    {
        node *new_node = malloc(sizeof(node));
        new_node->word=malloc(strlen(word)+1);
        if(new_node == NULL)
        {
            return false;
        }
        strcpy(new_node->word, word);
        int whashed = hash(word);
        if (hashtable[whashed]==NULL)
        {
            hashtable[whashed]=new_node;
            new_node->next=NULL;
        }
        else
        {   
            new_node->next=hashtable[whashed];
            hashtable[whashed]=new_node;
        }
    
        count++;

    }
    
    fclose(dfile);
    loaded = 1;
    return true;
    
}
int hash(const char *word)
{
    int index=0;
    for (int x = 0;word[x]!='\0';x++)
    {
        index += toupper(word[x]);
    }
    return index % SHASHTABLE;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if(loaded == 1 )
    return count;
    else 
    return 2;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for(int y = 0;y<SHASHTABLE;y++)
    {
        if (hashtable[y] == NULL)
            continue;
        node *cursor = hashtable[y];
        while (cursor != NULL)
            {
                node *n = cursor;
                cursor = cursor->next;
                free(n->word);
                free(n);
              }
        hashtable[y]=NULL;
        free(cursor);
    }
    
    return true;
}
