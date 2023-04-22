#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

int points_in_circle = 0;
sem_t sem;

default_random_engine floating_number_generator(time(NULL));
uniform_real_distribution<double> uniform_floating_number_distribution(-1, 1);

void* child(void* data) {
    double x = uniform_floating_number_distribution(floating_number_generator);
    double y = uniform_floating_number_distribution(floating_number_generator);

    sem_wait(&sem);

    if (x * x + y * y <= 1) {
        points_in_circle++;
    }

    sem_post(&sem);

    pthread_exit(NULL);
}

int main() {
    default_random_engine integer_generator(time(NULL));
    uniform_int_distribution<int> uniform_integer_distribution(0, 1e4);
    int points = uniform_integer_distribution(integer_generator);

    sem_init(&sem, 0, points);

    vector<pthread_t> threads(points);

    for (pthread_t thread : threads) {
        pthread_create(&thread, NULL, child, 0);
        pthread_join(thread, NULL);
    }
    
    cout << 4.0 * points_in_circle / points << "\n";
    sem_destroy(&sem);

    return 0;
}