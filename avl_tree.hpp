#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>

using namespace std;

// Prosta implementacja drzewa AVL
class AVLTree {
private:
    // Struktura wezla dla drzewa AVL
    struct Node {
        int key;
        int value;
        Node* left;
        Node* right;
        int height;

        Node(int k, int v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;
    int size;

    // Pobieranie wysokosci wezla
    int height(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    // Pobieranie wspolczynnika zbalansowania wezla
    int getBalance(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    // Rotacja w prawo
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Wykonanie rotacji
        x->right = y;
        y->left = T2;

        // Aktualizacja wysokosci
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    // Rotacja w lewo
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Wykonanie rotacji
        y->left = x;
        x->right = T2;

        // Aktualizacja wysokosci
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    // Wstawianie wezla do drzewa AVL
    Node* insertNode(Node* node, int key, int value) {
        // Wykonanie standardowego wstawiania BST
        if (node == nullptr) {
            size++;
            return new Node(key, value);
        }

        if (key < node->key) {
            node->left = insertNode(node->left, key, value);
        }
        else if (key > node->key) {
            node->right = insertNode(node->right, key, value);
        }
        else {
            // Klucz juz istnieje, aktualizacja wartosci
            node->value = value;
            return node;
        }

        // Aktualizacja wysokosci biezacego wezla
        node->height = 1 + max(height(node->left), height(node->right));

        // Pobranie wspolczynnika zbalansowania
        int balance = getBalance(node);

        // Przypadek Lewy-Lewy
        if (balance > 1 && key < node->left->key) {
            return rightRotate(node);
        }

        // Przypadek Prawy-Prawy
        if (balance < -1 && key > node->right->key) {
            return leftRotate(node);
        }

        // Przypadek Lewy-Prawy
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Przypadek Prawy-Lewy
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Znalezienie wezla z minimalna wartoscia
    Node* minValueNode(Node* node) {
        Node* current = node;

        while (current->left != nullptr) {
            current = current->left;
        }

        return current;
    }

    // Usuniecie wezla z drzewa AVL
    Node* deleteNode(Node* root, int key, bool& deleted) {
        // Wykonanie standardowego usuwania BST
        if (root == nullptr) {
            return root;
        }

        if (key < root->key) {
            root->left = deleteNode(root->left, key, deleted);
        }
        else if (key > root->key) {
            root->right = deleteNode(root->right, key, deleted);
        }
        else {
            // Wezel z kluczem do usuniecia

            // Wezel z jednym dzieckiem lub bez dzieci
            if (root->left == nullptr || root->right == nullptr) {
                Node* temp = root->left ? root->left : root->right;

                // Przypadek braku dzieci
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else {
                    // Przypadek jednego dziecka
                    *root = *temp;
                }

                delete temp;
                size--;
                deleted = true;
            }
            else {
                // Wezel z dwojgiem dzieci
                Node* temp = minValueNode(root->right);

                // Kopiowanie danych nastepnika w porzadku inorder do tego wezla
                root->key = temp->key;
                root->value = temp->value;

                // Usuniecie nastepnika w porzadku inorder
                root->right = deleteNode(root->right, temp->key, deleted);
            }
        }

        // Jesli drzewo mialo tylko jeden wezel
        if (root == nullptr) {
            return root;
        }

        // Aktualizacja wysokosci biezacego wezla
        root->height = 1 + max(height(root->left), height(root->right));

        // Pobranie wspolczynnika zbalansowania
        int balance = getBalance(root);

        // Lewa lewa
        if (balance > 1 && getBalance(root->left) >= 0) {
            return rightRotate(root);
        }

        // Lewa prawa
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Prawa prawa
        if (balance < -1 && getBalance(root->right) <= 0) {
            return leftRotate(root);
        }

        // Prawa lewa
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // Wyszukiwanie klucza w drzewie AVL
    Node* search(Node* root, int key) {
        if (root == nullptr || root->key == key) {
            return root;
        }

        if (root->key < key) {
            return search(root->right, key);
        }

        return search(root->left, key);
    }

    // Czyszczenie drzewa AVL
    void clearTree(Node* node) {
        if (node == nullptr) {
            return;
        }

        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
     
    // Funkcja pomocnicza do pobierania wszystkich par
    void getAllPairsHelper(Node* node, vector<pair<int, int>>& pairs) {
        if (node == nullptr) {
            return;
        }

        getAllPairsHelper(node->left, pairs);
        pairs.push_back(make_pair(node->key, node->value));
        getAllPairsHelper(node->right, pairs);
    }

public:
    AVLTree() : root(nullptr), size(0) {}


    // Pobranie wszystkich par klucz-wartosc z drzewa
    void getAllPairs(vector<pair<int, int>>& pairs) {
        getAllPairsHelper(root, pairs);
    }

    ~AVLTree() {
        clearTree(root);
    }

    // Wstawianie pary klucz-wartosc
    void insert(int key, int value) {
        root = insertNode(root, key, value);
    }

    // Usuwanie pary klucz-wartosc
    bool remove(int key) {
        bool deleted = false;
        root = deleteNode(root, key, deleted);
        return deleted;
    }

    // Pobieranie wartosci dla klucza
    int get(int key) {
        Node* node = search(root, key);
        if (node == nullptr) {
            return -1;
        }
        return node->value;
    }

    // Czyszczenie drzewa AVL
    void clear() {
        clearTree(root);
        root = nullptr;
        size = 0;
    }

    // Pobieranie aktualnego rozmiaru
    int getSize() {
        return size;
    }
};

#endif
