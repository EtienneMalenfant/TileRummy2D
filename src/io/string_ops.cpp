#include <io/string_ops.h>


void StringOps::trim(std::string& str) {
    size_t start = str.find_first_not_of(" \t\n");
    size_t end = str.find_last_not_of(" \t\n");
    if (start == std::string::npos) {
        str = "";
    } else {
        str = str.substr(start, end - start + 1);
    }
}

