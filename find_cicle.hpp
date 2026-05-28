#pragma once
#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <set> 
#include <unordered_map>
#include "directed_edge.hpp" 
#include "directed_graph.hpp" 
#include "components.hpp" 
#include "containers.hpp" 
#include "graph_visits.hpp" 
//#include <Eigen/Dense>


bool find_path(const undirected_graph& T, int u, int v) {
    std::vector<bool> visited(G.all_nodes().size()+1, false); // inizializzo un vettore di n = #nodi elementi a false 
    std:;
    visited[u] = true;
    path.add_edge(u,u);
    if (u == v) {
        return true;
    }
    for (auto n : T.neighbors(u)) {
        if (!visited[n]) {
            if find_path(T, n, v) {return true; }
        }
    }
    return path
}

undirected_graph create_path(const undirected_graph& C, int u, int v) {
    if (find_path(const undirected_graph& T, u, v )) {
        
    }
}
