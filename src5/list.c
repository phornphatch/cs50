#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    string phrase;
    struct node *next;
}
node;

#define LIST_SIZE 2

void unload(node *list);
void visualize(node *list);

int main(void)
{
    node *list = NULL;

    // Add items to list
    for (int i = 0; i < LIST_SIZE; i++)
    {
        string phrase = get_string("Enter a new phrase: ");

        // TODO: add phrase to new node in list ****
        node *n = malloc(sizeof(node)); // create some space to new node
        if (n == NULL)
        {
            printf("Couldn't allocate memory for node\n");
            return 1;
        }
        n->phrase = phrase;
        n->next = list; // update to the head of the list

        list = n;

        // Visualize list after adding a node.
        visualize(list);
    }

    unload(list);
}

void unload(node *list)
{
    // TODO: Free all allocated nodes ***** ถ้าไม่ unloading ตรงนี้ จะ memory leak
    while (list != NULL) // ถ้าไม่รู้ว่าเมื่อไรถึงจะจบแน่นอน ใช้ while loop ซะ
    {
        node *ptr = list->next;
        free(list);
        list = ptr;
    }
}

void visualize(node *list)
{
    printf("\n+-- List Visualizer --+\n\n");
    while (list != NULL)
    {
        printf("Location %p\n", list);
        printf("Phrase: \"%s\"\n", list->phrase);
        printf("Next: %p\n\n", list->next);
        list = list->next;
    }
    printf("+---------------------+\n\n");
}