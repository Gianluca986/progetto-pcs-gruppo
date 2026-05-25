#include <iostream>
#include <fstream>
#include <string>


int main(int argc, const char *argv[] ) {
    // se l'utente non specifica il file come parametro il programma restituisce errore e si chiude
    if (argc < 2) {
        std::cout << "Error: file name not declared as parameter. \n";
        return 1;
    }

    std::string file_name = argv[1];     // il nome del file è quello scritto nel terminale come secondo parametro
    std::ifstream ifs(file_name);        // apro il file

    if ( ifs.is_open() ) {
        while (!ifs.eof()) {
            std::string component;
            double value;
            int _from, _to;
            if  (ifs >> component >> value >> _from >> _to ) { 
                std::cout << component << value << _from << _to << std::endl;
            }
        }
    }
    else {
        std::cout << "Error: unable to open " << file_name << "\n";
        return 1;
    }
    return 0;
}

 

