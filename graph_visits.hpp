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
//#include <Eigen/Dense>


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
directed_graph graph_visit(const directed_graph& G, int source, T& container) {
    /* NOTA: il +1 nelle dimensione dei vettori è perché i nodi partono tutti da 1 e non da 0 */
    std::vector<bool> reached(G.all_nodes().size()+1, false); // inizializzo un vettore di n = #nodi elementi a false 
    directed_graph tree;

    container.put({source,source});

    while(!container.empty()) { 
        Journey step = container.get();
        int step_from = step.from;
        int step_to = step.to;
        
        if (reached[step_to]) {continue;}
        reached[step_to] = true;

        // Macchina
        if (step_from != step_to) {
            bool edge_added = false;

            // cerco se l'arco originale era un resitore
            for (const auto& r_edge : G.all_edges_res()) {
                if (r_edge.from() == step_from && r_edge.to() == step_to) {
                    tree.add_edge(r_edge); // Trovato! Lo aggiungo all'albero
                    edge_added = true;
                    break;
                }
            }
            
            // se non era un resitore, cerco tra i generatori
            if (!edge_added) {
                for (const auto& g_edge : G.all_edges_gen()) {
                    if (g_edge.from() == step_from && g_edge.to() == step_to) {
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

/* le chiamate per referenze "&" servono a far sì che la funzione helper modifica i valori quando chiamo la funzione principale //
void dfs_helper(const directed_graph& G, int u, std::vector<bool>& reached, directed_graph& tree) {
    reached[u] = true;
    for (auto w : G.neighbors(u)) {
        if (!reached[w]) {
            tree.add_edge({u,w});
            dfs_helper(G, w, reached, tree);
        }
    }

}

// Visita in profondità (DFS) implementata in modo ricorsivo.
// Anche se non uso esplicitamente la classe 'stack<T>', questa funzione 
// è a tutti gli effetti una visita LIFO. Sfrutta lo "Stack di sistema" (Call Stack)
// generato automaticamente dal C++ ad ogni chiamata di 'dfs_helper'.
directed_graph recursive_dfs(const directed_graph& G, int source) {
    std::vector<bool> reached(G.all_nodes().size()+1, false); 
    directed_graph tree;

    dfs_helper(G, source, reached, tree);
    return tree;
}
    */

class Compare {
public:
    bool operator()(std::pair<int, int> a, std::pair<int, int> b) {
        return a.second > b.second;
    }
};

/* NOTA: qui tutti i pesi valgono 1 
directed_graph dijkstra(const directed_graph& G, int source) {
    int n = G.all_nodes().size() + 1;
    std::vector<int> pred(n, -1);
    std::vector<int> dist(n, 10000000); 
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, Compare> pq;

    pred[source] = source;
    dist[source] = 0;

    for (int i=1; i < n; i++) {
        pq.push({i, dist[i]});
    }
    while (!pq.empty()) {
        int u = pq.top().first;
        int delta = pq.top().second; // distanza vecchia estratta dalla coda
        pq.pop();

        // LA GHIGLIOTTINA
        // Siccome la std::priority_queue non ha il comando Decrease-Key, quando trovo 
        // una scorciatoia infilo nella coda un "nuovo biglietto" aggiornato, lasciando
        // dentro quello vecchio. 
        // Questo 'if' controlla se il biglietto appena estratto (delta) è ormai "spazzatura"
        // rispetto alla vera distanza ottima che ho salvato sulla lavagna (dist[u]).        
        if (delta > dist[u]) {
            continue;
        }

        for (int w: G.neighbors(u)) {
            if (dist[w] > dist[u] + 1) { 
                dist[w] = dist[u] + 1;
                pred[w] = u;
                
                // IL DECREASE-KEY:
                pq.push({w, dist[w]}); 
            }
        }
    }

    unidirected_graph tree;
    for (int i = 0; i < n; i++) {
        if (pred[i] != -1 && pred[i] != i) { // Se ha un padre valido e non è la sorgente
            tree.add_edge({pred[i], i});
        }
    }
    
    return tree;
}
    
  
inline std::ostream& operator<<(std::ostream& os, const std::pair<int,int>& p)
{
    os << "(" << p.first << "," << p.second << ")";
    return os;
}

*/