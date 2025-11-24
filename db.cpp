#include "db.h"

Database::Database() {
    stackHead = nullptr;
    queueHead = nullptr;
    setHead = nullptr;
    hashHead = nullptr;
}

Database::~Database() {
    cleanup();
}

void Database::cleanup() {
    while (stackHead) {
        NamedStack* temp = stackHead;
        stackHead = stackHead->next;
        delete temp->stack;
        delete temp;
    }
    while (queueHead) {
        NamedQueue* temp = queueHead;
        queueHead = queueHead->next;
        delete temp->queue;
        delete temp;
    }
    while (setHead) {
        NamedSet* temp = setHead;
        setHead = setHead->next;
        delete temp->set;
        delete temp;
    }
    while (hashHead) {
        NamedHash* temp = hashHead;
        hashHead = hashHead->next;
        delete temp->ht;
        delete temp;
    }
}

// Убрана функция getStructureType

Stack* Database::getOrCreateStack(string name) {
    Stack* existing = findStack(name);
    if(existing) return existing;

    NamedStack* newNode = new NamedStack;
    newNode->name = name;
    newNode->stack = new Stack();
    newNode->next = stackHead;
    stackHead = newNode;
    return newNode->stack;
}

Queue* Database::getOrCreateQueue(string name) {
    Queue* existing = findQueue(name);
    if(existing) return existing;

    NamedQueue* newNode = new NamedQueue;
    newNode->name = name;
    newNode->queue = new Queue();
    newNode->next = queueHead;
    queueHead = newNode;
    return newNode->queue;
}

Set* Database::getOrCreateSet(string name) {
    Set* existing = findSet(name);
    if(existing) return existing;

    NamedSet* newNode = new NamedSet;
    newNode->name = name;
    newNode->set = new Set();
    newNode->next = setHead;
    setHead = newNode;
    return newNode->set;
}

HashTable* Database::getOrCreateHash(string name) {
    HashTable* existing = findHash(name);
    if(existing) return existing;

    NamedHash* newNode = new NamedHash;
    newNode->name = name;
    newNode->ht = new HashTable();
    newNode->next = hashHead;
    hashHead = newNode;
    return newNode->ht;
}

Stack* Database::findStack(string name) {
    NamedStack* curr = stackHead;
    while(curr) { 
        if(curr->name == name) return curr->stack; 
        curr = curr->next; 
    }
    return nullptr;
}
Queue* Database::findQueue(string name) {
    NamedQueue* curr = queueHead;
    while(curr) { 
        if(curr->name == name) return curr->queue; 
        curr = curr->next; 
    }
    return nullptr;
}
Set* Database::findSet(string name) {
    NamedSet* curr = setHead;
    while(curr) { 
        if(curr->name == name) return curr->set; 
        curr = curr->next; 
    }
    return nullptr;
}
HashTable* Database::findHash(string name) {
    NamedHash* curr = hashHead;
    while(curr) { 
        if(curr->name == name) return curr->ht; 
        curr = curr->next; 
    }
    return nullptr;
}

void Database::loadData(const string& path) {
    ifstream file(path);
    if (!file.is_open()) return;

    string type, name, key, val;
    while (file >> type >> name >> key) {
        if (type == "STACK") {
            Stack* s = getOrCreateStack(name);
            s->push(key);
        } else if (type == "QUEUE") {
            Queue* q = getOrCreateQueue(name);
            q->push(key);
        } else if (type == "SET") {
            Set* s = getOrCreateSet(name);
            s->add(key);
        } else if (type == "HASH") {
            file >> val; 
            HashTable* h = getOrCreateHash(name);
            h->set(key, val);
        }
    }
    file.close();
}

void Database::saveData(const string& path) {
    ofstream file(path);
    if (!file.is_open()) {
        cout << "Ошибка записи в файл" << endl;
        return;
    }

    NamedStack* sNode = stackHead;
    while (sNode) {
        sNode->stack->saveFile(file, sNode->name);
        sNode = sNode->next;
    }

    NamedQueue* qNode = queueHead;
    while (qNode) {
        qNode->queue->saveFile(file, qNode->name);
        qNode = qNode->next;
    }

    NamedSet* stNode = setHead;
    while (stNode) {
        stNode->set->saveFile(file, stNode->name);
        stNode = stNode->next;
    }

    NamedHash* hNode = hashHead;
    while (hNode) {
        hNode->ht->saveToFile(file, hNode->name); 
        hNode = hNode->next;
    }

    file.close();
}

void Database::parseQuery(const string& qStr, string& cmd, string& name, string& key, string& val) {
    cmd = ""; name = ""; key = ""; val = "";
    int idx = 0;
    int len = qStr.length();

    auto skipSpaces = [&](int& i) { while(i < len && (qStr[i] == ' ' || qStr[i] == '\t')) i++; };
    auto readWord = [&](int& i) -> string {
        string res = "";
        while(i < len && qStr[i] != ' ' && qStr[i] != '\t') {
            res += qStr[i++];
        }
        return res;
    };

    skipSpaces(idx);
    cmd = readWord(idx);
    skipSpaces(idx);
    name = readWord(idx);
    skipSpaces(idx);
    key = readWord(idx);
    skipSpaces(idx);
    val = readWord(idx);
}

string Database::executeQuery(string queryStr) {
    string cmd, name, key, val;
    parseQuery(queryStr, cmd, name, key, val);

    if (cmd == "") 
        return "";

    // Проверка на имя структуры
    if (name == "") {
        return "ОШИБКА: Не указано имя структуры";
    }

    // --- STACK ---
    if (cmd == "SPUSH") {
        if (key == "") return "ОШИБКА: Не указано значение для добавления";
        
        Stack* s = getOrCreateStack(name);
        s->push(key);
        return key;
    }
    if (cmd == "SPOP") {
        Stack* s = findStack(name);
        if (!s) {
            return "ОШИБКА: Стек '" + name + "' не найден";
        }
        return s->pop(); 
    }

    if (cmd == "QPUSH") {
        if (key == "") return "ОШИБКА: Не указано значение для добавления";

        Queue* q = getOrCreateQueue(name);
        q->push(key);
        return key;
    }
    if (cmd == "QPOP") {
        Queue* q = findQueue(name);
        if (!q) {
            return "ОШИБКА: Очередь '" + name + "' не найдена";
        }
        return q->pop(); 
    }

    // --- SET ---
    if (cmd == "SADD") {
        if (key == "") return "ОШИБКА: Не указан ключ для добавления";

        Set* s = getOrCreateSet(name);
        s->add(key);
        return key;
    }
    if (cmd == "SREM") {
        if (key == "") return "ОШИБКА: Не указан ключ для удаления";

        Set* s = findSet(name);
        if (!s) {
            return "ОШИБКА: Множество '" + name + "' не найдено";
        }
        return s->remove(key); 
    }
    if (cmd == "SISMEMBER") {
        if (key == "") return "ОШИБКА: Не указан ключ для проверки";

        Set* s = findSet(name);
        if (!s) {
             return "FALSE"; 
        }
        return s->isMember(key) ? "TRUE" : "FALSE";
    }

    // --- HASH ---
    if (cmd == "HSET") {
        if (key == "") 
            return "ОШИБКА: Не указан ключ";
        if (val == "") 
            return "ОШИБКА: Не указано значение";

        HashTable* h = getOrCreateHash(name);
        h->set(key, val);
        return key + "->" + val;
    }
    if (cmd == "HDEL") {
        if (key == "") return "ОШИБКА: Не указан ключ для удаления";

        HashTable* h = findHash(name);
        if (!h) {
            return "ОШИБКА: Хеш-таблица '" + name + "' не найдена";
        }
        return h->remove(key);
    }
    if (cmd == "HGET") {
        if (key == "") return "ОШИБКА: Не указан ключ для поиска";

        HashTable* h = findHash(name);
        if (!h) {
            return "ОШИБКА: Хеш-таблица " + name + " не найдена";
        }
        return h->get(key);
    }

    return "ОШИБКА: Неизвестная команда '" + cmd + "'";
}