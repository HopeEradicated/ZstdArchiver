#pragma once
#include <iostream>

class Logger {
public:
    template<typename T>
    static void info(const T& message) {
        std::cout << "[INFO] " << message << std::endl;
    }

    template<typename T>
    static void error(const T& message) {
        std::cerr << "[ERROR] " << message << std::endl;
    }

    template<typename T>
    static void warning(const T& message) {
        std::cerr << "[WARN] " << message << std::endl;
    }
};
