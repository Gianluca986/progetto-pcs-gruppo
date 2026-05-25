#pragma once
#include <iostream> 
#include <algorithm> 

template<typename T>
class directed_edge {
private: 
    int _from;
    int _to;
    /*std::string type_component;
    double value;
    std::pair<std::string, double> _weight;*/
    T _comp;

public:
    /*directed_edge(const std::pair<std::string, double>& w, int u, int v) {
        _from = u;
        _to = v;
        _weight = w;
        type_component = w.first;
        value = w.second;
    }*/

    directed_edge(T comp, int u, int v) {
        _from = u;
        _to = v;
        _comp = comp;
    }
    int from() const {return _from; } 
    int to()   const {return _to; } 
    T   comp() const {return _comp;}
    //std::pair<std::string, double> weight() const {return _weight; }

    bool operator<(const directed_edge& other) const {
        if (_from < other._from) return true;
        if (_from == other._from) {
            return _to < other._to;
        }
        return false;
    }

    bool operator==(const directed_edge& other) const {
        return (_from == other._from && _to == other._to);
    }
};

/*inline std::ostream& operator<<(std::ostream& os, const directed_edge& edge)
{   
    os << "{" << edge.weight().first << ", " << edge.weight().second << "} " << edge.from() << " --> " << edge.to();
    return os;
} */

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const directed_edge<T>& edge)
{   
    os << edge.comp() << " " << edge.from() << " --> " << edge.to();
    return os;
}

