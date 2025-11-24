#include "hash.h"

// --- HashChaining Implementation ---

int HashChaining::hashFunc(const string& key, int cap) const {
    int hash = 0;
    for (char ch : key) {
        hash = (hash * 31 + abs((int)ch)) % cap;
    }
    return hash;
}

HashChaining::HashChaining(int size) {
    this->capacity = size;
    this->count = 0;
    this->buckets = new HashNode*[size];
    for (int i = 0; i < size; i++) {
        this->buckets[i] = nullptr;
    }
}

HashChaining::~HashChaining() {
    for (int i = 0; i < capacity; i++) {
        HashNode* curr = buckets[i];
        while (curr != nullptr) {
            HashNode* next = curr->next;
            delete curr;
            curr = next;
        }
    }
    delete[] buckets;
}

void HashChaining::resize() {
    int newCapacity = capacity * 2;
    HashNode** newBuckets = new HashNode*[newCapacity];
    for(int i = 0; i < newCapacity; i++) 
        newBuckets[i] = nullptr;

    for (int i = 0; i < capacity; i++) {
        HashNode* curr = buckets[i];
        while (curr != nullptr) {
            HashNode* next = curr->next;
            int newIdx = hashFunc(curr->key, newCapacity);
            curr->next = newBuckets[newIdx];
            newBuckets[newIdx] = curr;
            curr = next;
        }
    }
    delete[] buckets;
    buckets = newBuckets;
    capacity = newCapacity;
}

void HashChaining::set(string key, string value) {
    int idx = hashFunc(key, capacity);
    HashNode* curr = buckets[idx];

    // Обновление
    while (curr != nullptr) {
        if (curr->key == key) {
            curr->value = value;
            return;
        }
        curr = curr->next;
    }

    // Ресайз
    if ((double)count / capacity >= 0.7) {
        resize();
        idx = hashFunc(key, capacity);
    }

    // Вставка
    HashNode* newNode = new HashNode(key, value, buckets[idx]);
    buckets[idx] = newNode;
    count++;
}

string HashChaining::get(string key) const {
    int idx = hashFunc(key, capacity);
    HashNode* curr = buckets[idx];
    
    while (curr != nullptr) {
        if (curr->key == key) {
            return curr->value;
        }
        curr = curr->next;
    }
    return "ОШИБКА: Ключ не найден"; 
}

string HashChaining::remove(string key) {
    int idx = hashFunc(key, capacity);
    HashNode* curr = buckets[idx];
    HashNode* prev = nullptr;

    while (curr != nullptr) {
        if (curr->key == key) {
            if (prev == nullptr) buckets[idx] = curr->next;
            else prev->next = curr->next;

            string val = curr->value;
            delete curr;
            count--;
            return val;
        }
        prev = curr;
        curr = curr->next;
    }
    return "ОШИБКА: Ключ не найден"; 
}

void HashChaining::saveToFile(ofstream& fileStream, const string& name) const {
    for (int i = 0; i < capacity; i++) {
        HashNode* curr = buckets[i];
        while (curr != nullptr) {
            fileStream << "HASH " << name << " " << curr->key << " " << curr->value << endl;
            curr = curr->next;
        }
    }
}


int HashOpenAddr::hashFunc(const string& key, int cap) const {
    int hash = 0;
    for (char ch : key) {
        hash = (hash * 31 + abs((int)ch)) % cap;
    }
    return hash;
}

HashOpenAddr::HashOpenAddr(int size) {
    this->capacity = size;
    this->count = 0;
    this->buckets = new HashEntry[size];
}

HashOpenAddr::~HashOpenAddr() {
    delete[] buckets;
}

void HashOpenAddr::set(string key, string value) {
    int index = hashFunc(key, capacity);
    int originalIndex = index;

    do {
        if (!buckets[index].occupied && !buckets[index].deleted) {
            buckets[index].key = key;
            buckets[index].value = value;
            buckets[index].occupied = true;
            buckets[index].deleted = false;
            count++;
            return;
        }
        if (buckets[index].occupied && buckets[index].key == key) {
            buckets[index].value = value;
            return;
        }
        if (buckets[index].deleted) {
            buckets[index].key = key;
            buckets[index].value = value;
            buckets[index].occupied = true;
            buckets[index].deleted = false;
            count++;
            return;
        }
        index = (index + 1) % capacity;
    } while (index != originalIndex);
}

string HashOpenAddr::get(string key) const {
    int index = hashFunc(key, capacity);
    int originalIndex = index;

    do {
        if (buckets[index].occupied && buckets[index].key == key) {
            return buckets[index].value;
        }
        if (!buckets[index].occupied && !buckets[index].deleted) {
            break;
        }
        index = (index + 1) % capacity;
    } while (index != originalIndex);
    return "ОШИБКА: Ключ не найден";
}

string HashOpenAddr::remove(string key) {
    int index = hashFunc(key, capacity);
    int originalIndex = index;

    do {
        if (buckets[index].occupied && buckets[index].key == key) {
            buckets[index].occupied = false;
            buckets[index].deleted = true;
            count--;
            return buckets[index].value;
        }
        if (!buckets[index].occupied && !buckets[index].deleted) {
            break;
        }
        index = (index + 1) % capacity;
    } while (index != originalIndex);
    return "ОШИБКА: Ключ не найден";
}

void HashOpenAddr::saveToFile(ofstream& fileStream, const string& name) const {
    for (int i = 0; i < capacity; i++) {
        if (buckets[i].occupied) {
            fileStream << "HASH " << name << " " << buckets[i].key << " " << buckets[i].value << endl;
        }
    }
}