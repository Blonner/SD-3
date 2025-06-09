#ifndef OPEN_ADDRESSING_HPP
#define OPEN_ADDRESSING_HPP

#include <iostream>

using namespace std;

// Prosta tablica mieszajaca z adresowaniem otwartym (sondowanie liniowe)
class HashTableOpenAddressing {
private:
    // Struktura pary klucz-wartosc
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

    // Prosta funkcja mieszajaca
    int hash(int key) {
        return abs(key) % capacity;
    }

    // Zmiana rozmiaru tablicy gdy wspolczynnik wypelnienia przekroczy prog
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

    // Wstawianie pary klucz-wartosc
    void insert(int key, int value) {
        // Sprawdzenie czy potrzebna jest zmiana rozmiaru
        if ((double)size / capacity >= LOAD_FACTOR_THRESHOLD) {
            resize();
        }

        int index = hash(key);
        int i = 0;

        // Sondowanie liniowe
        while (i < capacity) {
            int probeIndex = (index + i) % capacity;

            // Jesli miejsce jest puste lub oznaczone jako usuniete
            if (!table[probeIndex].isOccupied || table[probeIndex].isDeleted) {
                table[probeIndex].key = key;
                table[probeIndex].value = value;
                table[probeIndex].isOccupied = true;
                table[probeIndex].isDeleted = false;
                size++;
                return;
            }

            // Jesli klucz juz istnieje, aktualizuj wartosc
            if (table[probeIndex].key == key && !table[probeIndex].isDeleted) {
                table[probeIndex].value = value;
                return;
            }

            i++;
        }
    }

    // Usuwanie pary klucz-wartosc
    bool remove(int key) {
        int index = hash(key);
        int i = 0;

        while (i < capacity) {
            int probeIndex = (index + i) % capacity;

            // Jesli miejsce jest puste, klucz nie istnieje
            if (!table[probeIndex].isOccupied) {
                return false;
            }

            // Jesli klucz zostal znaleziony i nie jest usuniety
            if (table[probeIndex].key == key && !table[probeIndex].isDeleted) {
                table[probeIndex].isDeleted = true;
                size--;
                return true;
            }

            i++;
        }

        return false;
    }

    // Pobieranie wartosci dla klucza
    int get(int key) {
        int index = hash(key);
        int i = 0;

        while (i < capacity) {
            int probeIndex = (index + i) % capacity;

            // Jesli miejsce jest puste, klucz nie istnieje
            if (!table[probeIndex].isOccupied) {
                return -1;
            }

            // Jesli klucz zostal znaleziony i nie jest usuniety
            if (table[probeIndex].key == key && !table[probeIndex].isDeleted) {
                return table[probeIndex].value;
            }

            i++;
        }

        return -1;
    }

    // Czyszczenie tablicy mieszajacej
    void clear() {
        delete[] table;
        capacity = 16;
        size = 0;
        table = new Pair[capacity];
    }

    // Pobieranie aktualnego rozmiaru
    int getSize() {
        return size;
    }
};

#endif
