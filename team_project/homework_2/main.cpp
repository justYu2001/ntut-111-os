#include <iostream>
#include <queue>
#include <random>
#include <ctime>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

queue<int> que;

sem_t empty, full;

default_random_engine generator(time(NULL));
uniform_int_distribution<int> uniform_distribution(1, 10);

void random_sleep(string name) {
    int time = uniform_distribution(generator);
    cout << name << " is sleeping...\n";
    sleep(time);
    cout << name << " wake up.\n";
}

void* producer(void* data) {
    while (true) {
        random_sleep("Producer");
        int item = uniform_distribution(generator);
        sem_wait(&full);
        que.push(item);
        cout << "Producer produced a items: " << item << "\n";
        sem_post(&empty);
    }

    pthread_exit(NULL);
}

void* consumer(void* data) {
    while (true) {
        random_sleep("Consumer");
        sem_wait(&empty);
        que.pop();
        cout << "Consumer removed a item.\n";
        sem_post(&full);
    }

    pthread_exit(NULL);
}

int main() {
    int buffer_size = 0;

    while (buffer_size == 0) {
        cout << "Set buffer size: ";
        cin >> buffer_size;

        if (buffer_size == 0) {
            cout << "The buffer size must be greater than 0.\n";
        }
    }

    sem_init(&full, 0, buffer_size);
    sem_init(&empty, 0, 0);

    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, NULL, producer, 0);
    pthread_create(&consumer_thread, NULL, consumer, 0);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;
}