#pragma once
#include <vector>
#include <string>

bool MountEncryptedPak(const std::string& filename);

std::vector<char> EncryptBuffer(const std::vector<char>& data);
std::vector<char> DecryptBuffer(const std::vector<char>& data);