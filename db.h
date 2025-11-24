#ifndef DB_H
#define DB_H

#include <iostream>
#include <string>
#include <fstream>
#include "stack.h"
#include "queue.h"
#include "set.h"
#include "hash.h"

using namespace std;

class Database {
private:
    class NamedStack {
    public:
        string name;
        Stack* stack;
        NamedStack* next;
    };

    class NamedQueue {
    public:
        string name;
        Queue* queue;
        NamedQueue* next;
    };

    class NamedSet {
    public:
        string name;
        Set* set;
        NamedSet* next;
    };

    class NamedHash {
    public:
        string name;
        HashTable* ht;
        NamedHash* next;
    };

    NamedStack* stackHead;
    NamedQueue* queueHead;
    NamedSet* setHead;
    NamedHash* hashHead;

    Stack* getOrCreateStack(string name);
    Queue* getOrCreateQueue(string name);
    Set* getOrCreateSet(string name);
    HashTable* getOrCreateHash(string name);

    Stack* findStack(string name);
    Queue* findQueue(string name);
    Set* findSet(string name);
    HashTable* findHash(string name);

    string getStructureType(string name);

    void parseQuery(const string& qStr, string& cmd, string& name, string& key, string& val);
    void cleanup();

public:
    Database();
    ~Database();

    void loadData(const string& path);
    void saveData(const string& path);
    string executeQuery(string queryStr);
};

#endif