#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/ipc.h>

void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);

key_t key = IPC_PRIVATE;

void swap(int *xp, int *yp)
{
        int temp = *xp;
        *xp = *yp;
        *yp = temp;
}

void selectionSort(int arr[], int n)
{
        int i, j, min_idx;

        // One by one move boundary of unsorted subarray
        for (i = 0; i < n-1; i++)
        {
                // Find the minimum element in unsorted array
                min_idx = i;
                for (j = i+1; j < n; j++)
                        if (arr[j] < arr[min_idx])
                                min_idx = j;

                // Swap the found minimum element with the first element
                swap(&arr[min_idx], &arr[i]);
        }
}

pid_t createChild(int arr[], int l, int r) {
        int status;
        pid_t pid;

        if((pid=fork())<0)
        {
                perror("fork Failed");
                exit(1);
        }

        if(pid==0)
        {
                //printf("%d\n",getpid());
                mergeSort(arr, l, r);
                _exit(0);
        }
        else
        {
                return pid;
                //waitpid(pid, &status, 0);
        }
}

void mergeSort(int arr[], int l, int r)
{
        int s1, s2;
        if(r-l+1 < 5) selectionSort(arr+l, r-l+1);

        else if (l < r)
        {
                int m = l+(r-l)/2; //Same as (l+r)/2 but avoids overflow for large l & h                
                pid_t pid1 = createChild(arr, l, m);
                pid_t pid2 = createChild(arr, m+1, r);
                waitpid(pid1, s1, 0);
                waitpid(pid2, s2, 0);
                merge(arr, l, m, r);
        }
}

/* Function to merge the two haves arr[l..m] and arr[m+1..r] of array arr[] */
void merge(int arr[], int l, int m, int r)
{
        int i, j, k;
        int n1 = m - l + 1;
        int n2 =  r - m;

        /* create temp arrays */
        int L[n1], R[n2];

        /* Copy data to temp arrays L[] and R[] */
        for (i = 0; i < n1; i++)
                L[i] = arr[l + i];
        for (j = 0; j < n2; j++)
                R[j] = arr[m + 1+ j];

        /* Merge the temp arrays back into arr[l..r]*/
        i = 0;
        j = 0;
        k = l;
        while (i < n1 && j < n2)
        {
                if (L[i] <= R[j])
                {
                        arr[k] = L[i];
                        i++;
                }
                else
                {
                        arr[k] = R[j];
                        j++;
                }
                k++;
        }

        /* Copy the remaining elements of L[], if there are any */
        while (i < n1)
        {
                arr[k] = L[i];
                i++;
                k++;
        }

        /* Copy the remaining elements of R[], if there are any */
        while (j < n2)
        {
                arr[k] = R[j];
                j++;
                k++;
        }
}

int main()
{
        int *sharedArray;
        int shmid,i,j,k,status,n;
        scanf("%d",&n);

        if((shmid=shmget(key, sizeof(int) *n, IPC_CREAT | 0666)) < 0)
        {
                perror("Shmget failed");
                exit(1);
        }

        if((sharedArray = shmat(shmid, 0, 0)) == (int *) -1)
        {
                perror("Shmat failed");
                exit(1);
        }

        for(i=0;i<n;i++)
                scanf("%d",&sharedArray[i]);

        mergeSort(sharedArray, 0, n-1);

        for(i=0;i<n;i++)
                printf("%d ",sharedArray[i]);        
       
        printf("\n");
        
        return 0;
}
