#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

const int PAGE_SIZE = 256;

void read_from_disk(int page_number, int frame, char memory[PAGE_SIZE][PAGE_SIZE]) {
    ifstream backing_store_file("BACKING_STORE.bin", ios::in | ios::binary);

    if (!backing_store_file.is_open()) {
        cout << "Failed to open BACKING_STORE.bin\n";
        exit(1);
    }


    char buffer[PAGE_SIZE] = {0};
    backing_store_file.seekg(page_number * PAGE_SIZE);
    backing_store_file.read(buffer, sizeof(buffer));

    for (int i = 0; i < PAGE_SIZE; i++) {
        memory[frame][i] = buffer[i];
    }

    backing_store_file.close();
}

int main() {
    vector<string> logical_address_strings;
    int open_frame = 0;

    ifstream logical_address_file("./addresses.txt", ios::in);
    if (!logical_address_file.is_open()) {
        cout << "Failed to open file.\n";
        return 1;
    }

    string logical_address_string;
    while (logical_address_file >> logical_address_string) {
        logical_address_strings.emplace_back(logical_address_string.substr(16, 16));
    }

    logical_address_file.close();

    const int NUMBER_OF_PAGE_ENTRIES = 256;

    vector<int> page_table(NUMBER_OF_PAGE_ENTRIES, -1);
    unordered_map<int, int> tlb;
    char memory[NUMBER_OF_PAGE_ENTRIES][PAGE_SIZE];

    for (string logical_address_string: logical_address_strings) {
        cout << "Logical address: " << stoi(logical_address_string, 0, 2) << "\t";
        int frame;

        int page_number = stoi(logical_address_string.substr(0, 8), 0, 2);
        int page_offset = stoi(logical_address_string.substr(8, 8), 0 ,2);

        bool is_hit = tlb.find(page_number) != tlb.end();

        if (is_hit) {
            frame = tlb[page_number];
        } else if (page_table[page_number] == -1) {
            int physical_address = open_frame * PAGE_SIZE + page_offset;
            read_from_disk(page_number, open_frame, memory);

            frame = open_frame;
            open_frame++;
            page_table[page_number] = open_frame;
            tlb[page_number] = open_frame;
        }

        cout << "Physical address: " << frame * PAGE_SIZE + page_offset << "\t Value: " << memory[frame] << endl;
        // cout << "Physical address: " << frame * PAGE_SIZE + page_offset << endl;
    }

    return 0;
}