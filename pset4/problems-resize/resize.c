
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize number infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // make sure that n is an integer
    for(int z = 0;z<strlen(argv[1]);z++)
    {
        if(!isdigit(argv[1][z]))
        {
            printf("N must be a valid interger");
            return 2;
        }
    }
    int n = atoi(argv[1]);
    // make sure that n is in 1 to 100
    if(n<1||n>100)
    {
        printf("N must be a valid interger");
        return 2;
    }

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER inbf;
    fread(&inbf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER inbi;
    fread(&inbi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    if (inbf.bfType != 0x4d42 || inbf.bfOffBits != 54 || inbi.biSize != 40 || 
        inbi.biBitCount != 24 || inbi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    BITMAPINFOHEADER outbi = inbi;
    BITMAPFILEHEADER outbf = inbf;
    
    outbi.biWidth = inbi.biWidth * n;
    outbi.biHeight = inbi.biHeight * n;
    
    int inpadding =  (4 - (inbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outpadding =  (4 - (outbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    outbi.biSizeImage = (outbi.biWidth*sizeof(RGBTRIPLE)*abs(outbi.biHeight))+(outpadding*abs(outbi.biHeight));
    outbf.bfSize = outbi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&outbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    
    // write outfile's BITMAPINFOHEADER
    fwrite(&outbi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    int offset = inbi.biWidth * sizeof(RGBTRIPLE) + inpadding;
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(inbi.biHeight); i < biHeight; i++)
    {       
        for(int o = 0; o < n ; o++)
        {
           
            // iterate over pixels in scanline
            for (int j = 0; j < inbi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
                
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // write RGB triple to outfile
                for(int k = 0; k < n ; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            
            // skip over padding, if any
            fseek(inptr, inpadding, SEEK_CUR);
            
            for (int h = 0; h < outpadding; h++)
            {
                fputc(0x00, outptr);
            }
    
            fseek(inptr,-offset,SEEK_CUR);
            // move the pointer to the start of the next line
        
        
        }
        // move the pointer to the start of the next line
        fseek(inptr, offset , SEEK_CUR);
        
    }
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
