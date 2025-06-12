#include "Debug.hpp"

std::vector<std::string>& getDebugMessages() {
    static std::vector<std::string> Messages;
    return Messages;
}

void debugPrint(const std::string& message) {
    if (isDebug) {
        getDebugMessages().push_back(message);
    }
}

void printDebugMessagesEverySecond(float deltaTime) {
    static float elapsed = 0.f;
    elapsed += deltaTime;
    auto& Messages = getDebugMessages();
    if (elapsed >= 1.0f && isDebug) {
        for (const auto& msg : Messages) {
            std::cout << msg << std::endl;
        }
        elapsed = 0.f;
    }
    Messages.clear();
}