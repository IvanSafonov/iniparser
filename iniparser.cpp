#include "iniparser.h"

#include <fstream>
#include <regex>
#include <algorithm>
#include <set>

using namespace std;

IniParser::IniParser()
{

}

IniParser::~IniParser()
{

}

bool IniParser::load(const std::string &fileName) {
    ifstream file;
    file.open(fileName);
    if (!file)
        return false;

    regex rxComment{R"(^\s*[;#].*$)"};
    regex rxSection{R"(^\s*\[([^\]]+)\])"};
    regex rxOption{R"(^\s*([^=\s]+)\s*=\s*(.*)$)"};
    regex rxValueQuoted{R"r(^"([^"]*)"(\s*|\s*[;#].*)$)r"};
    regex rxValueStripComment{R"(^([^;#]*).*$)"};
    regex rxTrim{R"(^\s+|\s+$)"};

    string section = "default";
    for (string line; getline(file, line);) {
        if (regex_match(line, rxComment))
            continue;

        smatch matches;
        if (regex_search(line, matches, rxSection)) {
            section = regex_replace(matches[1].str(), rxTrim, "");
            toLower(section);
            data[section] = map<string, string>();
            continue;
        }

        if (regex_search(line, matches, rxOption)) {
            string option = regex_replace(matches[1].str(), rxTrim, "");
            toLower(option);
            string rawValue = matches[2].str();

            if (regex_search(rawValue, matches, rxValueQuoted))
                data[section][option] = matches[1].str();
            else if (regex_search(rawValue, matches, rxValueStripComment))
                data[section][option] = regex_replace(matches[1].str(), rxTrim, "");
        }
    }
    return true;
}

std::list<std::string> IniParser::sections() const
{
    list<string> sections;
    for (auto &i : data)
        sections.push_back(i.first);
    return sections;
}

std::list<std::string> IniParser::options(const std::string &section) const
{
    list<string> options;
    auto sectionIt = data.find(section);
    if (sectionIt == data.end())
        return options;
    for (auto &i : sectionIt->second)
        options.push_back(i.first);
    return options;
}

std::string IniParser::get(const std::string &section, const std::string &option,
        const std::string &def) const
{
    auto sectionIt = data.find(section);
    if (sectionIt == data.end())
        return def;
    auto &sec = sectionIt->second;
    auto optionIt = sec.find(option);
    if (optionIt == sec.end())
        return def;
    return optionIt->second;
}

bool IniParser::getBool(const std::string &section, const std::string &option, bool def) const
{
    string value = get(section, option);
    if (value.empty())
        return def;
    toLower(value);
    static const set<string> trueValues = {"on", "true", "1", "enable"};
    return trueValues.find(value) != trueValues.end();
}

int IniParser::getInt(const std::string &section, const std::string &option, int def) const
{
    string value = get(section, option);
    if (value.empty())
        return def;
    try {
        return stoi(value);
    } catch (...) {}
    return def;
}

double IniParser::getDouble(const std::string &section, const std::string &option, double def) const
{
    string value = get(section, option);
    if (value.empty())
        return def;
    try {
        return stod(value);
    } catch (...) {}
    return def;
}

void IniParser::toLower(std::string &str) const
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}
