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

    void loadFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            printw("Error: Failed to open file '%s' for reading.\n", filename.c_str());
            return;
        }

        clear(); // clear the current text before loading new content

        string line;
        while (getline(file, line))
        {
            for (char ch : line)
            {
                insert(ch); // insert each character into the list
            }
            insert('\n'); // add newline at the end of each line
        }

        if (file.bad())
        {
            printw("Error: An error occurred while reading the file.\n");
        }

        file.close();
    }

    void saveFile(const string &filename)
    {
        ofstream file(filename);
        if (!file.is_open())
        {
            printw("Error: Failed to open file '%s' for writing.\n", filename.c_str());
            return;
        }

        ListNode *temp = head;
        while (temp != nullptr)
        {
            file << temp->letter;
            if (file.fail())
            {
                printw("Error: An error occurred while writing to the file.\n");
                file.close();
                return;
            }
            temp = temp->next;
        }

        file.close();
        if (file.fail())
        {
            printw("Error: File did not close properly.\n");
        }
        else
        {
            printw("File saved successfully to '%s'.\n", filename.c_str());
        }
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

    string fileNameToLoad = "", fileNameToSave = ""; // strings for file names
    TextList textList;
    int ch;
    int cursorX = 0; // track cursor's X position for display
    int cursorY = 0; // track cursor's Y position for display

    while ((ch = getch()) != 27) // press ESC to quit
    {
        clear(); // clear screen before updating

        // check for valid characters (printable ASCII range)
        if (ch >= 32 && ch <= 126)
        {
            textList.insert(static_cast<char>(ch)); // insert character into the list
            cursorX++;                              // move cursor position to the right
        }
        else if (ch == 127 || ch == KEY_BACKSPACE) // handle backspace
        {
            textList.remove(); // remove character from the list
            if (cursorX > 0)
            {
                cursorX--; // move cursor left if possible
            }
            else if (cursorY > 0) // move to the previous line if possible
            {
                cursorY--;
                // Set cursorX to the length of the previous line (handle multiline)
                cursorX = 0; // recalculate to the actual length of that line if needed
            }
        }
        else if (ch == KEY_LEFT) // move cursor left
        {
            textList.moveCursorLeft(); // move cursor in the list
            if (cursorX > 0)
            {
                cursorX--; // move cursor left if possible
            }
            else if (cursorY > 0) // move to the previous line if possible
            {
                cursorY--;
                cursorX = 0; // adjust to the actual length of the previous line
            }
        }
        else if (ch == KEY_RIGHT) // move cursor right
        {
            textList.moveCursorRight(); // move cursor in the list
            cursorX++;                  // move cursor right
            // Handle moving past the end of a line (e.g., with newlines)
        }
        else if (ch == KEY_UP) // move cursor up
        {
            if (cursorY > 0)
            {
                cursorY--; // move cursor up if possible
                // Adjust cursorX to fit within the line length
            }
        }
        else if (ch == KEY_DOWN) // move cursor down
        {
            cursorY++; // move cursor down (ensure bounds check)
            // Adjust cursorX to fit within the new line length if needed
        }
        else if (ch == KEY_HOME) // move cursor to start of line
        {
            textList.moveCursorToStart(); // move cursor to start
            cursorX = 0;                  // reset cursor position to the start
        }
        else if (ch == KEY_END) // move cursor to end of line
        {
            textList.moveCursorToEnd(); // move cursor to the end of the list
            cursorX = textList.size();  // set cursor position to the end (adjust for multiline)
        }
        else if (ch == 10) // handle Enter key
        {
            textList.insert('\n'); // insert newline character
            cursorX = 0;           // reset column position
            cursorY++;             // move to the next line
        }
        else if (ch == 12) // Ctrl + L (load file)
        {
            echo();
            printw("Enter file name to load: ");
            char fileNameBuffer[256];        // buffer for file name input
            getstr(fileNameBuffer);          // get file name from user
            fileNameToLoad = fileNameBuffer; // store file name

            clear();                               // clear screen before loading
            textList.loadFromFile(fileNameToLoad); // load file content into the list

            // Reset cursor positions after loading
            cursorX = 0;
            cursorY = 0;
            ListNode *temp = textList.getHead();
            while (temp != nullptr)
            {
                if (temp->letter == '\n')
                {
                    cursorY++;
                    cursorX = 0;
                }
                else
                {
                    cursorX++;
                }
                temp = temp->next;
            }
        }
        else if (ch == 18) // Ctrl + R (save file)
        {
            echo();
            printw("Enter file name to save: ");
            char fileNameBuffer[256];        // buffer for file name input
            getstr(fileNameBuffer);          // get file name from user
            fileNameToSave = fileNameBuffer; // store file name

            clear();                           // clear screen before saving
            textList.saveFile(fileNameToSave); // save list content to the file
        }

        // print updated list content and handle line breaks properly
        string updatedText = textList.toString();
        int currentX = 0, currentY = 0;
        for (char c : updatedText)
        {
            if (c == '\n')
            {
                currentY++;
                currentX = 0;
            }
            else
            {
                mvaddch(currentY, currentX, c); // print each character at the appropriate position
                currentX++;
            }
        }

        // move the ncurses cursor to the correct position
        move(cursorY, cursorX); // set cursor position (row cursorY, column cursorX)

        refresh(); // refresh screen after every input
    }

    endwin(); // end ncurses
    return 0;
}
