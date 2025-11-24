#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

class HashNode {
public:
    string key;
    string value;
    HashNode* next;

    HashNode(string k, string v, HashNode* nxt = nullptr) 
        : key(k), value(v), next(nxt) {}
};

class HashChaining {
private:
    HashNode** buckets;
    int capacity;
    int count;

    int hashFunc(const string& key, int cap) const;
    void resize();

public:
    HashChaining(int size = 10);
    ~HashChaining();

    void set(string key, string value);
    string get(string key) const;
    string remove(string key); 
    
    // Добавили аргумент name для корректного сохранения формата
    void saveToFile(ofstream& fileStream, const string& name) const;
};

class HashEntry {
public:
    string key;
    string value;
    bool occupied;
    bool deleted;

    HashEntry() : occupied(false), deleted(false) {}
};

class HashOpenAddr {
private:
    HashEntry* buckets;
    int capacity;
    int count;

    int hashFunc(const string& key, int cap) const;

public:
    HashOpenAddr(int size = 10);
    ~HashOpenAddr();

    void set(string key, string value);
    string get(string key) const;
    string remove(string key); 

    void saveToFile(ofstream& fileStream, const string& name) const;
};

using HashTable = HashChaining;

#endif