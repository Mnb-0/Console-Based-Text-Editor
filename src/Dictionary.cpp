#include "Dictionary.h"

TreeNode::TreeNode(string word)
{
    this->word = word;
    left = nullptr;
    right = nullptr;
    height = 1;
}

Dictionary::Dictionary()
{
    root = nullptr;
}

Dictionary::~Dictionary()
{
    clear(root);
}

void Dictionary::clear(TreeNode *node)
{
    if (node == nullptr)
        return;
    clear(node->left);
    clear(node->right);
    delete node;
}

int Dictionary::getHeight(TreeNode *node)
{
    return node == nullptr ? 0 : node->height;
}

int Dictionary::getBalanceFactor(TreeNode *node)
{
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

TreeNode *Dictionary::rightRotate(TreeNode *y)
{
    TreeNode *x = y->left;
    TreeNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

TreeNode *Dictionary::leftRotate(TreeNode *x)
{
    TreeNode *y = x->right;
    TreeNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

TreeNode *Dictionary::insert(TreeNode *node, string word)
{
    if (node == nullptr)
        return new TreeNode(word);

    if (word < node->word)
        node->left = insert(node->left, word);
    else if (word > node->word)
        node->right = insert(node->right, word);
    else
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);

    if (balance > 1 && word < node->left->word)
        return rightRotate(node);

    if (balance < -1 && word > node->right->word)
        return leftRotate(node);

    if (balance > 1 && word > node->left->word)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && word < node->right->word)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void Dictionary::insert(string word)
{
    root = insert(root, word);
}

bool Dictionary::search(TreeNode *node, string word)
{
    if (node == nullptr)
        return false;
    if (word == node->word)
        return true;
    if (word < node->word)
        return search(node->left, word);
    return search(node->right, word);
}

bool Dictionary::search(string word)
{
    return search(root, word);
}
