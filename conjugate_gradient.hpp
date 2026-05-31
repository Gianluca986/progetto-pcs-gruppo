#include <iostream>
#include <Eigen/Dense>

/*
   Nota sull'argomento 'steps':
   Ho passato la variabile steps per riferimento (int&) per poter estrarre il 
   numero di iterazioni impiegate dall'algoritmo. Poiché la funzione 
   restituisce già il vettore soluzione (Eigen::VectorXd), questo approccio 
   mi permette di aggiornare direttamente una variabile del chiamante (es. nel test) 
   senza dover creare strutture dati complesse per un ritorno multiplo.
 */
Eigen::VectorXd conjugate_gradient(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, const Eigen::VectorXd& x0, int it_max, double res_tol, int& steps) {

    /*controllo che la matrice A sia simmetrica*/
    if (!A.isApprox(A.transpose())) {
        // se non è simmetrica allora restituisco un vettore di zeri
        return Eigen::VectorXd::Zero(x0.size());
    }
    int it = 0;
    Eigen::VectorXd res0 = b - A*x0;
    Eigen::VectorXd p = res0;    
    Eigen::VectorXd x = x0;
    double res0_norm = res0.norm();    

    Eigen::VectorXd res = res0;
    if (res0_norm <= 1.0e-16) {     // il sistema lineare è già risolto
        steps = 0;
        return x0; 
    }

    while (it < it_max && res.norm() > res_tol * res0_norm)  // norma del residuo RELATIVA
    {
        auto pAp = p.dot(A * p);

        const double alpha_k = p.dot(res) / pAp;

        x = x + alpha_k * p;
        res = b - A * x;

        const double beta_k = p.dot(A * res) / pAp;
        
        p = res - beta_k * p;
        it++;
    }

    
    steps = it;
    return x;
}