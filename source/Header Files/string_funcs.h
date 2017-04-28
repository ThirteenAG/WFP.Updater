#include <string>

std::wstring toWString(std::string& string)
{
    std::wstring wstring; std::copy(string.begin(), string.end(), std::back_inserter(wstring));
    return wstring;
}

std::string toString(std::wstring& wstring)
{
    std::string string; std::copy(wstring.begin(), wstring.end(), std::back_inserter(string));
    return string;
}

void removeQuotesFromString(std::string& string)
{
    if (string.at(0) == '\"' || string.at(0) == '\'')
        string.erase(0, 1);
    if (string.at(string.size() - 1) == '\"' || string.at(string.size() - 1) == '\'')
        string.erase(string.size() - 1);
}

size_t find_nth(const std::string& haystack, size_t pos, const std::string& needle, size_t nth)
{
    size_t found_pos = haystack.find(needle, pos);
    if (0 == nth || std::string::npos == found_pos)  return found_pos;
    return find_nth(haystack, found_pos + 1, needle, nth - 1);
}

size_t find_nth(const std::wstring& haystack, size_t pos, const std::wstring& needle, size_t nth)
{
    size_t found_pos = haystack.find(needle, pos);
    if (0 == nth || std::wstring::npos == found_pos)  return found_pos;
    return find_nth(haystack, found_pos + 1, needle, nth - 1);
}

bool string_replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool string_replace(std::wstring& str, const std::wstring& from, const std::wstring& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::wstring::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

std::string formatBytes(int32_t bytes, int32_t precision = 2)
{
    if (bytes == 0)
        return std::string("0 Bytes");

    auto k = 1000;
    char* sizes[] = { "Bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
    size_t i = (size_t)floor(log(bytes) / log(k));
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << (bytes / pow(k, i));
    return std::string(out.str() + ' ' + sizes[i]);
};

std::wstring formatBytesW(int32_t bytes, int32_t precision = 2)
{
    return toWString(formatBytes(bytes, precision));
};

std::wstring GetLongestCommonSubstring(const std::wstring & first, const std::wstring & second)
{
    auto findSubstrings = [](const std::wstring& word, std::set<std::wstring>& substrings)->void
    {
        int l = word.length();
        for (int start = 0; start < l; start++) {
            for (int length = 1; length < l - start + 1; length++) {
                substrings.insert(word.substr(start, length));
            }
        }
    };

    std::set<std::wstring> firstSubstrings, secondSubstrings;
    findSubstrings(first, firstSubstrings);
    findSubstrings(second, secondSubstrings);
    std::set<std::wstring> common;
    std::set_intersection(firstSubstrings.begin(), firstSubstrings.end(), secondSubstrings.begin(), secondSubstrings.end(), std::inserter(common, common.begin()));
    std::vector<std::wstring> commonSubs(common.begin(), common.end());
    std::sort(commonSubs.begin(), commonSubs.end(), [](const std::wstring &s1, const std::wstring &s2) { return s1.length() > s2.length(); });
    if (!commonSubs.empty())
        return *(commonSubs.begin());
    else
        return L"";
}

bool folderExists(const char* folderName) {
    if (_access(folderName, 0) == -1) {
        //File not found
        return false;
    }

    DWORD attr = GetFileAttributes((LPCSTR)folderName);
    if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        // File is not a directory
        return false;
    }

    return true;
}

bool folderExists(const wchar_t* folderName) {
    if (_waccess(folderName, 0) == -1) {
        //File not found
        return false;
    }

    DWORD attr = GetFileAttributesW((LPCWSTR)folderName);
    if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        // File is not a directory
        return false;
    }

    return true;
}

bool createFolder(std::string folderName) {
    std::list<std::string> folderLevels;
    char* c_str = (char*)folderName.c_str();

    // Point to end of the string
    char* strPtr = &c_str[strlen(c_str) - 1];

    // Create a list of the folders which do not currently exist
    do {
        if (folderExists(c_str)) {
            break;
        }
        // Break off the last folder name, store in folderLevels list
        do {
            strPtr--;
        } while ((*strPtr != '\\') && (*strPtr != '/') && (strPtr >= c_str));
        folderLevels.push_front(std::string(strPtr + 1));
        strPtr[1] = 0;
    } while (strPtr >= c_str);

    if (_chdir(c_str)) {
        return true;
    }

    // Create the folders iteratively
    for (std::list<std::string>::iterator it = folderLevels.begin(); it != folderLevels.end(); it++) {
        if (CreateDirectory(it->c_str(), NULL) == 0) {
            return true;
        }
        _chdir(it->c_str());
    }

    return false;
}

bool createFolder(std::wstring folderName) {
    std::list<std::wstring> folderLevels;
    wchar_t* c_str = (wchar_t*)folderName.c_str();

    // Point to end of the string
    wchar_t* strPtr = &c_str[wcslen(c_str) - 1];

    // Create a list of the folders which do not currently exist
    do {
        if (folderExists(c_str)) {
            break;
        }
        // Break off the last folder name, store in folderLevels list
        do {
            strPtr--;
        } while ((*strPtr != '\\') && (*strPtr != '/') && (strPtr >= c_str));
        folderLevels.push_front(std::wstring(strPtr + 1));
        strPtr[1] = 0;
    } while (strPtr >= c_str);

    if (_wchdir(c_str)) {
        return true;
    }

    // Create the folders iteratively
    for (std::list<std::wstring>::iterator it = folderLevels.begin(); it != folderLevels.end(); it++) {
        if (CreateDirectoryW(it->c_str(), NULL) == 0) {
            return true;
        }
        _wchdir(it->c_str());
    }

    return false;
}