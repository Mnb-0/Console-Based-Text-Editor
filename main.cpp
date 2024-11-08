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

class WordStack
{
    StackNode *top;

public:
    WordStack()
    {
        top = nullptr;
    }

    ~WordStack()
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

/*Doubly Linked List: Store the text in the notepad, with each letter as a node.*/
struct ListNode
{
    char letter;
    ListNode *next;
    ListNode *prev;

    ListNode(char letter = ' ')
    {
        this->letter = letter;
        this->next = nullptr;
        this->prev = nullptr;
    }

    ~ListNode()
    {
        next = nullptr;
        prev = nullptr;
    }

};

class TextList
{
    ListNode *head;
    ListNode *tail;
    ListNode *cursor;

public:

    TextList()
    {
        head = nullptr;
        tail = nullptr;
        cursor = nullptr;
    }

    ~TextList()
    {
        while (head != nullptr)
        {
            ListNode *temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insert(char letter)
    {
        ListNode *newNode = new ListNode(letter);
        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
            cursor = newNode;
        }
        else
        {
            if (cursor == nullptr)
            {
                cursor = tail;
            }
            if (cursor == tail)
            {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
            else
            {
                newNode->next = cursor->next;
                newNode->prev = cursor;
                cursor->next->prev = newNode;
                cursor->next = newNode;
            }
            cursor = newNode;
        }
    }

    void remove()
    {
        if (cursor == nullptr)
        {
            return;
        }
        if (cursor == head)
        {
            head = head->next;
            if (head != nullptr)
            {
                head->prev = nullptr;
            }
            delete cursor;
            cursor = head;
        }
        else if (cursor == tail)
        {
            tail = tail->prev;
            tail->next = nullptr;
            delete cursor;
            cursor = nullptr;
        }
        else
        {
            ListNode *temp = cursor;
            cursor->prev->next = cursor->next;
            cursor->next->prev = cursor->prev;
            cursor = cursor->next;
            delete temp;
        }
    }

    void moveCursorLeft()
    {
        if (cursor != nullptr && cursor != head)
        {
            cursor = cursor->prev;
        }
    }

    void moveCursorRight()
    {
        if (cursor != nullptr && cursor != tail)
        {
            cursor = cursor->next;
        }
    }

    void moveCursorToStart()
    {
        cursor = head;
    }

    void moveCursorToEnd()
    {
        cursor = tail;
    }

    void print()
    {
        ListNode *temp = head;
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
        ListNode *temp = head;
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
        ListNode *temp = head;
        while (temp != nullptr)
        {
            count++;
            temp = temp->next;
        }
        return count;
    }

    ListNode *getHead()
    {
        return head;
    }

    ListNode *getTail()
    {
        return tail;
    }

    ListNode *getCursor()
    {
        return cursor;
    }

    void setCursor(ListNode *cursor)
    {
        this->cursor = cursor;
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