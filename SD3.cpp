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
#include "avl.hpp"

using namespace std;

// Generowanie losowej liczby 
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Testowanie wydajnosci operacji 
void testPerformance() {
    // Rozmiary struktur do testowania
    const int sizes[] = { 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000, 150000, 200000 };
    const int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    // Liczba zestawow danych
    const int n = 10;

    // Liczba powtorzen dla kazdego zestawu danych
    const int rep = 100;

    // Pomiar calkowitego czasu
    auto fullTimeStart = chrono::high_resolution_clock::now();

    // Utworzenie pliku wyjsciowego
    ofstream outFile("wyniki.xlsx");
    outFile << "Rozmiar\tAdresowanie otwarte Wstawianie (ns)\tLancuchowanie Wstawianie (ns)\tAVL Wstawianie (ns)\t"
        << "Adresowanie otwarte Usuwanie (ns)\tLancuchowanie Usuwanie (ns)\tAVL Usuwanie (ns)\n";

    // Dla kazdego rozmiaru
    for (int s = 0; s < numSizes; s++) {
        int size = sizes[s];
        cout << "Testowanie dla rozmiaru: " << size << endl;

        // Zmienne do przechowywania srednich czasow
        double avgOpenAddressingInsert = 0;
        double avgChainingInsert = 0;
        double avgAVLInsert = 0;
        double avgOpenAddressingRemove = 0;
        double avgChainingRemove = 0;
        double avgAVLRemove = 0;

        // Dla kazdego zestawu danych
        for (int dataSet = 0; dataSet < n; dataSet++) {
            cout << "  Zestaw danych " << dataSet + 1 << " z " << n << endl;

            // Inicjalizacja generatora losowego z roznym ziarnem dla kazdego zestawu danych
            srand(time(nullptr) + dataSet);

            // Generowanie losowych kluczy i wartosci
            vector<int> keys(size);
            vector<int> values(size);

            for (int i = 0; i < size; i++) {
                keys[i] = randomInt(1, size * 10);
                values[i] = randomInt(1, 1000);
            }

            // Dla kazdego powtorzenia
            for (int r = 0; r < rep; r++) {
                // Tworzenie tablic mieszajacych
                HashTableOpenAddressing openAddressingTable;
                HashTableChaining chainingTable;
                HashTableAVL avlTable;

                // Test operacji wstawiania dla adresowania otwartego
                auto start = chrono::high_resolution_clock::now();
                for (int i = 0; i < size; i++) {
                    openAddressingTable.insert(keys[i], values[i]);
                }
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                avgOpenAddressingInsert += duration / (double)size;

                // Test operacji wstawiania dla lancuchowania
                start = chrono::high_resolution_clock::now();
                for (int i = 0; i < size; i++) {
                    chainingTable.insert(keys[i], values[i]);
                }
                end = chrono::high_resolution_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                avgChainingInsert += duration / (double)size;

                // Test operacji wstawiania dla AVL
                start = chrono::high_resolution_clock::now();
                for (int i = 0; i < size; i++) {
                    avlTable.insert(keys[i], values[i]);
                }
                end = chrono::high_resolution_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                avgAVLInsert += duration / (double)size;

                // Test operacji usuwania dla adresowania otwartego
                start = chrono::high_resolution_clock::now();
                for (int i = 0; i < size; i++) {
                    openAddressingTable.remove(keys[i]);
                }
                end = chrono::high_resolution_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                avgOpenAddressingRemove += duration / (double)size;

                // Test operacji usuwania dla lancuchowania
                start = chrono::high_resolution_clock::now();
                for (int i = 0; i < size; i++) {
                    chainingTable.remove(keys[i]);
                }
                end = chrono::high_resolution_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                avgChainingRemove += duration / (double)size;

                // Test operacji usuwania dla AVL
                start = chrono::high_resolution_clock::now();
                for (int i = 0; i < size; i++) {
                    avlTable.remove(keys[i]);
                }
                end = chrono::high_resolution_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                avgAVLRemove += duration / (double)size;
            }
        }

        // Obliczanie srednich czasow
        avgOpenAddressingInsert /= (n * rep);
        avgChainingInsert /= (n * rep);
        avgAVLInsert /= (n * rep);
        avgOpenAddressingRemove /= (n * rep);
        avgChainingRemove /= (n * rep);
        avgAVLRemove /= (n * rep);

        // Zapisywanie wynikow do pliku
        outFile << size << "\t"
            << avgOpenAddressingInsert << "\t"
            << avgChainingInsert << "\t"
            << avgAVLInsert << "\t"
            << avgOpenAddressingRemove << "\t"
            << avgChainingRemove << "\t"
            << avgAVLRemove << "\n";

        // Wyswietlanie wynikow w konsoli
        cout << "  Wyniki dla rozmiaru " << size << ":" << endl;
        cout << "    Adresowanie otwarte Wstawianie: " << avgOpenAddressingInsert << " ns" << endl;
        cout << "    Lancuchowanie Wstawianie: " << avgChainingInsert << " ns" << endl;
        cout << "    AVL Wstawianie: " << avgAVLInsert << " ns" << endl;
        cout << "    Adresowanie otwarte Usuwanie: " << avgOpenAddressingRemove << " ns" << endl;
        cout << "    Lancuchowanie Usuwanie: " << avgChainingRemove << " ns" << endl;
        cout << "    AVL Usuwanie: " << avgAVLRemove << " ns" << endl;
    }

    outFile.close();

    // Obliczanie i wyswietlanie calkowitego czasu
    auto fullTimeEnd = chrono::high_resolution_clock::now();
    auto fullTimeDuration = chrono::duration_cast<chrono::minutes>(fullTimeEnd - fullTimeStart).count();
    cout << "Calkowity czas pomiaru: " << fullTimeDuration << " minut" << endl;
}

// Menu glowne
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
    cout << "=== IMPLEMENTACJE SLOWNIKA OPARTEGO NA TABLICY MIESZAJACEJ ===" << endl;
    cout << "1. Tablica mieszajaca z adresowaniem otwartym" << endl;
    cout << "2. Tablica mieszajaca z lancuchowaniem (listy powiazane)" << endl;
    cout << "3. Tablica mieszajaca z lancuchowaniem (drzewa AVL)" << endl;

    mainMenu();

    return 0;
}
