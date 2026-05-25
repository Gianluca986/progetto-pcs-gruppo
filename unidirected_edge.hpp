#pragma once
#include <iostream> 
#include <algorithm> 

class unidirected_edge {
private: 
    int node1;
    int node2;

    void node_sort() {
        if (node1 > node2) {
            std::swap(node1, node2);
        }
    }
public:

    unidirected_edge(int u, int v) {
        node1 = u;
        node2 = v;
        node_sort();
    }

    int from() const {return node1; } 
    int to()   const {return node2; } 

    bool operator<(const unidirected_edge& other) const {
        if (node1 < other.node1) return true;
        if (node1 == other.node1) {
            return node2 < other.node2;
        }
        return false;
    }

    bool operator==(const unidirected_edge& other) const {
        if (node1 == other.node1 && node2 == other.node2) return true;
        return false;
    }
};

inline std::ostream& operator<<(std::ostream& os, const unidirected_edge& edge)
{   
    os << "(" << edge.from() << "," << edge.to() << ")";
    return os;
}


