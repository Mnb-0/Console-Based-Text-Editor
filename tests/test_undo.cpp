#include "core/Document.h"
#include "harness.h"

using namespace editor;

TEST(typing_coalesces_into_one_undo_step) {
    Document doc;
    doc.typeChar('a');
    doc.typeChar('b');
    doc.typeChar('c');
    CHECK_EQ(doc.buffer().lines()[0], std::string("abc"));
    CHECK(doc.undo());
    CHECK_EQ(doc.buffer().lines()[0], std::string(""));
}

TEST(newline_breaks_coalescing) {
    Document doc;
    doc.typeChar('a');
    doc.newline();
    doc.typeChar('b');
    CHECK(doc.undo()); // removes 'b'
    CHECK_EQ(doc.buffer().lines().size(), static_cast<size_t>(2));
    CHECK(doc.undo()); // removes the newline
    CHECK_EQ(doc.buffer().lines().size(), static_cast<size_t>(1));
    CHECK_EQ(doc.buffer().lines()[0], std::string("a"));
}

TEST(redo_restores_after_undo) {
    Document doc;
    doc.typeChar('x');
    doc.undo();
    CHECK_EQ(doc.buffer().lines()[0], std::string(""));
    CHECK(doc.redo());
    CHECK_EQ(doc.buffer().lines()[0], std::string("x"));
}

TEST(new_edit_clears_redo_stack) {
    Document doc;
    doc.typeChar('x');
    doc.undo();
    doc.typeChar('y');
    CHECK(!doc.redo()); // redo history invalidated by the new edit
}

TEST(backspace_join_is_undoable) {
    Document doc;
    doc.typeChar('a');
    doc.newline();
    doc.typeChar('b');
    doc.buffer().setCursor({1, 0});
    doc.backspace(); // joins the two lines
    CHECK_EQ(doc.buffer().lines().size(), static_cast<size_t>(1));
    CHECK(doc.undo());
    CHECK_EQ(doc.buffer().lines().size(), static_cast<size_t>(2));
}

TEST(undo_on_empty_document_is_a_no_op) {
    Document doc;
    CHECK(!doc.undo());
    CHECK(!doc.redo());
}

TEST(cut_and_paste_round_trip) {
    Document doc;
    doc.typeChar('a');
    doc.typeChar('b');
    doc.typeChar('c');
    doc.buffer().setCursor({0, 0});
    doc.startSelection();
    doc.buffer().setCursor({0, 2});

    Clipboard clip;
    doc.cutSelection(clip);
    CHECK_EQ(doc.buffer().lines()[0], std::string("c"));
    CHECK_EQ(clip.get(), std::string("ab"));

    doc.buffer().setCursor({0, 1});
    doc.pasteFrom(clip);
    CHECK_EQ(doc.buffer().lines()[0], std::string("cab"));
}
