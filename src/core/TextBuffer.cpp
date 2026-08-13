#include "core/TextBuffer.h"

#include <algorithm>
#include <cctype>

namespace editor {

Position advance(Position start, const std::string& text) {
    Position p = start;
    for (char c : text) {
        if (c == '\n') {
            p.row++;
            p.col = 0;
        } else {
            p.col++;
        }
    }
    return p;
}

TextBuffer::TextBuffer() {
    lines_.push_back("");
}

Position TextBuffer::clampPosition(Position p) const {
    if (lines_.empty()) return {0, 0};
    p.row = std::clamp(p.row, 0, static_cast<int>(lines_.size()) - 1);
    p.col = std::clamp(p.col, 0, static_cast<int>(lines_[p.row].size()));
    return p;
}

void TextBuffer::setCursor(Position p) {
    cursor_ = clampPosition(p);
    desiredCol_ = cursor_.col;
}

void TextBuffer::moveLeft() {
    if (cursor_.col > 0) {
        cursor_.col--;
    } else if (cursor_.row > 0) {
        cursor_.row--;
        cursor_.col = static_cast<int>(lines_[cursor_.row].size());
    }
    desiredCol_ = cursor_.col;
}

void TextBuffer::moveRight() {
    if (cursor_.col < static_cast<int>(lines_[cursor_.row].size())) {
        cursor_.col++;
    } else if (cursor_.row + 1 < lineCount()) {
        cursor_.row++;
        cursor_.col = 0;
    }
    desiredCol_ = cursor_.col;
}

void TextBuffer::moveUp() {
    if (cursor_.row > 0) {
        cursor_.row--;
        cursor_.col = std::min(desiredCol_, static_cast<int>(lines_[cursor_.row].size()));
    }
}

void TextBuffer::moveDown() {
    if (cursor_.row + 1 < lineCount()) {
        cursor_.row++;
        cursor_.col = std::min(desiredCol_, static_cast<int>(lines_[cursor_.row].size()));
    }
}

void TextBuffer::moveHome() {
    cursor_.col = 0;
    desiredCol_ = 0;
}

void TextBuffer::moveEnd() {
    cursor_.col = static_cast<int>(lines_[cursor_.row].size());
    desiredCol_ = cursor_.col;
}

void TextBuffer::movePageUp(int pageSize) {
    cursor_.row = std::max(0, cursor_.row - pageSize);
    cursor_.col = std::min(desiredCol_, static_cast<int>(lines_[cursor_.row].size()));
}

void TextBuffer::movePageDown(int pageSize) {
    cursor_.row = std::min(lineCount() - 1, cursor_.row + pageSize);
    cursor_.col = std::min(desiredCol_, static_cast<int>(lines_[cursor_.row].size()));
}

Position TextBuffer::insertText(Position at, const std::string& text) {
    at = clampPosition(at);

    std::vector<std::string> parts;
    size_t segStart = 0;
    for (size_t i = 0; i <= text.size(); ++i) {
        if (i == text.size() || text[i] == '\n') {
            parts.push_back(text.substr(segStart, i - segStart));
            segStart = i + 1;
        }
    }

    std::string& line = lines_[at.row];
    std::string suffix = line.substr(at.col);
    line = line.substr(0, at.col) + parts.front();

    Position end;
    if (parts.size() == 1) {
        line += suffix;
        end = {at.row, at.col + static_cast<int>(parts.front().size())};
    } else {
        std::vector<std::string> newLines;
        for (size_t i = 1; i + 1 < parts.size(); ++i) newLines.push_back(parts[i]);
        newLines.push_back(parts.back() + suffix);
        lines_.insert(lines_.begin() + at.row + 1, newLines.begin(), newLines.end());
        end = {at.row + static_cast<int>(parts.size()) - 1, static_cast<int>(parts.back().size())};
    }

    cursor_ = end;
    desiredCol_ = end.col;
    modified_ = true;
    return end;
}

std::string TextBuffer::eraseRange(Position from, Position to) {
    from = clampPosition(from);
    to = clampPosition(to);
    if (to < from) std::swap(from, to);

    std::string erased;
    if (from.row == to.row) {
        erased = lines_[from.row].substr(from.col, to.col - from.col);
        lines_[from.row].erase(from.col, to.col - from.col);
    } else {
        erased = lines_[from.row].substr(from.col);
        for (int r = from.row + 1; r < to.row; ++r) {
            erased += "\n" + lines_[r];
        }
        erased += "\n" + lines_[to.row].substr(0, to.col);

        lines_[from.row] = lines_[from.row].substr(0, from.col) + lines_[to.row].substr(to.col);
        lines_.erase(lines_.begin() + from.row + 1, lines_.begin() + to.row + 1);
    }

    cursor_ = from;
    desiredCol_ = from.col;
    modified_ = true;
    return erased;
}

std::string TextBuffer::textInRange(Position from, Position to) const {
    from = clampPosition(from);
    to = clampPosition(to);
    if (to < from) std::swap(from, to);

    if (from.row == to.row) {
        return lines_[from.row].substr(from.col, to.col - from.col);
    }
    std::string result = lines_[from.row].substr(from.col);
    for (int r = from.row + 1; r < to.row; ++r) result += "\n" + lines_[r];
    result += "\n" + lines_[to.row].substr(0, to.col);
    return result;
}

namespace {
bool isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '\'';
}
} // namespace

WordSpan TextBuffer::wordAt(Position p) const {
    p = clampPosition(p);
    const std::string& line = lines_[p.row];
    int s = p.col, e = p.col;

    // If the cursor sits right after a word (e.g. just after typing it, or
    // right after a trailing space), look at the character to the left.
    if (e >= static_cast<int>(line.size()) || !isWordChar(line[e])) {
        if (s > 0 && isWordChar(line[s - 1])) {
            s--;
            e = s;
        }
    }
    while (s > 0 && isWordChar(line[s - 1])) s--;
    while (e < static_cast<int>(line.size()) && isWordChar(line[e])) e++;

    return {line.substr(s, e - s), {p.row, s}, {p.row, e}};
}

void TextBuffer::loadLines(std::vector<std::string> lines) {
    if (lines.empty()) lines.push_back("");
    lines_ = std::move(lines);
    cursor_ = {0, 0};
    desiredCol_ = 0;
    modified_ = false;
}

} // namespace editor
