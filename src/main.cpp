#include "archiver.h"
#include "logger.h"
#include "signal_handler.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Mode parse_mode(const std::string& mode_str) {
    if (mode_str == "a") return Mode::Compress;
    if (mode_str == "e") return Mode::Decompress;
    throw std::invalid_argument("Неизвестный режим: " + mode_str);
}

void run(Mode mode, const fs::path& input, const fs::path& output) {
    if (mode == Mode::Compress) {
        Compressor().compress(input, output);
        Logger::info("Файл успешно заархивирован: " + output.string());
    } else {
        Decompressor().decompress(input, output);
        Logger::info("Файл успешно распакован: " + output.string());
    }
}

int main(int argc, char* argv[]) {
    SignalHandler handler;

    if (argc != 4) {
        Logger::error("Неверное количество аргументов.\n"
                      "Использование:\n"
                      "  myapp a <input_file> <archive_file>\n"
                      "  myapp e <archive_file> <output_file>");
        return 1;
    }

    try {
        Mode mode = parse_mode(argv[1]);
        fs::path input(argv[2]);
        fs::path output(argv[3]);

        run(mode, input, output);
    } catch (const std::exception& e) {
        Logger::error(e.what());
        return 1;
    }

    return 0;
}
