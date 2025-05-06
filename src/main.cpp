#include "archiver.h"
#include "logger.h"
#include "signal_handler.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <stdexcept>

namespace fs = std::filesystem;

Mode parse_mode(const std::string& mode_str) {
    if (mode_str == "a") return Mode::Compress;
    if (mode_str == "e") return Mode::Decompress;
    throw std::invalid_argument("Неизвестный режим: " + mode_str);
}

void print_usage() {
    Logger::info(
        "Использование:\n"
        "  myapp a [-l <уровень>] <input_file> <archive_file>\n"
        "  myapp e <archive_file> <output_file>\n"
        "\n"
        "Параметры:\n"
        "  -l <уровень>  уровень сжатия от 1 до 22 (по умолчанию 1)"
    );
}

void run(Mode mode, const fs::path& input, const fs::path& output, int compressionLevel) {
    if (mode == Mode::Compress) {
        Compressor(compressionLevel).compress(input, output);
        Logger::info("Файл успешно заархивирован: " + output.string());
    }
    else {
        Decompressor().decompress(input, output);
        Logger::info("Файл успешно распакован: " + output.string());
    }
}

int main(int argc, char* argv[]) {
    SignalHandler handler;

    try {
        if (argc < 4) {
            print_usage();
            return 1;
        }

        std::string mode_str = argv[1];
        Mode mode = parse_mode(mode_str);

        int compressionLevel = 1;
        fs::path input, output;

        if (mode == Mode::Compress) {
            int arg_idx = 2;
            if (std::string(argv[arg_idx]) == "-l") {
                if (argc < 6) {
                    Logger::error("Недостаточно аргументов для уровня сжатия");
                    print_usage();
                    return 1;
                }
                compressionLevel = std::stoi(argv[arg_idx + 1]);
                if (compressionLevel < 1 || compressionLevel > 22) {
                    throw std::out_of_range("Уровень сжатия должен быть в диапазоне 1-22");
                }
                input = argv[arg_idx + 2];
                output = argv[arg_idx + 3];
            }
            else {
                input = argv[arg_idx];
                output = argv[arg_idx + 1];
            }
        }
        else {
            input = argv[2];
            output = argv[3];
        }

        run(mode, input, output, compressionLevel);
    }
    catch (const std::exception& e) {
        Logger::error(e.what());
        return 1;
    }

    return 0;
}
