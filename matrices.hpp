#pragma once
#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <set> 
#include <unordered_map>
#include <Eigen/Dense>
#include "undirected_edge.hpp" 
#include "undirected_graph.hpp" 
#include "components.hpp" 
#include "conjugate_gradient.hpp" 


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
bool is_in_cycle_forward(const undirected_edge<T>& res, const std::vector<int>& cycle) {
    for (size_t i = 0; i < cycle.size() - 1; i++) {
        if (cycle[i] == res.from() && cycle[i+1] == res.to()) {
            return true;
        }
    }
    return false;
}

template<typename T>
bool is_in_cycle_backward(const undirected_edge<T>& res, const std::vector<int>& cycle) {
    for (size_t i = 0; i < cycle.size() - 1; i++) {
        if (cycle[i+1] == res.from() && cycle[i] == res.to()) {
            return true;
        }
    }
    return false;
}

/* Funzione per costruire la matrice di incidenza B (m × n) */
Eigen::MatrixXd build_incidence_matrix(const undirected_graph& G) {
    std::vector<undirected_edge<resistor>> res_edges = G.all_edges_res();
    
    std::sort(res_edges.begin(), res_edges.end(), SortByComponentNumber<resistor>()); 

    std::vector<std::vector<int>> cycles = get_fundamental_cycles(G);
    
    int m = res_edges.size();
    int n = cycles.size(); // n = |E| - |V| + 1
    
    // Inizializza la matrice già perfettamente riempita di 0
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
Eigen::MatrixXd build_resistance_matrix(const undirected_graph& G) {
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
Eigen::VectorXd build_generator_vector(const undirected_graph& G) {
    std::vector<undirected_edge<generator>> gen_edges = G.all_edges_gen();

    std::sort(gen_edges.begin(), gen_edges.end(), SortByComponentNumber<generator>());

    std::vector<std::vector<int>> cycles = get_fundamental_cycles(G);
    
    int n = cycles.size(); // n = |E| - |V| + 1
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

/* forse conviene unire gli output per non sprecare i conti*/
Eigen::VectorXd find_current(const undirected_graph& G) {
    Eigen::MatrixXd B = build_incidence_matrix(G);
    Eigen::MatrixXd R = build_resistance_matrix(G);
    Eigen::VectorXd v = build_generator_vector(G); 
    Eigen::MatrixXd A = B.transpose() * R * B;
    Eigen::VectorXd x0 = Eigen::VectorXd::Random(v.size());
    int steps = 0;
    int it_max = 1000;
    const double res_tol = 1.0e-12;
    Eigen::VectorXd i = conjugate_gradient(A, v, x0, it_max, res_tol, steps);
    return i;
}

Eigen::VectorXd find_current_resistor(const undirected_graph& G) {
    Eigen::MatrixXd B = build_incidence_matrix(G);
    Eigen::MatrixXd R = build_resistance_matrix(G);
    Eigen::MatrixXd A = R * B;
    Eigen::VectorXd v_R = R * B * find_current(G);
    return v_R;
}



