/*
OS Programming Lab 10: Next fit memory management algorithm.
20BCS041
Mohammed Abbas Ansari
*/
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 10");
    printf("\nEnter Number of blocks: ");
    int numBlocks;
    scanf("%d%*c", &numBlocks);
    int *blocks = (int *)malloc(numBlocks * sizeof(int));
    int *flags = (int *)malloc(numBlocks * sizeof(int));
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
    int allocatedIdx = -1;
    for (int i = 0; i < numProcs; i++)
    {
        int blocksChecked = 0;
        int j = 0;
        if(allocatedIdx != -1)
            j = allocatedIdx;
        while(blocksChecked != numBlocks)
        {
            if (flags[j] == 0 && blocks[j] >= procs[i])
            {
                // found a memory block that can fit the process
                allocation[j] = i;
                flags[j] = 1;
                allocatedIdx = j;
                numAllocated++;
                break;
            }
            j = (j+1)%numBlocks; // cyclic increment
            blocksChecked++;
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
            printf("\n%d \t\t   %d \t\t   %d(P%d) \t\t %d", i, blocks[i], procs[allocation[i]], allocation[i], internalFrag);
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
OS Program 10
Enter Number of blocks: 5
Enter memory blocks (seperated by a space):200 100 300 400 500

Enter number of processes: 4
Enter memory of processes (seperated by a space):450 210 100 250

Block No         Size of Block   Process Allocated       IF
0                  200             ---                   0
1                  100             ---                   0
2                  300             210(P1)               90
3                  400             100(P2)               300
4                  500             450(P0)               50
Total internal fragmentation: 440
Total External Fragmentation: 300

*/