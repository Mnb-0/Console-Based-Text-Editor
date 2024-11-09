#ifndef TEXTLIST_H
#define TEXTLIST_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct ListNode
{
    char letter;
    ListNode *next;
    ListNode *prev;

    ListNode(char letter = ' ');
    ~ListNode();
};

class TextList
{
    ListNode *head;
    ListNode *tail;
    ListNode *cursor;

public:
    TextList();
    ~TextList();

    void insert(char letter);
    void remove();
    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorToStart();
    void moveCursorToEnd();
    void print();
    string toString();
    int size();
    ListNode *getHead();
    ListNode *getTail();
    ListNode *getCursor();
    void setCursor(ListNode *cursor);
    void loadFromFile(const string &filename);
    void saveFile(const string &filename);
};

#endif // TEXTLIST_H
