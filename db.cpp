#include "db.h"


//функция для прверки является ли строка числом
bool isInteger(string& s) {
    if (s.empty())//пустота
        return false;
    size_t i = 0;
    if (s[0] == '-' || s[0] == '+') { //если строка только из знака
        i = 1; //пропуск знака
        if (s.length() == 1) 
            return false; 
    }
    for (; i < s.length(); ++i) {
        if (s[i] < '0' || s[i] > '9') //48-57 ascii
            return false;
    }
    return true;
}

//конструктор
Database::Database() {
    stackHead = nullptr;
    queueHead = nullptr;
    setHead = nullptr;
    hashHead = nullptr;
    treeHead = nullptr;
}

Database::~Database() {
    cleanup();
}

//удаляет все объекты
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
    while (treeHead) {
        NamedTree* temp = treeHead;
        treeHead = treeHead->next;
        delete temp->tree;
        delete temp;
    }
}

//создаём новую структуру 
Stack* Database::getCreateStack(string name) {
    Stack* exis = findStack(name);
    if(exis) 
        return exis;

    NamedStack* newNode = new NamedStack;
    newNode->name = name;
    newNode->stack = new Stack();
    newNode->next = stackHead;
    stackHead = newNode;
    return newNode->stack;
}

Queue* Database::getCreateQueue(string name) {
    Queue* existing = findQueue(name);
    if(existing) 
        return existing;

    NamedQueue* newNode = new NamedQueue;
    newNode->name = name;
    newNode->queue = new Queue();
    newNode->next = queueHead;
    queueHead = newNode;
    return newNode->queue;
}

Set* Database::getCreateSet(string name) {
    Set* existing = findSet(name);
    if(existing) 
        return existing;

    NamedSet* newNode = new NamedSet;
    newNode->name = name;
    newNode->set = new Set();
    newNode->next = setHead;
    setHead = newNode;
    return newNode->set;
}

HashTable* Database::getCreateHash(string name) {
    HashTable* existing = findHash(name);
    if(existing) 
        return existing;

    NamedHash* newNode = new NamedHash;
    newNode->name = name;
    newNode->ht = new HashTable();
    newNode->next = hashHead;
    hashHead = newNode;
    return newNode->ht;
}

CompleteBinaryTree* Database::getCreateTree(string name) {
    CompleteBinaryTree* existing = findTree(name);
    if(existing) 
        return existing;

    NamedTree* newNode = new NamedTree;
    newNode->name = name;
    newNode->tree = new CompleteBinaryTree();
    newNode->next = treeHead;
    treeHead = newNode;
    return newNode->tree;
}

Stack* Database::findStack(string name) {
    NamedStack* curr = stackHead;
    while(curr) { 
        if(curr->name == name) 
            return curr->stack; 
        curr = curr->next; 
    }
    return nullptr;
}
Queue* Database::findQueue(string name) {
    NamedQueue* curr = queueHead;
    while(curr) { 
        if(curr->name == name) 
            return curr->queue; 
        curr = curr->next; 
    }
    return nullptr;
}
Set* Database::findSet(string name) {
    NamedSet* curr = setHead;
    while(curr) { 
        if(curr->name == name)
            return curr->set; 
        curr = curr->next; 
    }
    return nullptr;
}
HashTable* Database::findHash(string name) {
    NamedHash* curr = hashHead;
    while(curr) { 
        if(curr->name == name) 
            return curr->ht; 
        curr = curr->next; 
    }
    return nullptr;
}
CompleteBinaryTree* Database::findTree(string name) {
    NamedTree* curr = treeHead;
    while(curr) { 
        if(curr->name == name) return curr->tree; 
        curr = curr->next; 
    }
    return nullptr;
}

void Database::loadData(const string& path) {
    ifstream file(path);
    if (!file.is_open()) return;

    string type, name, key, val;
    
    while (file >> type) {
        if (type == "STACK") {
            file >> name >> key;
            Stack* s = getCreateStack(name);
            s->push(key);
        } else if (type == "QUEUE") {
            file >> name >> key;
            Queue* q = getCreateQueue(name);
            q->push(key);
        } else if (type == "SET") {
            file >> name >> key;
            Set* s = getCreateSet(name);
            s->add(key);
        } else if (type == "HASH") {
            file >> name >> key >> val;
            HashTable* h = getCreateHash(name);
            h->set(key, val);
        } else if (type == "TREE") {
            file >> name >> key;
            if (isInteger(key)) {
                CompleteBinaryTree* t = getCreateTree(name);
                t->insert(stoi(key));
            }
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
    
    NamedTree* tNode = treeHead;
    while (tNode) {
        tNode->tree->saveToFile(file, tNode->name);
        tNode = tNode->next;
    }

    file.close();
}

void Database::parseQuery(const string& qStr, string& cmd, string& name, string& key, string& val) {
    cmd = ""; name = ""; key = ""; val = "";
    int idx = 0;
    int len = qStr.length();

    //перепрыгиваем через пробелы [&] захват переменных по ссылке
    auto skipSpaces = [&](int& i) 
    { 
    while(i < len && (qStr[i] == ' ' || qStr[i] == '\t')) 
        i++; 
    };

    auto readWord = [&](int& i) -> string {
        string res = "";
        while(i < len && qStr[i] != ' ' && qStr[i] != '\t') 
        {
            res += qStr[i++];
        }
        return res;
    };

    skipSpaces(idx); //все пробелы в начале строки пропускаем
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
    if (name == "")
        return "Ошибка: Не указано имя структуры";

    if (cmd == "SPUSH") {
        if (key == "") 
            return "Ошибка: Не указано значение для добавления";
        Stack* s = getCreateStack(name);
        s->push(key);
        return key;
    }
    if (cmd == "SPOP") {
        Stack* s = findStack(name);
        if (!s) 
            return "Ошибка: Стек " + name + " не найден";
        return s->pop(); 
    }

    if (cmd == "QPUSH") {
        if (key == "") 
            return "Ошибка: Не указано значение для добавления";
        Queue* q = getCreateQueue(name);
        q->push(key);
        return key;
    }
    if (cmd == "QPOP") {
        Queue* q = findQueue(name);
        if (!q) 
            return "Ошибка: Очередь'" + name + " не найдена";
        return q->pop(); 
    }

    if (cmd == "SADD") {
        if (key == "") 
            return "Ошибка: Не указан ключ для добавления";
        Set* s = getCreateSet(name);
        s->add(key);
        return key;
    }
    if (cmd == "SREM") {
        if (key == "")
            return "Ошибка: Не указан ключ для удаления";
        Set* s = findSet(name);
        if (!s) 
            return "Ошибка: Множество " + name + " не найдено";
        return s->remove(key); 
    }
    if (cmd == "SISMEMBER") {
        if (key == "") 
            return "Ошибка: Не указан ключ для проверки";
        Set* s = findSet(name);
        if (!s) 
            return "FALSE"; 
        return s->isMember(key) ? "TRUE" : "FALSE";
    }

    if (cmd == "HSET") {
        if (key == "") 
            return "Ошибка: Не указан ключ";
        if (val == "") 
            return "Ошибка: Не указано значение";
        HashTable* h = getCreateHash(name);
        h->set(key, val);
        return key + "->" + val;
    }
    if (cmd == "HDEL") {
        if (key == "") 
            return "Ошибка: Не указан ключ для удаления";
        HashTable* h = findHash(name);
        if (!h) 
            return "Ошибка: Хеш-таблица '" + name + "' не найдена";
        return h->remove(key);
    }
    if (cmd == "HGET") {
        if (key == "") 
            return "Ошибка: Не указан ключ для поиска";
        HashTable* h = findHash(name);
        if (!h) 
            return "Ошибка: Хеш-таблица " + name + " не найдена";
        return h->get(key);
    }

    if (cmd == "TPUSH") {
        if (key == "") 
            return "Ошибка: Не указано значение для добавления ";
        if (!isInteger(key)) 
            return "Ошибка: Значение дерева должно быть числом";
        
        int valInt = stoi(key); //строка в число
        CompleteBinaryTree* t = getCreateTree(name);
        t->insert(valInt);
        return key;
    }
    
    if (cmd == "TDEL") {
        if (key == "")
            return "Ошибка: Не указано значение для удаления";
        if (!isInteger(key)) 
            return "Ошибка: Значение дерева должно быть числом";

        int valInt = stoi(key);
        CompleteBinaryTree* t = findTree(name);
        if (!t) 
            return "Ошибка: Дерево " + name + " не найдено";
        
        string check = t->getNodeInfo(valInt);
        if (check == "Элемент не найден" || check == "Дерево пустое") {
            return "Ошибка: Элемент не найден";
        }
        
        t->remove(valInt);
        return "Удалено: " + key;
    }

    if (cmd == "TSEARCH") {
        if (key == "") 
            return "Ошибка: Не указано значение для поиска";
        if (!isInteger(key)) 
            return "Ошибка: Значение дерева должно быть числом";

        int valInt = stoi(key);
        CompleteBinaryTree* t = findTree(name);
        if (!t) return "Ошибка: Дерево " + name + " не найдено";
        
        return t->getNodeInfo(valInt);
    }
    
    return "Неизвестная команда";
}