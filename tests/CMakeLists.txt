add_executable(unit_tests
    test_main.cpp
    test_textbuffer.cpp
    test_undo.cpp
    test_dictionary.cpp
    test_suggester.cpp
    test_eventqueue.cpp
    test_fileio.cpp
)
target_link_libraries(unit_tests PRIVATE editor_core)
add_test(NAME unit_tests COMMAND unit_tests)
