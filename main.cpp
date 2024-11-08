#include <iostream>
#include <string>
#include <fstream>
#include <ncurses.h>
using namespace std;

class StackNode
{
public:
    char data;
    StackNode *next;

    StackNode(char data = 0)
    {
        this->data = data;
        this->next = NULL;
    }

    ~StackNode()
    {
        next = NULL;
    }
};

//Stack to hold characters of words inserted by user which form words
class Stack
{
public:
    StackNode *top;

    Stack()
    {
        top = NULL;
    }

    ~Stack()
    {
        while (top != NULL)
        {
            StackNode *temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(char data)
    {
        StackNode *newNode = new StackNode(data);
        newNode->next = top;
        top = newNode;
    }

    char pop()
    {
        if (top == NULL)
        {
            return 0;
        }
        StackNode *temp = top;
        top = top->next;
        char data = temp->data;
        delete temp;
        return data;
    }

    char peek()
    {
        if (top == NULL)
        {
            return 0;
        }
        return top->data;
    }

    bool isEmpty()
    {
        return top == NULL;
    }

};

int main()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    int ch;
    while ((ch = getch()) != 'q')
    {
        printw("Key: %c\n", ch);
    }
    endwin();
    return 0;
}