#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // get credit cardnumber
    long number = get_long("Number: ");

    // calculate checksum
    long last_number = number % 10;
    int i = 0;
    if (last_number < 10) {
        printf("hello, %li", last_number);
        double double = 
        number = number - (last_number * double);
        printf("hello, %li", number);
        i++;
    }
    // check card length

    // check starting number
}