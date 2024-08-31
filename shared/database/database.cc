#include "database.h"

std::unique_ptr<Database> Database::instance = nullptr;

Database::Database(_constructor_tag)
    :dbFilePath(DB_PATH){}

Database::~Database() {}

Database* Database::getInstance(const std::string& path) {
    if (!instance) {
        instance = std::unique_ptr<Database>(new Database(_constructor_tag()));
        instance->dbFilePath = path;
        instance->loadDB();
    }
    return instance.get();
}

void Database::loadDB() {
    if (!instance) {
        getInstance();
    }
    std::ifstream ifs(dbFilePath);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open database file for reading." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string key, type, value;

        if (!(iss >> key >> type)) {
            std::cerr << "Error reading key or type from line: " << line << std::endl;
            continue;
        }

        std::getline(iss, value);  // Read the rest of the line
        if (value.empty() || value.front() != ' ' || value[1] != '"' || value.back() != '"') {
            std::cerr << "Error reading quoted value from line: " << line << std::endl;
            continue;
        }

        value = value.substr(2, value.size() - 3);

        // Add entry to the database
        if (!add(key, type, value)) {
            std::cerr << "Error adding <" << key << ", " << type << ", " << value << ">" << std::endl;
            continue;
        }
    }
    ifs.close();
}

int Database::size() {
    if(!instance) {
        getInstance();
    }
    return instance->m_map.size();
}

bool Database::isEmpty() {
    if(!instance) {
        getInstance();
    }
    return (0 == instance->size());
}

std::string Database::get(const std::string& key) {
    if (isEmpty())
    {
        std::cerr << "Empty Database." << std::endl;
        return std::string();
    }
    
    auto it = m_map.find(key);
    if (it == m_map.end()) {
        std::cerr << "Key not found." << std::endl;
        return std::string();
    }
    return it->second.getValue();
}

bool Database::add(const std::string& key, const std::string& type, const std::string& value) {
    if (m_map.find(key) != m_map.end()) {
        std::cerr << "Key already exists." << std::endl;
        return false;
    }
    
    if (Data::dataType(type) == Data::Type::Undefined) {
        std::cerr << "Unsupported type." << std::endl;
        return false;
    }
    
    try
    {
        m_map[key] = Data(type, value);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    return true;
}

bool Database::update(const std::string& key, const std::string& value) {
    if (isEmpty())
    {
        std::cerr << "Empty Database." << std::endl;
        return false;
    }

    auto it = m_map.find(key);
    if (it == m_map.end()) {
        std::cerr << "Key not found." << std::endl;
        return false;
    }
    try
    {
        Data data(it->second.getType(), value);
        it->second = data;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

bool Database::remove(const std::string& key) {
    if (isEmpty())
    {
        std::cerr << "Empty Database." << std::endl;
        return false;
    }

    if (m_map.erase(key)) {
        return true;
    }
    std::cerr << "Key not found." << std::endl;
    return false;
}

bool Database::addForce(const std::string& key, const std::string& type, const std::string& value) {
    if(!add(key, type, value)) {
        return false;
    }
    
    std::ofstream file(dbFilePath, std::ios::app);
    if (!file) {
        return false;
    }
    file << key << " " << type << " \"" << value << "\"" << std::endl;
    file.close();
    
    return true;
}

bool Database::updateForce(const std::string& key, const std::string& value) {
    if(!update(key, value)) {
        return false;
    }

    std::ifstream file(dbFilePath);
    if (!file) {
        return false;
    }
    
    std::ostringstream oss;
    std::string line;
    std::regex keyRegex("^" + key + " .*");

    bool updated = false;
    std::regex keyPattern(key + " (\\w+) \"(.*)\"");

    while (std::getline(file, line)) {
        if (std::regex_match(line, keyRegex)) {
            std::smatch match;
            if (std::regex_search(line, match, keyPattern)) {
                std::string newLine = key + " " + match[1].str() + " \"" + value + "\"";
                oss << newLine << std::endl;
                updated = true;
            }
        } else {
            oss << line << std::endl;
        }
    }

    if (!updated) {
        return false;
    }

    std::ofstream outFile(dbFilePath);
    if (!outFile) {
        return false;
    }
    outFile << oss.str();
    return true;
}

bool Database::removeForce(const std::string& key) {
    if (!remove(key)) {
        return false;
    }

    std::ifstream file(dbFilePath);
    if (!file) {
        return false;
    }

    std::ostringstream oss;
    std::string line;
    std::regex keyRegex("^" + key + " .*");

    bool removed = false;

    while (std::getline(file, line)) {
        if (!std::regex_match(line, keyRegex)) {
            oss << line << std::endl;
        } else {
            removed = true;
        }
    }

    if (!removed) {
        return false;
    }

    std::ofstream outFile(dbFilePath);
    if (!outFile) {
        return false;
    }
    outFile << oss.str();
    return true;
}

std::map<std::string, Data> Database::prefixFind(const std::string& prefix) const {
    std::map<std::string, Data> result;

    for (const auto& [key, data] : m_map) {
        if (key.find(prefix) == 0) {
            result[key] = data;
        }
    }

    return result;
}

std::string Database::toString() {
    return dataMap2Str(m_map);
}

std::string Database::dataMap2Str(std::map<std::string, Data> map) {
    std::ostringstream oss;
    for (const auto& [key, data] : map) {
        oss << key << " " << data << std::endl;
    }
    return oss.str();
} 

/* Data Implement*/
Data::Type Data::dataType(const std::string& type) {
    if (type == "int")
        return Data::Type::Int;
    if (type == "uint") 
        return Data::Type::UInt;
    if (type == "float")
        return Data::Type::Float;
    if (type == "double")
        return Data::Type::Double;
    if (type == "char")
        return Data::Type::Char;
    return Data::Type::Undefined;
}

Data::Data(const std::string& type, const std::string& value) 
    : m_type(type) {
    switch (dataType(type)) {
        case Type::Int: {
            m_variantValue = std::stoi(value);
            break;
        }
        case Type::UInt: {
            m_variantValue = static_cast<unsigned int>(std::stoul(value));
            break;
        }
        case Type::Float: {
            m_variantValue = std::stof(value);
            break;
        }
        case Type::Double: {
            m_variantValue = std::stod(value);
            break;
        }
        case Type::Char: {
            m_variantValue = value;
            break;
        }
        default:
            throw std::invalid_argument("Unsupported type");
    }
}
Data::Data()
    : m_type(std::string()), m_variantValue(VariantType()) {}

Data::Data(const Data& other) 
    : m_type(other.m_type), m_variantValue(other.m_variantValue) {}

Data::Data(Data&& other) noexcept 
    : m_type(std::move(other.m_type)), m_variantValue(std::move(other.m_variantValue)) {}

Data& Data::operator=(const Data& other) {
    if (this != &other) {
        m_type = other.m_type;
        m_variantValue = other.m_variantValue;
    }
    return *this;
}

Data& Data::operator=(Data&& other) noexcept {
    if (this != &other) {
        m_type = std::move(other.m_type);
        m_variantValue = std::move(other.m_variantValue);
    }
    return *this;
}

std::string Data::getValue() const {
    std::ostringstream oss;
    std::visit([&oss](const auto& value) {
        oss << value;
    }, m_variantValue);
    return oss.str();
}

std::string Data::getType() const {
    return m_type;
}

std::ostream& operator<<(std::ostream& os, const Data& data) {
    os << data.m_type << " "<< data.getValue();
    return os;
}