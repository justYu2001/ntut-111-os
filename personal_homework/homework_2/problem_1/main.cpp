#include <iostream>
#include <random>
#include <ctime>
#include <pthread.h>

using namespace std;

int points_in_circle = 0;

void* child(void* data) {
    int points = ((int*) data)[0];
    default_random_engine generator(time(NULL));
    uniform_real_distribution<double> uniform_distribution(-1, 1);

    for (int i = 0; i < points; i++) {
        double x = uniform_distribution(generator);
        double y = uniform_distribution(generator);

        if (x * x + y * y <= 1) {
            points_in_circle++;
        }
    }

    pthread_exit(NULL);
}

int main() {
    default_random_engine generator(time(NULL));
    uniform_int_distribution<int> uniform_distribution(0, 1e4);
    int points = uniform_distribution(generator);

    int input[1] = {points};

    pthread_t thread;
    pthread_create(&thread, NULL, child, (void *) input);
    pthread_join(thread, NULL);

    cout << 4.0 * points_in_circle / points <<  "\n";

    return 0;
}