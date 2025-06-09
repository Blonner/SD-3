#ifndef AVL_HPP
#define AVL_HPP

#include <iostream>
#include "avl_tree.hpp"
#include <vector>
#include <utility>

using namespace std;

// Tablica mieszajaca z lancuchowaniem wykorzystujaca drzewa AVL
class HashTableAVL {
private:
    AVLTree* table;
    int capacity;
    int size;
    const double LOAD_FACTOR_THRESHOLD = 1.0;

    // Prosta funkcja mieszajaca
    int hash(int key) {
        return abs(key) % capacity;
    }

    // Zmiana rozmiaru tablicy gdy wspolczynnik wypelnienia przekroczy prog
    void resize() {
        int oldCapacity = capacity;
        AVLTree* oldTable = table;

        capacity *= 2;                    // Podwajamy rozmiar
        table = new AVLTree[capacity];    // Nowa tablica
        size = 0;                         // Resetujemy size

        // Przechodzimy przez ka¿dy kube³ek starej tablicy
        for (int i = 0; i < oldCapacity; i++) {
            vector<pair<int, int>> pairs;
            oldTable[i].getAllPairs(pairs);  // Pobieramy wszystkie pary z drzewa AVL

            // Kazda para do nowej tablicy
            for (const auto& p : pairs) {
                insert(p.first, p.second);   // Nowa funkcja hash
            }
        }

        delete[] oldTable;
    }

public:
    HashTableAVL() {
        capacity = 16;
        size = 0;
        table = new AVLTree[capacity];
    }

    ~HashTableAVL() {
        delete[] table;
    }

    // Wstawianie pary klucz-wartosc
    void insert(int key, int value) {
        // Sprawdzenie czy potrzebna jest zmiana rozmiaru
        if ((double)size / capacity >= LOAD_FACTOR_THRESHOLD) {
            resize();
        }

        int index = hash(key);

        // Sprawdzenie czy klucz juz istnieje
        int oldValue = table[index].get(key);
        if (oldValue == -1) {
            // Klucz nie istnieje, zwieksz rozmiar
            size++;
        }

        table[index].insert(key, value);
    }

    // Usuwanie pary klucz-wartosc
    bool remove(int key) {
        int index = hash(key);

        bool removed = table[index].remove(key);
        if (removed) {
            size--;
        }

        return removed;
    }

    // Pobieranie wartosci dla klucza
    int get(int key) {
        int index = hash(key);
        return table[index].get(key);
    }

    // Czyszczenie tablicy mieszajacej
    void clear() {
        for (int i = 0; i < capacity; i++) {
            table[i].clear();
        }

        delete[] table;
        capacity = 16;
        size = 0;
        table = new AVLTree[capacity];
    }

    // Pobieranie aktualnego rozmiaru
    int getSize() {
        return size;
    }
};

#endif
