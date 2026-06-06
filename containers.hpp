#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>

template<typename T>
inline void print_vector(const std::vector<T>& v)
{
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << " " << std::endl;        
    }
    std::cout << "\n";
}

/* Struttura dati PILA con politica LIFO (Last In First Out)*/
/* Analogia con una PILA di libri: posiziono un libro nella cima della pila con put();
   e prendo il libro dalla cima della pila con get() [altrimenti la pila casca] */
template<typename T> 
class stack{
private: 
    std::list<T> l;
public:
    stack() {}

    // Inserisce un elemento in CIMA allo stack (in testa alla lista).
    // Complessità O(1).
    void put(const T& obj) {             // la push delle note
        l.push_front(obj);
    }      

    // Legge e rimuove l'elemento in CIMA allo stack (in testa alla lista).
    // Essendo una politica LIFO, estraggo l'ultimo elemento che è stato inserito.
    // Lancia un'eccezione se lo stack è vuoto.
    T get() {                            // la pop delle note
        if (l.empty()) {
            throw std::out_of_range("Stack is Empty!");   // lancio un'eccezione
        }
        T obj = l.front();
        l.pop_front();
        return obj;
    }

    bool empty() const {
        return l.empty();
    }

    friend std::ostream& operator<<(std::ostream& os, const stack<T>& st)
    {
        for (const auto& element : st.l) {
            os << element << " ";
        }
        os << "\n";
        return os;
    }
};

/* Struttura dati CODA con politica FIFO (First In First Out)*/
/* Analogia con una CODA alle casse: l'ultimo che arriva si mette in coda con put(); 
   e il primo cliente è quello che se ne va con get() */
template<typename T> 
class queue{
private: 
    std::list<T> l;
public:
    queue() {}

    // Inserisce un nuovo elemento mettendolo all'INIZIO della lista (spingendo gli altri indietro).
    // In ottica FIFO, la coda della lista rappresenta l'uscita, quindi inserisco dall'ingresso (la testa).
    void put(const T& obj) {             // la enqueue delle note
        l.push_front(obj);
    }      

    // Legge e rimuove l'elemento alla FINE della lista.
    // Essendo una politica FIFO, estraggo l'elemento più "vecchio" (quello che è stato
    // spinto fino in fondo alla lista dai successivi inserimenti).
    T get() {                            // la dequeue delle note
        if (l.empty()) {
            throw std::out_of_range("Queue is Empty!");   // lancio un'eccezione
        }
        T obj = l.back();
        l.pop_back();
        return obj;
    }

    bool empty() const {
        return l.empty();
    }

    friend std::ostream& operator<<(std::ostream& os, const queue<T>& q)
    {
        for (const auto& element : q.l) {
            os << element << " ";
        }
        os << "\n";
        return os;
    }
};
