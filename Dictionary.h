#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <string>
using namespace std;

struct TreeNode
{
    string word;
    TreeNode *left;
    TreeNode *right;
    int height;

    TreeNode(string word = "");
};

class Dictionary
{
    TreeNode *root;

public:
    Dictionary();
    ~Dictionary();

    void insert(string word);
    bool search(string word);

private:
    void clear(TreeNode *node);
    TreeNode *insert(TreeNode *node, string word);
    bool search(TreeNode *node, string word);
    int getHeight(TreeNode *node);
    int getBalanceFactor(TreeNode *node);
    TreeNode *rightRotate(TreeNode *y);
    TreeNode *leftRotate(TreeNode *x);
};

#endif // DICTIONARY_H
