#include "core/TextBuffer.h"
#include "harness.h"

using namespace editor;

TEST(insert_and_read_back) {
    TextBuffer buf;
    buf.insertText({0, 0}, "hello");
    CHECK_EQ(buf.lines()[0], std::string("hello"));
    CHECK_EQ(buf.cursor().col, 5);
}

TEST(newline_splits_line) {
    TextBuffer buf;
    buf.insertText({0, 0}, "helloworld");
    buf.insertText({0, 5}, "\n");
    CHECK_EQ(buf.lineCount(), 2);
    CHECK_EQ(buf.lines()[0], std::string("hello"));
    CHECK_EQ(buf.lines()[1], std::string("world"));
}

TEST(erase_range_single_line) {
    TextBuffer buf;
    buf.insertText({0, 0}, "hello");
    std::string erased = buf.eraseRange({0, 1}, {0, 3});
    CHECK_EQ(erased, std::string("el"));
    CHECK_EQ(buf.lines()[0], std::string("hlo"));
}

TEST(erase_range_across_lines_joins) {
    TextBuffer buf;
    buf.insertText({0, 0}, "foo\nbar");
    std::string erased = buf.eraseRange({0, 2}, {1, 1});
    CHECK_EQ(buf.lineCount(), 1);
    CHECK_EQ(buf.lines()[0], std::string("foar"));
    CHECK_EQ(erased, std::string("o\nb"));
}

TEST(move_up_down_preserves_desired_column) {
    TextBuffer buf;
    buf.insertText({0, 0}, "hi\nlonger line\nhi");
    buf.setCursor({1, 8});
    buf.moveDown();
    CHECK_EQ(buf.cursor().row, 2);
    CHECK_EQ(buf.cursor().col, 2); // clamped to the short line's length
    buf.moveUp();
    CHECK_EQ(buf.cursor().row, 1);
    CHECK_EQ(buf.cursor().col, 8); // desired column restored on the longer line
}

TEST(cursor_clamped_on_empty_buffer) {
    TextBuffer buf;
    CHECK_EQ(buf.lineCount(), 1);
    CHECK_EQ(buf.lines()[0], std::string(""));
    buf.setCursor({5, 5});
    CHECK_EQ(buf.cursor().row, 0);
    CHECK_EQ(buf.cursor().col, 0);
}

TEST(move_left_at_line_start_goes_to_previous_line_end) {
    TextBuffer buf;
    buf.insertText({0, 0}, "ab\ncd");
    buf.setCursor({1, 0});
    buf.moveLeft();
    CHECK_EQ(buf.cursor().row, 0);
    CHECK_EQ(buf.cursor().col, 2);
}

TEST(word_at_cursor_right_after_word) {
    TextBuffer buf;
    buf.insertText({0, 0}, "hello world");
    buf.setCursor({0, 5}); // right after "hello"
    auto w = buf.wordAt(buf.cursor());
    CHECK_EQ(w.text, std::string("hello"));
}

TEST(word_at_cursor_inside_word) {
    TextBuffer buf;
    buf.insertText({0, 0}, "hello world");
    buf.setCursor({0, 8});
    auto w = buf.wordAt(buf.cursor());
    CHECK_EQ(w.text, std::string("world"));
}

TEST(insert_multiline_text_at_position) {
    TextBuffer buf;
    buf.insertText({0, 0}, "ac");
    buf.insertText({0, 1}, "b\nb");
    CHECK_EQ(buf.lineCount(), 2);
    CHECK_EQ(buf.lines()[0], std::string("ab"));
    CHECK_EQ(buf.lines()[1], std::string("bc"));
}

TEST(long_line_operations) {
    TextBuffer buf;
    std::string longStr(2000, 'x');
    buf.insertText({0, 0}, longStr);
    CHECK_EQ(buf.lines()[0].size(), static_cast<size_t>(2000));
    buf.setCursor({0, 1000});
    buf.insertText(buf.cursor(), "Y");
    CHECK_EQ(buf.lines()[0][1000], 'Y');
}

TEST(erase_range_order_independent) {
    TextBuffer buf;
    buf.insertText({0, 0}, "hello");
    std::string erased = buf.eraseRange({0, 3}, {0, 1}); // reversed on purpose
    CHECK_EQ(erased, std::string("el"));
}
