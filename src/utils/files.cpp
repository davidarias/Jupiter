#include <utils/files.hpp>

std::string getFileName(const std::string& s){
    char sep = '/';

    #ifdef _WIN32
    sep = '\\';
    #endif

    size_t i = s.rfind(sep, s.length());
    if (i != std::string::npos) {
        return(s.substr(i+1, s.length() - i));
    }

    return("");
}
