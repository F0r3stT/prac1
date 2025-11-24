#include "stack.h"

Stack::Stack() {
    this->topNode = nullptr;
}

Stack::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

void Stack::push(string value) {
    StackNode* newNode = new StackNode(value, this->topNode);
    this->topNode = newNode;
}

string Stack::pop() {
    if (isEmpty()) 
        return "ОШИБКА: Стек пуст"; 
    
    StackNode* temp = this->topNode;
    string val = temp->value;
    
    this->topNode = this->topNode->next;
    delete temp;
    return val;
}
string Stack::peek() const {
    if (isEmpty()) 
        return "ОШИБКА: Стек пуст";
    return this->topNode->value;
}

bool Stack::isEmpty() const {
    return this->topNode == nullptr;
}

void Stack::saveFile(ofstream& fileStream, const string& name) const {
    StackNode* current = this->topNode;
    while (current != nullptr) {
        fileStream << "STACK " << name << " " << current->value << endl;
        current = current->next;
    }
}