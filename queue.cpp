#include "queue.h"

Queue::Queue() {
    this->frontNode = nullptr;
    this->rearNode = nullptr;
}

//для создания новой очереди из старой
Queue::Queue(const Queue& other) {
    this->frontNode = nullptr;
    this->rearNode = nullptr;

    //проходим по другой очереди и копируем элементы себе
    QNode* temp = other.frontNode;
    while (temp != nullptr) {
        push(temp->value); // push сам создаст новые узлы
        temp = temp->next;
    }
}

//вызывается при приравнивании уже существующих очередей
Queue& Queue::operator=(const Queue& other) {
    if (this == &other) {
        return *this;
    }

    //очистка чтобы не было утечки памяти
    while (!isEmpty()) {
        pop();
    }

    //копия данных из другой очереди
    QNode* temp = other.frontNode;
    while (temp != nullptr) {
        push(temp->value);
        temp = temp->next;
    }

    return *this;
}

Queue::~Queue() {
    while (!isEmpty()) {
        pop();
    }
}

void Queue::push(string value) {
    QNode* newNode = new QNode(value);
    if (isEmpty()) {
        this->frontNode = newNode;
        this->rearNode = newNode;
    } 
    else {
        this->rearNode->next = newNode;
        this->rearNode = newNode;
    }
}

string Queue::pop() {
    if (isEmpty()) 
        return "ОШИБКА: Очередь пуста";

    QNode* temp = this->frontNode;
    string val = temp->value;

    this->frontNode = this->frontNode->next;
    if (this->frontNode == nullptr) {
        this->rearNode = nullptr;
    }
    delete temp;
    return val;
}

bool Queue::isEmpty() const {
    return this->frontNode == nullptr;
}


void Queue::saveFile(ofstream& fileStream, const string& name) const {
    QNode* current = this->frontNode;
    while (current != nullptr) {
        fileStream << "QUEUE " << name << " " << current->value << endl;
        current = current->next;
    }
}