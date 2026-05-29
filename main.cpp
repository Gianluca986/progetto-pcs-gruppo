#include <iostream>
#include <fstream>
#include <vector>
#include "containers.hpp"
#include "directed_edge.hpp"
#include "directed_graph.hpp"
//#include "graph_visits.hpp"
#include "cycles.hpp"
#include "components.hpp"

/*file che si occupa di trasformare l'input.txt in una struttura grafo*/

template<typename T>
inline void print_vector(const std::vector<T>& v)
{
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << " " << std::endl;        
    }
    std::cout << "\n";
}


int main(int argc, const char *argv[] ) {
    // se l'utente non specifica il file come parametro il programma restituisce errore e si chiude
    /*if (argc < 2) {
        std::cout << "Error: file name not declared as parameter. \n";
        return 1;
    }*/

    std::string file_name = "input.txt";     
   // std::string file_name = argv[1];     // il nome del file è quello scritto nel terminale come secondo parametro
    std::ifstream ifs(file_name);          // apro il file

    std::vector<undirected_edge<generator>> gen_edges;
    std::vector<undirected_edge<resistor>>  res_edges;
    undirected_graph G;
    
    if (ifs.is_open()) {
        std::string component;
        double value;
        int _from, _to;
        

        // Il ciclo continua finché la lettura di tutti i dati va a buon fine
        while (ifs >> component >> value >> _from >> _to) { 
            if (component[0] == 'V') {
                generator gen(component, value);
                undirected_edge<generator> edge_g(gen, _from, _to);
                gen_edges.push_back(edge_g);
                G.add_edge(edge_g);
            }
            else if (component[0] == 'R') {
                resistor res(component, value);
                undirected_edge<resistor> edge_r(res, _from, _to);
                res_edges.push_back(edge_r);
                G.add_edge(edge_r);
            }
            
            // std::cout << component << " " << value << " " << _from << " " << _to << std::endl;
        }
    }
    else {
        std::cout << "Error: unable to open " << file_name << "\n";
        return 1;
    }
    //print_vector(res_edges);
    //print_vector(gen_edges);

    stack<Journey> s;  // uso uno stack cosi che graph_visit diventi una dfs
    std::cout << G << std::endl;
    undirected_graph T = dfs(G, 1, s);
    int k = G.all_edges_gen().size() + G.all_edges_res().size() - G.all_nodes().size() + 1; // rappresenta il numero di cicli dati da |E|-|V|+1
    for (int i=0; i<k; i++) {
        print_vector(get_fundamental_cycles(G)[i]);
    }
    std::cout << T << std::endl;

    std::cout << G - T << std::endl;
    return 0;


}
