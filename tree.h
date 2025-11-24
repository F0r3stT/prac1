#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class CompleteBinaryTree {
private:
    class TreeNode {
    public:
        int data;
        TreeNode* left;
        TreeNode* right;
        
        TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
    };

    TreeNode* root;
    int size;

    void deleteTreeRecurs(TreeNode* node);
    TreeNode* findMin(TreeNode* node);
    TreeNode* deleteNodeRecurs(TreeNode* root, int value, int& sizeRef);
    void saveRecurs(TreeNode* node, ofstream& fileStream, const string& name);

public:
    CompleteBinaryTree();
    ~CompleteBinaryTree();

    void insert(int value);
    void remove(int value);
    string getNodeInfo(int value);
    void saveToFile(ofstream& fileStream, const string& name);
};

#endif