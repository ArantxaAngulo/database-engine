#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 100  // MAXIMUM HASHTABLE SIZE
#define  NAME_TABLE_SIZE 50 // MAXIMUM NAME TABLE SIZE

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
Node* nameIndex[NAME_TABLE_SIZE]; // Creates an array called "nameIndex" of Node pointers with size NAME_TABLE_SIZE

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
// Secondary hash function for names
int hashName(const char *name) { // Function "hashName" that receives parameter pointer "name" of size char, and returns an int
    int sum = 0; 
    for (int i = 0; name[i] != '\0'; i++) { 
        sum += name[i]; // Count each name character
    }
    return sum % NAME_TABLE_SIZE; // Hash it
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
// Secondary insert function for names
void insertName(Record *r) { // Function insert, that receives parameter pointer "r" of size Record
    int index = hashName(r->name); // Index set to hashed value of the name (from the new Record sent)
    Node* newNode = (Node*)malloc(sizeof(Node)); // Create a new node
    
    newNode->data = *r; // Copy the Record data sent into the new node
    newNode->next = nameIndex[index]; // CHAINING!!! "new" NewNode is now the front of the index, and its Next pointer address set to "old" newNode
    
    nameIndex[index] = newNode; // Insert newNode
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
// Secondary retrive function for names
Record* searchByName(const char *name) { // Function retrive, that returns a pointer of size Record and receives parameter char pointer name
    int index = hashName(name); // Find the index by hashing the name sent
    Node* current = nameIndex[index];
    
    while (current != NULL) { // While current IS NOT NULL...
        if (strcmp(current->data.name, name) == 0) { // If the current index's ID is equal to the ID sent...
            return &current->data; // Go to the address of current and return the data that lives there 
        }
        current = current->next; // If he current index's ID is NOT equal to the ID sent... go to the next node/entry in list 
    }
    return NULL; // If current IS NULL... Return null
}

// Save to database
void saveToFile(const char *filename) { // Function that receives parameter pointer "filename"
    FILE* file = fopen(filename, "wb"); // Open file and write in binary mode
    if (!file) { // Error Handling
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < TABLE_SIZE; i++) { // For the size of the hashtable...
        Node *current = hashTable[i]; // Pointer "current" of size Node set to the current index of the hashtable
        while (current != NULL) { // While the current index of the hashtable IS NOT NULL...
            fwrite(&current->data, sizeof(Record), 1, file); // Write the data of the current index the file onto the file
            current = current->next; // Go to next one
        }
    }
    fclose(file); // Close the file
}
void loadFromFile(const char* filename) { // Function that receives parameter pointer "filename"
    FILE* file = fopen(filename, "rb"); // Open file and read in binary mode
    if (!file) { // Error Handling
        perror("Error opening file");
        return;
    }
    
    Record record;
    while (fread(&record, sizeof(Record), 1, file)) { // While it reads...
        insert(&record); //Insert data
    }
    fclose(file); // Close the file
}
bool updateFile(int id, const char *newName, int newAge) { // Function that receives parameter int "ID", pointer "newName" and int newAAge
    int index = hash(id);  // Find the index in the hash table
    Node *current = hashTable[index];

    // Traverse the linked list at this index
    while (current != NULL) {
        if (current->data.id == id) {  // If record is found
            // Update fields
            strncpy(current->data.name, newName, sizeof(current->data.name) - 1);
            current->data.name[sizeof(current->data.name) - 1] = '\0';  // Ensure null termination
            current->data.age = newAge;
            return true;  // Record was found and updated
        }
        current = current->next;  // Move to the next node in the list
    }

    return false;  // Record with the given ID was not found
}
bool deleteRecord(int id) {
    int index = hash(id);  // Calculate the index using the hash function
    Node *current = hashTable[index];
    Node *previous = NULL;

    // Traverse the linked list at the given index to find the target id
    while (current != NULL) {
        if (current->data.id == id) {  // If the record is found
            if (previous == NULL) {
                // Case: the node to delete is the first node in the list
                hashTable[index] = current->next;
            } else {
                // Case: the node to delete is not the first node
                previous->next = current->next;
            }
            free(current);  // Free memory allocated for the node
            return true;     // Record successfully deleted
        }
        previous = current;     // Move previous to the current node
        current = current->next;  // Move to the next node
    }
    return false;  // Record with the given id was not found
}

// QUERY FUNCTIONS
// Get all records of X age
void getRecordsByAge(int minAge) { // Function that receives parameter int "minAge"
    for (int i = 0; i < TABLE_SIZE; i++) { // For the lenght of the hashTable(100)
        Node *current = hashTable[i]; // Pointer "current" of size Node set to the index of hashTable
        
        while (current != NULL) { // While current IS NOT NULL...
            if (current->data.age > minAge) { // If the current index's age is less than the minAge sent...
                printf("ID: %d, Name: %s, Age: %d\n", current->data.id, current->data.name, current->data.age);
            }
            current = current->next;
        } // Print all records that fit the filter
    }
}

// menu
void menu() {
    int choice;
    Record record;
    char name[50];
    int id, age;
    char inputBuffer[100];  // Buffer to hold the entire input line for parsing
    
    while (1) {
        printf("1. Insert\n2. Search by ID\n3. Search by Name\n4. Update\n5. Delete\n6. Save\n7. Download Database\n8. Print\n9. Exit\n");
        printf("Enter your choice: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);  // Read the full line into inputBuffer
        inputBuffer[strcspn(inputBuffer, "\n")] = 0;  // Remove newline if fgets captures it
        sscanf(inputBuffer, "%d", &choice);  // Parse the integer choice from the buffer

        switch (choice) {
            case 1:
                printf("Enter ID, Name, Age: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);  // Read the entire line
                inputBuffer[strcspn(inputBuffer, "\n")] = 0;  // Remove newline if fgets captures it
                // Modify sscanf to handle commas and spaces correctly
                sscanf(inputBuffer, "%d ,%49[^,] ,%d", &record.id, record.name, &record.age);  // Parse ID, name, and age
                insert(&record);
                insertName(&record);
                break;
            case 2:
                printf("Enter ID: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);  // Read the entire line
                inputBuffer[strcspn(inputBuffer, "\n")] = 0;  // Remove newline if fgets captures it
                sscanf(inputBuffer, "%d", &id);  // Parse ID from inputBuffer
                Record* result = retrieve(id);
                if (result) printf("Found: %s, %d\n", result->name, result->age);
                else printf("Not found.\n");
                break;
            case 3:
                printf("Enter Name: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);  // Read the entire line
                inputBuffer[strcspn(inputBuffer, "\n")] = 0;  // Remove newline if fgets captures it
                sscanf(inputBuffer, "%49[^\n]", name);  // Parse the name (reads until newline)
                result = searchByName(name);
                if (result) printf("Found: ID %d, Age %d\n", result->id, result->age);
                else printf("Not found.\n");
                break;
            case 4:
                printf("Enter ID, New Name, New Age: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);  // Read the entire line
                inputBuffer[strcspn(inputBuffer, "\n")] = 0;  // Remove newline if fgets captures it
                sscanf(inputBuffer, "%d , %49s , %d", &id, name, &age);  // Parse ID, name, and age
                if (updateFile(id, name, age)) printf("Record updated.\n");
                else printf("Record not found.\n");
                break;
            case 5:
                printf("Enter ID to delete: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);  // Read the entire line
                inputBuffer[strcspn(inputBuffer, "\n")] = 0;  // Remove newline if fgets captures it
                sscanf(inputBuffer, "%d", &id);  // Parse ID from inputBuffer
                if (deleteRecord(id)) printf("Record deleted.\n");
                else printf("Record not found.\n");
                break;
            case 6:
                saveToFile("database.dat");
                printf("Data saved to file.\n");
                break;
            case 7:
                loadFromFile("database.dat");
                printf("Data loaded from file.\n");
                break;
            case 8:
                print_hashTable();
                break;
            case 9:
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}


// Main function
int main() {
    init_hashTable();
    menu();
    return 0;
}
