#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    if (argc != 2) // check : should have 2nd argument
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // 1. open memory card
    FILE *raw_file = fopen(argv[1], "r"); // r is reading mode

    // check if file exists
    if (raw_file == NULL)
    {
        printf("No such file found.\n");
        return 1;
    }

    // look for beginning of jpeg
    typedef uint8_t BYTE;
    BYTE buffer[512];

    int file_number = 0;

    // Check buffer match with JPEG?
    while (fread(buffer, 512, 1, raw_file) == 1) // (location, size of block to read, how many block to read, location to read from)
    {
        if (buffer[0] == 0xff & buffer[1] == 0xd8 & buffer[2] == 0xff & (buffer[3] & 0xf0) == 0xe0)
        {
            printf("It's jpeg.\n");
            // found begining of jpeg
            char *filename = malloc(4));
            sprintf(filename, "%03i.jpg", file_number);
            FILE *img = fopen(filename, "w");
            file_number++;
        }
        else
        {
            printf("It's not a  jpeg !!!!\n");
        }
    }
    fclose(raw_file); // ****** ถ้าไม่ close จะ leak memory ถ้า run valgrind ./pdf test.pdf ดู
    // --- end of check: JPEG? ---

    // open new jpeg file

    // Create a new block of memory to store filename
    // int filename_length = 3;
    // char *filename = malloc(sizeof(char) * (filename_length + 1)); // ### + \0

    // Copy argv[1] into block of memory for filename
    // sprintf(filename, "%03i.jpg", 2);

    // sprintf(filename, "%03i.jpg", 2);
    // FILE *img = fopen(filename, "w");

    // write 512 bytes until a new jpeg is found
    // fwrite(data, size, number, outptr);

    // find the end of the file
}