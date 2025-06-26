#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#define CLEAR "cls"
#else
#include <unistd.h>
#define CLEAR "clear"
#endif

using namespace std;


struct Product {
    string name;
    int price;
    int stock;
};


string priceBar(int price) {
    if (price < 20) return "\033[32m"; // Green
    if (price < 50) return "\033[33m"; // Yellow
    return "\033[31m"; // Red
}


void displayProducts(const vector<Product>& products, string sortBy) {
    system(CLEAR);
    cout << "Name\t\tPrice\tStock\tVisual\n";
    for (const auto& p : products) {
        string color = priceBar(p.price);
        cout << p.name << "\t\t₹" << p.price << "\t" << p.stock << "\t" << color;
        for (int i = 0; i < p.price / 10; ++i) cout << "|";
        cout << "\033[0m\n";
    }
    this_thread::sleep_for(chrono::milliseconds(600));
}


bool compare(const Product& a, const Product& b, string key) {
    if (key == "price") return a.price > b.price;
    if (key == "name") return a.name > b.name;
    if (key == "stock") return a.stock > b.stock;
    return false;
}


void bubbleSort(vector<Product>& p, string key) {
    int n = p.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++) {
            if (compare(p[j], p[j + 1], key)) {
                swap(p[j], p[j + 1]);
                displayProducts(p, key);
            }
        }
}


void selectionSort(vector<Product>& p, string key) {
    int n = p.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (compare(p[minIdx], p[j], key)) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(p[i], p[minIdx]);
            displayProducts(p, key);
        }
    }
}


void insertionSort(vector<Product>& p, string key) {
    int n = p.size();
    for (int i = 1; i < n; ++i) {
        Product temp = p[i];
        int j = i - 1;
        while (j >= 0 && compare(p[j], temp, key)) {
            p[j + 1] = p[j];
            j--;
        }
        p[j + 1] = temp;
        displayProducts(p, key);
    }
}


bool mergeCompare(const Product& a, const Product& b, const string& key) {
    if (key == "price") return a.price < b.price;
    if (key == "name") return a.name < b.name;
    if (key == "stock") return a.stock < b.stock;
    return false;
}

void merge(vector<Product>& p, int l, int m, int r, string key) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<Product> L(p.begin() + l, p.begin() + m + 1);
    vector<Product> R(p.begin() + m + 1, p.begin() + r + 1);

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (mergeCompare(L[i], R[j], key)) p[k++] = L[i++];
        else p[k++] = R[j++];
        displayProducts(p, key);
    }
    while (i < n1) p[k++] = L[i++];
    while (j < n2) p[k++] = R[j++];
    displayProducts(p, key);
}

void mergeSort(vector<Product>& p, int l, int r, string key) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(p, l, m, key);
        mergeSort(p, m + 1, r, key);
        merge(p, l, m, r, key);
    }
}


int main() {
    vector<Product> products = {
        {"Soap", 40, 20},
        {"Toothpaste", 10, 50},
        {"Detergent", 60, 15},
        {"Shampoo", 30, 35}
    };

    int algoChoice, fieldChoice;
    cout << "Choose Sorting Algorithm:\n";
    cout << "1. Bubble Sort\n2. Selection Sort\n3. Insertion Sort\n4. Merge Sort\n> ";
    cin >> algoChoice;

    cout << "\nSort by:\n1. Price\n2. Name\n3. Stock\n> ";
    cin >> fieldChoice;

    string key;
    if (fieldChoice == 1) key = "price";
    else if (fieldChoice == 2) key = "name";
    else key = "stock";

    switch (algoChoice) {
        case 1: bubbleSort(products, key); break;
        case 2: selectionSort(products, key); break;
        case 3: insertionSort(products, key); break;
        case 4: mergeSort(products, 0, products.size() - 1, key); break;
        default: cout << "Invalid choice.\n"; return 1;
    }

    cout << "\n\033[1;36mFinal Sorted Products:\033[0m\n";
    displayProducts(products, key);
    cout << "\n\033[1;32mSorting Completed! ✅\033[0m\n";
    return 0;
}
