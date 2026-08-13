# Console Text Editor

A multi-line, multi-threaded console text editor built with ncurses. It supports real editing (not just single-line typing), background spell-checking against a 455K-word dictionary, undo/redo, selection/clipboard, find and replace, and autosave - all without ever blocking the UI thread.

---

## Building

Requires a C++17 compiler, CMake 3.16+, and ncurses development headers.

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev cmake
cmake -S . -B build
cmake --build build -j4
./build/texteditor            # start empty
./build/texteditor some_file  # open a file
```

`dictionary.txt` must be present in the working directory the editor is run from - it's loaded in the background on startup.

### Running the tests

```bash
cmake --build build -j4
./build/tests/unit_tests
```

30+ zero-dependency unit tests (`tests/harness.h` - no vendored framework) cover the text buffer, undo coalescing, dictionary normalization, suggestion generation, file round-tripping, and the thread pool / event queue under concurrent load.

### Sanitizer builds

```bash
# Memory + undefined behavior
cmake -S . -B build-asan -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer" -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined"
cmake --build build-asan -j4 && ./build-asan/tests/unit_tests

# Data races (separate build - can't combine with ASan)
cmake -S . -B build-tsan -DCMAKE_CXX_FLAGS="-fsanitize=thread -fno-omit-frame-pointer" -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=thread"
cmake --build build-tsan -j4 && ./build-tsan/tests/unit_tests
```

(Under WSL specifically, TSan needs `setarch "$(uname -m)" -R <binary>` to work around a memory-mapping incompatibility - not needed on native Linux.)

### Smoke-testing the interactive binary

ncurses needs a real terminal, so `tools/drive.py` drives the binary through a pseudo-terminal and reports how it exited:

```bash
python3 tools/drive.py build/texteditor "This  is"
```

---

## Key bindings

| Key | Action |
|---|---|
| Arrow keys | Move cursor |
| Home / End | Start / end of line |
| Page Up / Page Down | Scroll a page |
| Enter | New line |
| Backspace / Delete | Delete character |
| Ctrl+A | Toggle selection mode (extend with arrow keys) |
| Ctrl+K | Copy selection |
| Ctrl+X | Cut selection |
| Ctrl+V | Paste |
| Ctrl+Z / Ctrl+Y | Undo / redo |
| Ctrl+W | Show suggestions for the word at the cursor |
| Ctrl+L | Load a file |
| Ctrl+R | Save (uses the remembered filename, or prompts once) |
| Ctrl+D | Save as (always prompts) |
| Ctrl+F | Find next |
| Ctrl+E | Find and replace all |
| ESC | Quit (confirms if there are unsaved changes) |

The editor also autosaves a few seconds after you stop typing, if it already has a filename.

---

## Architecture

```
src/
  core/          TextBuffer (lines + cursor), UndoStack, Document, Clipboard
  spell/         Dictionary (unordered_set), Suggester, background scanner
  ui/             Screen (RAII ncurses), Renderer, StatusBar, Prompt, Editor (event loop)
  io/             File load/save
  concurrent/    ThreadPool, EventQueue, Snapshot
tests/           Zero-dependency unit tests
tools/drive.py   pty-based smoke test driver
```

`core`, `spell`, `io`, and `concurrent` have no ncurses dependency, which is what makes them unit-testable and lets `ui/Editor.cpp` be the only place that has to reason about the terminal.

### Threading model

**ncurses is not thread-safe**, so the rule throughout is: *the main thread exclusively owns the screen and the live `Document`.* Worker threads (via `ThreadPool`) never call ncurses and never touch mutable document state - they receive an immutable `BufferSnapshot` (a `shared_ptr<const vector<string>>`) and post results back through `EventQueue`, which the main thread drains once per loop iteration.

Two techniques keep this race-free without heavyweight locking:

- **Version staleness.** Every edit bumps an `atomic<int>` document version. A background spell scan captures that version before it starts; if the version has moved by the time results come back, they're discarded instead of overwriting newer state.
- **Per-request cancellation.** Suggestion requests (Ctrl+W) each own a `shared_ptr<atomic<bool>>` cancel flag. Requesting a new suggestion flips the *previous* request's flag, so an in-flight computation for a word you've since moved away from can bail out early - without a single flag being reused unsafely across requests.

Shutdown order matters and is enforced by C++'s own member-destruction rules: `ThreadPool` is declared last in `Editor`, so it's destroyed *first* (members are torn down in reverse declaration order), joining every worker thread before the objects they might still reference - the event queue, the dictionary - go away. `Screen` is declared first, so it's destroyed *last*, keeping the terminal alive until everything else has shut down cleanly. See the comment on `Editor`'s member list in `src/ui/Editor.h` before reordering anything there.

### History

This started as a single `main.cpp` built around a hand-rolled doubly linked list of characters, a stack, and an AVL-tree dictionary - a design that made multi-line editing (no Enter key handling, no up/down navigation) and cursor tracking (the screen cursor was recomputed by walking the whole buffer every frame, independent of the actual cursor position) essentially unworkable. A crash fix along the way (commit `e1308b4`) addressed an unsigned-integer underflow in the spell checker's suggestion loop, triggered by pressing space twice in a row. This rewrite keeps that fix (now `spell/Suggester.cpp`, with a regression test in `tests/test_suggester.cpp`) and replaces the rest with the module layout above.
