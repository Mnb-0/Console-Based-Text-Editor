//Console Based Text Editor Using Ncurses Library
#include <iostream>
#include <string>
#include <fstream>
#include <ncurses.h>
using namespace std;

/*Stack/Queue etc for Word Formation: Track the letters of the most recent
word in a stack or queue, allowing easy access when spell-checking is
triggered.*/

struct StackNode
{
    char letter;
    StackNode *next;

    StackNode(char letter = ' ')
    {
        this->letter = letter;
        this->next = NULL;
    }

    ~StackNode()
    {
        next = NULL;
    }
};

class Stack
{
    StackNode *top;

public:
    Stack()
    {
        top = nullptr;
    }

    ~Stack()
    {
        while (top != nullptr)
        {
            StackNode *temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(char letter)
    {
        StackNode *newNode = new StackNode(letter);
        newNode->next = top;
        top = newNode;
    }

    char pop()
    {
        if (top == nullptr)
        {
            return ' ';
        }
        StackNode *temp = top;
        top = top->next;
        char letter = temp->letter;
        delete temp;
        return letter;
    }

    char peek()
    {
        if (top == nullptr)
        {
            return ' ';
        }
        return top->letter;
    }

    bool isEmpty()
    {
        return top == nullptr;
    }

    void clear()
    {
        while (top != nullptr)
        {
            StackNode *temp = top;
            top = top->next;
            delete temp;
        }
    }

    void print()
    {
        StackNode *temp = top;
        while (temp != nullptr)
        {
            cout << temp->letter;
            temp = temp->next;
        }
        cout << endl;
    }

    string toString()
    {
        string str = "";
        StackNode *temp = top;
        while (temp != nullptr)
        {
            str += temp->letter;
            temp = temp->next;
        }
        return str;
    }

    int size()
    {
        int count = 0;
        StackNode *temp = top;
        while (temp != nullptr)
        {
            count++;
            temp = temp->next;
        }
        return count;
    }

    StackNode *getTop()
    {
        return top;
    }

    void setTop(StackNode *top)
    {
        this->top = top;
    }

};

class ListNode
{
public:
    string word;
    ListNode *next;

    ListNode(string word = "")
    {
        this->word = word;
        this->next = NULL;
    }

    ~ListNode()
    {
        next = NULL;
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