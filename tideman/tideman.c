#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
// เก็บเป็น boolean ว่าใครชนะใคร
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    // print pair before sorting
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("pair %i - %i is %i \n", pairs[i].winner, pairs[i].loser, preferences[pairs[i].winner][pairs[i].loser]);
    // }
    sort_pairs();
    // print pair after sorting
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("pair %i - %i is %i \n", pairs[i].winner, pairs[i].loser, preferences[pairs[i].winner][pairs[i].loser]);
    // }
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    // look for candidate called 'name'
    // if candidate found, update 'ranks' and return 'true'. 'ranks[i]' is the voter's ith perference
    // if no candidate found, dont update any ranks and return false
    bool voted = false;
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0) // look for candidate called 'name' and candidate found
        {
            ranks[rank] = i; // update 'ranks'
            voted = true;    // and return 'true'
        }
    }
    return voted; // candidate not found return false
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // update the preferences array based on the current voter's ranks
    // i j คือดำแหน่งในตาราง
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i; j < candidate_count; j++)
        {
            // ต้องเชคก่อนว่าใครอยุก่อน
            if (i != j)
            {
                preferences[ranks[i]][ranks[j]] += 1; // ถ้า rank i ชนะ j ให้ +1 ให้ตำแหน่งนั้น
            }
            // printf("pref %i over %i %d\n", i, j, preferences[i][j]);
        }
    }

    // PRINT preference table (matrix)
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%i ", preferences[i][j]);
    //         if ((j + 1) % 3 == 0)
    //         {
    //             printf("\n");
    //         }
    //     }
    // }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // add each pair of candidates to 'pairs' array if one candidate is preferred over the other
    // update global variable 'pair_count' to be the total number of pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] >
                preferences[j][i]) // เทียบแต่ละคู่ ว่า i มากกว่า j มั้ย
            {
                // create stucture var of pair
                pair p;
                p.winner = i;          // ถ้าใช่ให้ i เปน winner
                p.loser = j;           // j เปน loser
                pairs[pair_count] = p; // เชคคู่ถัดไป
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int strength[pair_count];
    // find strength by pairs[i].winner - pairs[i].loser
    for (int i = 0; i < pair_count; i++) // ดูแต่ละคู่
    {
        strength[i] =
            preferences[pairs[i].winner][pairs[i].loser] -
            preferences[pairs[i].loser][pairs[i].winner]; // หา strength ของแค่ละคู่
        // printf("strength of %i is %i\n", i, strength[i]);
    }

    int i, j, k, winner1, winner2;
    bool swapped;

    // sort pairs in order by decreasing strength of victory
    for (i = 0; i < pair_count - 1; i++)
    {
        // Bubble sort
        for (j = 0; j < pair_count - i - 1; j++)
        {
            // printf("strength %i %i\n", strength[j], strength[j + 1]);
            // สลับตัวแปร : กรณีที่ตัวถัดไปมากกว่าตัวแรก
            if (strength[j] < strength[j + 1])
            {
                // swap pairs
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
                // swap strength
                int t = strength[j];
                strength[j] = strength[j + 1];
                strength[j + 1] = t;
                // printf("pairs j is %i\n", pairs[j].winner);
            }
        }
    }
}

// Sorted Pairs = [(d, a), (a, b), (b, c), (c, a), (d, b), (d, c)]  ตย ของ 4 candidates
bool check(int winner, int loser)
{
    if (winner == loser)
    {
        return false; // ถ้าเจอว่าคนเดียวกันให้ false เลย
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner])
        {
            return check(i, loser); // recursive โดยเอา loser ของคู่ที่เจอไปเช็คใหม่
        }
    }
    return true; // ถ้าไม่เจอว่าคนเดียวกันให้คู่นั้น true
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // Update 'locked' to create the locked graph by adding all edges in decreasing order of victory strength, as long as there is
    // no cycle
    for (int i = 0; i < pair_count; i++)
    {
        int current_winner = pairs[i].winner;
        int current_loser = pairs[i].loser;
        if (check(current_winner, current_loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        else
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }

    // print locked table (matrix)
    // for (int i = 0; i < pair_count; i++)
    // {
    //     for (int j = 0; j < pair_count; j++)
    //     {
    //         printf("%i ", locked[i][j]);
    //         if ((j + 1) % 3 == 0)
    //         {
    //             printf("\n");
    //         }
    //     }
    // }
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // Print out the winner of the election, who will be the source of the graph.
    // You may assume there will not be more than one source.
    bool candidate_won[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        candidate_won[i] = true;
    }
    for (int i = 0; i < candidate_count; i++) // i is row
    {
        for (int j = 0; j < candidate_count; j++) // j is col
        {
            if (locked[j][i] == true)
            {
                candidate_won[i] = false;
            }
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidate_won[i])
        {
            printf("%s\n", candidates[i]);
        }
    }
}