#include <iostream>

#include "harness.h"

int main() {
    int total = 0;
    for (auto& t : test::registry()) {
        test::currentTest = t.name;
        int before = test::failures;
        t.fn();
        ++total;
        if (test::failures == before) std::cout << "  ok  " << t.name << "\n";
    }
    std::cout << total << " tests run, " << test::failures << " failure(s).\n";
    return test::failures == 0 ? 0 : 1;
}
