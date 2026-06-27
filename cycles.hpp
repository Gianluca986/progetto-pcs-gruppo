#pragma once
#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <set> 
#include <unordered_map>
#include <map>
#include <queue>
#include "undirected_edge.hpp" 
#include "undirected_graph.hpp" 
#include "components.hpp" 
#include "containers.hpp" 
#include "graph_visits.hpp" 


template<typename T>
inline void print_cycle(const std::vector<T>& v)
{
    std::cout << "(";
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << ",";        
    }
    std::cout << ")\n";
}

/* ================================================================
    1. CREAZIONE CICLI CON DFS
   ================================================================ */
inline bool find_path(const undirected_graph& T, int u, int v, std::unordered_map<int, bool>& visited, std::vector<int>& path) { 
    visited[u] = true;
    path.push_back(u);
    if (u == v) {
        return true;
    }
    for (int n : T.neighbors(u)) {
        if (!visited[n]) {
            if (find_path(T, n, v, visited, path)) {
                return true;
            }
        }
    }
    path.pop_back();
    return false;   

}

inline std::vector<int> create_path(const undirected_graph& T, int u, int v) {
    std::vector<int> path;
    std::unordered_map<int, bool> visited;
    find_path(T, u, v, visited, path);
    return path;
}

inline std::vector<std::vector<int>> get_fundamental_cycles_dfs(const undirected_graph& G) {
    int source = *G.all_nodes().begin(); // prendo il primo nodo come sorgente
    stack<Journey> s;
    undirected_graph T = graph_visit(G, source, s);
    undirected_graph C = G - T;
    size_t k = G.all_edges_gen().size() + G.all_edges_res().size() - G.all_nodes().size() + 1; // rappresenta il numero di cicli dati da |E|-|V|+1

    std::vector<std::vector<int>> cycles; // inizializzo un vettore di vettori
    for (auto& edge : C.all_edges_gen()) { 
        int _from = edge.from();
        int _to = edge.to();
        std::vector<int> cycle = create_path(T, _from, _to);
        cycle.push_back(_from);
        cycles.push_back(cycle);
    }
    for (auto& edge : C.all_edges_res()) { 
        int _from = edge.from();
        int _to = edge.to();
        std::vector<int> cycle = create_path(T, _from, _to);
        cycle.push_back(_from);
        cycles.push_back(cycle);
    }
    // la pratica conferma la teoria? 
    if (cycles.size() != k) { // nota: cycles.size() restituisce SOLO il numero di righe
        std::cerr << "Warning: Found " << cycles.size() << " cycles, but expected cycles are: " << k << "!\n";
    }
    return cycles;

}

/* ================================================================
    1. CREAZIONE CICLI CON *MINIMI* CON DE PINA 
   ================================================================ */

inline int scalar_product(const std::vector<bool>& S, const std::vector<bool>& P) {
    int ret = 0;
    for (size_t i = 0; i < P.size(); i++) {
        ret += S[i] * P[i];
    }
    return ret % 2;
}

inline std::vector<int> de_pina_shortest_cycle(const undirected_graph& G, const std::vector<bool>& S_j) {
    std::vector<int> best_cycle;
    size_t min_length = 100000; //imposto un numero grande (infinito)

    // faccio la ricerca per tutti i nodi come nodo di partenza
    for (int start_node : G.all_nodes()) {
        std::map<std::pair<int, int>, std::pair<int, int>> parent;
        std::set<std::pair<int,int>> visited;
        std::queue<std::pair<int, int>> q;

        std::pair<int, int> start_state = {start_node, 0};
        std::pair<int, int> end_state = {start_node, 1}; 
        
        q.push(start_state);
        visited.insert(start_state);
        bool found = false;

        // bfs 
        while (!q.empty()) {
            auto curr_state = q.front();
            q.pop();

            if (curr_state == end_state) {
                found = true;
                break;
            }

            int curr_node = curr_state.first;
            int curr_plane = curr_state.second;

            for (int next_node : G.neighbors(curr_node)) {
                bool is_active = false;
                const auto& edges = G.all_edges();

                for (size_t i = 0; i < edges.size(); i++) {
                    if ((edges[i].from() == curr_node && edges[i].to() == next_node) ||  
                    (edges[i].from() == next_node && edges[i].to() == curr_node)) {
                        is_active = S_j[i];
                        break;
                    }
                }

                //cambio piano se l'arco è attivo in S_j
                int next_plane = is_active ? (1-curr_plane) : curr_plane;
                std::pair<int, int> next_state = {next_node, next_plane};

                if (visited.count(next_state) == 0) {
                    visited.insert(next_state);
                    parent[next_state] = curr_state;
                    q.push(next_state);
                }
            }
        }     
        
        // ricostruzione
        if (found) {
            std::vector<int> current_cycle;
            std::pair<int, int> curr = end_state;

            while (curr != start_state) {
                current_cycle.push_back(curr.first);
                curr = parent[curr];
            }
            current_cycle.push_back(start_node);
            std::reverse(current_cycle.begin(), current_cycle.end());

            if (current_cycle.size() < min_length) {
                min_length = current_cycle.size();
                best_cycle = current_cycle;
            }
        }
    }
    return best_cycle;

}

// NOTA: prima di usare questo codice occorre fare G.sort_all_edges() 
inline std::vector<std::vector<int>> get_fundamental_cycles_dePina(const undirected_graph& G) {
    
    int source = *G.all_nodes().begin(); // prendo il primo nodo come sorgente
    stack<Journey> s;
    undirected_graph T = graph_visit(G, source, s);
    undirected_graph C = G - T; 

    C.sort_all_edges();

    int k = C.all_edges().size();  // k = |E|-|V|+1 
    int m = G.all_edges().size();  // m = |E|

    // Inizializza S: la matrice dello spazio dei cicli (k x m)
    std::vector<std::vector<bool>> S(k, std::vector<bool>(m, false));    

    for (int j = 0; j < k; j++) {
        UniversalEdge cotree_edge = C.all_edges()[j];
        for (int i = 0; i < m; i++) {
            UniversalEdge g_edge = G.all_edges()[i];
            if ((g_edge.from() == cotree_edge.from() && g_edge.to() == cotree_edge.to()) || 
                    (g_edge.from() == cotree_edge.to()   && g_edge.to() == cotree_edge.from()) ) {
                S[j][i] = true;
                break;
            }
        }
    }

    std::vector<std::vector<int>> cycles;

    for (int i = 0; i < k; i++) {
        
        std::vector<int> path = de_pina_shortest_cycle(G, S[i]);
        cycles.push_back(path);

        // vettore di incidenza C_i
        std::vector<bool> C_i(m, false);
        for (size_t n = 0; n < path.size() - 1; n++) {
            for (int h = 0; h < m; h++) {
                UniversalEdge g_edge = G.all_edges()[h];
                if ((g_edge.from() == path[n] && g_edge.to() == path[n+1]) || 
                    (g_edge.from() == path[n+1] && g_edge.to() == path[n])) {
                        C_i[h] = true;
                        break;
                    }
            }
        }

        // aggiornamento ortogonale (XOR)
        for (int j = i+1; j < k; j++) {
            if (scalar_product(C_i, S[j]) == 1) {
                for (int h = 0; h < m; h++) {
                    S[j][h] = S[j][h] ^ S[i][h];   // ^ = XOR  
                }
            }
        }
    }
    
    return cycles;
}


