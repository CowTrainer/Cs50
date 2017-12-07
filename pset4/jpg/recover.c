/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t  BYTE;
int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./copy infile \n");
        return 1;
    }
    
    // remember filenames
    char *infile = argv[1];
    int jpgcounter =  0 ;
    uint8_t buf[512];
    FILE *writefile = NULL;
   // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    while (fread(buf, 512, 1, inptr))
    {
        if (buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff && (buf[3] == 0xe0 || buf[3] == 0xe1))
        {
            // Close the file, if it is opened
            if (writefile != NULL)
                fclose(writefile);
            
            char filename[8];
            sprintf(filename, "%03d.jpg", jpgcounter);
                
            // Open a new JPEG file for writing
            writefile = fopen(filename, "w");
            
            jpgcounter++;
        }
        
        if (writefile != NULL)
            fwrite(buf, 512, 1, writefile);
    }
    
    if (writefile != NULL)
        fclose(writefile);
    
    fclose(inptr);
 
    return 0;

}