#include <iostream>
#include <random>
#include <ctime>
#include <pthread.h>
#include <unistd.h>

using namespace std;

int north_farmer_number = 1;
int south_farmer_number = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

default_random_engine generator(time(NULL));
uniform_int_distribution<int> uniform_distribution(1, 3);

void cross(string village, int number) {
    int time_to_cross = uniform_distribution(generator);
    cout << village << " farmer " << number << " is crossing the bridge ...\n";
    sleep(time_to_cross);
}

void* cross_to_south(void* arg) {
    pthread_mutex_lock(&mutex);
    cout << "North farmer " << north_farmer_number << " can cross the bridge.\n";
    cross("North", north_farmer_number);
    cout << "North farmer " << north_farmer_number << " has left the bridge.\n";
    north_farmer_number++;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* cross_to_north(void* arg) {
    int *number = (int *)arg;
    pthread_mutex_lock(&mutex);
    cout << "South farmer " << south_farmer_number << " can cross the bridge.\n";
    cross("South", south_farmer_number);
    cout << "South farmer " << south_farmer_number << " has left the bridge.\n";
    south_farmer_number++;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main() {
    const int NUMBER_OF_FARMERS = 5;
    pthread_t north_farmers[NUMBER_OF_FARMERS];
    pthread_t south_farmers[NUMBER_OF_FARMERS];
    
    pthread_mutex_init(&mutex, 0);

    for (int i = 0; i < NUMBER_OF_FARMERS; i++) {
        pthread_create(&north_farmers[i], NULL, cross_to_south, NULL);
        pthread_create(&south_farmers[i], NULL, cross_to_north, NULL);
    }

    for (int i = 0; i < NUMBER_OF_FARMERS; i++) {
        pthread_join(north_farmers[i], NULL);
        pthread_join(south_farmers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}