#pragma once
#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <set> 
#include <unordered_map>
#include "directed_edge.hpp" 
#include "components.hpp" 
#include <Eigen/Dense>




class directed_graph {
private: 
    std::unordered_map<int, std::set<int>> graph;
    std::set<int> nodes;    // memorizzo i nodi in un set (che ignora i duplicati)
    std::vector<directed_edge<generator>> gen_edges; 
    std::vector<directed_edge<resistor>> res_edges; 
    //std::vector<directed_edge<T>> _edges;  


public:

    directed_graph() {}
    
    // copy constructor
    directed_graph(const directed_graph& other) = default;

    /* il metodo neighbors restituisce i nodi collegati nel senso di arco diretto */
    std::set<int> neighbors(const int node) const {

        if (nodes.count(node)) {
            
            auto it = graph.find(node); // cerco l'iteratore dove si trova il nodo all'interno della mappa di adiacenza
            
            // Se l'iteratore non punta alla fine, il nodo esiste
            if (it != graph.end()) {
                return it->second; // 'it->' = vai all'oggetto puntato (la std::pair); '.second' = prendi il valore associato alla chiave (il set dei nodi vicini)            
            }
            
            // Se il nodo è nel grafo ma non nella mappa, significa che non ha archi uscenti
            return {};
        }
        
        // Se non è nemmeno in "nodes", non fa parte del grafo
        return {};
    }
    
    void add_edge(const directed_edge<generator>& edge) {
        /* se l'arco già appartiene al grafo non ha senso inserirlo */
        if (std::find(gen_edges.begin(), gen_edges.end(), edge) != gen_edges.end() ) {
            return;
        }
        /* inserisco l'arco nella struttura dati */
        nodes.insert(edge.from());
        nodes.insert(edge.to());
        graph[edge.from()].insert(edge.to());  

        /*aggiungo l'arco ad un vettore di archi*/
        gen_edges.push_back(edge);
    }

    void add_edge(const directed_edge<resistor>& edge) {
        /* se l'arco già appartiene al grafo non ha senso inserirlo */
        if (std::find(res_edges.begin(), res_edges.end(), edge) != res_edges.end() ) {
            return;
        }
        /* inserisco l'arco nella struttura dati */
        nodes.insert(edge.from());
        nodes.insert(edge.to());
        graph[edge.from()].insert(edge.to());  

        /*aggiungo l'arco ad un vettore di archi*/
        res_edges.push_back(edge);
    }
    
    std::vector<directed_edge<generator>> all_edges_gen() const {
        return gen_edges;
    }

    std::vector<directed_edge<resistor>> all_edges_res() const {
        return res_edges;
    }

    std::set<int> all_nodes() const {
        return nodes;
    }


                     /* SOTTRAZIONE */
    /*  Calcola la differenza tra grafi (G - G'):
        1. Parto da un grafo risultato vuoto. (result)
        2. Scorro tutti gli archi di questo grafo (G). 
        3. Se un arco NON viene trovato in G' (std::find restituisce .end()), 
           significa che appartiene alla differenza, quindi lo aggiungo al risultato. */
    /*directed_graph operator-(const directed_graph& other) const {
        unidirected_graph result; // grafo vuoto 
        for (const auto& edge : v_edges) { // per ogni edge appartenente a v_edges 
            auto found = std::find(other.v_edges.begin(), other.v_edges.end(), edge);
            if (found == other.v_edges.end()) { // -> l'arco non è in other.v_edges
                result.add_edge(edge);
            }
        }
        return result;
                  
    }*/


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
inline std::ostream& operator<<(std::ostream& os, const directed_graph& graph)
{
    for (const auto& node : graph.all_nodes()) {
        os << node << " : " << graph.neighbors(node) << "\n";
    }
    return os;
}
