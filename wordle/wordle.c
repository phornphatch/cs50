#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// each of our text files contains 1000 words
#define LISTSIZE 1000

// values for colors and score (EXACT == right letter, right place; CLOSE == right letter, wrong place; WRONG == wrong letter)
#define EXACT 2
#define CLOSE 1
#define WRONG 0

// ANSI color codes for boxed in letters
#define GREEN "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET "\e[0;39m"

// user-defined function prototypes
string get_guess(int wordsize);
int check_word(string guess, int wordsize, int status[], string choice);
void print_word(string guess, int wordsize, int status[]);
int size_number = 0;
bool is_wordsize_valid = false;
int point = 0;

int main(int argc, string argv[])
{
    // ensure proper usage
    // TODO #1
    // check has argv[1] ?
    if (argc == 2)
    {
        size_number = atoi(argv[1]);              // convert argv[1] to number (integer)
        if (size_number >= 5 && size_number <= 8) // check number
        {
            is_wordsize_valid = true;
        }
        else if (isdigit(size_number))
        {
            printf("Error: wordsize must be either 5, 6, 7, or 8\n"); // error if number not in 5-8 range
        }
        else
        {
            printf("Usage: %s wordsize\n", argv[0]); // error if argv[1] is not a number
        }
    }
    else
    {
        printf("Usage: %s wordsize\n", argv[0]); // error if no argv[1]
    }
    // ---- DONE check argv[1] ----

    int wordsize = 0;
    if (is_wordsize_valid == true)
    {
        wordsize = size_number;
        // printf("wordsize is %i\n", size_number);
    }

    // ensure argv[1] is either 5, 6, 7, or 8 and store that value in wordsize instead
    // TODO #2 - เช็คแล้วด้านบน
    // ---- DONE set wordsize to int ----

    // open correct file, each file has exactly LISTSIZE words
    char wl_filename[6];
    sprintf(wl_filename, "%i.txt", wordsize);
    FILE *wordlist = fopen(wl_filename, "r");
    if (wordlist == NULL)
    {
        printf("Error opening file %s.\n", wl_filename);
        return 1;
    }

    // load word file into an array of size LISTSIZE
    char options[LISTSIZE][wordsize + 1];

    for (int i = 0; i < LISTSIZE; i++)
    {
        fscanf(wordlist, "%s", options[i]);
    }

    // pseudorandomly select a word for this game
    srand(time(NULL));
    string choice = options[rand() % LISTSIZE];

    // allow one more guess than the length of the word
    int guesses = wordsize + 1;
    bool won = false;

    // print greeting, using ANSI color codes to demonstrate
    printf(GREEN "This is WORDLE50" RESET "\n");
    printf("You have %i tries to guess the %i-letter word I'm thinking of\n", guesses, wordsize);

    // main game loop, one iteration for each guess
    for (int i = 0; i < guesses; i++)
    {
        // obtain user's guess
        string guess = get_guess(wordsize); // see get_guess function at TODO #3

        // array to hold guess status, initially set to zero
        int status[wordsize];

        // set all elements of status array initially to 0, aka WRONG
        // TODO #4
        for (int j = 0; j < wordsize; j++)
        {
            status[j] = 0;
        }

        // Calculate score for the guess
        int score = check_word(guess, wordsize, status, choice); // choice is เฉลย as TODO #5 below

        printf("Guess %i: ", i + 1);

        // Print the guess
        print_word(guess, wordsize, status); // TODO #6 as below

        // if they guessed it exactly right, set terminate loop
        if (score == EXACT * wordsize)
        {
            won = true;
            point = i;
            break;
        }
    }

    // Print the game's result
    // TODO #7
    if (won)
    {
        printf("You won!\n");
    }
    else
    {
        printf("You lose. Your score is %i/6.\n", point);
        printf("The word is %s\n", choice);
    }

    // that's all folks!
    return 0;
}

string get_guess(int wordsize)
{
    string guess = "";

    // ensure users actually provide a guess that is the correct length
    // TODO #3
    int guess_length = strlen(guess); // find guess word length
    while (strlen(guess) != wordsize) // if not equal wordsize, ask again
    {
        printf("Input a %i-letter word: ", wordsize);
        guess = get_string("");
    }
    return guess; // if equal wordsize, continye
}

int check_word(string guess, int wordsize, int status[], string choice)
{
    int score = 0;

    // compare guess to choice and score points as appropriate, storing points in status
    // TODO #5

    // HINTS
    // iterate over each letter of the guess
    // iterate over each letter of the choice
    // compare the current guess letter to the current choice letter
    // if they're the same position in the word, score EXACT points (green) and break so you don't compare that letter further
    // if it's in the word, but not the right spot, score CLOSE point (yellow)
    // keep track of the total score by adding each individual letter's score from above
    for (int guess_letterposition = 0; guess_letterposition < wordsize; guess_letterposition++)
    {
        if (guess[guess_letterposition] == choice[guess_letterposition]) // same alphabet, same position
        {
            status[guess_letterposition] = EXACT; // same alphabet, same position
            score += EXACT;                       // same alphabet, same position
        }
        else
        {
            for (int choice_letterposition = 0; choice_letterposition < wordsize;
                 choice_letterposition++) // check guess alphabet with all choice alphabet
            {
                if (guess[guess_letterposition] == choice[choice_letterposition] &&
                    status[guess_letterposition] != EXACT) // same alphabet, diff position and not exact
                {
                    status[guess_letterposition] = CLOSE;
                    score += CLOSE;
                    break;
                }
            }
        }
    }

    return score;
}

void print_word(string guess, int wordsize, int status[])
{
    // print word character-for-character with correct color coding, then reset terminal font to normal
    // TODO #6 print color for each char (ตาม status)
    for (int i = 0; i < wordsize; i++)
    {
        if (status[i] == EXACT)
        {
            printf(GREEN "%c" RESET, guess[i]);
        }
        else if (status[i] == CLOSE)
        {
            printf(YELLOW "%c" RESET, guess[i]);
        }
        else
        {
            printf(RED "%c" RESET, guess[i]);
        }
    }

    printf("\n");
    return;
}
