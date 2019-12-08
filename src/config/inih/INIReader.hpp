/*
 * Read an INI file into easy-to-access name/value pairs.
 *
 */

#ifndef SPARKLE_SPK_INIH_INIREADER_H_
#define SPARKLE_SPK_INIH_INIREADER_H_

#include <map>
#include <string>

class INIReader
{
public:
    explicit INIReader(const std::string& filename);
    int ParseError() const;

    std::string Get(const std::string& section, const std::string& name,
                    const std::string& default_value) const;

    std::string GetString(const std::string& section, const std::string& name,
                    const std::string& default_value) const;
    long GetInteger(const std::string& section, const std::string& name, long default_value) const;
    double GetReal(const std::string& section, const std::string& name, double default_value) const;
    bool GetBoolean(const std::string& section, const std::string& name, bool default_value) const;

    bool HasSection(const std::string& section) const;
    bool HasValue(const std::string& section, const std::string& name) const;

public:
    std::map<std::string, std::string> _values;

private:
    int _error;
    static std::string MakeKey(const std::string& section, const std::string& name);
    static int ValueHandler(void* user, const char* section, const char* name,
                            const char* value);
};

#endif  // SPARKLE_SPK_INIH_INIREADER_H_
