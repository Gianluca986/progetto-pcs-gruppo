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


struct Journey{
    int from;      // nodo padre
    int to;        // nodo figlio
};

/* Non salvo solo il nodo da visitare, ma anche chi l'ha visitato {from, to} [struct Journey].
 * Questo serve perché, quando estraggo un nodo dal contenitore, devo 
 * sapere chi lo ha "scoperto" per poter ricostruire l'arco nell'albero finale.
 * * LOGICA "TO -> FROM":
 * All'interno del ciclo sui vicini, il nodo appena raggiunto (step_to) cambia ruolo:
 * smette di essere la 'destinazione' del viaggio attuale e diventa la 'partenza' 
 * (from) per tutti i viaggi verso i suoi vicini non ancora visitati.
 */

template<typename T>
undirected_graph graph_visit(const undirected_graph& G, int source, T& container) {
    /* NOTA: allocazione dinamica sicura per identificatori non sequenziali */
    std::unordered_map<int, bool> reached; 
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

