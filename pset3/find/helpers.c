/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if(n<0)
    {
        return false;
    }
    for(int count=0;count<n;count++)
    {
        if(value==values[count])
        {
            return true;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    for(int count = 0;count<n;count++)
    {
        int minvalue = values[count];
        int mincount = count;
        int holder= 0;
        for(int count2=0;count2<n;count2++)
        {   
            if(values[count2]<minvalue)
            {
                minvalue=values[count2];
                mincount=count2;
            }
        }
        holder=values[count];
        values[count]= values[mincount];
        values[mincount]= holder;
        
    }
    return;
}