#pragma once
#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <set> 
#include <unordered_map>
#include "undirected_edge.hpp" 
#include "undirected_graph.hpp" 
#include "components.hpp" 
#include "containers.hpp" 


template<typename T>
inline void print_cycle(const std::vector<T>& v)
{
    std::cout << "(";
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << ",";        
    }
    std::cout << ")\n";
}


/* 1. CREAZIONE CICLI CON DFS*/

struct Journey{
    int from;      // nodo padre
    int to;        // nodo figlio
};
template<typename T>
undirected_graph dfs(const undirected_graph& G, int source, T& container) {
    /* NOTA: il +1 nelle dimensione dei vettori è perché i nodi partono tutti da 1 e non da 0 */
    std::vector<bool> reached(G.all_nodes().size()+1, false); // inizializzo un vettore di n = #nodi elementi a false 
    undirected_graph tree;

    container.put({source,source});

    while(!container.empty()) { 
        Journey step = container.get();
        int step_from = step.from;
        int step_to = step.to;
        
        if (reached[step_to]) {continue;}
        reached[step_to] = true;

        if (step_from != step_to) {
            bool edge_added = false;

            // cerco se l'arco originale era un resitore
            for (const auto& r_edge : G.all_edges_res()) {
                if ((r_edge.from() == step_from && r_edge.to() == step_to) || (r_edge.to() == step_from && r_edge.from() == step_to)) {
                    tree.add_edge(r_edge); // Trovato! Lo aggiungo all'albero
                    edge_added = true;
                    break;
                }
            }
            
            // se non era un resitore, cerco tra i generatori
            if (!edge_added) {
                for (const auto& g_edge : G.all_edges_gen()) {
                    if ((g_edge.from() == step_from && g_edge.to() == step_to) ||  (g_edge.to() == step_from && g_edge.from() == step_to)) {
                        tree.add_edge(g_edge); // Trovato! Lo aggiungo all'albero
                        break;
                    }
                }
            }
        }
        
        for (auto w : G.neighbors(step_to)) {
            if (!reached[w]) {            
                container.put({step_to, w});
            }
        }
    }
    return tree;
}

bool find_path(const undirected_graph& T, int u, int v, std::vector<bool>& visited, std::vector<int>& path) { 
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

std::vector<int> create_path(const undirected_graph& T, int u, int v) {
    std::vector<int> path;
    std::vector<bool> visited(T.all_nodes().size() + 1, false);

    find_path(T, u, v, visited, path);

    return path;
}

std::vector<std::vector<int>> get_fundamental_cycles_dfs(const undirected_graph& G) {
    int source = *G.all_nodes().begin(); // prendo il primo nodo come sorgente
    stack<Journey> s;
    undirected_graph T = dfs(G, source, s);
    undirected_graph C = G - T;
    int k = G.all_edges_gen().size() + G.all_edges_res().size() - G.all_nodes().size() + 1; // rappresenta il numero di cicli dati da |E|-|V|+1

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


/* 2. CREAZIONE CICLI *MINIMI* CON DE PINA*/
int scalar_product(const std::vector<bool>& S, const std::vector<bool>& P) {
    int ret = 0;
    for (int i = 0; i < P.size(); i++) {
        ret += S[i] * P[i];
    }
    return ret % 2;
}


// NOTA: prima di usare questo codice occorre fare G.sort_all_edges() 
std::vector<std::vector<int>> get_fundamental_cycles_dePina(const undirected_graph& G) {
   
    int source = *G.all_nodes().begin(); // prendo il primo nodo come sorgente
    stack<Journey> s;
    undirected_graph T = dfs(G, source, s);
    undirected_graph C = G - T;
    C.sort_all_edges();

    int k = C.all_edges().size(); // k = |E|-|V|+1 
    int m = G.all_edges().size();  // m = |E|

    // Crea k righe, ognuna contenente un vettore di m elementi inizializzati a false (0)
    std::vector<std::vector<bool>> S_(k, std::vector<bool>(m, false));    

    for (int j = 0; j < k; j++) {
        for (int i = 0; i < m; i++) {
            if (G.all_edges()[i] == C.all_edges()[i])
                S_[k][i] = 1;
        }
    }
    for (int i = 0; i < k; i++) {
        print_vector(S_[i]);
    }
    
    std::vector<bool> P;






    std::vector<std::vector<int>> cycles;




    return cycles;
}


