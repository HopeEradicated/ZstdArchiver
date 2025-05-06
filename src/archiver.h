#pragma once

#include <filesystem>

enum class Mode { Compress, Decompress };

class Compressor {
public:
    explicit Compressor(int compressionLevel = 1)
        : compressionLevel_(compressionLevel) {
    }

    void compress(const std::filesystem::path& input, const std::filesystem::path& output);

private:
    int compressionLevel_;
};

class Decompressor {
public:
    void decompress(const std::filesystem::path& input, const std::filesystem::path& output);
};
