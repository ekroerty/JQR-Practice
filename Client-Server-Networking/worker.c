#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "./worker.h"
#include "./server.h"
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void decide_winner(void * p_data_void)
{
    fight_data_t * p_data = (fight_data_t *)p_data_void;
    fighter_t ** pp_fighters = p_data->pp_fighter;
    fighter_t * p_fighter1 = pp_fighters[0];
    fighter_t * p_fighter2 = pp_fighters[1];

    srand(time(NULL));
    int round = 0;
    p_fighter1->health = 100;
    p_fighter2->health = 100;
    printf("\n----------------------------BEGIN FIGHT----------------------------\n");
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

    if (winner)
    {
        printf("WINNER: %s!\n", winner->name);
    }
    else
    {
        printf("TIE!\n");
    }
    printf("-----------------------------END FIGHT-----------------------------\n\n");
    
    char buffer[1024];
    char tie[4] = "TIE!";
    int32_t name_len;

    memset(buffer, '\0', sizeof(buffer));
    if (!winner)
    {
        name_len = 4;
        snprintf(buffer, name_len+1, "%s", tie);
    }
    else
    {
        name_len = strnlen(winner->name, 10);
        snprintf(buffer, name_len+1, "%s", winner->name);

    }

    send(p_data->client_fds[0], buffer, name_len, 0);
    send(p_data->client_fds[1], buffer, name_len, 0);
    close(p_data->client_fds[0]);
    printf("[DISCONNECTED] Connection from CLIENT %d closed.\n", p_data->client_fds[0]);
    close(p_data->client_fds[1]);
    printf("[DISCONNECTED] Connection from CLIENT %d closed.\n\n", p_data->client_fds[1]);

}

#if 0
int main()
{
    fighter_t * f1 = calloc(1, sizeof(fighter_t));
    fighter_t * f2 = calloc(1, sizeof(fighter_t));
    char emma[4] = "Emma";
    strncat(f1->name, emma, 4);
    f1->attack = 40;
    f1->dodge = 10;
    f1->luck = 50;
    // f1->health = 100;
    char matt[4] = "Matt";
    strncat(f2->name, matt, 4);
    f2->attack = 70;
    f2->dodge = 10;
    f2->luck = 20;
    // f2->health = 100;
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
#endif