#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 100  // MAXIMUM HASHTABLE SIZE

// Custom data type to hold basic data 
typedef struct {
    int id;
    char name[50];
    int age;
} Record; //size ~58 bytes

// Node that holds Record information and the next node address
typedef struct Node {
    Record data; // Record Variable "data" 
    struct Node* next; // Pointer to the next Node in the linked list
} Node; //size ~66 bytes

// Hash table structure
Node* hashTable[TABLE_SIZE]; // Creates an array called "hashTable" of Node pointers with size TABLE_SIZE
// Each element will point to the head of a Node struct (if created)

//FUNCTIONS
// Setting all pointers to NULL
void init_hashTable() {
    for(int i = 0; i < TABLE_SIZE; i++) {
    hashTable[i] = NULL;
    } 
}

// Hash function
unsigned int hash(int key) { // Function "hash" that receives a parameter int "KEY" and returns an int
    return (key % TABLE_SIZE); // Sets index at hashtable
}

// Print the hashtable data
void print_hashTable(){
    for(int i = 0; i < TABLE_SIZE; i++) { // For the lenght of the hashTable(100)
        if(hashTable[i] == NULL){ // If the position holds no info
            printf("position %d is empty\n", i); // If position is empty prints "empty"
        } else { // If the position holds info
            printf("positon %d holds ", i);
            Node *current = hashTable[i]; // Pointer "current" with size Node, holds address of index
            while (current != NULL) { // and, while the current index is NOT empty(NULL)
                printf("ID: %d, Name: %s, Age: %d\n", current->data.id, current->data.name, current->data.age);
                current = current->next; //prints and goes to the next index
            }
        }
    } 
}

// Insert to hashtable 
bool insert(Record *r){ // Function insert, that returns true or false and receives parameter pointer "r" of size Record
    if(r == NULL){ // Error handling
        return false; 
    }
    int index = hash(r->id); // Index set to hashed value of the ID (from the new Record sent)

    Node* newNode = (Node*)malloc(sizeof(Node));  // Create a new node
    if (newNode == NULL) {
        return false;  // Memory allocation failed
    }
    newNode->data = *r;   // Copy the Record data sent into the new node
    newNode->next = NULL; // Initialize next pointer address to NULL

    if (hashTable[index] == NULL) {  // If hashTable[index] is empty... 
        hashTable[index] = newNode; // That means there is no collision, directly insert newNode
    } else { // If there is a collision...
        newNode->next = hashTable[index]; // CHAINING!!! "new" NewNode is now the front of the index, and its Next pointer address set to "old" newNode
        hashTable[index] = newNode; // Now there is no collision, directly insert newNode
    }
    return true;
}

// Retrieve from hashtable
Record* retrieve(int id){ // Function retrive, that returns a pointer of size Record and receives parameter ID
    int index = hash(id); // Find the index by hashing the ID sent
    Node *current = hashTable[index]; // Pointer "current" of size Node set to the index of hashTable
    
    while (current != NULL) { // While current IS NOT NULL...
        if (current->data.id == id) { // If the current index's ID is equal to the ID sent...
            return &current->data; // Go to the address of current and return the data that lives there 
        }
        current = current->next; // If he current index's ID is NOT equal to the ID sent... go to the next node/entry in list 
    }
    return NULL; // If current IS NULL... Return null
}


int main(){
    init_hashTable();

    Record r1 = {.id = 751976, .name = "Adrian", .age = 22};

    insert(&r1); // Sends the ADDRESS of r1 rather than the info it holds
    print_hashTable();

    return 0;
}
