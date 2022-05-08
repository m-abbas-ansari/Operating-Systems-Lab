#include <stdio.h>
#include <stdlib.h>
struct node
{
    char process[10];
    int priority;
    struct node *next;
};
void enqueue(struct node **start)
{
    struct node *new = (struct node *)malloc(sizeof(struct node));
    printf("\nEnter process name: ");
    scanf("%s%*c", new->process);
    printf("Enter priority for the process: ");
    scanf("%d%*c", &new->priority);
    new->next = NULL;
    if (!*start)
    {
        // Queue is empty
        *start = new;
    }
    else if (new->priority < (*start)->priority)
    {
        // Insert at beginning
        new->next = *start;
        *start = new;
    }
    else
    {
        // Queue is not empty
        struct node *t = *start;
        while (t->next && new->priority >= t->next->priority)
        {
            t = t->next;
        }
        new->next = t->next;
        t->next = new;
    }
}
void dequeue(struct node **start)
{
    if (!*start)
    {
        printf("Undrflow");
    }
    else
    {
        struct node *t = *start;
        *start = t->next;
        printf("Executing %s with priority %d", t->process, t->priority);
        free(t);
    }
}
void display(struct node *start)
{
    struct node *t = start;
    printf("\nPriority Queue: ");

    while (t)
    {
        printf("|%s|%d|-> ", t->process, t->priority);
        t = t->next;
    }
    printf("\n");
}
char *rear(struct node *start)
{
    struct node *t = start;
    while (t->next)
    {
        t = t->next;
    }
    return t->process;
}
int total(struct node *start)
{
    struct node *t = start;
    int count = 0;
    while (t)
    {
        t = t->next;
        count++;
    }
    return count;
}
int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari");
    struct node *start = NULL;
    printf("\n==== Scheduling Algorithm using Priority Queue ====");
    printf("\n1. Insert Process name & its priority");
    printf("\n2. Execute Process");
    printf("\n3. Total no of processes");
    printf("\n4. Display");
    printf("\n5. Exit");
    while (1)
    {
        printf("\nEnter choice: ");
        int ch;
        scanf("%d%*c", &ch);
        switch (ch)
        {
        case 1:
            enqueue(&start);
            display(start);
            break;
        case 2:
            dequeue(&start);
            display(start);
            break;
        case 3:
            printf("Total no of processes = %d", total(start));
            break;
        case 4:
            display(start);
            break;
        case 5:

            printf("\nExiting...\n");
            break;
        default:
            printf("Please enter a valid choice");
        }
        if (ch == 5)
            break;
    }
    return 0;
}