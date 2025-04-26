#include <io/paths.h>
#include <filesystem>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
#endif


const std::string paths::getExecutablePath() {
#if defined(_WIN32)
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return std::filesystem::path(path).parent_path().string();

#elif defined(__linux__)
    char path[4096];
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path));
    return std::filesystem::path(std::string(path, (count > 0) ? count : 0)).parent_path();
#endif
}
