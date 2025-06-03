#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include "open_addressing.hpp"
#include "chaining.hpp"

using namespace std;

// Losowa liczba
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Test wydajnosci
void testPerformance() {
    // Rozmiary do testowania
    const int sizes[] = { 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000, 150000, 200000 };
    const int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    
    // Liczba zestawow danych
    const int n = 5;
    
    // Liczba powtorzen
    const int rep = 10;
    
    // Pomiar calkowitego czasu
    auto fullTimeStart = chrono::high_resolution_clock::now();
    
    // Plik wynikow
    ofstream outFile("wyniki.xlsx");
    outFile << "Rozmiar\tAdresowanie otwarte Wstawianie (ns)\tLancuchowanie Wstawianie (ns)\t"
            << "Adresowanie otwarte Usuwanie (ns)\tLancuchowanie Usuwanie (ns)\n";
    
    // Dla kazdego rozmiaru
    for (int s = 0; s < numSizes; s++) {
        int size = sizes[s];
        cout << "Testowanie dla rozmiaru: " << size << endl;
        
        // Srednie czasy
        double avgOpenAddressingInsert = 0;
        double avgChainingInsert = 0;
        double avgOpenAddressingRemove = 0;
        double avgChainingRemove = 0;
        
        // Dla kazdego zestawu danych
        for (int dataSet = 0; dataSet < n; dataSet++) {
            cout << "  Zestaw danych " << dataSet + 1 << " z " << n << endl;
            
            // Generator losowy
            srand(time(nullptr) + dataSet);
            
            // Losowe klucze i wartosci
            vector<int> keys(size);
            vector<int> values(size);
            
            for (int i = 0; i < size; i++) {
                keys[i] = randomInt(1, size * 10);
                values[i] = randomInt(1, 1000);
            }
            
            // Dla kazdego powtorzenia
            for (int r = 0; r < rep; r++) {
                // Nowe tablice
                HashTableOpenAddressing openAddressingTable;
                HashTableChaining chainingTable;
                
                // Test wstawiania - adresowanie otwarte
                auto start = chrono::high_resolution_clock::now();
                for (int i = 0; i < size; i++) {
                    openAddressingTable.insert(keys[i], values[i]);
                }
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                avgOpenAddressingInsert += duration / (double)size;
                
                // Test wstawiania - lancuchowanie
                start = chrono::high_resolution_clock::now();
                for (int i = 0; i < size; i++) {
                    chainingTable.insert(keys[i], values[i]);
                }
                end = chrono::high_resolution_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                avgChainingInsert += duration / (double)size;
                
                // Test usuwania - adresowanie otwarte
                start = chrono::high_resolution_clock::now();
                for (int i = 0; i < size; i++) {
                    openAddressingTable.remove(keys[i]);
                }
                end = chrono::high_resolution_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                avgOpenAddressingRemove += duration / (double)size;
                
                // Test usuwania - lancuchowanie
                start = chrono::high_resolution_clock::now();
                for (int i = 0; i < size; i++) {
                    chainingTable.remove(keys[i]);
                }
                end = chrono::high_resolution_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                avgChainingRemove += duration / (double)size;
            }
        }
        
        // Oblicz srednie
        avgOpenAddressingInsert /= (n * rep);
        avgChainingInsert /= (n * rep);
        avgOpenAddressingRemove /= (n * rep);
        avgChainingRemove /= (n * rep);
        
        // Zapisz wyniki
        outFile << size << "\t"
                << avgOpenAddressingInsert << "\t"
                << avgChainingInsert << "\t"
                << avgOpenAddressingRemove << "\t"
                << avgChainingRemove << "\n";
        
        // Wyswietl wyniki
        cout << "  Wyniki dla rozmiaru " << size << ":" << endl;
        cout << "    Adresowanie otwarte Wstawianie: " << avgOpenAddressingInsert << " ns" << endl;
        cout << "    Lancuchowanie Wstawianie: " << avgChainingInsert << " ns" << endl;
        cout << "    Adresowanie otwarte Usuwanie: " << avgOpenAddressingRemove << " ns" << endl;
        cout << "    Lancuchowanie Usuwanie: " << avgChainingRemove << " ns" << endl;
    }
    
    outFile.close();
    
    // Calkowity czas
    auto fullTimeEnd = chrono::high_resolution_clock::now();
    auto fullTimeDuration = chrono::duration_cast<chrono::minutes>(fullTimeEnd - fullTimeStart).count();
    cout << "Calkowity czas pomiaru: " << fullTimeDuration << " minut" << endl;
}

// Menu
void mainMenu() {
    int choice;
    bool exit = false;
    
    while (!exit) {
        cout << "\n=== MENU GLOWNE ===" << endl;
        cout << "1. Rozpoczecie pomiarow wydajnosci" << endl;
        cout << "0. Wyjscie" << endl;
        cout << "Wybierz opcje: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                testPerformance();
                break;
            case 0:
                exit = true;
                break;
            default:
                cout << "Nieprawidlowa opcja. Sprobuj ponownie." << endl;
        }
    }
}

int main() {
    cout << "=== SLOWNIK OPARTY NA TABLICY MIESZAJACEJ ===" << endl;
    cout << "1. Tablica mieszajaca z adresowaniem otwartym" << endl;
    cout << "2. Tablica mieszajaca z lancuchowaniem" << endl;
    cout << "3. WIP Tablica mieszajaca z lancuchowaniem (drzewa AVL)" << endl;
    
    mainMenu();
    
    return 0;
}
