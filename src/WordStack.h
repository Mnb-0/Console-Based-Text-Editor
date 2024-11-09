#ifndef WORDSTACK_H
#define WORDSTACK_H

#include <iostream>
using namespace std;

struct StackNode
{
    char letter;
    StackNode *next;

    StackNode(char letter = ' ');
    ~StackNode();
};

class WordStack
{
    StackNode *top;

public:
    WordStack();
    ~WordStack();

    void push(char letter);
    char pop();
    char peek();
    bool isEmpty();
    void clear();
    void print();
    string toString();
    int size();
    StackNode *getTop();
    void setTop(StackNode *top);
};

#endif // WORDSTACK_H
