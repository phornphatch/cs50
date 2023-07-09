#include "helpers.h"
#include "stdio.h"
#include "stdlib.h"

// TODO implement all filters function
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // find avg of red green blue
    //.rgbtRed + .rgbtGreen + .rgbtBlue / 3
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int num_red = image[i][j].rgbtRed; // -> 0x00 or 0xff
            int num_green = image[i][j].rgbtGreen;
            int num_blue = image[i][j].rgbtBlue;
            int num_avg = (int) ((num_red + num_green + num_blue) / 3);
            // printf("%d", num_avg);
            image[i][j].rgbtRed = num_avg;
            image[i][j].rgbtGreen = num_avg;
            image[i][j].rgbtBlue = num_avg;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
     // Create array to store plate numbers
    char *plates[8];

    // memory allocation
    for (int i = 0; i <= 8; i++)
    {
        plates[i] = malloc(7);
        if (plates[i] == NULL) // ถ้า mem ไม่พอ
        {
            return 1;
        }
    }

    for (int i = 0; i <= height; i++) // access to each row
    {
        // switch (mirror)
        for (int j = 0; i <= width; i++)
        {
             strcpy(plates[idx], buffer);
            image[i][j].rgbtRed = image[i][width - j].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j].rgbtBlue;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
