/*
OS Programming Lab 13: First In First Out(FIFO) page replacement algorithm
20BCS041
Mohammed Abbas Ansari
*/
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 13");
    printf("\nEnter the number of pages in reference string: ");
    int n_pages;
    scanf("%d", &n_pages);
    printf("\nEnter the input reference string (seperated by commas): ");
    int *ref_string = (int *)malloc(n_pages * sizeof(int));
    for (int i = 0; i < n_pages; i++)
    {
        scanf("%d,", &ref_string[i]);
    }
    scanf("%*c");
    printf("\nEnter the number of frames: ");
    int n_frames;
    scanf("%d", &n_frames);
    int *frames = (int *)malloc(n_frames * sizeof(int));
    for (int i = 0; i < n_frames; i++)
    {
        frames[i] = -1;
    }
    // First in First Out Algorithm
    int fifo_index = -1;
    int num_faults = 0;
    for (int i = 0; i < n_pages; i++)
    {
        int flag = 0;
        for (int j = 0; j < n_frames; j++)
        {
            if (ref_string[i] == frames[j])
            {
                flag = 1; // page hit
                break;
            }
        }
        if(flag == 1)
        {
            printf("\nPage %d hit", ref_string[i]);
        }
        else
        {
            printf("\nPage %d fault.", ref_string[i]);
            num_faults++;
            int empty_flag = 0;
            for (int j = 0; j < n_frames; j++)
            {
                if (frames[j] == -1)
                {
                    frames[j] = ref_string[i];
                    if(fifo_index == -1)
                        fifo_index = j;
                    empty_flag = 1;
                    break;
                }
            }
            if(empty_flag == 0)
            {
                int temp = frames[fifo_index];
                frames[fifo_index] = ref_string[i];
                fifo_index = (fifo_index + 1) % n_frames;
                printf(" Page %d replaced by %d", temp, ref_string[i]);
            }
        }
        printf("\nFrames: |");
        for(int j = 0; j < n_frames; j++)
        {
            if(frames[j] == -1)
                printf(" - |");
            else
                printf(" %d |", frames[j]);
        }
        printf("\n");
    }
    printf("\nNumber of page faults: %d", num_faults);
    printf("\nAverage page faults: %f\n", (float)num_faults / n_pages);
    return 0;
}
/*
Output:
➜  OS ./fifo            

20BCS041 Mohammed Abbas Ansari
OS Program 13
Enter the number of pages in reference string: 15

Enter the input reference string (seperated by commas): 7,0,1,2,0,3,0,4,2,3,0,3,1,2,0,

Enter the number of frames: 3

Page 7 fault.
Frames: | 7 | - | - |

Page 0 fault.
Frames: | 7 | 0 | - |

Page 1 fault.
Frames: | 7 | 0 | 1 |

Page 2 fault. Page 7 replaced by 2
Frames: | 2 | 0 | 1 |

Page 0 hit
Frames: | 2 | 0 | 1 |

Page 3 fault. Page 0 replaced by 3
Frames: | 2 | 3 | 1 |

Page 0 fault. Page 1 replaced by 0
Frames: | 2 | 3 | 0 |

Page 4 fault. Page 2 replaced by 4
Frames: | 4 | 3 | 0 |

Page 2 fault. Page 3 replaced by 2
Frames: | 4 | 2 | 0 |

Page 3 fault. Page 0 replaced by 3
Frames: | 4 | 2 | 3 |

Page 0 fault. Page 4 replaced by 0
Frames: | 0 | 2 | 3 |

Page 3 hit
Frames: | 0 | 2 | 3 |

Page 1 fault. Page 2 replaced by 1
Frames: | 0 | 1 | 3 |

Page 2 fault. Page 3 replaced by 2
Frames: | 0 | 1 | 2 |

Page 0 hit
Frames: | 0 | 1 | 2 |

Number of page faults: 12
Average page faults: 0.800000
*/
