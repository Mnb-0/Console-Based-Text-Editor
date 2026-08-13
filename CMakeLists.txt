cmake_minimum_required(VERSION 3.16)
project(texteditor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

set(CORE_SOURCES
    src/core/TextBuffer.cpp
    src/core/UndoStack.cpp
    src/core/Document.cpp
    src/spell/Dictionary.cpp
    src/spell/Suggester.cpp
    src/spell/SpellChecker.cpp
    src/io/FileIO.cpp
    src/concurrent/ThreadPool.cpp
)

add_library(editor_core STATIC ${CORE_SOURCES})
target_include_directories(editor_core PUBLIC src)
target_link_libraries(editor_core PUBLIC Threads::Threads)

add_executable(texteditor
    src/main.cpp
    src/ui/Screen.cpp
    src/ui/Renderer.cpp
    src/ui/StatusBar.cpp
    src/ui/Prompt.cpp
    src/ui/Editor.cpp
)
target_include_directories(texteditor PRIVATE ${CURSES_INCLUDE_DIR} src)
target_link_libraries(texteditor PRIVATE editor_core ${CURSES_LIBRARIES})

option(BUILD_TESTS "Build the zero-dependency unit test suite" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
