#include "WordStack.h"

StackNode::StackNode(char letter)
{
    this->letter = letter;
    this->next = nullptr;
}

StackNode::~StackNode()
{
    next = nullptr;
}

WordStack::WordStack()
{
    top = nullptr;
}

WordStack::~WordStack()
{
    while (top != nullptr)
    {
        StackNode *temp = top;
        top = top->next;
        delete temp;
    }
}

void WordStack::push(char letter)
{
    StackNode *newNode = new StackNode(letter);
    newNode->next = top;
    top = newNode;
}

char WordStack::pop()
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

char WordStack::peek()
{
    if (top == nullptr)
    {
        return ' ';
    }
    return top->letter;
}

bool WordStack::isEmpty()
{
    return top == nullptr;
}

void WordStack::clear()
{
    while (top != nullptr)
    {
        StackNode *temp = top;
        top = top->next;
        delete temp;
    }
}

void WordStack::print()
{
    StackNode *temp = top;
    while (temp != nullptr)
    {
        cout << temp->letter;
        temp = temp->next;
    }
    cout << endl;
}

string WordStack::toString()
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

int WordStack::size()
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

StackNode *WordStack::getTop()
{
    return top;
}

void WordStack::setTop(StackNode *top)
{
    this->top = top;
}
