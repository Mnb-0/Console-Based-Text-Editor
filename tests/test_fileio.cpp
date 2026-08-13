#include <cstdio>
#include <fstream>
#include <sstream>

#include "harness.h"
#include "io/FileIO.h"

using namespace editor;

namespace {
std::string readRaw(const char* path) {
    std::ifstream f(path, std::ios::binary);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}
} // namespace

TEST(round_trip_preserves_trailing_newline) {
    const char* path = "test_fileio_tmp1.txt";
    { std::ofstream f(path, std::ios::binary); f << "a\nb\n"; }

    LoadResult loaded = loadFile(path);
    CHECK(loaded.success);
    CHECK(loaded.trailingNewline);
    CHECK_EQ(loaded.lines.size(), static_cast<size_t>(2));

    saveFile(path, loaded.lines, loaded.trailingNewline);
    CHECK_EQ(readRaw(path), std::string("a\nb\n"));
    std::remove(path);
}

TEST(round_trip_preserves_missing_trailing_newline) {
    const char* path = "test_fileio_tmp2.txt";
    { std::ofstream f(path, std::ios::binary); f << "a\nb"; }

    LoadResult loaded = loadFile(path);
    CHECK(loaded.success);
    CHECK(!loaded.trailingNewline);
    CHECK_EQ(loaded.lines.size(), static_cast<size_t>(2));

    saveFile(path, loaded.lines, loaded.trailingNewline);
    CHECK_EQ(readRaw(path), std::string("a\nb"));
    std::remove(path);
}

TEST(load_empty_file_yields_one_empty_line) {
    const char* path = "test_fileio_tmp3.txt";
    { std::ofstream f(path, std::ios::binary); }

    LoadResult loaded = loadFile(path);
    CHECK(loaded.success);
    CHECK_EQ(loaded.lines.size(), static_cast<size_t>(1));
    CHECK_EQ(loaded.lines[0], std::string(""));
    std::remove(path);
}

TEST(load_missing_file_reports_failure) {
    LoadResult loaded = loadFile("definitely_missing_file_xyz.txt");
    CHECK(!loaded.success);
}
