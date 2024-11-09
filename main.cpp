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
    int height;

    TreeNode(string word = "")
    {
        this->word = word;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

class Dictionary
{
    TreeNode *root;

public:
    Dictionary() { root = nullptr; }

    ~Dictionary() { clear(root); }

    // Destructor helper function
    void clear(TreeNode *node)
    {
        if (node == nullptr)
            return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    int getHeight(TreeNode *node)
    {
        return node == nullptr ? 0 : node->height;
    }

    int getBalanceFactor(TreeNode *node)
    {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    TreeNode *rightRotate(TreeNode *y)
    {
        TreeNode *x = y->left;
        TreeNode *T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        // Return new root
        return x;
    }

    TreeNode *leftRotate(TreeNode *x)
    {
        TreeNode *y = x->right;
        TreeNode *T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        // Return new root
        return y;
    }

    TreeNode *insert(TreeNode *node, string word)
    {
        // Standard BST insertion
        if (node == nullptr)
            return new TreeNode(word);

        if (word < node->word)
            node->left = insert(node->left, word);
        else if (word > node->word)
            node->right = insert(node->right, word);
        else // Duplicate words not allowed
            return node;

        // Update height of this ancestor node
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        // Get the balance factor
        int balance = getBalanceFactor(node);

        // If the node becomes unbalanced, then there are 4 cases

        // Left Left Case
        if (balance > 1 && word < node->left->word)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && word > node->right->word)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && word > node->left->word)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && word < node->right->word)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // Return the unchanged node pointer
        return node;
    }

    void insert(string word)
    {
        root = insert(root, word);
    }

    bool search(TreeNode *node, string word)
    {
        if (node == nullptr)
            return false;
        if (word == node->word)
            return true;
        if (word < node->word)
            return search(node->left, word);
        return search(node->right, word);
    }

    bool search(string word)
    {
        return search(root, word);
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
    WordStack wordStack;
    int ch;
    int cursorX = 0; // track cursor's X position for display
    int cursorY = 1; // reserve the first row for status messages

    while ((ch = getch()) != 27) // press ESC to quit
    {
        // Print status message at the top row
        move(0, 0); // Move to the first row
        clrtoeol(); // Clear the line to avoid overwritten text
        printw("Press ESC to quit. Ctrl + L to load. Ctrl + R to save.");

        // Clear the area below the status bar without clearing the status bar itself
        move(1, 0); // Move to the line below the status bar
        clrtobot(); // Clear from the current line to the bottom of the screen

        // Check for valid characters (printable ASCII range)
        if (ch >= 32 && ch <= 126)
        {
            textList.insert(static_cast<char>(ch)); // insert character into the list
            wordStack.push(static_cast<char>(ch));  // push character into the stack
            cursorX++;                              // move cursor position to the right
        }
        else if (ch == 127 || ch == KEY_BACKSPACE) // handle backspace
        {
            if (cursorX > 0) // If not at the start of the line
            {
                textList.remove();      // remove character from the list
                cursorX--;              // move cursor left
                move(cursorY, cursorX); // move the cursor position
                clrtoeol();             // clear the rest of the line to remove any leftover characters
            }
            else if (cursorY > 1) // If at the start of the line and there is a line above
            {
                // Move to the previous line
                cursorY--;
                textList.moveCursorLeft(); // move cursor to the end of the previous line

                // Calculate the new cursorX position to the end of the previous line
                ListNode *temp = textList.getHead();
                int newCursorX = 0;
                int lineCount = 1;

                while (temp != nullptr)
                {
                    if (lineCount == cursorY)
                    {
                        if (temp->letter == '\n')
                            break;
                        newCursorX++;
                    }
                    if (temp->letter == '\n')
                        lineCount++;
                    temp = temp->next;
                }

                cursorX = newCursorX;   // set cursorX to the end of the previous line
                textList.remove();      // remove character at the new cursor position
                move(cursorY, cursorX); // move the cursor to the new position
                clrtoeol();             // clear the rest of the line
            }
        }
        else if (ch == KEY_LEFT) // move cursor left
        {
            textList.moveCursorLeft(); // move cursor in the list
            if (cursorX > 0)
            {
                cursorX--; // move cursor left if possible
            }
            else if (cursorY > 1) // move to the previous line if possible
            {
                cursorY--;
                cursorX = 0; // adjust to the actual length of the previous line

                // Calculate the length of the previous line
                ListNode *temp = textList.getHead();
                int newCursorX = 0;
                int lineCount = 1;
                while (temp != nullptr)
                {
                    if (lineCount == cursorY)
                    {
                        if (temp->letter == '\n')
                            break;
                        newCursorX++;
                    }
                    if (temp->letter == '\n')
                        lineCount++;
                    temp = temp->next;
                }
                cursorX = newCursorX; // set cursorX to the end of the previous line
            }
        }
        else if (ch == KEY_RIGHT) // move cursor right
        {
            textList.moveCursorRight(); // move cursor in the list
            cursorX++;                  // move cursor right
        }
        else if (ch == KEY_UP) // move cursor up
        {
            if (cursorY > 1)
            {
                cursorY--; // move cursor up if possible
            }
        }
        else if (ch == KEY_DOWN) // move cursor down
        {
            cursorY++; // move cursor down (ensure bounds check)
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
            move(0, 0);
            clrtoeol();
            printw("Enter file name to load: ");
            char fileNameBuffer[256];        // buffer for file name input
            getstr(fileNameBuffer);          // get file name from user
            fileNameToLoad = fileNameBuffer; // store file name
            noecho();
            clear();                               // clear screen before loading
            textList.loadFromFile(fileNameToLoad); // load file content into the list

            // reset cursor positions after loading
            cursorX = 0;
            cursorY = 1;
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
            move(0, 0);
            clrtoeol();
            printw("Enter file name to save: ");
            char fileNameBuffer[256];        // buffer for file name input
            getstr(fileNameBuffer);          // get file name from user
            fileNameToSave = fileNameBuffer; // store file name
            noecho();
            clear();                           // clear screen before saving
            textList.saveFile(fileNameToSave); // save list content to the file
            move(0, 0);                        // Move back to the status line
            clrtoeol();
            printw("File saved successfully to '%s'.", fileNameToSave.c_str());
        }

        // Print updated list content and handle line breaks properly
        string updatedText = textList.toString();
        int currentX = 0, currentY = 1; // Start printing from the second row
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

        // Move the ncurses cursor to the correct position
        move(cursorY, cursorX); // set cursor position (row cursorY, column cursorX)

        refresh(); // refresh screen after every input
    }

    endwin(); // end ncurses
    return 0;
}
