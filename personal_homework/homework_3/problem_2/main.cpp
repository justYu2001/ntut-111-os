#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
    stringstream stream;
    stream << argv[1];
    unsigned int virtual_address = 0;
    stream >> virtual_address;

    const unsigned int PAGE_SIZE = 4 * 1024;
    int page_number = virtual_address / PAGE_SIZE;
    int offset = virtual_address % PAGE_SIZE;

    cout << "The address " << virtual_address << " contains:\n";
    cout << "page number=" << page_number << endl;
    cout << "offset=" << offset << endl;
    
    return 0;
}