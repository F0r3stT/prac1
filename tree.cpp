#include "tree.h"

using namespace std;


CompleteBinaryTree::CompleteBinaryTree() {
    root = nullptr;
    size = 0;
}

CompleteBinaryTree::~CompleteBinaryTree() {
    deleteTreeRecurs(root);
}

void CompleteBinaryTree::deleteTreeRecurs(TreeNode* node) {
    if (node == nullptr)
         return;
    deleteTreeRecurs(node->left);
    deleteTreeRecurs(node->right);
    delete node;
}

//поиск минимума для удаления
auto CompleteBinaryTree::findMin(TreeNode* node) -> TreeNode* {
    while (node && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

//рекурсивное удаление
auto CompleteBinaryTree::deleteNodeRecurs(TreeNode* root, int value, int& sizeRef) -> TreeNode* {
    if (root == nullptr) return nullptr;

    if (value < root->data) {
        root->left = deleteNodeRecurs(root->left, value, sizeRef);
    } else if (value > root->data) {
        root->right = deleteNodeRecurs(root->right, value, sizeRef);
    } else {
        if (root->left == nullptr) {
            TreeNode* temp = root->right;
            delete root;
            sizeRef--;
            return temp;
        } else if (root->right == nullptr) {
            TreeNode* temp = root->left;
            delete root;
            sizeRef--;
            return temp;
        }
        
        TreeNode* successor = findMin(root->right);
        root->data = successor->data;
        root->right = deleteNodeRecurs(root->right, successor->data, sizeRef);
    }
    return root;
}

void CompleteBinaryTree::saveRecurs(TreeNode* node, ofstream& fileStream, const string& name) {
    if (node == nullptr) 
        return;
    fileStream << "TREE " << name << " " << node->data << endl;
    saveRecurs(node->left, fileStream, name);
    saveRecurs(node->right, fileStream, name);
}


void CompleteBinaryTree::insert(int value) {
    TreeNode* newNode = new TreeNode(value);
    
    if (root == nullptr) {
        root = newNode;
        size++;
        return;
    }
    
    TreeNode* current = root;
    while (true) {
        if (value < current->data) {
            if (current->left == nullptr) {
                current->left = newNode;
                size++;
                return;
            }
            current = current->left;
        } else if (value > current->data) {
            if (current->right == nullptr) {
                current->right = newNode;
                size++;
                return;
            }
            current = current->right;
        } else {
            delete newNode;
            return;
        }
    }
}

void CompleteBinaryTree::remove(int value) {
    if (root == nullptr) return;
    root = deleteNodeRecurs(root, value, size);
}

//Чтение
string CompleteBinaryTree::getInfo(int value) {
    if (root == nullptr) 
        return "Дерево пустое";
    
    TreeNode* current = root;
    int layer = 1;
    string direction = "Корень"; 
    
    while(current != nullptr) {
        if(current->data == value) {
            return "Слой: " + to_string(layer) + " Значение: " + to_string(value) + " Поз: " + direction;
        }
        
        layer++;
        if (value < current->data) {
            current = current->left;
            direction = "(L)";
        } else {
            current = current->right;
            direction = "(R)";
        }
    }
    return "Элемент не найден";
}

void CompleteBinaryTree::saveToFile(ofstream& fileStream, const string& name) {
    saveRecurs(root, fileStream, name);
}