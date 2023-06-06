#include <iostream>
#include <vector>
#include <sstream>
#include <ctime>
#include <random>
#include <algorithm>

using namespace std;

const int NUMBER_OF_REQUESTS = 10;

int convert_characters_to_integer(char* characters) {
    stringstream ss(characters);

    int number = 0;
    ss >> number;

    return number;
}

void fcfs(int initial_position, int* requests) {
    int result = abs(initial_position - requests[0]);

    for (int i = 0; i < NUMBER_OF_REQUESTS - 1; i++) {
        result += abs(requests[i] - requests[i + 1]);
    }

    cout << "FCFS: " << result << endl;
}

void sstf(int initial_position, int* requests) {
    int start = initial_position;

    vector<bool> book(NUMBER_OF_REQUESTS, false);
    int book_counter = 0;

    int result = 0;

    while (book_counter < NUMBER_OF_REQUESTS) {
        int min =  1e8;
        int min_index = 0;

        for (int i = 0; i < NUMBER_OF_REQUESTS; i++) {
            if (book[i]) {
                continue;
            }

            int new_min = abs(requests[i] - start);

            if (min > new_min) {
                min = new_min;
                min_index = i;
            }
        }

        book[min_index] = true;
        result += min;
        book_counter++;
        start = requests[min_index];
    }

    cout << "SSTF: " << result << endl;;
}

void scan(int initial_position, int* requests) {
    int min_position = *min_element(requests, requests + NUMBER_OF_REQUESTS);
    int result = 4999 - initial_position + 4999 - min_position;
    cout << "SCAN: " << result << endl;
}

void cscan(int initial_position, int* requests) {
    int min_position = *min_element(requests, requests + NUMBER_OF_REQUESTS);
    int max_position = *max_element(requests, requests + NUMBER_OF_REQUESTS);

    int result = max_position - initial_position + max_position - min_position;
    cout << "C-SCAN: " << result << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Please input initial position.\n";
        return 0;
    }

    int initial_position = convert_characters_to_integer(argv[1]);

    default_random_engine generator(time(NULL));
    uniform_int_distribution<int> uniform_distribution(0, 5000);

    int requests[NUMBER_OF_REQUESTS];

    cout << "Requests: ";

    for (int i = 0; i < NUMBER_OF_REQUESTS; i++) {
        requests[i] = uniform_distribution(generator);
        
        cout << requests[i];

        if (i != NUMBER_OF_REQUESTS - 1) {
            cout << ", ";
        }
    }

    cout << endl << endl;

    cout << "4999 is the last position\n\n";

    fcfs(initial_position, requests);
    sstf(initial_position, requests);
    scan(initial_position, requests);
    cscan(initial_position, requests);
    return 0;
}