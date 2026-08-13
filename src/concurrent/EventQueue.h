#pragma once
#include <mutex>
#include <queue>
#include <string>
#include <variant>
#include <vector>

#include "spell/SpellChecker.h"
#include "spell/Suggester.h"

namespace editor {

struct DictionaryLoadedEvent { size_t wordCount; };
struct SpellScanEvent { int version; std::vector<MisspelledSpan> spans; };
struct SuggestEvent { int version; SuggestionResult result; };
struct SaveCompleteEvent { bool success; std::string path; std::string error; };
struct LoadCompleteEvent {
    bool success;
    std::vector<std::string> lines;
    std::string path;
    std::string error;
    bool trailingNewline = true;
};

using Event = std::variant<DictionaryLoadedEvent, SpellScanEvent, SuggestEvent, SaveCompleteEvent, LoadCompleteEvent>;

// Worker -> main-thread mailbox. Workers only ever call push(); the main
// thread drains it once per loop iteration. This is the only channel
// through which background results reach the UI - workers never call
// ncurses or touch the live Document directly.
class EventQueue {
public:
    void push(Event e) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(e));
    }

    std::vector<Event> drainAll() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Event> out;
        out.reserve(queue_.size());
        while (!queue_.empty()) {
            out.push_back(std::move(queue_.front()));
            queue_.pop();
        }
        return out;
    }

private:
    std::queue<Event> queue_;
    std::mutex mutex_;
};

} // namespace editor
