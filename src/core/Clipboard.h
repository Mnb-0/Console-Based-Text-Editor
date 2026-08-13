#pragma once
#include <string>

namespace editor {

class Clipboard {
public:
    void set(std::string text) { text_ = std::move(text); }
    const std::string& get() const { return text_; }
    bool empty() const { return text_.empty(); }

private:
    std::string text_;
};

} // namespace editor
