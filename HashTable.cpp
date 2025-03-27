//============================================================================
// Name        : HashTable.cpp
// Author      : Ryan Grunest
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 4-2 Hash Table
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
    size_t Size();
private: 
    void DisplayBid(Bid bid);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // Initalize node structure by resizing tableSize
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this->
    this->tableSize = size;
    // resize nodes size
    nodes.resize(tableSize);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // Iterate through each bucket in the hash table
    for (unsigned int i = 0; i < tableSize; ++i) {
        // Get the head of the chain at this bucket
        Node* current = nodes[i].next;

        // Traverse the chain and delete each node
        while (current != nullptr) {
            Node* temp = current; // Store the current node
            current = current->next; // Move to the next node
            delete temp; // Delete the stored node
        }
    }
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    return key % tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // Calculate the hash key for the given bid ID
    unsigned int key = hash(atoi(bid.bidId.c_str()));

    // Create a new node for the bid
    Node* newNode = new Node(bid, key);

    // If the bucket at the hash key is empty (no chain exists)
    if (nodes[key].key == UINT_MAX) {
        // Assign the new node to this bucket
        nodes[key] = *newNode;
    } else {
        // Otherwise, traverse the chain to find the end
        Node* current = &nodes[key];
        while (current->next != nullptr) {
            current = current->next;
        }
        // Add the new node to the end of the chain
        current->next = newNode;
    }
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // Iterate through each bucket in the hash table
    for (unsigned int i = 0; i < tableSize; ++i) {
        // Get the current node in the bucket
        Node* current = &nodes[i];

        // If the bucket is not empty (key is not UINT_MAX)
        if (current->key != UINT_MAX) {
            // Print the bid in the current node
            DisplayBid(current->bid);

            // Traverse the chain (if any) and print all bids
            current = current->next;

            while (current != nullptr) {
                DisplayBid(current->bid);
                current = current->next;
            }
        }
    }
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // set key equal to hash atoi bidID cstring
    unsigned int key = hash(atoi(bidId.c_str()));
    
    // Get the head node of the bucket
    Node* current = &nodes[key];
    Node* previous = nullptr;

    // Traverse the chain to find the bid
    while (current != nullptr && current->key != UINT_MAX) {
        // Check if the current node matches the bidId
        if (current->bid.bidId == bidId) {
            // If the node is the head of the bucket
            if (previous == nullptr) {
                // If there is a next node, replace the head with the next node
                if (current->next != nullptr) {
                    nodes[key] = *current->next;
                } else {
                    // Otherwise, reset the head node to an unused state
                    nodes[key].key = UINT_MAX;
                    nodes[key].bid = Bid();
                    nodes[key].next = nullptr;
                }
            } else {
                // If the node is not the head, bypass it in the chain
                previous->next = current->next;
            }

            // Delete the current node if it was dynamically allocated
            if (previous != nullptr) {
                delete current;
            }

            cout << "Bid " << bidId << " removed." << endl;
            return;
        }

        // Move to the next node
        previous = current;
        current = current->next;
    }

    // If the bid was not found
    cout << "Bid " << bidId << " not found." << endl;
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 * @return The bid if found, otherwise an empty bid
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // Calculate the hash key for the given bid ID
    unsigned int key = hash(atoi(bidId.c_str()));

    // Get the head node of the bucket
    Node* current = &nodes[key];

    // Traverse the chain to find the bid
    while (current != nullptr && current->key != UINT_MAX) {
        // Check if the current node matches the bidId
        if (current->bid.bidId == bidId) {
            // If found, return the bid
            return current->bid;
        }

        // Move to the next node in the chain
        current = current->next;
    }

    // If no entry is found, return an empty bid
    return bid;
}

void HashTable::DisplayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
         << bid.fund << endl;
    return;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98223";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;
    bidTable = new HashTable();
    
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
