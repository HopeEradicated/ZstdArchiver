#pragma once
#include <csignal>

class SignalHandler {
public:
    static bool isInterrupted() { return interrupted; }

    SignalHandler() {
        std::signal(SIGINT, handleSignal);
    }

private:
    static inline volatile bool interrupted = false;

    static void handleSignal(int) {
        interrupted = true;
    }
};
