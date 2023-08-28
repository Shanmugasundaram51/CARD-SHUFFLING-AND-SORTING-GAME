#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pthread.h>

void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) 
    {
        key = arr[i];
        j = i - 1;
  
        /* Move elements of arr[0..i-1], 
           that are greater than key, 
           to one position ahead of 
           their current position */
        while (j >= 0 && arr[j] > key) 
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

#define NUM_THREADS 4

void *childThread(void *arg) {
    int i, j, k, thread_num = (int)arg;
    int deck[52];
    int suits[4][13], numbers[4][13];
    int status;
    // Shuffle and split the deck into 4 groups of 13
    for(i=0; i<52; i++)
        deck[i] = i;

    for(i=0; i<52; i++) {
        int random = rand() % 52;
        int temp = deck[i];
        deck[i] = deck[random];
        deck[random] = temp;
    }

    for(i=0; i<4; i++) {
        for(j=0; j<13; j++) {
            suits[i][j] = deck[i*13 + j] / 13;
            numbers[i][j] = deck[i*13 + j] % 13;
        }
    }

    // Arrange each group of cards according to the suits
    for(i=0; i<4; i++) {
        for(j=0; j<13; j++) {
            for(k=j+1; k<13; k++) {
                if(suits[i][j] > suits[i][k]) {
                    int temp1 = suits[i][j];
                    suits[i][j] = suits[i][k];
                    suits[i][k] = temp1;
                    int temp2 = numbers[i][j];
                    numbers[i][j] = numbers[i][k];
                    numbers[i][k] = temp2;
                }
            }
        }
    }

    // Sort and print the cards for each suit in this thread's portion of the deck
    int a[13], ai=0;
    int b[13], bi=0;
    int c[13], ci=0;
    int d[13], di=0;
    for(i=(thread_num*13); i<((thread_num+1)*13); i++) {
        if(suits[thread_num][i-(thread_num*13)]==0){
            a[ai]=numbers[thread_num][i-(thread_num*13)];
            ai++;
        }
        if(suits[thread_num][i-(thread_num*13)]==1){
            b[bi]=numbers[thread_num][i-(thread_num*13)];
            bi++;
        }
        if(suits[thread_num][i-(thread_num*13)]==2){
            c[ci]=numbers[thread_num][i-(thread_num*13)];
            ci++;
        }
        if(suits[thread_num][i-(thread_num*13)]==3){
            d[di]=numbers[thread_num][i-(thread_num*13)];
            di++;
        }
      printf("%d-%d ",suits[thread_num][i-(thread_num*13)],numbers[thread_num][i-(thread_num*13)]);
    }
  printf("\n");
    // Sort the cards in each suit using insertion sort
    insertionSort(a, ai);
    insertionSort(b, bi);
    insertionSort(c, ci);
    insertionSort(d, di);

    // Print the sorted cards for this thread's portion of the deck
    printf("Thread %d:\n", thread_num);
    printf("Suit 0: ");
    for(i=0; i<ai; i++)
        printf("%d ", a[i]);
    printf("\n");
    printf("Suit 1: ");
    for(i=0; i<bi; i++)
        printf("%d ", b[i]);
    printf("\n");
    printf("Suit 2: ");
    for(i=0; i<ci; i++)
        printf("%d ", c[i]);
    printf("\n");
    printf("Suit 3: ");
    for(i=0; i<di; i++)
    printf("%d ", d[i]);
    printf("\n");

pthread_exit(NULL);
}

int main() {
pthread_t threads[4];
int thread_nums[4] = {0, 1, 2, 3};
int i, status;
for(i=0; i<4; i++) {
status = pthread_create(&threads[i], NULL, childThread, &thread_nums[i]);
  printf("\n");
printf("Thread created with ID: %ld\n", (long)threads[i]);
if(status != 0) {
printf("Error creating thread!\n");
exit(-1);
}
}
for(i=0; i<4; i++) {
pthread_join(threads[i], NULL);
}
return 0;
}
