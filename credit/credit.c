#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // get credit cardnumber
    long number = get_long("Number: ");

    // calculate checksum
    int i;
    while ()
    {
    long last_number = number % 10;
    if (last_number < 10) {
        printf("last number is, %li", last_number);
        number = number / 10;
    }
    printf("remaining number is, %li", number);
    }

    // check card length

    // check starting number
}