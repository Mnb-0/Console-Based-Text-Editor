// Console Based Text Editor Using Ncurses Library
#include <iostream>
#include <string>
#include <fstream>
#include <ncurses.h>
using namespace std;

/* Stack/Queue etc for Word Formation: Track the letters of the most recent
word in a stack or queue, allowing easy access when spell-checking is
triggered. */

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

    void loadFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            printw("Error: Failed to open file '%s' for reading.\n", filename.c_str());
            return;
        }

        string word;
        while (file >> word)
        {
            insert(word);
        }

        if (file.bad())
        {
            printw("Error: An error occurred while reading the file.\n");
        }

        file.close();
    }
};

void spellCheck(Dictionary &dictionary, const string &word)
{
    if (dictionary.search(word))
    {
        printw("'%s' is correctly spelled.\n", word.c_str());
    }
    else
    {
        printw("'%s' is misspelled. Suggestions:\n", word.c_str());

        // Letter Substitution
        for (unsigned int i = 0; i < word.length(); i++)
        {
            for (char c = 'a'; c <= 'z'; c++)
            {
                if (c != word[i])
                {
                    string modified = word;
                    modified[i] = c;
                    if (dictionary.search(modified))
                    {
                        printw("Substitution: %s\n", modified.c_str());
                    }
                }
            }
        }

        // Letter Omission
        for (unsigned int i = 0; i < word.length(); i++)
        {
            string modified = word.substr(0, i) + word.substr(i + 1);
            if (dictionary.search(modified))
            {
                printw("Omission: %s\n", modified.c_str());
            }
        }

        // Letter Insertion
        for (unsigned int i = 0; i <= word.length(); i++) // Note <= for insertion at the end
        {
            for (char c = 'a'; c <= 'z'; c++)
            {
                string modified = word.substr(0, i) + c + word.substr(i);
                if (dictionary.search(modified))
                {
                    printw("Insertion: %s\n", modified.c_str());
                }
            }
        }

        // Letter Reversal
        for (unsigned int i = 0; i < word.length() - 1; i++)
        {
            string modified = word;
            swap(modified[i], modified[i + 1]);
            if (dictionary.search(modified))
            {
                printw("Reversal: %s\n", modified.c_str());
            }
        }
    }
}
void displayStatus()
{
    move(0, 0); // Move to the first row
    clrtoeol(); // Clear the line to avoid overwritten text
    printw("Press ESC to quit. Ctrl + L to load. Ctrl + R to save.");
}

void displaySuggestions(const string &currentWord, Dictionary &dictionary)
{
    move(1, 0); // Move to the reserved row for suggestions
    clrtoeol();
    if (!currentWord.empty())
    {
        printw("Suggestions: ");
        for (char c = 'a'; c <= 'z'; ++c)
        {
            string temp = currentWord + c;
            if (dictionary.search(temp))
            {
                printw("%s ", temp.c_str());
            }
        }
    }
}

void handleBackspace(TextList &textList, WordStack &wordStack, string &currentWord, int &cursorX, int cursorY)
{
    if (cursorX > 0)
    {
        textList.remove(); // Remove character from the list
        wordStack.pop();   // Remove character from the stack
        if (!currentWord.empty())
        {
            currentWord.pop_back(); // Remove last character from currentWord
        }
        cursorX--;              // Move cursor left
        move(cursorY, cursorX); // Move cursor to the new position
        clrtoeol();             // Clear the rest of the line to remove leftover character
    }
}

void handleSpace(TextList &textList, WordStack &wordStack, string &currentWord, Dictionary &dictionary, int &cursorX)
{
    textList.insert(' ');                // Insert space into the list
    spellCheck(dictionary, currentWord); // Check the current word
    wordStack.clear();                   // Clear the stack for the next word
    currentWord.clear();                 // Reset current word tracker
    cursorX++;                           // Move cursor position to the right
}

void handleFileLoad(TextList &textList, int &cursorX, int &cursorY)
{
    echo();
    move(0, 0);
    clrtoeol();
    printw("Enter file name to load: ");
    char fileNameBuffer[256];               // Buffer for file name input
    getstr(fileNameBuffer);                 // Get file name from user
    string fileNameToLoad = fileNameBuffer; // Store file name
    noecho();
    clear();                               // Clear screen before loading
    textList.loadFromFile(fileNameToLoad); // Load file content into the list

    // Reset cursor positions after loading
    cursorX = 0;
    cursorY = 2;
}

void handleFileSave(TextList &textList)
{
    echo();
    move(0, 0);
    clrtoeol();
    printw("Enter file name to save: ");
    char fileNameBuffer[256];               // Buffer for file name input
    getstr(fileNameBuffer);                 // Get file name from user
    string fileNameToSave = fileNameBuffer; // Store file name
    noecho();
    clear();                           // Clear screen before saving
    textList.saveFile(fileNameToSave); // Save list content to the file
    move(0, 0);                        // Move back to the status line
    clrtoeol();
    printw("File saved successfully to '%s'.", fileNameToSave.c_str());
}

void printTextContent(TextList &textList, int &cursorX, int &cursorY)
{
    string updatedText = textList.toString();
    int currentX = 0, currentY = 2; // Start printing from the third row
    for (char c : updatedText)
    {
        if (c == '\n')
        {
            currentY++;
            currentX = 0;
        }
        else
        {
            mvaddch(currentY, currentX, c); // Print each character at the appropriate position
            currentX++;
        }
    }
    cursorX = currentX; // Update cursor position
    cursorY = currentY;
}

int main()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    string fileNameToLoad = "", fileNameToSave = ""; // Strings for file names
    TextList textList;
    WordStack wordStack;
    Dictionary dictionary; // Added dictionary for suggestions
    dictionary.loadFromFile("dictionary.txt");

    int ch;
    int cursorX = 0;         // Track cursor's X position for display
    int cursorY = 2;         // Start text entry below the "Suggestions" section
    string currentWord = ""; // Track current word being formed

    while ((ch = getch()) != 27) // Press ESC to quit
    {
        displayStatus();
        displaySuggestions(currentWord, dictionary);

        // Handle keyboard inputs
        if (ch >= 32 && ch <= 126) // Printable ASCII range
        {
            textList.insert(static_cast<char>(ch)); // Insert character into the list
            wordStack.push(static_cast<char>(ch));  // Push character into the stack
            currentWord += static_cast<char>(ch);   // Build the current word
            cursorX++;                              // Move cursor position to the right
        }
        else if (ch == 127 || ch == KEY_BACKSPACE)
        {
            handleBackspace(textList, wordStack, currentWord, cursorX, cursorY);
        }
        else if (ch == ' ') // Space indicates end of word
        {
            handleSpace(textList, wordStack, currentWord, dictionary, cursorX);
        }
        else if (ch == KEY_LEFT) // Move cursor left
        {
            textList.moveCursorLeft(); // Move cursor in the list
            if (cursorX > 0)
            {
                cursorX--; // Move cursor left if possible
            }
        }
        else if (ch == KEY_RIGHT) // Move cursor right
        {
            textList.moveCursorRight(); // Move cursor in the list
            cursorX++;                  // Move cursor right
        }
        else if (ch == 12) // Ctrl + L (load file)
        {
            handleFileLoad(textList, cursorX, cursorY);
        }
        else if (ch == 18) // Ctrl + R (save file)
        {
            handleFileSave(textList);
        }

        // Print updated text content and handle line breaks properly
        printTextContent(textList, cursorX, cursorY);

        // Move the ncurses cursor to the correct position
        move(cursorY, cursorX); // Set cursor position (row cursorY, column cursorX)

        refresh(); // Refresh screen after every input
    }

    endwin(); // End ncurses
    return 0;
}