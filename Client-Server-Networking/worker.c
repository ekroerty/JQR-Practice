#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "./worker.h"

fighter_t * decide_winner(fighter_t * p_fighter1, fighter_t * p_fighter2)
{
    srand(time(NULL));
    int round = 0;
    printf("Fighter Stats:\n\n");
    printf("FIGHTER 1\nName: %s | Attack: %d | Dodge: %d | Luck: %d\n",
        p_fighter1->name, p_fighter1->attack, p_fighter1->dodge, p_fighter1->luck);
    printf("FIGHTER 2\nName: %s | Attack: %d | Dodge: %d | Luck: %d\n\n",
        p_fighter2->name, p_fighter2->attack, p_fighter2->dodge, p_fighter2->luck); 
    while ((p_fighter1->health > 0) && (p_fighter2->health > 0))
    {
        round++;
        printf("ROUND %d\n", round);

        int rand1 = rand()%100;
        int rand2 = rand()%100;
        int rand3 = rand()%100;
        int rand4 = rand()%100;
        
        if (rand1 > p_fighter2->dodge) //fighter2 didn't dodge
        {
            if (rand2 <= p_fighter1->luck)
            {
                printf("%s's attack doubled!\n", p_fighter1->name);
                p_fighter2->health -= 2*p_fighter1->attack; //fighter1 attack doubles
            }
            else
            {
                p_fighter2->health -= p_fighter1->attack;
            }
        }

        if (rand3 > p_fighter1->dodge) //fighter1 didn't dodge
        {
            if (rand4 <= p_fighter2->luck)
            {
                printf("%s attack doubled!\n", p_fighter2->name);
                p_fighter1->health -= 2*p_fighter2->attack; //fighter2 attack doubles
            }
            else
            {
                p_fighter1->health -= p_fighter2->attack;
            }
        }

        printf("%s's health: %d............%s's health: %d\n\n", 
                    p_fighter1->name, p_fighter1->health, p_fighter2->name, p_fighter2->health);
    }

    fighter_t * winner = NULL;
    if (p_fighter1->health > p_fighter2->health)
    {
        winner = p_fighter1;
    }
    else if (p_fighter1->health < p_fighter2->health)
    {
        winner = p_fighter2;
    }
    else
    {
        winner = NULL;
    }

    return winner;
}

int main()
{
    fighter_t * f1 = calloc(1, sizeof(fighter_t));
    fighter_t * f2 = calloc(1, sizeof(fighter_t));
    f1->name = "Emma";
    f1->attack = 40;
    f1->dodge = 10;
    f1->luck = 50;
    f1->health = 100;
    f2->name = "Matt";
    f2->attack = 70;
    f2->dodge = 10;
    f2->luck = 20;
    f2->health = 100;
    fighter_t * winner = decide_winner(f1, f2);
    if (!winner)
    {
        printf("%s vs. %s\nDRAW!\n\n", f1->name, f2->name);
    }
    if (winner && f1 && f2)
    {
        printf("%s vs. %s\nWINNER: %s\n\n", f1->name, f2->name, winner->name);
    }
    free(f1);
    free(f2);
}