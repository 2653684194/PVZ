//只能按f5 debug 因为输出文件被文件夹包含，路径不同
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;

// 大小写不敏感的比较函数
struct CaseInsensitiveCompare {
    bool operator()(const string& a, const string& b) const {
        return lexicographical_compare(
            a.begin(), a.end(),
            b.begin(), b.end(),
            [](char c1, char c2) { return tolower(c1) < tolower(c2); }
        );
    }
};

struct ModuleEntry {
    string moduleName;
    string fileName;
    
    // 大小写不敏感的相等比较
    bool operator==(const ModuleEntry& other) const {
        if (moduleName.size() != other.moduleName.size()) return false;
        for (size_t i = 0; i < moduleName.size(); ++i) {
            if (tolower(moduleName[i]) != tolower(other.moduleName[i])) {
                return false;
            }
        }
        return fileName == other.fileName;
    }
};

vector<ModuleEntry> parseDllList(const vector<string>& lines) {
    vector<ModuleEntry> entries;
    bool startParsing = false;
    
    for (const auto& line : lines) {
        if (line.find("ModuleName") != string::npos && line.find("FileName") != string::npos) {
            startParsing = true;
            continue;
        }
        
        if (startParsing && !line.empty() && line.find("---") == string::npos) {
            size_t firstNonSpace = line.find_first_not_of(' ');
            if (firstNonSpace == string::npos) continue;
            
            size_t nameEnd = line.find(' ', firstNonSpace);
            string moduleName = line.substr(firstNonSpace, nameEnd - firstNonSpace);
            
            size_t pathStart = line.find_first_not_of(' ', nameEnd);
            string fileName = (pathStart != string::npos) ? line.substr(pathStart) : "";
            
            if (!moduleName.empty() && !fileName.empty()) {
                entries.push_back({moduleName, fileName});
            }
        }
    }
    
    return entries;
}

void compareDllLists(const vector<ModuleEntry>& list1, const vector<ModuleEntry>& list2) {
    // 使用大小写不敏感的map来存储DLL信息
    map<string, pair<ModuleEntry, ModuleEntry>, CaseInsensitiveCompare> dllMap;
    
    // 填充第一个日期的DLL信息
    for (const auto& entry : list1) {
        dllMap[entry.moduleName].first = entry;
    }
    
    // 填充第二个日期的DLL信息
    for (const auto& entry : list2) {
        dllMap[entry.moduleName].second = entry;
    }
    
    cout << left << setw(30) << "DLL名称" << "路径" << endl;
    cout << string(80, '-') << endl;
    
    // 输出比较结果
    cout << "只在第一个日期中出现的DLL:\n";
    bool hasDiff = false;
    for (const auto& [name, entries] : dllMap) {
        const auto& [entry1, entry2] = entries;
        if (entry2.moduleName.empty()) {
            cout << "  - " << setw(26) << entry1.moduleName << entry1.fileName << "\n";
            hasDiff = true;
        }
    }
    if (!hasDiff) cout << "  (无)\n";
    
    cout << "\n只在第二个日期中出现的DLL:\n";
    hasDiff = false;
    for (const auto& [name, entries] : dllMap) {
        const auto& [entry1, entry2] = entries;
        if (entry1.moduleName.empty()) {
            cout << "  + " << setw(26) << entry2.moduleName << entry2.fileName << "\n";
            hasDiff = true;
        }
    }
    if (!hasDiff) cout << "  (无)\n";
    
    cout << "\n名称相同但大小写不同或路径不同的DLL:\n";
    hasDiff = false;
    for (const auto& [name, entries] : dllMap) {
        const auto& [entry1, entry2] = entries;
        if (!entry1.moduleName.empty() && !entry2.moduleName.empty()) {
            bool nameDiff = false;
            if (entry1.moduleName.size() == entry2.moduleName.size()) {
                for (size_t i = 0; i < entry1.moduleName.size(); ++i) {
                    if (tolower(entry1.moduleName[i]) != tolower(entry2.moduleName[i])) {
                        nameDiff = true;
                        break;
                    }
                }
            } else {
                nameDiff = true;
            }
            
            if (nameDiff || entry1.fileName != entry2.fileName) {
                cout << "  ≠ " << setw(26) << entry1.moduleName 
                     << "原路径: " << entry1.fileName << "\n"
                     << "     新名称: " << entry2.moduleName << "\n"
                     << "     新路径: " << entry2.fileName << "\n";
                hasDiff = true;
            }
        }
    }
    if (!hasDiff) cout << "  (无)\n";
    
    // 精确统计
    int onlyInFirst = 0;
    int onlyInSecond = 0;
    int caseOrPathDiff = 0;
    int exactMatches = 0;

    for (const auto& [name, entries] : dllMap) {
        const auto& [entry1, entry2] = entries;
        if (entry2.moduleName.empty()) {
            onlyInFirst++;
        } else if (entry1.moduleName.empty()) {
            onlyInSecond++;
        } else if (entry1 == entry2) {
            exactMatches++;
        } else {
            caseOrPathDiff++;
        }
    }

    cout << "\n精确统计:\n";
    cout << "  - 第一个日期特有DLL: " << onlyInFirst << "\n";
    cout << "  + 第二个日期特有DLL: " << onlyInSecond << "\n";
    cout << "  ≠ 名称/路径差异DLL: " << caseOrPathDiff << "\n";
    cout << "  = 完全相同DLL: " << exactMatches << "\n";
    cout << "总计差异: " << (onlyInFirst + onlyInSecond + caseOrPathDiff) << "\n";
}

int main() {
    ifstream file("zzz_checkDLL.txt");
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    
    // 查找分隔两个日期的位置
    auto splitPos = find_if(lines.begin(), lines.end(), [](const string& s) {
        return s.find("second") != string::npos;
    });
    
    if (splitPos == lines.end()) {
        cerr << "Couldn't find the separator between dates" << endl;
        return 1;
    }
    
    vector<string> firstDateLines(lines.begin(), splitPos);
    vector<string> secondDateLines(splitPos, lines.end());
    
    auto firstDateDlls = parseDllList(firstDateLines);
    auto secondDateDlls = parseDllList(secondDateLines);
    
    cout << "第一个日期DLL数量: " << firstDateDlls.size() << endl;
    cout << "第二个日期DLL数量: " << secondDateDlls.size() << endl << endl;
    
    compareDllLists(firstDateDlls, secondDateDlls);
    
    return 0;
}