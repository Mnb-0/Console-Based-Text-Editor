// Console Based Text Editor Using Ncurses Library
#include <iostream>
#include <string>
#include <fstream>
#include <ncurses.h>
#include "TextList.h"
#include "WordStack.h"
#include "Dictionary.h"
using namespace std;

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
