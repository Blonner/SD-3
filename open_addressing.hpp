#ifndef OPEN_ADDRESSING_HPP
#define OPEN_ADDRESSING_HPP

#include <iostream>

using namespace std;

class HashTableOpenAddressing {
private:
    // Para klucz-wartosc
    struct Pair {
        int key;
        int value;
        bool isOccupied;
        bool isDeleted;

        Pair() : key(0), value(0), isOccupied(false), isDeleted(false) {}
    };

    Pair* table;
    int capacity;
    int size;
    const double LOAD_FACTOR_THRESHOLD = 0.7;

    // Funkcja mieszajaca
    int hash(int key) {
        return abs(key) % capacity;
    }

    // Zmiana rozmiaru tablicy
    void resize() {
        int oldCapacity = capacity;
        Pair* oldTable = table;
        
        capacity *= 2;
        table = new Pair[capacity];
        size = 0;
        
        for (int i = 0; i < oldCapacity; i++) {
            if (oldTable[i].isOccupied && !oldTable[i].isDeleted) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
        
        delete[] oldTable;
    }

public:
    HashTableOpenAddressing() {
        capacity = 16;
        size = 0;
        table = new Pair[capacity];
    }

    ~HashTableOpenAddressing() {
        delete[] table;
    }

    // Wstawianie klucz-wartosc
    void insert(int key, int value) {
        if ((double)size / capacity >= LOAD_FACTOR_THRESHOLD) {
            resize();
        }
        
        int index = hash(key);
        int i = 0;
        
        // Sondowanie liniowe
        while (i < capacity) {
            int probeIndex = (index + i) % capacity;
            
            // Jesli miejsce puste lub usuniete
            if (!table[probeIndex].isOccupied || table[probeIndex].isDeleted) {
                table[probeIndex].key = key;
                table[probeIndex].value = value;
                table[probeIndex].isOccupied = true;
                table[probeIndex].isDeleted = false;
                size++;
                return;
            }
            
            // Jesli klucz juz istnieje
            if (table[probeIndex].key == key && !table[probeIndex].isDeleted) {
                table[probeIndex].value = value;
                return;
            }
            
            i++;
        }
    }

    // Usuwanie klucz-wartosc
    bool remove(int key) {
        int index = hash(key);
        int i = 0;
        
        while (i < capacity) {
            int probeIndex = (index + i) % capacity;
            
            // Jesli miejsce puste
            if (!table[probeIndex].isOccupied) {
                return false;
            }
            
            // Jesli znaleziono klucz
            if (table[probeIndex].key == key && !table[probeIndex].isDeleted) {
                table[probeIndex].isDeleted = true;
                size--;
                return true;
            }
            
            i++;
        }
        
        return false;
    }

    // Pobieranie wartosci
    int get(int key) {
        int index = hash(key);
        int i = 0;
        
        while (i < capacity) {
            int probeIndex = (index + i) % capacity;
            
            // Jesli miejsce puste
            if (!table[probeIndex].isOccupied) {
                return -1;
            }
            
            // Jesli znaleziono klucz
            if (table[probeIndex].key == key && !table[probeIndex].isDeleted) {
                return table[probeIndex].value;
            }
            
            i++;
        }
        
        return -1;
    }

    // Czyszczenie
    void clear() {
        delete[] table;
        capacity = 16;
        size = 0;
        table = new Pair[capacity];
    }

    // Rozmiar
    int getSize() {
        return size;
    }
};

#endif
