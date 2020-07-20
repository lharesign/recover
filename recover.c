#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //checking the file opens and program is used correctly
    if (argc != 2)
    {
        printf("Usage: recover [filename]\n");
        return 1;
    }

    FILE *recovery_file = fopen(argv[1], "r");

    if (!recovery_file)
    {
        printf("Could not open file\n");
        return 1;
    }

    int file_counter = 0; //Setting counter to keep track of file name
    char filename[8]; //Creating filename string
    BYTE buffer[512]; //Creating buffer that we will store read data in
    FILE *img;
    bool found_first = false;
    fread(buffer, sizeof(BYTE), 512, recovery_file);

    while (fread(buffer, sizeof(BYTE), 512, recovery_file) != 0)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (found_first == false)
            {
                found_first = true;
                sprintf(filename, "%03i.jpg", file_counter); //Creating filename
                img = fopen(filename, "w"); //Opening file with name filename to write to
                fwrite(buffer, sizeof(BYTE), 512, img); //Writing the information stored in buffer to img
                file_counter++; //Incrementing file counter for file names
            }

            else

            {
                fclose(img); //Closing open file first
                sprintf(filename, "%03i.jpg", file_counter); //Creating filename
                img = fopen(filename, "w"); //Opening file with name from 000.jpg up
                fwrite(buffer, sizeof(BYTE), 512, img); //Writing the information stored in buffer to img
                file_counter++; //Incrementing file counter for file names
            }
        }

        else if (found_first == true) //If we've found first jpeg but not start of a new one, keep writing data
        {
            fwrite(buffer, sizeof(BYTE), 512, img); //Continue writing the information stored in buffer to img
        }
    }
    fclose(img);
    fclose(recovery_file);
}
