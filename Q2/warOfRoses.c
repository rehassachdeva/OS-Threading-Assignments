#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

#define MAX 10000
int loops = 10;
int size;
int num = 0;
int end[2], front[2];
int tot = 0;


int type;
time_t t;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void pc_init() {
        int i;
        end[0] = end[1] = front[0] = front[1] = 0;
        srand((unsigned) time(&t));
}

typedef struct queue {
        int id, in;
} queue;

queue q[MAX][2];


pthread_cond_t wait_for_turn[MAX];

void *soldier(void *arg) {

        int i, id = (int)arg, category = (rand() % 2);

        pthread_mutex_lock(&mutex);
        q[end[category]][category].id = id;
        q[end[category]++][category].in = tot++;     

        if(tot == 1 || num==0) type = category;

        if(category == 0) 
                printf("Arrived soldier %d of York\n", id);
        else
                printf("Arrived soldier %d of Lanncaster\n", id);
        printf("-------------------------------------------\n");

        if(type == 1 - category || q[front[category]][category].id != id || num == size) {
                pthread_cond_wait(&wait_for_turn[id], &mutex);
        }
        if(end[category] > front[category] ) 
                front[category]++;
        num++;
        type = category;

        if(category == 0)
                printf("Enter soldier %d of York\n", id);
        else
                printf("Enter soldier %d of Lanncaster\n", id);
        printf("-------------------------------------------\n");

        if(front[category] < end[category] && num < size) {
                pthread_cond_signal(&wait_for_turn[q[front[category]][category].id]);
        }
        pthread_mutex_unlock(&mutex);


        sleep((rand() % 2) + 1);
        pthread_mutex_lock(&mutex);
        num--;

        if(category == 0)
                printf("Leaving soldier %d of York\n", id);
        else printf("Leaving soldier %d of Lanncaster\n", id);
        printf("-------------------------------------------\n");

        if(num == 0) {
                if((front[0] < end[0]) && (front[1] < end[1])) {
                        if(q[front[0]][0].in < q[front[1]][1].in) {
                                type = 0;
                                pthread_cond_signal(&wait_for_turn[q[front[0]][0].id]);
                        }
                        else {
                                type = 1;
                                pthread_cond_signal(&wait_for_turn[q[front[1]][1].id]);
                        }
                }
                else if(front[0] < end[0]) {
                        type = 0;
                        pthread_cond_signal(&wait_for_turn[q[front[0]][0].id]);
                }
                else if(front[1] < end[1]) {
                        type = 1;
                        pthread_cond_signal(&wait_for_turn[q[front[1]][1].id]);
                }
        }
        else if(front[category] < end[category]) {
                pthread_cond_signal(&wait_for_turn[q[front[category]][category].id]);
        }
        pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[]) {

        int i, n;

        if (argc != 3) {
                fprintf(stderr, "usage: n (size of inn) n (number of soldiers) \n");
                exit(1);
        }

        pthread_t threads[MAX];

        n = atoi(argv[1]);
        size = n;

        loops = atoi(argv[2]);

        pc_init();

        for(i=0; i < loops; i++) {
                pthread_create(&threads[i], NULL, soldier, i);
                sleep(rand() %2);
        }


        for(i=0; i < loops; i++)
                pthread_join(threads[i], NULL);

        return 0;
}
