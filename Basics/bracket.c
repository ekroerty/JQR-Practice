// #include "./server.h"
#include "./CSD-D-JQR-Project-6-thread-pool/module_1/src/queue.c"

void print_queue(queue_t *);

int enter_arena(int f1, int f2)
{
    if (f1 >= f2)
    {
        printf("%d vs. %d winner: %d\n", f1, f2, f1);
        return f1;
    }
    printf("%d vs. %d winner: %d\n", f1, f2, f2);
    return f2;
}

int perfect_bracket(queue_t * p_queue)
{
    int rounds = ((p_queue->size/4) + 1);
    printf("Rounds: %d\n", rounds);
    for (int i = 0; i < rounds; i++)
    {
        int size = p_queue->size;
        for (int j = 0; j < size/2; j++)
        {
            int * f1 = queue_dequeue(p_queue);
            int * f2 = queue_dequeue(p_queue);
            int * winner = calloc(1, sizeof(int));
            (*winner) = enter_arena(*f1, *f2);
            FREE(f1);
            FREE(f2);
            queue_enqueue(p_queue, winner, free);
        }
        print_queue(p_queue);
    }
    int ret = (*(int *)queue_peek(p_queue));
    queue_destroy(&p_queue);
    return ret;
}

int set_bracket(queue_t * p_fight_queue)
{
    if (1 == p_fight_queue->size)
    {
        return *(int *)queue_peek(p_fight_queue);
    }
    if (2 == p_fight_queue->size)
    {
        int f1 = *(int *)queue_dequeue(p_fight_queue);
        int f2 = *(int *)queue_dequeue(p_fight_queue);
        int p_win_int = enter_arena(f1, f2);
        return p_win_int;

    }
    if (p_fight_queue->size % 4 == 0)
    {
        return perfect_bracket(p_fight_queue);
    }
    queue_t * winner_queue = queue_create();
    if (3 == p_fight_queue->size % 4)
    {
        int * frst_rd_bye = calloc(1, sizeof(int));
        (*frst_rd_bye) = *(int *)queue_dequeue(p_fight_queue);
        queue_enqueue(winner_queue, frst_rd_bye, free);
        uint32_t size = p_fight_queue->size;
        for (uint32_t iter = 0; iter < size/2; iter++)
        {
            int f1 = *(int *)queue_dequeue(p_fight_queue);
            int f2 = *(int *)queue_dequeue(p_fight_queue);
            int * t_winner = calloc(1, sizeof(int));
            (*t_winner) = enter_arena(f1, f2);
            queue_enqueue(winner_queue, t_winner, free);
        }
    }
    else if (2 == p_fight_queue->size % 4)
    {
        int * frb1 = calloc(1, sizeof(int));
        (*frb1) = *(int *)queue_dequeue(p_fight_queue);
        int * frb2 = calloc(1, sizeof(int));
        (*frb2) = *(int *)queue_dequeue(p_fight_queue);
        queue_enqueue(winner_queue, frb1, free);
        queue_enqueue(winner_queue, frb2, free);
        uint32_t size = p_fight_queue->size;
        for (uint32_t iter = 0; iter < size/2; iter++)
        {
            int f1 = *(int *)queue_dequeue(p_fight_queue);
            int f2 = *(int *)queue_dequeue(p_fight_queue);
            int * two_winner = calloc(1, sizeof(int));
            (*two_winner) = enter_arena(f1, f2);
            queue_enqueue(winner_queue, two_winner, free);
        }
        print_queue(winner_queue);
    }
    else if (1 == p_fight_queue->size % 4)
    {
        int play_in1 = *(int *)queue_dequeue(p_fight_queue);
        int play_in2 = *(int *)queue_dequeue(p_fight_queue);
        int * play_in_win = calloc(1, sizeof(int));
        (*play_in_win) = enter_arena(play_in1, play_in2);
        queue_enqueue(p_fight_queue, play_in_win, free);
        uint32_t size = p_fight_queue->size;
        for (uint32_t iter = 0; iter < size/2; iter++)
        {
            int f1 = *(int *)queue_dequeue(p_fight_queue);
            int f2 = *(int *)queue_dequeue(p_fight_queue);
            int * o_winner = calloc(1, sizeof(int));
            (*o_winner) = enter_arena(f1, f2);
            queue_enqueue(winner_queue, o_winner, free);
        }
    }

    queue_destroy(&p_fight_queue);
    return perfect_bracket(winner_queue);
}

void print_queue(queue_t * p_queue)
{
    printf("Queue: ");
    if (!p_queue)
    {
        return;
    }
    queue_node_t * temp = p_queue->p_head;
    for (uint32_t iter = 0; iter < p_queue->size; iter++)
    {
        printf("%d ", *(int *)temp->p_data);
        temp = temp->p_next;
    }
    printf("\n");
}

int main()
{
    int a = 1;
    int b = 3;
    int c = 15;
    int d = 7;
    int e = 5;
    int f = 21;
    int g = 6;
    int h = 20;
    // int nums = {1, 3, 15, 7, 5, 21};
    queue_t * p_queue = queue_create();
    queue_enqueue(p_queue, &a, NULL);
    queue_enqueue(p_queue, &b, NULL);
    queue_enqueue(p_queue, &c, NULL);
    queue_enqueue(p_queue, &d, NULL);
    queue_enqueue(p_queue, &e, NULL);
    queue_enqueue(p_queue, &f, NULL);
    print_queue(p_queue);
    int ret = set_bracket(p_queue);
    printf("Bracket winner expected: %d\nActual: %d\n", f, ret);
    // queue_destroy(&p_queue);
}
