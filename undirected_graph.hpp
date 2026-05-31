#pragma once
#include <iostream> 
#include <string> 
#include <algorithm> 
#include <vector> 
#include <set> 
#include <unordered_map>
#include "undirected_edge.hpp" 
#include "components.hpp" 
#include <Eigen/Dense>

class UniversalEdge {
private:
    int _from;
    int _to;
    bool _is_generator;
    std::string _name;
    double _value;

public:
    UniversalEdge(std::string name, double value, bool gen, int u, int v) {
        _from = u;
        _to = v;
        _name = name;
        _value = value;
        _is_generator = gen;
    }

    int         from()         const {return _from; } 
    int         to()           const {return _to; } 
    bool        is_generator() const {return _is_generator;}
    std::string name()         const {return _name;}
    double      value()        const {return _value;}



    bool operator<(const UniversalEdge& other) const {

        /* necessario perché non c'è il metodo void_sort che ordina i nodi */
        int my_min = std::min(_from, _to);
        int my_max = std::max(_from, _to);

        int other_min = std::min(other._from, other._to);
        int other_max = std::max(other._from, other._to);

        if (my_min < other_min) return true;
        if (my_min == other_min) {
            return my_max < other_max;
        }
        return false;
    }
};

inline std::ostream& operator<<(std::ostream& os, const UniversalEdge& edge) {
    os << "Edge " << edge.name() << " (" << edge.value() << ") | Nodes: " 
       << edge.from() << " -> " << edge.to() 
       << " | Generator: " << std::boolalpha << edge.is_generator();
    return os;
}


class undirected_graph {
private: 
    std::unordered_map<int, std::set<int>> graph;
    std::set<int> nodes;    // memorizzo i nodi in un set 
    std::vector<undirected_edge<generator>> gen_edges; 
    std::vector<undirected_edge<resistor>> res_edges; 
    std::vector<UniversalEdge> edges;  

    /* funzione che controlla se l'arco "edge" è già arco del grafo salvato in "vec"*/
    template<typename U, typename V>
    bool has_same_nodes(std::vector<undirected_edge<U>> vec, undirected_edge<V> edge) {
        for (const auto& existing_edge : vec) {
                if (edge.from() == existing_edge.from() && edge.to() == existing_edge.to()) {
                    return true; 
                }

        }
        return false;
    }


public:

    undirected_graph() {}
    
    // copy constructor
    undirected_graph(const undirected_graph& other) = default;

    /* il metodo neighbors restituisce i nodi collegati nel senso di arco non diretto */
    std::set<int> neighbors(const int node) const {

        if (nodes.count(node)) {
            return graph.at(node);
        }
        else return {};
    }

    
    /* add_edge per i generatori */
    void add_edge(const undirected_edge<generator>& edge) {
        /* Controllo incrociato: */
        if (has_same_nodes(res_edges, edge) ) {
            std::cerr << "Nodes of edge " << edge << " already is a resistor edge! \nCheck input file!!" << std::endl; 
            return;
        }

        /* Controllo normale: l'arco esiste già nei generatori? */
        if (has_same_nodes(gen_edges, edge)) {
            return;
        }
        /* inserisco l'arco nella struttura dati */
        nodes.insert(edge.from());
        nodes.insert(edge.to());
        graph[edge.from()].insert(edge.to());
        graph[edge.to()].insert(edge.from());  

        edges.push_back(UniversalEdge(edge.comp().name(), edge.comp().value(), true, edge.from(), edge.to()));
        gen_edges.push_back(edge);
    }

    /* add_edge per i resistori */
    void add_edge(const undirected_edge<resistor>& edge) {
        
        /* Controllo incrociato: */
        if (has_same_nodes(res_edges, edge) ) {
            std::cerr << "Nodes of edge " << edge << " already is a resistor edge! \nCheck input file!!" << std::endl; 
            return;
        }

        /* Controllo normale: l'arco esiste già nei generatori? */
        if (has_same_nodes(gen_edges, edge)) {
            return;
        }
        /* inserisco l'arco nella struttura dati */
        nodes.insert(edge.from());
        nodes.insert(edge.to());
        graph[edge.from()].insert(edge.to());  
        graph[edge.to()].insert(edge.from());  

        /*aggiungo l'arco ad un vettore di archi*/
        edges.push_back(UniversalEdge(edge.comp().name(), edge.comp().value(), false, edge.from(), edge.to()));
        res_edges.push_back(edge);
    }

    void sort_all_edges() {
        std::sort(edges.begin(), edges.end()); 
    }
    
    std::vector<undirected_edge<generator>> all_edges_gen() const {
        return gen_edges;
    }

    std::vector<undirected_edge<resistor>> all_edges_res() const {
        return res_edges;
    }

    std::vector<UniversalEdge> all_edges() const {
        return edges;
    }

    std::set<int> all_nodes() const {
        return nodes;
    }
    int edge_number(const int u, const int v) const {
    // Si presume che sort_all_edges() sia GIA' stato chiamato nel main
    
    int dim = edges.size();
    for (int i = 0; i < dim; i++) {
        // Controllo se i nodi combaciano in un verso o nell'altro
        if ((edges[i].from() == u && edges[i].to() == v) || 
            (edges[i].to() == u && edges[i].from() == v)) {
            
            return i+1;
        }
    }
    
    std::cerr << "Error: Edge between " << u << " and " << v << " not found!\n";
    return -1; 
}

    /*
    1 : {2,3}       (1,2)      (4,3)
    2 : {3,4}       (1,3)
    3 : {}          (2,3) 
    4 : {3}         (2,4)

    1 : {2,3}       (1,2)
    2 : {4}         (1,3)           ==>     (2,3) 2 : {3}
    3 : {}          (2,4)                   (4,3) 4 : {3}
    4 : {}
    */

                     /* SOTTRAZIONE */
    /*  Calcola la differenza tra grafi (G - G'):
        1. Parto da un grafo risultato vuoto. (result)
        2. Scorro tutti gli archi di questo grafo (G). 
        3. Se un arco NON viene trovato in G' (std::find restituisce .end()), 
           significa che appartiene alla differenza, quindi lo aggiungo al risultato. */
    undirected_graph operator-(const undirected_graph& other) const {
        undirected_graph result; // grafo vuoto 
        // prima controllo tra gli archi dei resistori
        for (const auto& r_edge : res_edges) { 
            auto found = std::find(other.res_edges.begin(), other.res_edges.end(), r_edge);
            if (found == other.res_edges.end()) { // -> l'arco non è in other.v_edges
                result.add_edge(r_edge);
            }
        }

        for (const auto& g_edge : gen_edges) {
            auto found = std::find(other.gen_edges.begin(), other.gen_edges.end(), g_edge);
            if (found == other.gen_edges.end()) {
                result.add_edge(g_edge);
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
inline std::ostream& operator<<(std::ostream& os, const undirected_graph& graph)
{
    /*for (const auto& node : graph.all_nodes()) {
        os << node << " : " << graph.neighbors(node) << "\n";
    }*/

    for (const auto& edge : graph.all_edges_gen()) {
        os << edge << "\n";
    }
    for (const auto& edge : graph.all_edges_res()) {
        os << edge << "\n";
    }
    return os;
}

