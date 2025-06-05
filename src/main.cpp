// Copyright 2022 NNTU-CS
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>  
#include <random>   
#include <chrono> 
#include <stack>
#include "tree.h"

using namespace std;
using namespace chrono;

void printPermutations(const vector<vector<char>>& perms) {
    for (size_t i = 0; i < perms.size(); ++i) {
        cout << i + 1 << ": ";
        for (char c : perms[i]) {
            cout << c;
        }
        cout << endl;
    }
}

void demonstratePermutations() {
    cout << "=== Demonstration of the work ===" << endl;

    // Пример с 3 элементами
    vector<char> elements = { '1', '2', '3' };
    PMTree tree(elements);

    cout << "\nAll permutations for {'1','2','3'}:" << endl;
    auto all_perms = getAllPerms(tree);
    printPermutations(all_perms);

    cout << "\nGetting individual permutations:" << endl;
    for (int i = 1; i <= 6; ++i) {
        auto perm1 = getPerm1(tree, i);
        auto perm2 = getPerm2(tree, i);

        cout << "The permutation " << i << ": ";
        for (char c : perm1) cout << c;
        cout << " (getPerm1), ";
        for (char c : perm2) cout << c;
        cout << " (getPerm2)" << endl;
    }
}

void runPerformanceTest() {
    cout << "n\tAllPerms(us)\tPerm1(us)\tPerm2(us)\tPermsCount" << endl;

    random_device rd;
    mt19937 gen(rd());
    const int tests_count = 100;
    const int max_n = 8;

    for (int n = 1; n <= max_n; ++n) {
        vector<char> elements(n);
        iota(elements.begin(), elements.end(), 'A');

        PMTree tree(elements);
        int total_perms = tree.getAllPerms().size();

        // Подготовка тестовых номеров
        vector<int> test_nums;
        if (total_perms > 0) {
            if (total_perms <= tests_count) {
                test_nums.resize(total_perms);
                iota(test_nums.begin(), test_nums.end(), 1);
                shuffle(test_nums.begin(), test_nums.end(), gen);
            }
            else {
                uniform_int_distribution<> dist(1, total_perms);
                for (int i = 0; i < tests_count; ++i) {
                    test_nums.push_back(dist(gen));
                }
            }
        }

        // Замер getAllPerms
        auto start = high_resolution_clock::now();
        auto perms = getAllPerms(tree); // Прогрев
        auto end = high_resolution_clock::now();
        auto all_time = duration_cast<microseconds>(end - start).count();

        // Замер getPerm1 и getPerm2
        double perm1_time = 0, perm2_time = 0;
        if (!test_nums.empty()) {
            // getPerm1
            start = high_resolution_clock::now();
            for (int num : test_nums) {
                auto perm = getPerm1(tree, num);
            }
            end = high_resolution_clock::now();
            perm1_time = duration_cast<microseconds>(end - start).count() / double(test_nums.size());

            // getPerm2
            start = high_resolution_clock::now();
            for (int num : test_nums) {
                auto perm = getPerm2(tree, num);
            }
            end = high_resolution_clock::now();
            perm2_time = duration_cast<microseconds>(end - start).count() / double(test_nums.size());
        }

        cout << n << "\t"
            << all_time << "\t\t"
            << perm1_time << "\t\t"
            << perm2_time << "\t\t"
            << total_perms << endl;
    }
}

int main() {
    demonstratePermutations();
    runPerformanceTest();
    return 0;
}
