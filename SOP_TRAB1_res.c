#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sched.h>

sem_t semA;
sem_t semBC1;
sem_t semBC2;
sem_t semBC3;
int value;

void* A(void *args);
void* B(void *args);
void* C(void *args);
int main() {

sem_init(&semA, 0, 0);
sem_init(&semBC1, 0, 0);
sem_init(&semBC2, 0, 0);

pthread_t threads[3];
int i;

pthread_create(&threads[0],NULL,A,NULL);
pthread_create(&threads[1],NULL,B,NULL);
pthread_create(&threads[2],NULL,C,NULL);

for(i=0;i<3;i++){
    pthread_join(threads[i], NULL);
}

return 0;
}

void* A(void *args) {
    while(1) {
        sem_wait(&semBC1);
        printf("O");
        sem_post(&semA);
        sem_wait(&semBC2);
    }
}

void* B(void *args) {
    while(1) {
        printf("CA");
        sem_post(&semBC1);
        sem_post(&semBC1);
        sem_wait(&semA);
        sem_wait(&semA);
        printf("!\n");
        sem_post(&semBC2);
        sem_post(&semBC2);
    }
}

void* C(void *args) {
    while(1) {
        sem_wait(&semBC1);
        printf("S");
        sem_post(&semA);
        sem_wait(&semBC2);
    }
}