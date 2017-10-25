#ifndef INIPARSER_H
#define INIPARSER_H

#include <map>
#include <list>
#include <string>

class IniParser
{
public:
    IniParser();
    virtual ~IniParser();

    bool load(const std::string &fileName);
    std::list<std::string> sections() const;
    std::list<std::string> options(const std::string &section) const;
    std::string get(const std::string &section, const std::string &option,
            const std::string &def = std::string()) const;
    bool getBool(const std::string &section, const std::string &option, bool def = false) const;
    int getInt(const std::string &section, const std::string &option, int def = 0) const;
    double getDouble(const std::string &section, const std::string &option, double def = 0.0) const;

private:
    void toLower(std::string &str) const;
    std::map<std::string, std::map<std::string, std::string>> data;

};


#endif // INIPARSER_H
