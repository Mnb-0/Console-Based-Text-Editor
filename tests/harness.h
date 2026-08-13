#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <vector>

// A ~50-line zero-dependency test harness: no vendored framework, just
// enough registration + assertion machinery to get readable pass/fail
// output and a non-zero exit code on failure.

namespace test {

struct TestCase {
    std::string name;
    std::function<void()> fn;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

struct Registrar {
    Registrar(std::string name, std::function<void()> fn) {
        registry().push_back({std::move(name), std::move(fn)});
    }
};

inline int failures = 0;
inline std::string currentTest;

inline void reportFailure(const std::string& expr, const char* file, int line) {
    std::cerr << "  FAIL [" << currentTest << "] " << file << ":" << line << ": " << expr << "\n";
    ++failures;
}

} // namespace test

#define TEST(name)                                                     \
    static void name();                                                \
    static test::Registrar registrar_##name(#name, name);              \
    static void name()

#define CHECK(cond)                                                    \
    do {                                                                \
        if (!(cond)) test::reportFailure(#cond, __FILE__, __LINE__);   \
    } while (0)

#define CHECK_EQ(a, b)                                                                     \
    do {                                                                                    \
        if (!((a) == (b))) {                                                                \
            std::cerr << "  FAIL [" << test::currentTest << "] " << __FILE__ << ":"        \
                       << __LINE__ << ": " #a " == " #b " (got '" << (a) << "' vs '" << (b) \
                       << "')\n";                                                           \
            ++test::failures;                                                               \
        }                                                                                    \
    } while (0)
