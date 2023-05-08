#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

void print_page_frame(vector<int> frames) {
    for (int i = 0; i < frames.size(); i++) {
        if (frames[i] == -1) {
            break;
        }

        if (i > 0) {
            cout << ", ";
        }

        cout << frames[i];
    }

    cout << endl;
}

void fifo(vector<int> page_reference, int frame_size) {
    int page_fault = 0;

    cout << "FIFO page frames:\n";

    vector<int> page_frame(frame_size, -1);
    set<int> page_set;

    for (int i = 0; i < page_reference.size();) {
        for (int j = 0; j < frame_size; j++, i++) {
            if (page_set.find(page_reference[i]) != page_set.end()) {
                j--;
                continue;
            }

            page_set.erase(page_frame[j]);
            page_frame[j] = page_reference[i];
            page_fault++;
            page_set.insert(page_frame[j]);
            print_page_frame(page_frame);
        }
    }
    
    cout << "\nFIFO page fault=" << page_fault << endl;
}

void lru(vector<int> page_reference, int frame_size) {
    int page_fault = 0;

    cout << "LRU page frames:\n";

    vector<int> page_frame;
    unordered_map<int, list<int>::iterator> hash_table;
    list<int> stack;
    set<int> page_set;

    for (int i = 0; i < page_reference.size(); i++) {
        int page = page_reference[i];

        if (page_set.find(page) != page_set.end()) {
            list<int>::iterator node = hash_table[page];

            if (node != stack.end()) {
                stack.erase(hash_table[page]);
            }

            stack.push_back(page);
            hash_table[page] = --stack.end();

            continue;
        }

        stack.push_back(page);
        hash_table[page] = --stack.end();

        if (page_frame.size() < frame_size) {
            page_frame.emplace_back(page);
        } else {
            int removed_page = stack.front();
            stack.pop_front();
            page_set.erase(removed_page);

             for (int j = 0; j < frame_size; j++) {
                if (page_frame[j] == removed_page) {
                    page_frame[j] = page;
                    break;
                }
            }
        }

        page_set.insert(page);

        print_page_frame(page_frame);
        page_fault++;
    }

    cout << "\nLRU page fault=" << page_fault << endl;
}

void optimal(vector<int> page_reference, int frame_size) {
    int page_fault = 0;

    cout << "Optimal page frames:\n";

    vector<int> page_frame;
    set<int> page_set;

    for (int i = 0; i < page_reference.size(); i++) {
        int page = page_reference[i];

        if (page_set.find(page) != page_set.end()) {
            continue;
        }

        if (page_frame.size() < frame_size) {
            page_frame.emplace_back(page);
        } else {
            int longest_index = 0;
            int removed_index = 0;

            for (int j = 0; j < frame_size; j++) {
                int index = find(page_reference.begin() + i, page_reference.end(), page_frame[j]) - page_reference.begin();
                if (index > longest_index) {
                    removed_index = j;
                    longest_index = index;
                }
            }

            page_set.erase(page_frame[removed_index]);
            page_frame[removed_index] = page;
        }

        page_set.insert(page);
        page_fault++;
        print_page_frame(page_frame);
    }

    cout << "\nOptimal page fault=" << page_fault << endl;
}

int main() {
    default_random_engine generator(time(NULL));
    uniform_int_distribution<int> uniform_distribution(0, 9);

    vector<int> page_reference(20);

    for (int i = 0; i < page_reference.size(); i++) {
        page_reference[i] = uniform_distribution(generator);
    }

    int frame_size;
    cout << "Please input page frame size:";
    cin >> frame_size;
    cout << endl;

    cout << "Page Reference: ";
    print_page_frame(page_reference);
    cout << endl;

    fifo(page_reference, frame_size);
    cout << "----------------------------\n";
    lru(page_reference, frame_size);
    cout << "----------------------------\n";
    optimal(page_reference, frame_size);

    return 0;
}