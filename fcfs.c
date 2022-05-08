// OS Programming Lab 2: FCFS Scheduling
// 20BCS041 Mohammed Abbas Ansari

#include <stdio.h>
#include <stdlib.h>

struct node
{
    char process[10];
    int bt;  //burst time
    int at;  //arrival time
    int ct;  //completion time
    int wt;  //waiting time = response time for fcfs
    int tat; //turn around time
    struct node *next;
};

void freeq(struct node **start); // function for freeing up nodes of the priority queue

void enqueue(struct node **start)
{
    struct node *new = (struct node *)malloc(sizeof(struct node));
    printf("\nEnter process name: ");
    scanf("%s%*c", new->process);
    printf("Arrival time: ");
    scanf("%d%*c", &new->at);
    printf("Burst time: ");
    scanf("%d%*c", &new->bt);
    new->next = NULL;
    if (!*start)
    {
        //Queue is empty
        *start = new;
    }
    else if (new->at < (*start)->at)
    {
        //Insert at beginning
        new->next = *start;
        *start = new;
    }
    else
    {
        //Queue is not empty
        struct node *t = *start;
        while (t->next && new->at >= t->next->at)
        {
            t = t->next;
        }
        new->next = t->next;
        t->next = new;
    }
}

void fcfs(struct node **start)
{
    struct node *p = *start;
    //struct node *t2 = t1->next;
    // first process in the queue is processed first
    // t1->ct = t1->at + t1->bt;
    // t1->wt = 0;
    // t1->tat = t1->bt;
    int time = 0; //timer begins from the arrival of first process
    while (p)
    {
        if (time < p->at)
        {
            time = p->at;
        }
        time += p->bt;
        p->ct = time;
        p->tat = time - p->at;
        p->wt = p->tat - p->bt;
        p = p->next;
    }
}

void display(struct node *start, int n)
{
    struct node *t = start;
    float total_ct = 0, total_wt = 0, total_tat = 0, total_rt = 0;
    printf("\nOutput:\n");
    printf("\nProcess BT AT CT WT  TAT RT\n");
    while (t)
    {
        printf("%s      %2d %2d %2d %2d  %2d %2d\n", t->process, t->bt, t->at, t->ct, t->wt, t->tat, t->wt);
        total_ct += t->ct;
        total_wt += t->wt;
        total_tat += t->tat;
        total_rt += t->wt;
        t = t->next;
    }
    printf("\nAvg CT = %.2f", total_ct / (float)n);
    printf("\nAvg WT = %.2f", total_wt / (float)n);
    printf("\nAvg TAT = %.2f", total_tat / (float)n);
    printf("\nAvg RT = %.2f", total_rt / (float)n);
}

void ganttChart(struct node *start)
{
    struct node *t = start;
    int time = 0;
    printf("\nGantt Chart:\n");
    while (t)
    {
        if (time == 0 | time < t->at)
        {
            for (int i = 0; i < (t->at - time); i++)
            {
                printf(" ");
            }
            printf("|");
            time = t->at;
        }
        for (int i = 0; i < t->bt - 1; i++)
            printf(" ");
        printf("%s", t->process);
        for (int i = 0; i < t->bt - 1; i++)
            printf(" ");
        printf("|");
        time += t->bt;
        t = t->next;
    }
    t = start;
    time = 0;
    printf("\n");
    while (t)
    {
        if (time == 0 | time < t->at)
        {
            for (int i = 0; i < (t->at - time) - 1; i++)
            {
                printf(" ");
            }
            if (t->ct < 9)
                printf("%d ", t->at);
            else
                printf("%d", t->at);
            time = t->at;
        }
        for (int i = 0; i < 2 * (t->bt) - 1; i++)
            printf(" ");
        if (t->ct < 9)
            printf("%d ", t->ct);
        else
            printf("%d", t->ct);
        time += t->bt;
        t = t->next;
    }
    printf("\n");
}

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari");
    printf("\nOS Program 2");
    struct node *start = NULL; // ready queue
    printf("\nEnter the number of processes: ");
    int n;
    scanf("%d%*c", &n);
    printf("Enter the details of the processes.\n");
    for (int i = 0; i < n; i++)
    {
        enqueue(&start);
        // queuing the processes in the ready queue depening on the arrival time
        // the process that arrives earlier will be at front of the queue to be processes => First come first serve
    }
    fcfs(&start);      // calculating the metrics
    display(start, n); // displaying the table and average metrics
    ganttChart(start); // Gantt Chart
    freeq(&start);     // freeing up memory
    return 0;
}

void freeq(struct node **start)
{
    struct node *t = *start;
    while (t)
    {
        struct node *del = t;
        t = t->next;
        free(del);
    }
}
/*
Output:
20BCS041 Mohammed Abbas Ansari
OS Program 2
Enter the number of processes: 3
Enter the details of the processes.

Enter process name: p1
Arrival time: 0
Burst time: 2

Enter process name: p2
Arrival time: 3
Burst time: 1

Enter process name: p3
Arrival time: 5
Burst time: 6

Output:

Process BT AT CT WT  TAT RT
p1       2  0  2  0   2  0
p2       1  3  4  0   1  0
p3       6  5 11  0   6  0

Avg CT = 5.67
Avg WT = 0.00
Avg TAT = 3.00
Avg RT = 0.00
Gantt Chart:
| p1 | |p2| |     p3     |
0    2 3  4 5           11
*/