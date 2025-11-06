#include <string>

static std::string converToABIIndependentName(const std::string& funcName) {
#ifdef _WIN32
    return funcName;
#elif __APPLE__
    return "_" + funcName;
#elif __linux__
    return funcName;
#elif __unix__ // Catches other Unix-like systems if not specifically covered
    return "_" + funcName;
#else
    throw("Unknown or unsupported operating system");
#endif
}