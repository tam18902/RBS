#ifndef _RBS_DATABASE
#define _RBS_DATABASE 1

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <variant>
#include <regex>

#define DB_PATH "database.txt"
using VariantType = std::variant<int, uint, float, double, std::string>;

class Data {
public:
    enum Type {
        Int,
        UInt,
        Float,
        Double,
        Char,
        Undefined
    };
    static Type dataType(const std::string& type);
    
    template <typename T>
    static bool datacast(const VariantType& value, const std::string& type, T& returnValue);

    Data();
    Data(const std::string& type, const std::string& value);
    Data(const Data& other);
    Data(Data&& other) noexcept;
    Data& operator=(const Data& data);
    Data& operator=(Data&& other) noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Data& data);
    std::string getValue() const;
    std::string getType() const;

private:
    std::string m_type;
    VariantType m_variantValue;
};

class Database
{
private:
    struct _constructor_tag { explicit _constructor_tag() = default; };
public:
    // Singleton template for Logger
    Database(_constructor_tag);
    bool add(const std::string& key, const std::string& type, const std::string& value);
    bool update(const std::string& key, const std::string& value);
    bool remove(const std::string& key);
    
    bool addForce(const std::string& key, const std::string& type, const std::string& value);
    bool updateForce(const std::string& key, const std::string& value);
    bool removeForce(const std::string& key);

    std::map<std::string, Data> prefixFind(const std::string& prefix) const;
    std::string get(const std::string& key);
    std::string toString();
    int size();
    bool isEmpty();

    static std::string dataMap2Str(std::map<std::string, Data>);
    static Database* getInstance(const std::string& path = DB_PATH);
    virtual ~Database();
    
private:
    static std::unique_ptr<Database> instance;
    // Key-Value tag
    std::map<std::string, Data> m_map;
    std::string dbFilePath;
    void loadDB();
};


template <typename T>
bool Data::datacast(const VariantType& value, const std::string& type, T& returnValue) {
    switch (dataType(type))
    {
    case Type::Int:
        if(std::is_same_v<T, int>) 
            returnValue = std::get<int>(value);
        break;
    case Type::UInt:
        if(std::is_same_v<T, unsigned int>) 
            returnValue = std::get<unsigned int>(value);
        break;
    case Type::Float:
        if(std::is_same_v<T, float>) 
            returnValue = std::get<float>(value);
        break;
    case Type::Double:
        if(std::is_same_v<T, double>) 
            returnValue = std::get<double>(value);
        break;
    case Type::Char:
        if(std::is_same_v<T, std::string>) 
            returnValue = std::get<std::string>(value);
        break;
    
    case Type::Undefined:
    default:
        return false;
    }
    return true;
}

#endif /* _RBS_DATABASE */
