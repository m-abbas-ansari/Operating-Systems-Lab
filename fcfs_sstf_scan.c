/*
OS Programming Lab 15: FCFS, SSTF, and SCAN (Elevator) disk schedulling algorithms
20BCS041
Mohammed Abbas Ansari
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 15");
    int choice = 0;
    while(1) {
        printf("\n\nSelect which Disk Schedulling Algorithm to execute: ");
        printf("\n1. FCFS");
        printf("\n2. SSTF");
        printf("\n3. SCAN (Elevator)");
        printf("\nEnter -1 to exit");
        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);
        if (choice != -1 && choice < 1 || choice > 3)
        {
            printf("\nInvalid choice");
            return 1;
        }
        else if (choice == -1)
        {
            printf("\nExiting...");
            return 0;
        }

        printf("\n\nEnter the number of tracks: ");
        int n_tracks;
        scanf("%d", &n_tracks);
        printf("\nEnter the number of requests: ");
        int n_requests;
        scanf("%d", &n_requests);
        printf("\nEnter the requests sequence (seperated by commas): ");
        int *requests = (int *)malloc(n_requests * sizeof(int));
        for (int i = 0; i < n_requests; i++)
        {
            scanf("%d,", &requests[i]);
            if (requests[i] > n_tracks)
            {
                printf("\nInvalid request: %d", requests[i]);
                return 1;
            }
        }
        scanf("%*c");
        printf("\nEnter the starting head position: ");
        int start_track;
        scanf("%d", &start_track);
        
        int current_track = start_track;
        int next_track;
        int total_head_movements = 0;
        switch (choice)
        {
        case 1:
            printf("\nFCFS: ");
            total_head_movements = 0;
            for (int i = 0; i < n_requests; i++)
            {
                int head_movement = abs(current_track - requests[i]);
                printf("\n%d -> %d [%d tracks changed]", current_track, requests[i], head_movement);
                total_head_movements += head_movement;
                current_track = requests[i];
            }
            printf("\nAverage Head Movement: %.2f", (float)total_head_movements / n_requests);
            break;

        case 2:
            printf("\nSSTF: ");
            total_head_movements = 0;
            current_track = start_track;
            int next_track_idx;
            int head = 0; // points to the start idx for the pool of requests not handled yet
            for (int i = 0; i < n_requests; i++)
            {
                int min_head_movement = __INT_MAX__;
                // only look at the unhandled requests to find the next closest track
                for (int j = head; j < n_requests; j++) 
                {
                    int head_movement = abs(current_track - requests[j]);
                    if (head_movement < min_head_movement && head_movement != 0)
                    {
                        min_head_movement = head_movement;
                        next_track_idx = j;
                    }
                }
                int closest_track = requests[next_track_idx];
                printf("\n%d -> %d [%d tracks changed]", current_track, closest_track, min_head_movement);
                total_head_movements += min_head_movement;
                current_track = closest_track;
                requests[next_track_idx] = requests[head];
                head++;
            }
            printf("\nAverage Head Movement: %.2f", (float)total_head_movements / n_requests);
            break;

        case 3:
            printf("\nEnter direction of head movement (0 for up, 1 for down): ");
            int direction;
            scanf("%d", &direction);
            if (direction != 0 && direction != 1)
            {
                printf("\nInvalid direction");
                return 1;
            }
            printf("\nSCAN (Elevator): ");
            // sorting the requests array
            int dir_flag;
            if (direction == 0)
            {
                dir_flag = 1; // if going up, we will sort the requests array in ascending order
            }
            else
            {
                dir_flag = -1; // if going down, we will sort the requests array in descending order
            }
            for (int i = 0; i < n_requests; i++) // bubble sort
            {
                int flag = 0;
                for (int j = 0; j < n_requests - 1; j++)
                {
                    if (dir_flag * requests[j] > dir_flag * requests[j + 1])
                    {
                        int temp = requests[j];
                        requests[j] = requests[j+1];
                        requests[j+1] = temp;
                        flag = 1;
                    }
                }
                if (flag == 0)
                    break;
            }
            // printf("\nSorted Requests: ");
            // for(int i = 0; i < n_requests; i++)
            // {
            //     printf("%d ", requests[i]);
            // }
            current_track = start_track;
            total_head_movements = 0;
            //int next_track_idx;
            for (int i = 0; i < n_requests; i++)
            {
                if (dir_flag * current_track > dir_flag * requests[i])
                {
                    next_track_idx = i;
                }
                else
                {
                    next_track_idx++;
                    break;
                }
            }
            //printf("\nNext closest track: %d", requests[next_track_idx]);
            for (int i = next_track_idx; i < n_requests; i++)
            {
                int head_movement = abs(current_track - requests[i]);
                printf("\n%d -> %d [%d tracks changed]", current_track, requests[i], head_movement);
                total_head_movements += head_movement;
                current_track = requests[i];
            }
            if (direction == 0)
                next_track = n_tracks - 1; // going till last track
            else 
                next_track = 0; // going till first track
            int head_movement = abs(current_track - next_track);
            printf("\n%d -> %d [%d tracks changed]", current_track, next_track, head_movement);
            total_head_movements += head_movement;
            current_track = next_track;
            for(int i = next_track_idx - 1; i >= 0; i--)
            {
                int head_movement = abs(current_track - requests[i]);
                printf("\n%d -> %d [%d tracks changed]", current_track, requests[i], head_movement);
                total_head_movements += head_movement;
                current_track = requests[i];
            }
            printf("\nAverage Head Movement: %.2f", (float)total_head_movements / n_requests);
            break;
        default:
            printf("\nInvalid choice");
            return 1;
        }
    }
    
    return 0;
}
/*
Output:
➜  OS ./disk                 

20BCS041 Mohammed Abbas Ansari
OS Program 15

Select which Disk Schedulling Algorithm to execute: 
1. FCFS
2. SSTF
3. SCAN (Elevator)
Enter -1 to exit

Enter your choice: 1


Enter the number of tracks: 200

Enter the number of requests: 7

Enter the requests sequence (seperated by commas): 93,176,42,148,27,14,180

Enter the starting head position: 55

FCFS: 
55 -> 93 [38 tracks changed]
93 -> 176 [83 tracks changed]
176 -> 42 [134 tracks changed]
42 -> 148 [106 tracks changed]
148 -> 27 [121 tracks changed]
27 -> 14 [13 tracks changed]
14 -> 180 [166 tracks changed]
Average Head Movement: 94.43

Select which Disk Schedulling Algorithm to execute: 
1. FCFS
2. SSTF
3. SCAN (Elevator)
Enter -1 to exit

Enter your choice: 2


Enter the number of tracks: 100

Enter the number of requests: 10

Enter the requests sequence (seperated by commas): 45,21,67,90,4,89,52,61,87,25

Enter the starting head position: 50

SSTF: 
50 -> 52 [2 tracks changed]
52 -> 45 [7 tracks changed]
45 -> 61 [16 tracks changed]
61 -> 67 [6 tracks changed]
67 -> 87 [20 tracks changed]
87 -> 89 [2 tracks changed]
89 -> 90 [1 tracks changed]
90 -> 25 [65 tracks changed]
25 -> 21 [4 tracks changed]
21 -> 4 [17 tracks changed]
Average Head Movement: 14.00

Select which Disk Schedulling Algorithm to execute: 
1. FCFS
2. SSTF
3. SCAN (Elevator)
Enter -1 to exit

Enter your choice: 3


Enter the number of tracks: 200

Enter the number of requests: 8

Enter the requests sequence (seperated by commas): 98,183,41,122,14,124,65,67

Enter the starting head position: 53

Enter direction of head movement (0 for up, 1 for down): 0

SCAN (Elevator): 
53 -> 65 [12 tracks changed]
65 -> 67 [2 tracks changed]
67 -> 98 [31 tracks changed]
98 -> 122 [24 tracks changed]
122 -> 124 [2 tracks changed]
124 -> 183 [59 tracks changed]
183 -> 199 [16 tracks changed]
199 -> 41 [158 tracks changed]
41 -> 14 [27 tracks changed]
Average Head Movement: 41.38

Select which Disk Schedulling Algorithm to execute: 
1. FCFS
2. SSTF
3. SCAN (Elevator)
Enter -1 to exit

Enter your choice: -1

Exiting...% 
*/