#include "file.h"

std::string File::get_current_directory() {
#if defined(_WIN32) || defined(_WIN64)
    std::vector<char> buffer(MAX_PATH);
    DWORD size = GetCurrentDirectory(static_cast<DWORD>(buffer.size()), buffer.data());
    if (size == 0) {
        throw std::runtime_error("获取当前目录失败: " + std::to_string(GetLastError()));
    }
    return std::string(buffer.data(), size);
#else
    std::vector<char> buffer(1024);
    while (getcwd(buffer.data(), buffer.size()) == nullptr) {
        if (errno == ERANGE) {
            buffer.resize(buffer.size() * 2);
        } else {
            throw std::runtime_error("获取当前目录失败: " + std::string(std::strerror(errno)));
        }
    }
    return std::string(buffer.data());
#endif
}