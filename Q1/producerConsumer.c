#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

#define MAX 1000
#define MOD 10000007

int buffer[MAX];
int bufptr = 0;
int fill  = 0;
int use[MAX];
int loops = 20;
int size;

sem_t empty[MAX];
sem_t full[MAX];
sem_t mutex;

time_t t;

void put(int value) {
        buffer[fill] = value;
        fill = (fill + 1) % size;
}

int get(int id) {
        int tmp = buffer[use[id]];
        use[id] = (use[id] + 1) % size;
        return tmp;
}

void pc_init() {
        int i;
        for(i = 0; i < size; i++) {
                sem_init(&empty[i], 0, size);
                sem_init(&full[i], 0, 0);
        }
        sem_init(&mutex, 0, 1);
        srand((unsigned) time(&t));
}

void *producer(void *arg) {
        int i, j, data;
        for (i = 0; i < loops; i++) {
                for(j = 0; j < size; j++) sem_wait(&empty[j]);
                sem_wait(&mutex);
                data = rand() % 50;
                printf("Wrote data %d\n", data);
                put(data);
                sem_post(&mutex);
                for(j = 0; j < size; j++) sem_post(&full[j]);
        }
}

void *consumer(void *arg) {
        int i;
        int id = (int)arg;
        for(i = 0; i < loops; i++) {
                sem_wait(&full[id]);
                sem_wait(&mutex);
                int tmp = get(id);
                printf("Read data %d by thread %d\n", tmp, id);
                sem_post(&mutex);
                sem_post(&empty[id]);               
        }
}

int main(int argc, char *argv[]) {

        int i, n;

        if (argc != 3) {
                fprintf(stderr, "usage: n (size of buffer) loops (number of times random data is to be written)\n");
                exit(1);
        }

        pthread_t threads[MAX];

        n = atoi(argv[1]);
        size = n;

        loops = atoi(argv[2]);

        pc_init();

        for(i=0; i<n; i++)
                pthread_create(&threads[i], NULL, consumer, i);

        pthread_create(&threads[n], NULL, producer, NULL);

        for(i=0; i<=n; i++)
                pthread_join(threads[i], NULL);

        return 0;
}
