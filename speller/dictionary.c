// Implements a dictionary's functionality

#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next; // pointer = address of the next node
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;
// larger N means more buckets.
// if your function ends up with a value > N, you can take the value % N toget a value in the appropriate range

// Hash table
node *table[N];

char w_word[100];
int num_word = 0;
int num_free = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // return true if the word is in the dictory, otherwise false
    // case sensitive
    // hash word to obtain a hash value
    // access linked list at that index in the hash table
    // Traverse linked list, looking for the word (strcasecmp : campare to string with case sensitive)
    int word_index = hash(word);
    // start with cursor set to first item in linked list
    node *current_node = table[word_index];
    // printf("current_node->word : %s\n", current_node->next->word);
    // keep moving cursor untill you get NULL, checking each node for the word
    if (current_node == NULL)
    {
        return false;
    }

    while (current_node != NULL)
    {
        // printf("%s - %s\n", current_node->word, word);
        if (strcasecmp(current_node->word, word) == 0)
        {
            return true;
        }
        current_node = current_node->next; // moooooove
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // take a word and input withalphabetical characters and apostopphes
    // output a number corresponding to which "bucket" to store the word in (numerical index between 0 and N -1)
    // each index inside hash table will be linked list (node : value + pointer to next node)
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // open dictionary file
    // use fopen
    FILE *file = fopen(dictionary, "r");
    // remember to check if return value is null
    if (file == NULL)
    {
        printf("No such file found.\n");
        return false;
    }
    // read strings from file one at a time
    //  use fscanf(file, %s , word)
    // fscan will return EOF once it reaches end of file
    while (fscanf(file, "%s", w_word) == 1)
    {
        // create a new node for each word
        // use malloc
        node *new = malloc(sizeof(node));
        new->next = NULL;
        num_word++;
        // remember to check if return value is null
        // copy the word into that node using strcpy function
        strcpy(new->word, w_word);
        // hash word using hash function to obtain a hash value
        // use hash function
        // function takes a string and return an index
        int index = hash(w_word);
        // insert node into hash table at the location
        if (table[index] == NULL)
        {
            table[index] = new;
        }
        else
        {
            node *current_node = table[index];
            current_node->next = table[index]->next;
            while (current_node->next != NULL)
            {
                current_node = current_node->next;
            }
            current_node->next = new;
        }
    }
    fclose(file);
    // recall that hash table is an array of linked list
    // besure to set pointers in the correct order
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO return number of word
    // keep track number of word that you added (เพิ่ม num_word++ ใน load function)
    return num_word;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // อย่าลิมว่าต้องเปา tmp มารับก่อน แล้วเลื่อน cursor แล้วค่อยฟรีตัวนั้นไม่งี้จะเสียไป track of the rest of the linked list
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
            num_free++;
        }
    }
    // printf("word: %i free: %i \n", num_word, num_free);
    return true;
}
