#pragma once
#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <set> 
#include <unordered_map>
#include "unidirected_edge.hpp" 

template<typename T>
inline void print_vector(const std::vector<T>& v)
{
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
        
    }
    std::cout << "\n";
}



class unidirected_graph {
private: 
    std::unordered_map<int, std::set<int>> graph;
    std::set<int> nodes;    // memorizzo i nodi in un set (che ignora i duplicati)
    std::vector<unidirected_edge> v_edges;  // vettore di archi

public:

    unidirected_graph() {}
    
    // copy constructor
    unidirected_graph(const unidirected_graph& other) = default;

    std::set<int> neighbors(const int node) const {
        if (nodes.count(node)) {
        // if (nodes.contains(node)) 
            std::set<int> ret = graph.at(node);
            return ret;
        }
        else return {};
    }
    
    void add_edge(const unidirected_edge& edge) {
        /* se l'arco già appartiene al grafo non ha senso inserirlo */
        if (std::find(v_edges.begin(), v_edges.end(), edge) != v_edges.end() ) {
            // std::cout << "Graph already has " << edge << std::endl; 
            return;
            }

        /* inserisco l'arco nella struttura dati */
        nodes.insert(edge.from());
        nodes.insert(edge.to());
        graph[edge.from()].insert(edge.to());  
        graph[edge.to()].insert(edge.from());  

        /*aggiungo l'arco ad un vettore di archi*/
        v_edges.push_back(edge);
    }
    
    std::vector<unidirected_edge> all_edges() const {
        return v_edges;
    }
    
    std::set<int> all_nodes() const {
        return nodes;
    }

    /* la funzione edge_number coincide con il cercare l'indice 
    nel vettore v_edges in cui è memorizzato l'arco richiesto  */
    int edge_number(const unidirected_edge& edge) const {
        int dim = v_edges.size(); 
        for (int i = 0; i < dim; i++) {
            if (v_edges[i] == edge) { // -> ho trovato l'arco all'indice i
                return i; 
            }    
        }
        std::cerr << "Edge not found\n";
        return -1; // uso -1 per indicare che non ho trovato l'arco

    }
    
    /* la funzione edge_at coincide con il cercare l'elemento che si trova all'indice -numero d'arco- */ 
    unidirected_edge edge_at(size_t index) const {
        if (index < v_edges.size()) {return v_edges[index];}
        else {
            std::cerr << "Edge Number not found\n";
            return unidirected_edge(-1,-1);
        }
    }

                     /* SOTTRAZIONE */
    /*  Calcola la differenza tra grafi (G - G'):
        1. Parto da un grafo risultato vuoto. (result)
        2. Scorro tutti gli archi di questo grafo (G). 
        3. Se un arco NON viene trovato in G' (std::find restituisce .end()), 
           significa che appartiene alla differenza, quindi lo aggiungo al risultato. */
    unidirected_graph operator-(const unidirected_graph& other) const {
        unidirected_graph result; // grafo vuoto 
        for (const auto& edge : v_edges) { // per ogni edge appartenente a v_edges 
            auto found = std::find(other.v_edges.begin(), other.v_edges.end(), edge);
            if (found == other.v_edges.end()) { // -> l'arco non è in other.v_edges
                result.add_edge(edge);
            }
        }
        return result;
                  
    }


};

// operatore << per i set //
inline std::ostream& operator<<(std::ostream& os, const std::set<int>& s)
{
    os << "{";
    
    int i = 0;
    int dim = s.size(); 
    
    for (int j : s) {
        os << j;
        i++;
        
        if (i < dim) {
            os << ",";
        }
    }
    
    os << "}";
    return os;
}

// operatore << per il grafo //
inline std::ostream& operator<<(std::ostream& os, const unidirected_graph& graph)
{
    for (const auto& node : graph.all_nodes()) {
        os << node << " : " << graph.neighbors(node) << "\n";
    }
    return os;
}
