
# Console-Based Text Editor Using Ncurses Library

This project is a console-based text editor built with the **ncurses** library. It includes features like text editing, file handling (load/save), and spell-checking. The editor leverages data structures such as stacks, doubly linked lists, and AVL trees to manage its operations effectively.

---

## Features

- **Text Editing**: Supports text insertion, deletion, and cursor movement.
- **Spell Checking**: Validates words using a dictionary and provides suggestions.
- **File Handling**: Load and save files for editing.
- **Interactive Interface**: Uses ncurses for terminal-based UI.
- **Data Structures**:
  - **WordStack**: Tracks the current word for spell-checking.
  - **TextList**: Doubly linked list to store the text dynamically.
  - **Dictionary**: AVL tree implementation for fast dictionary lookups.

---

## Prerequisites

- **C++ Compiler** (e.g., g++)
- **Ncurses Library**: Install with:
  ```bash
  sudo apt-get install libncurses5-dev libncursesw5-dev
  ```

---

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/text-editor-ncurses.git
   ```
2. Navigate to the project directory:
   ```bash
   cd text-editor-ncurses
   ```
3. Compile the code:
   ```bash
   g++ -o text_editor main.cpp -lncurses
   ```

---

## Usage

1. Run the application:
   ```bash
   ./text_editor
   ```
2. Key Bindings:
   - **ESC**: Exit the editor.
   - **Ctrl + L**: Load a file into the editor.
   - **Ctrl + R**: Save the current text to a file.
   - **Arrow Keys**: Navigate through the text.
   - **Backspace**: Delete a character.
   - **Space**: Finalize a word and trigger spell-check.

3. The application uses a dictionary file (`dictionary.txt`) for spell-checking. Make sure this file exists in the same directory.

---

## File Handling

### Load a File
- Use `Ctrl + L` and enter the file name to load content into the editor.

### Save a File
- Use `Ctrl + R` and enter the desired file name to save the current text.

---

## Spell-Checking and Suggestions

The application checks the spelling of words and suggests corrections in the following ways:
- **Substitution**: Suggests replacing a letter with another.
- **Omission**: Suggests removing a letter.
- **Insertion**: Suggests adding a letter.
- **Reversal**: Suggests swapping adjacent letters.

Suggestions are displayed dynamically as you type.

---

## Code Highlights

### Core Classes and Structures
1. **`WordStack`**: Stack for managing the current word during text entry.
2. **`TextList`**: Doubly linked list for storing the text, allowing efficient editing.
3. **`Dictionary`**: AVL tree for fast dictionary lookups and loading dictionary data from files.

### Key Functions
- **File Operations**:
  - `loadFromFile`: Load text or dictionary data from a file.
  - `saveFile`: Save the current text to a file.
- **Spell Check**:
  - Checks if a word exists in the dictionary and provides suggestions.

---

## Future Enhancements

- Add support for:
  - Multi-line editing improvements.
  - Additional keyboard shortcuts.
  - Dynamic resizing for larger terminal screens.

---

## Contributing

Contributions are welcome! To contribute:
1. Fork the repository.
2. Create a new branch:
   ```bash
   git checkout -b feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -m "Description of changes"
   ```
4. Push to the branch:
   ```bash
   git push origin feature-name
   ```
5. Submit a pull request.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
