#include <iostream>
#include <fstream>
#include <vector>

// ===== 通用 XOR =====
void xorBuffer(std::vector<char>& data, unsigned char key)
{
    for (auto& b : data)
        b ^= key;
}

// ===== 加密任意文件 =====
bool encryptFile(const std::string& input,
                 const std::string& output,
                 unsigned char key)
{
    std::ifstream in(input, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Cannot open " << input << "\n";
        return false;
    }

    std::vector<char> data;
    data.assign(
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>()
    );
    in.close();

    xorBuffer(data, key);

    std::ofstream out(output, std::ios::binary);
    out.write(data.data(), data.size());
    out.close();

    std::cout << "Encrypted: " << input
              << " -> " << output << "\n";

    return true;
}

int main(){
    // ===== pak 的 key =====
    unsigned char pakKey = 's' ^ 'n' ^ 'l' ^ 'b' ^ 'c';
    encryptFile("data.pak", "data.sxk", pakKey);
    
    return 0;
}