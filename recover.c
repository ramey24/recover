#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: need infile only\n");
        return 1;
    }
     
    // open file
    FILE *file = fopen(argv[1], "r");
    
    // make sure file exists
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    printf("size of file: %lu\n", sizeof(file));
    
    // no of jpegs already created
    int jpegNo = 0;
    
    // array to store card's data
    BYTE buffer[sizeof(file) * 1000000];
            
    // initialize name of img
    char filename[8];
    
    FILE *img;
    
    // while it's not the end of the infile
    while (fread(buffer, 1, 512, file) == 512)
    {
        // read in 512 blocks of data of size 1 byte.
        fread(buffer, 1, 512, file);
        
        // if start of a jpeg image
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
           (buffer[3] & 0xf0) == 0xe0)
        {
            // if it's not the start of the very first jpeg in the card 
            if (jpegNo != 0)
            {
                // if the previous image is not nul;
                if (img !=  NULL)
                {
                    // close the previous image
                    fclose(img);
                }
            }
            
            // create a new jpeg filename
            sprintf(filename, "%03i.jpg", jpegNo);
            
            printf("filename: %s\n", filename);
                
            // open the file
            img = fopen(filename, "w");
            
            // write to the img 512 blocks of size 1 byte
            fwrite(buffer, 1, 512, img);
            
            jpegNo += 1;
        }
        
        // if the 512 bytes currently being read is already a part of a jpeg
        else if (jpegNo != 0)
        {
            fwrite(buffer, 1, 512, img);
        }
        
    }
    fclose(img);
}
