#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <physfs.h>

std::vector<char> LoadFile(const std::string& path);