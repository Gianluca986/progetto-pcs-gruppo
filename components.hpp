#pragma once
#include <iostream> 
#include <algorithm> 

class generator {
private: 
    std::string _name;
    double _value;

public:
    generator() {} 

    generator(std::string name, double value) {
        _name = name;
        _value = value;
    }

    std::string name()  const {return _name; } 
    double      value() const {return _value; } 
    bool operator==(const generator& other) const {
        return (_name == other._name && _value == other._value);
    }

};

inline std::ostream& operator<<(std::ostream& os, const generator& gen)
{   
    os << "{" << gen.name() << ", " << gen.value() << "}";
    return os;
}

class resistor {
private: 
    std::string _name;
    double _value;

public:
    resistor() {}

    resistor(std::string name, double value) {
        _name = name;
        _value = value;
    }

    std::string name()  const {return _name; } 
    double      value() const {return _value; } 

    bool operator==(const resistor& other) const {
        return (_name == other._name && _value == other._value);
    }
   
};

inline std::ostream& operator<<(std::ostream& os, const resistor& res)
{   
    os << "{" << res.name() << ", " << res.value() << "}";
    return os;
}



