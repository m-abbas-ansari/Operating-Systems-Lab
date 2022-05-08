/*
OS Programming Lab 9: First fit memory management algorithm.
20BCS041
Mohammed Abbas Ansari
*/
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 9");
    printf("\nEnter Number of blocks: ");
    int numBlocks;
    scanf("%d%*c", &numBlocks);
    int *blocks = (int *)malloc(numBlocks * sizeof(int)); // array of memory blocks
    int *flags = (int *)malloc(numBlocks * sizeof(int)); // array of flags indicating whether or not filled
    int *allocation = (int *)malloc(numBlocks * sizeof(int)); 

    printf("Enter memory blocks (seperated by a space):");
    for (int i = 0; i < numBlocks; i++)
    {
        scanf("%d%*c", &blocks[i]);
        flags[i] = 0;
        allocation[i] = -1;
    }
    scanf("%*c");
    printf("\nEnter number of processes: ");
    int numProcs;
    scanf("%d%*c", &numProcs);
    int *procs = (int *)malloc(numProcs * sizeof(int));

    printf("Enter memory of processes (seperated by a space):");
    for (int i = 0; i < numProcs; i++)
    {
        scanf("%d%*c", &procs[i]);
    }
    int numAllocated = 0;
    for (int i = 0; i < numProcs; i++)
    {
        for (int j = 0; j < numBlocks; j++)
        {
            if (flags[j] == 0 && blocks[j] >= procs[i])
            {
                // found a memory block that can fit the process
                allocation[j] = i;
                flags[j] = 1;
                numAllocated++;
                break;
            }
        }
    }
    printf("\nBlock No \t Size of Block \t Process Allocated \t IF");
    int totInternalFrag = 0;
    int totExternalFrag = 0;
    for (int i = 0; i < numBlocks; i++)
    {
        if (allocation[i] != -1)
        {
            int internalFrag = blocks[i] - procs[allocation[i]];
            printf("\n%d \t\t   %d \t\t   %d(P%d) \t\t %d", i, blocks[i], procs[allocation[i]], i, internalFrag);
            totInternalFrag += internalFrag;
        }
        else
        {
            printf("\n%d \t\t   %d \t\t   --- \t\t\t 0", i, blocks[i]);
            if (numAllocated < numProcs)
                totExternalFrag += blocks[i];
        }
    }
    printf("\nTotal internal fragmentation: %d", totInternalFrag);
    printf("\nTotal External Fragmentation: %d\n", totExternalFrag);
    return 0;
}

/*
Output
20BCS041 Mohammed Abbas Ansari
OS Program 9
Enter Number of blocks: 5
Enter memory blocks (seperated by a space):200 100 300 400 500 

Enter number of processes: 4
Enter memory of processes (seperated by a space):250 200 100 350 

Block No         Size of Block   Process Allocated       IF
0                  200             200(P0)               0
1                  100             100(P1)               0
2                  300             250(P2)               50
3                  400             350(P3)               50
4                  500             ---                   0
Total internal fragmentation: 100
Total External Fragmentation: 0

20BCS041 Mohammed Abbas Ansari
OS Program 9
Enter Number of blocks: 5
Enter memory blocks (seperated by a space):200 100 300 400 500 

Enter number of processes: 4
Enter memory of processes (seperated by a space):450 210 210 250 

Block No         Size of Block   Process Allocated       IF
0                  200             ---                   0
1                  100             ---                   0
2                  300             210(P2)               90
3                  400             210(P3)               190
4                  500             450(P4)               50
Total internal fragmentation: 330
Total External Fragmentation: 300

*/