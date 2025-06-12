#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "main.hpp"

// Messagesへのアクセス用
std::vector<std::string>& getDebugMessages();

// デバッグメッセージを追加
void debugPrint(const std::string& message);

// 1秒ごとにMessagesを一行ずつ表示し、表示後はクリア
void printDebugMessagesEverySecond(float deltaTime);