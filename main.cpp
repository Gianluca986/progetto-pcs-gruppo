#include <iostream>
#include <fstream>
#include <vector>
#include "containers.hpp"
#include "undirected_edge.hpp"
#include "undirected_graph.hpp"
//#include "graph_visits.hpp"
#include "cycles.hpp"
#include "components.hpp"
#include "matrices.hpp"

/* file che si occupa di trasformare l'input.txt in una struttura grafo */

int main(int argc, const char *argv[] ) {

    /* INPUT */
    // std::string file_name = "input.txt";   
    if (argc < 2) {
        std::cerr << "Error: file name not declared as parameter via command line! \n";
        return 1;
    }

    std::string file_name = argv[1];
    std::ifstream ifs(file_name);          

    std::vector<undirected_edge<generator>> gen_edges;
    std::vector<undirected_edge<resistor>>  res_edges;
    undirected_graph G;
    
    if (ifs.is_open()) {
        std::string component;
        double value;
        int _from, _to;
        
        while (ifs >> component >> value >> _from >> _to) { 
            if (component[0] == 'V') {
                generator gen(component, value);
                undirected_edge<generator> edge_g(gen, _from, _to);
                gen_edges.push_back(edge_g);
                
                // se l'inserimento dell'arco fallisce (es, componenenti paralleli), termina
                if (!G.add_edge(edge_g)) {
                    return 1; 
                }
            }
            else if (component[0] == 'R') {
                resistor res(component, value);
                undirected_edge<resistor> edge_r(res, _from, _to);
                res_edges.push_back(edge_r);

                // se l'inserimento dell'arco fallisce (es, componenenti paralleli), termina
                if (!G.add_edge(edge_r)) {
                    return 1;
                }
            }
        }
    }
    else {
        std::cerr << "Error: unable to open " << file_name << "\n";
        return 1;
    }


    
    /* ORDINAMENTO DEI DATI (Fondamentale per la matematica e l'output) */
    std::sort(res_edges.begin(), res_edges.end(), SortByComponentNumber<resistor>());
    std::sort(gen_edges.begin(), gen_edges.end(), SortByComponentNumber<generator>());
    G.sort_all_edges(); 
    
    /* CALCOLO DEL SISTEMA LINEARE
      trova i cicli, crea B, R, v e risolve il sistema */
    Eigen::VectorXd v_R = find_current_resistor(G);
    
    /* OUTPUT */
    for (size_t j = 0; j < res_edges.size(); j++ ) {
        // Legge di Ohm: i = V / R
        double current = v_R[j] / res_edges[j].comp().value();

        std::cout << res_edges[j].comp().name() << ": V = " << v_R[j]
                  << " volts, I = " << current << " amps." << std::endl;
    }

    // ---------------------------------------------------------------------------------------------------------------------------------------


    stack<Journey> s;
    undirected_graph T = dfs(G,1,s);
    //std::cout << T;
    std::cout << G-T;


    std::vector<bool> P = {false, false, true, true, true};
    std::vector<bool> S = {true, true, false, true, true};
    std::cout << scalar_product(P,S) << "\n";

  

    return 0;
}

