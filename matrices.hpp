#pragma once
#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <set> 
#include <string> 
#include <unordered_map>
#include <Eigen/Dense>
#include "undirected_edge.hpp" 
#include "undirected_graph.hpp" 
#include "components.hpp" 
#include "conjugate_gradient.hpp" 
#include "cycles.hpp" 


template<typename T>
struct SortByComponentNumber {
    
    bool operator()(const undirected_edge<T>& a, const undirected_edge<T>& b) const {

        std::string name_a = a.comp().name();
        std::string name_b = b.comp().name();

        // Rimuovo la prima lettera e converto in intero
        int num_a = std::stoi(name_a.substr(1));  //  String To Integer
        int num_b = std::stoi(name_b.substr(1));  // .substr(1) -> crea una sottostringa dall'elemento in posizione 1 in poi

       
        return num_a < num_b;
    }
};

/* Funzioni che controllano se l'arco è nel verso giusto rispetto al ciclo oppure no */
template<typename T>
inline bool is_in_cycle_forward(const undirected_edge<T>& res, const std::vector<int>& cycle) {
    for (size_t i = 0; i < cycle.size() - 1; i++) {
        if (cycle[i] == res.from() && cycle[i+1] == res.to()) {
            return true;
        }
    }
    return false;
}

template<typename T>
inline bool is_in_cycle_backward(const undirected_edge<T>& res, const std::vector<int>& cycle) {
    for (size_t i = 0; i < cycle.size() - 1; i++) {
        if (cycle[i+1] == res.from() && cycle[i] == res.to()) {
            return true;
        }
    }
    return false;
}

/* Funzione per costruire la matrice di incidenza B (m × n) */
inline Eigen::MatrixXd build_incidence_matrix(const undirected_graph& G, bool use_de_pina = false) {
    std::vector<undirected_edge<resistor>> res_edges = G.all_edges_res();
    
    std::sort(res_edges.begin(), res_edges.end(), SortByComponentNumber<resistor>()); 

    // selezione dinamica dell'algoritmo
    std::vector<std::vector<int>> cycles = use_de_pina ? get_fundamental_cycles_dePina(G) : get_fundamental_cycles_dfs(G);
    
    int m = res_edges.size();
    int n = cycles.size(); // n = |E|-|V|+1
    
    // Inizializza la matrice già riempita di 0
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, n);

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            if (is_in_cycle_forward(res_edges[i], cycles[j])) {
                B(i, j) = 1;
            } 
            else if (is_in_cycle_backward(res_edges[i], cycles[j])) {
                B(i, j) = -1;
            }
        }
    }
    
    return B;
}

/* Funzione per costruire la matrice diagonale delle resistenze R (m x m) */
inline Eigen::MatrixXd build_resistance_matrix(const undirected_graph& G) {
    std::vector<undirected_edge<resistor>> res_edges = G.all_edges_res();
    
    std::sort(res_edges.begin(), res_edges.end(), SortByComponentNumber<resistor>()); 

    int m = res_edges.size();

    // Creo un vettore Eigen per memorizzare solo i VALORI delle resistenze di dimensione m
    Eigen::VectorXd R_vec(m);

    for (int i = 0; i < m; i++) {
        R_vec(i) = res_edges[i].comp().value(); 
    }

    // L'equivalente di diag() in MATLAB: converte il vettore in una matrice diagonale
    Eigen::MatrixXd R = R_vec.asDiagonal();

    return R;
}

/* Funzione per costruire il vettore dei generatori v (h) */
inline Eigen::VectorXd build_generator_vector(const undirected_graph& G, bool use_de_pina = false) {
    std::vector<undirected_edge<generator>> gen_edges = G.all_edges_gen();

    std::sort(gen_edges.begin(), gen_edges.end(), SortByComponentNumber<generator>());

    // selezione dinamica dell'algoritmo
    std::vector<std::vector<int>> cycles = use_de_pina ? get_fundamental_cycles_dePina(G) : get_fundamental_cycles_dfs(G);
    
    int n = cycles.size(); // n = |E|-|V|+1
    int h = gen_edges.size();

    Eigen::VectorXd v = Eigen::VectorXd::Zero(n);

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < h; i++) {
            if (is_in_cycle_forward(gen_edges[i], cycles[j])) {
                v(j) -= gen_edges[i].comp().value();
            } 
            else if (is_in_cycle_backward(gen_edges[i], cycles[j])) {
                v(j) += gen_edges[i].comp().value();
            }
        }
    }
    
    return v;
}

inline Eigen::VectorXd find_current(const undirected_graph& G, bool use_de_pina = false) {
    Eigen::MatrixXd B = build_incidence_matrix(G, use_de_pina);
    Eigen::MatrixXd R = build_resistance_matrix(G);
    Eigen::VectorXd v = build_generator_vector(G, use_de_pina); 

    Eigen::MatrixXd A = B.transpose() * R * B;
    Eigen::VectorXd x0 = Eigen::VectorXd::Random(v.size());

    int steps = 0;
    int it_max = 1000;
    const double res_tol = 1.0e-12;

    Eigen::VectorXd i = conjugate_gradient(A, v, x0, it_max, res_tol, steps);
    return i;
}

inline Eigen::VectorXd find_current_resistor(const undirected_graph& G, bool use_de_pina = false) {
    Eigen::MatrixXd B = build_incidence_matrix(G, use_de_pina);
    Eigen::MatrixXd R = build_resistance_matrix(G);
    Eigen::VectorXd v_R = R * B * find_current(G, use_de_pina);
    return v_R;
}
