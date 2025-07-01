#include "Debug.hpp"
#include <sstream>
#include <map>

std::vector<std::string>& getDebugMessages() {
    static std::vector<std::string> Messages;
    return Messages;
}

std::vector<int>& getDebugCounts() {
    static std::vector<int> Counts;
    return Counts;
}

// ラベルと値を分けて管理
void debugPrint(const std::string& message) {
    if (isDebug) {
        // ラベルと値を分割（例: "HP: 100" → label="HP", value="100"）
        std::string label = message;
        std::string value;
        size_t pos = message.find(':');
        if (pos != std::string::npos) {
            label = message.substr(0, pos);
            value = message.substr(pos + 1);
        }

        auto& Messages = getDebugMessages();
        auto& Counts = getDebugCounts();

        // すでに同じラベルがあるか検索
        bool found = false;
        for (size_t i = 0; i < Messages.size(); ++i) {
            size_t labelPos = Messages[i].find(':');
            std::string existingLabel = (labelPos != std::string::npos) ? Messages[i].substr(0, labelPos) : Messages[i];
            if (existingLabel == label) {
                // ラベルが一致したら値を更新し、カウントを+1
                Messages[i] = message;
                Counts[i] += 1;
                found = true;
                break;
            }
        }
        if (!found) {
            Messages.push_back(message);
            Counts.push_back(1);
        }
    }
}

void print(const std::string& message) {
    std::cout << message << std::endl;
}

void printDebugMessagesEverySecond(float deltaTime) {
    static float elapsed = 0.f;
    elapsed += deltaTime;
    auto& Messages = getDebugMessages();
    auto& Counts = getDebugCounts();
    if (elapsed >= 1.0f && isDebug) {
        std::cout << "===========================" << std::endl;
        for (size_t i = 0; i < Messages.size(); ++i) {
            if (Counts[i] > 1) {
                std::cout << Messages[i] << " *" << Counts[i] << std::endl;
            } else {
                std::cout << Messages[i] << std::endl;
            }
        }
        elapsed = 0.f;
        Messages.clear();
        Counts.clear();
    }
}