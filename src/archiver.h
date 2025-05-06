#pragma once
#include <filesystem>

enum class Mode { Compress, Decompress };

class Compressor {
public:
    void compress(const std::filesystem::path& input, const std::filesystem::path& output);
};

class Decompressor {
public:
    void decompress(const std::filesystem::path& input, const std::filesystem::path& output);
};
