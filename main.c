#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
// Define a Node for the Graph Database
typedef struct Node
{
    int id;
    struct Node **adjacencyList;
    int adjacencyCount;
} Node;

// Define a Document for the Document-Based Database
typedef struct Document
{
    char *key;
    char *value;
} Document;

// Define a KeyValue for the Key-Value Database
typedef struct KeyValue
{
    char *key;
    void *value;
} KeyValue;

// Define a Vector for the Vector Database
typedef struct Vector
{
    int *data;
    int size;
} Vector;

// Define a Table for the Relational Database
typedef struct Table
{
    char **columnNames;
    int columnCount;
    char ***rows;
    int rowCount;
} Table;

typedef struct InMemoryDatabase
{
    int *data;
    int size;
} InMemoryDatabase;

// Function to create a new In-Memory Database
InMemoryDatabase *createInMemoryDatabase()
{
    InMemoryDatabase *db = (InMemoryDatabase *)malloc(sizeof(InMemoryDatabase));
    db->data = NULL;
    db->size = 0;
    return db;
}

// Function to insert data into the In-Memory Database
void insertData(InMemoryDatabase *db, int value)
{
    db->size++;
    db->data = (int *)realloc(db->data, db->size * sizeof(int));
    db->data[db->size - 1] = value;
}

// Function to retrieve data from the In-Memory Database
int getData(InMemoryDatabase *db, int index)
{
    if (index >= 0 && index < db->size)
    {
        return db->data[index];
    }
    return -1; // Return -1 if index is out of bounds
}

// Define the Hybrid Database
typedef struct HybridDatabase
{
    Node **graphDatabase;
    int graphDatabaseSize;
    Document **documentDatabase;
    int documentDatabaseSize;
    KeyValue **keyValueDatabase;
    int keyValueDatabaseSize;
    Vector **vectorDatabase;
    int vectorDatabaseSize;
    Table **relationalDatabase;
    int relationalDatabaseSize;
    InMemoryDatabase *inMemoryDatabase;
    int inMemoryDatabaseSize;
} HybridDatabase;

// Function to create a new Hybrid Database
HybridDatabase *createHybridDatabase()
{
    HybridDatabase *db = (HybridDatabase *)malloc(sizeof(HybridDatabase));
    db->graphDatabaseSize = 0;
    db->documentDatabaseSize = 0;
    db->keyValueDatabaseSize = 0;
    db->vectorDatabaseSize = 0;
    db->relationalDatabaseSize = 0;
    return db;
}

int main()
{
    HybridDatabase *db = createHybridDatabase();
    InMemoryDatabase *inMemoryDb1 = db->inMemoryDatabase = createInMemoryDatabase();
    InMemoryDatabase *inMemoryDb = createInMemoryDatabase();

    // Use the databases...
    insertData(inMemoryDb, 10);
    insertData(inMemoryDb, 20);
    int value = getData(inMemoryDb, 0);
    printf("Value at index 1: %d\n", value);

    free(inMemoryDb->data);
    free(inMemoryDb);
    free(db);
    return 0;
}