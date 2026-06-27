#pragma once
#include <iostream> 
#include <algorithm> 

template<typename T>
class undirected_edge {
private: 
    int _from;
    int _to;
    T _comp;


public:
    void node_sort() {
        if (_from > _to) {
            std::swap(_from, _to);
        }
    }

    undirected_edge(T comp, int u, int v) {
        _from = u;
        _to = v;
        _comp = comp;
    }
    int from() const {return _from; } 
    int to()   const {return _to; } 
    T   comp() const {return _comp;}
    

    bool operator<(const undirected_edge<T>& other) const {
        if (_from < other._from) return true;
        if (_from == other._from) {
            return _to < other._to;
        }
        return false;
    }

    bool operator==(const undirected_edge<T>& other) const {
        return (_from == other._from && _to == other._to && _comp == other._comp);
    }
};


template<typename T>
inline std::ostream& operator<<(std::ostream& os, const undirected_edge<T>& edge)
{   
    os << edge.comp() << " " << edge.from() << " -- " << edge.to();
    return os;
}

