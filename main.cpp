// Console Based Text Editor Using Ncurses Library
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

        if (cursor == head) // deleting at head
        {
            head = head->next;
            if (head != nullptr)
            {
                head->prev = nullptr;
            }
            delete cursor;
            cursor = head;
        }
        else if (cursor == tail) // deleting at end
        {
            tail = tail->prev;
            if (tail != nullptr)
            {
                tail->next = nullptr;
            }
            delete cursor;
            cursor = tail;
        }
        else // removing a node in the middle
        {
            ListNode *temp = cursor;
            cursor->prev->next = cursor->next;
            cursor->next->prev = cursor->prev;
            cursor = cursor->prev; // move left after deletion
            delete temp;
        }

        if (head == nullptr) // empty list
        {
            cursor = nullptr;
            tail = nullptr;
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

/*Store dictionary words to support fast lookups and suggest
corrections.*/
struct TreeNode
{
    string word;
    TreeNode *left;
    TreeNode *right;

    TreeNode(string word = "")
    {
        this->word = word;
        left = nullptr;
        right = nullptr;
    }

    ~TreeNode()
    {
        left = nullptr;
        right = nullptr;
    }
};

class Dictionary
{
    TreeNode *root;

public:
    Dictionary()
    {
        root = nullptr;
    }

    ~Dictionary()
    {
        clear(root);
    }

    void clear(TreeNode *node)
    {
        if (node == nullptr)
        {
            return;
        }
        clear(node->left);
        clear(node->right);
        delete node;
    }

    void insert(string word)
    {
        root = insert(root, word);
    }

    TreeNode *insert(TreeNode *node, string word)
    {
        if (node == nullptr)
        {
            return new TreeNode(word);
        }
        if (word < node->word)
        {
            node->left = insert(node->left, word);
        }
        else if (word > node->word)
        {
            node->right = insert(node->right, word);
        }
        return node;
    }

    bool search(string word)
    {
        return search(root, word);
    }

    bool search(TreeNode *node, string word)
    {
        if (node == nullptr)
        {
            return false;
        }
        if (word == node->word)
        {
            return true;
        }
        if (word < node->word)
        {
            return search(node->left, word);
        }
        return search(node->right, word);
    }

    void suggest(string word)
    {
        suggest(root, word);
    }

    void suggest(TreeNode *node, string word)
    {
        if (node == nullptr)
        {
            return;
        }
        if (node->word.find(word) == 0)
        {
            cout << node->word << endl;
        }
        if (word < node->word)
        {
            suggest(node->left, word);
        }
        suggest(node->right, word);
    }

    TreeNode *getRoot()
    {
        return root;
    }

    void setRoot(TreeNode *root)
    {
        this->root = root;
    }

    void print()
    {
        print(root);
    }

    void print(TreeNode *node)
    {
        if (node == nullptr)
        {
            return;
        }
        print(node->left);
        cout << node->word << endl;
        print(node->right);
    }

    void load(string filename)
    {
        ifstream file(filename);
        string word;
        while (file >> word)
        {
            insert(word);
        }
        file.close();
    }

    void save(string filename)
    {
        ofstream file(filename);
        save(root, file);
        file.close();
    }

    void save(TreeNode *node, ofstream &file)
    {
        if (node == nullptr)
        {
            return;
        }
        save(node->left, file);
        file << node->word << endl;
        save(node->right, file);
    }
};

int main()
{
    initscr();
    noecho();             // disable echoing to allow for control over display
    cbreak();             // disable line buffering so enter key not needed
    keypad(stdscr, TRUE); // enable arrow keys

    TextList textList;
    int ch;
    int cursorX = 0; // track cursor's X position for display

    while ((ch = getch()) != 27) // press ESC to quit
    {
        clear(); // clear screen before updating

        // checks for valid characters (printable ASCII range)
        if (ch >= 32 && ch <= 126)
        {
            textList.insert(static_cast<char>(ch)); // insert character into the list
            cursorX++;                              // move cursor position to the right
        }
        else if (ch == 127 || ch == KEY_BACKSPACE) // backspace
        {
            textList.remove(); // remove character from the list
            if (cursorX > 0)
            {
                cursorX--; // move cursor left if possible
            }
        }
        else if (ch == KEY_LEFT)
        {
            textList.moveCursorLeft(); // move cursor to the left in the list
            if (cursorX > 0)
            {
                cursorX--; // move cursor left if possible
            }
        }
        else if (ch == KEY_RIGHT)
        {
            textList.moveCursorRight(); // move cursor to the right in the list
            if (cursorX < textList.size())
            {
                cursorX++; // move cursor right if possible
            }
        }
        else if (ch == KEY_HOME)
        {
            textList.moveCursorToStart(); // move cursor to the start of the list
            cursorX = 0;                  // reset cursor position to the start
        }
        else if (ch == KEY_END)
        {
            textList.moveCursorToEnd(); // move cursor to the end of the list
            cursorX = textList.size();  // set cursor position to the end
        }

        // print updated list
        string updatedText = textList.toString();
        printw("%s", updatedText.c_str());

        // move the ncurses cursor to the correct position
        move(0, cursorX); // set cursor position (row 0, column cursorX)

        refresh(); // refresh after every input
    }

    endwin(); // end ncurses
    return 0;
}
