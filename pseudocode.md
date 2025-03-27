# Global Definitions
Define DEFAULT_SIZE = 179 (default size of the hash table).
Declare a helper function strToDouble to convert a string to a double after removing unwanted characters.

# Bid Structure
Define a structure Bid with:
- bidId: Unique identifier (string).
- title: Title of the bid (string).
- fund: Fund associated with the bid (string).
- amount: Amount of the bid (double, default is 0.0).

# HashTable Class
- -Node():
  - Bid bid: The bid data.
  - unsigned int key: The hash key.
  - Node* next: Pointer to the next node (for chaining).
  - Constructors:
    - Default constructor initializes key to UINT_MAX and next to nullptr.
    - Constructor to initialize with a Bid.
    - Constructor to initialize with a Bid and a key.
- -vector<Node> nodes: Vector to store the hash table buckets.
- -unsigned int tableSize: Size of the hash table (default is DEFAULT_SIZE).
- -unsigned int hash(int key): Function to calculate the hash value.

+HashTable(): Default constructor to initialize the hash table with DEFAULT_SIZE.
+HashTable(size): Constructor to initialize the hash table with a custom size.
+~HashTable(): Destructor to free dynamically allocated memory.
+Insert(bid): Insert a bid into the hash table.
+PrintAll(): Print all bids in the hash table.
+Remove(bidId): Remove a bid by its bidId.
+Search(bidId): Search for a bid by its bidId.
+Size(): Return the size of the hash table.

-DisplayBid(bid): Print the details of a single bid.

HashTable::HashTable():
- Resize the nodes vector to tableSize.

HashTable::HashTable(size):
- Set tableSize to the given size.
- Resize the nodes vector to tableSize.

HashTable::~HashTable():
- For each bucket in the hash table:
- Traverse the chain in the bucket.
- Delete all dynamically allocated nodes.

HashTable::hash(key):
- Return key % tableSize to calculate the hash value.

HashTable::Insert(bid):
- Calculate the hash key for the bid ID.
- Create a new node for the bid.
- If the bucket is empty:
- Assign the new node to the bucket.
- Otherwise:
- Traverse the chain in the bucket.
- Add the new node to the end of the chain.

HashTable::PrintAll():
- For each bucket in the hash table:
- If the bucket is not empty:
- Print the bid in the current node using DisplayBid.
- Traverse the chain in the bucket and print all bids.

HashTable::Remove(bidId):
- Calculate the hash key for the bid ID.
- Traverse the chain in the bucket to find the bid.
- If the bid is found:
- If it is the head of the bucket:
- Replace it with the next node or reset the bucket.
- If it is not the head:
- Bypass it in the chain.
- Delete the node if it was dynamically allocated.
- Print a message indicating whether the bid was removed or not found.

HashTable::Search(bidId):
- Calculate the hash key for the bid ID.
- Traverse the chain in the bucket to find the bid.
- If the bid is found:
  - Return it.
- If not:
  - Return an empty bid.

HashTable::DisplayBid(bid):
- Print the details of the bid (bidId, title, amount, fund).

# Static Methods
displayBid(bid):
- Print the details of the bid (bidId, title, amount, fund).

loadBids(csvPath, hashTable):
- Print a message indicating the CSV file is being loaded.
- Initialize a CSV parser with the given path.
- Read and display the header row (optional).
- For each row in the CSV file:
  - Create a Bid object with the row data.
  - Insert the bid into the hash table.

strToDouble(str, ch):
- Remove the character ch from the string str.
- Convert the resulting string to a double and return it.

# Main Function
Initialize Variables:
- csvPath: Path to the CSV file (default is "eBid_Monthly_Sales.csv").
- bidKey: Key of the bid to search for (default is "98223").
- ticks: Timer variable for performance measurement.
- bidTable: Pointer to a HashTable object.

Menu Loop:
- Display a menu with options:
  - Load Bids:
    - Start the timer.
    - Call loadBids to load bids from the CSV file into the hash table.
    - Display the elapsed time.
  - Display All Bids:
    - Call PrintAll to display all bids in the hash table.
  - Find Bid:
    - Start the timer.
    - Call Search to find a bid by its bidKey.
    - Display the bid if found, or print a "not found" message.
    - Display the elapsed time.
  - Remove Bid:
    - Call Remove to remove a bid by its bidKey.
  - Exit:
    - Exit the program.
    - Exit Message:
    - Print "Good bye." and terminate the program.