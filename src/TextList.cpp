#include "TextList.h"

ListNode::ListNode(char letter)
{
    this->letter = letter;
    this->next = nullptr;
    this->prev = nullptr;
}

ListNode::~ListNode()
{
    next = nullptr;
    prev = nullptr;
}

TextList::TextList()
{
    head = nullptr;
    tail = nullptr;
    cursor = nullptr;
}

TextList::~TextList()
{
    while (head != nullptr)
    {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
}

void TextList::insert(char letter)
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

void TextList::remove()
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
        if (tail != nullptr)
        {
            tail->next = nullptr;
        }
        delete cursor;
        cursor = tail;
    }
    else
    {
        ListNode *temp = cursor;
        cursor->prev->next = cursor->next;
        cursor->next->prev = cursor->prev;
        cursor = cursor->prev;
        delete temp;
    }

    if (head == nullptr)
    {
        cursor = nullptr;
        tail = nullptr;
    }
}

void TextList::moveCursorLeft()
{
    if (cursor != nullptr && cursor != head)
    {
        cursor = cursor->prev;
    }
}

void TextList::moveCursorRight()
{
    if (cursor != nullptr && cursor != tail)
    {
        cursor = cursor->next;
    }
}

void TextList::moveCursorToStart()
{
    cursor = head;
}

void TextList::moveCursorToEnd()
{
    cursor = tail;
}

void TextList::print()
{
    ListNode *temp = head;
    while (temp != nullptr)
    {
        cout << temp->letter;
        temp = temp->next;
    }
    cout << endl;
}

string TextList::toString()
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

int TextList::size()
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

ListNode *TextList::getHead()
{
    return head;
}

ListNode *TextList::getTail()
{
    return tail;
}

ListNode *TextList::getCursor()
{
    return cursor;
}

void TextList::setCursor(ListNode *cursor)
{
    this->cursor = cursor;
}

void TextList::loadFromFile(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Failed to open file '" << filename << "' for reading." << endl;
        return;
    }

    head = tail = cursor = nullptr; // clear current content

    string line;
    while (getline(file, line))
    {
        for (char ch : line)
        {
            insert(ch);
        }
        insert('\n'); // insert newline at the end of each line
    }

    if (file.bad())
    {
        cerr << "Error: An error occurred while reading the file." << endl;
    }

    file.close();
}

void TextList::saveFile(const string &filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Failed to open file '" << filename << "' for writing." << endl;
        return;
    }

    ListNode *temp = head;
    while (temp != nullptr)
    {
        file << temp->letter;
        temp = temp->next;
    }

    if (file.fail())
    {
        cerr << "Error: An error occurred while writing to the file." << endl;
    }

    file.close();
}
