#include "archiver.h"
#include "logger.h"
#include "signal_handler.h"

#include <zstd.h>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;

constexpr size_t CHUNK_SIZE = 1 << 20; // 1MB

void Compressor::compress(const fs::path& input, const fs::path& output) {
    std::ifstream in(input, std::ios::binary);
    if (!in) throw std::runtime_error("Не удалось открыть входной файл: " + input.string());

    std::ofstream out(output, std::ios::binary);
    if (!out) throw std::runtime_error("Не удалось создать архивный файл: " + output.string());

    std::vector<char> in_buf(CHUNK_SIZE);
    std::vector<char> out_buf(ZSTD_compressBound(CHUNK_SIZE));

    while (!in.eof() && !SignalHandler::isInterrupted()) {
        in.read(in_buf.data(), CHUNK_SIZE);
        std::streamsize bytes_read = in.gcount();
        if (bytes_read <= 0) break;

        size_t compressed_size = ZSTD_compress(out_buf.data(), out_buf.size(),
            in_buf.data(), bytes_read, compressionLevel_);
        if (ZSTD_isError(compressed_size)) {
            throw std::runtime_error("Ошибка сжатия: " + std::string(ZSTD_getErrorName(compressed_size)));
        }

        out.write(reinterpret_cast<char*>(&compressed_size), sizeof(size_t));
        out.write(out_buf.data(), compressed_size);
    }

    if (SignalHandler::isInterrupted())
        throw std::runtime_error("Операция архивирования прервана пользователем");
}

void Decompressor::decompress(const fs::path& input, const fs::path& output) {
    std::ifstream in(input, std::ios::binary);
    if (!in) throw std::runtime_error("Не удалось открыть архив: " + input.string());

    std::ofstream out(output, std::ios::binary);
    if (!out) throw std::runtime_error("Не удалось создать файл: " + output.string());

    std::vector<char> in_buf(CHUNK_SIZE * 2);
    std::vector<char> out_buf(CHUNK_SIZE);

    while (!in.eof() && !SignalHandler::isInterrupted()) {
        size_t compressed_size = 0;
        in.read(reinterpret_cast<char*>(&compressed_size), sizeof(size_t));
        if (in.eof()) break;

        if (compressed_size == 0 || compressed_size > in_buf.size()) {
            throw std::runtime_error("Ошибка: повреждённый архив или неверный формат");
        }

        in.read(in_buf.data(), compressed_size);
        if (in.gcount() != static_cast<std::streamsize>(compressed_size)) {
            throw std::runtime_error("Ошибка чтения сжатых данных");
        }

        size_t decompressed_size = ZSTD_decompress(out_buf.data(), out_buf.size(),
            in_buf.data(), compressed_size);
        if (ZSTD_isError(decompressed_size)) {
            throw std::runtime_error("Ошибка распаковки: " + std::string(ZSTD_getErrorName(decompressed_size)));
        }

        out.write(out_buf.data(), decompressed_size);
    }

    if (SignalHandler::isInterrupted())
        throw std::runtime_error("Операция распаковки прервана пользователем");
}
