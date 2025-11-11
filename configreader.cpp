#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

class ConfigReader {
private:
    std::unordered_map<std::string, std::string> configMap;

public:
    bool loadConfig(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            // 跳过空行和注释
            if (line.empty() || line[0] == '#') continue;
            
            std::istringstream iss(line);
            std::string key, value;
            
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                // 去除前后空格
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                
                configMap[key] = value;
            }
        }
        file.close();
        return true;
    }

    template<typename T>
    T getValue(const std::string& key, T defaultValue = T()) const {
        auto it = configMap.find(key);
        if (it != configMap.end()) {
            std::istringstream iss(it->second);
            T value;
            if (iss >> value) {
                return value;
            }
        }
        return defaultValue;
    }

    // 字符串特化版本
    std::string getValue(const std::string& key, const std::string& defaultValue = "") const {
        auto it = configMap.find(key);
        if (it != configMap.end()) {
            return it->second;
        }
        return defaultValue;
    }
};


