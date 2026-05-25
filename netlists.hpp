#include <iostream>
#include <fstream>
#include <vector>
//#include "containers.hpp"
#include "directed_edge.hpp"
//#include "graph_visits.hpp"

/*file che si occupa di trasformare l'input.txt in una struttura grafo*/

int main(int argc, const char *argv[] ) {
    // se l'utente non specifica il file come parametro il programma restituisce errore e si chiude
    /*if (argc < 2) {
        std::cout << "Error: file name not declared as parameter. \n";
        return 1;
    }*/

    std::string file_name = "input.txt";     
   // std::string file_name = argv[1];     // il nome del file è quello scritto nel terminale come secondo parametro
    std::ifstream ifs(file_name);          // apro il file

    std::vector<directed_edge> edges;
    
    if (ifs.is_open()) {
        std::string component;
        double value;
        int _from, _to;

        // Il ciclo continua finché la lettura di tutti i dati va a buon fine
        while (ifs >> component >> value >> _from >> _to) { 
            directed_edge edge({component, value}, _from, _to);
            edges.push_back(edge);
            // std::cout << component << " " << value << " " << _from << " " << _to << std::endl;
        }
    }
    else {
        std::cout << "Error: unable to open " << file_name << "\n";
        return 1;
    }
    print_vector(edges);

    return 0;


}
