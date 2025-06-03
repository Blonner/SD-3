#ifndef CHAINING_HPP
#define CHAINING_HPP

#include <iostream>

using namespace std;

class HashTableChaining {
private:
    // Wezel listy
    struct Node {
        int key;
        int value;
        Node* next;
        
        Node(int k, int v) : key(k), value(v), next(nullptr) {}
    };
    
    Node** table;
    int capacity;
    int size;
    const double LOAD_FACTOR_THRESHOLD = 1.0;
    
    // Funkcja mieszajaca
    int hash(int key) {
        return abs(key) % capacity;
    }
    
    // Zmiana rozmiaru
    void resize() {
        int oldCapacity = capacity;
        Node** oldTable = table;
        
        capacity *= 2;
        table = new Node*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
        
        size = 0;
        
        // Przenies wszystkie elementy
        for (int i = 0; i < oldCapacity; i++) {
            Node* current = oldTable[i];
            while (current != nullptr) {
                insert(current->key, current->value);
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        
        delete[] oldTable;
    }

public:
    HashTableChaining() {
        capacity = 16;
        size = 0;
        table = new Node*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }
    
    ~HashTableChaining() {
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }
    
    // Wstawianie
    void insert(int key, int value) {
        if ((double)size / capacity >= LOAD_FACTOR_THRESHOLD) {
            resize();
        }
        
        int index = hash(key);
        
        // Sprawdz czy klucz istnieje
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        // Dodaj nowy wezel na poczatek
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        size++;
    }
    
    // Usuwanie
    bool remove(int key) {
        int index = hash(key);
        
        Node* current = table[index];
        Node* prev = nullptr;
        
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                
                delete current;
                size--;
                return true;
            }
            
            prev = current;
            current = current->next;
        }
        
        return false;
    }
    
    // Pobieranie wartosci
    int get(int key) {
        int index = hash(key);
        
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        
        return -1;
    }
    
    // Czyszczenie
    void clear() {
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        
        delete[] table;
        capacity = 16;
        size = 0;
        table = new Node*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }
    
    // Rozmiar
    int getSize() {
        return size;
    }
};

#endif
