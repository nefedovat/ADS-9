// Copyright 2022 NNTU-CS
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include "tree.h"

void printPermutations(const std::vector<std::vector<char>>& perms) {
  for (size_t i = 0; i < perms.size(); ++i) {
    std::cout << i + 1 << ": ";
    for (char c : perms[i]) {
      std::cout << c;
    }
    std::cout << std::endl;
  }
}

void demonstratePermutations() {
  std::cout << "=== Demonstration of the work ===" << std::endl;

  std::vector<char> elements = {'1', '2', '3'};
  PMTree tree(elements);

  std::cout << "\nAll permutations for {'1','2','3'}:" << std::endl;
  auto all_perms = getAllPerms(tree);
  printPermutations(all_perms);

  std::cout << "\nGetting individual permutations:" << std::endl;
  for (int i = 1; i <= 6; ++i) {
    auto perm1 = getPerm1(tree, i);
    auto perm2 = getPerm2(tree, i);

    std::cout << "The permutation " << i << ": ";
    for (char c : perm1) std::cout << c;
    std::cout << " (getPerm1), ";
    for (char c : perm2) std::cout << c;
    std::cout << " (getPerm2)" << std::endl;
  }
}

void runPerformanceTest() {
  std::cout << "n\tAllPerms(us)\tPerm1(us)\tPerm2(us)\tPermsCount" << std::endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  const int tests_count = 100;
  const int max_n = 8;

  for (int n = 1; n <= max_n; ++n) {
    std::vector<char> elements(n);
    std::iota(elements.begin(), elements.end(), 'A');

    PMTree tree(elements);
    int total_perms = tree.getAllPerms().size();

    std::vector<int> test_nums;
    if (total_perms > 0) {
      if (total_perms <= tests_count) {
        test_nums.resize(total_perms);
        std::iota(test_nums.begin(), test_nums.end(), 1);
        std::shuffle(test_nums.begin(), test_nums.end(), gen);
      } else {
        std::uniform_int_distribution<> dist(1, total_perms);
        for (int i = 0; i < tests_count; ++i) {
          test_nums.push_back(dist(gen));
        }
      }
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto perms = getAllPerms(tree);
    auto end = std::chrono::high_resolution_clock::now();
    auto all_time = std::chrono::duration_cast<std::chrono::microseconds>(
        end - start).count();

    double perm1_time = 0, perm2_time = 0;
    if (!test_nums.empty()) {
      start = std::chrono::high_resolution_clock::now();
      for (int num : test_nums) {
        auto perm = getPerm1(tree, num);
      }
      end = std::chrono::high_resolution_clock::now();
      perm1_time = static_cast<double>(
          std::chrono::duration_cast<std::chrono::microseconds>(
              end - start).count()) / test_nums.size();

      start = std::chrono::high_resolution_clock::now();
      for (int num : test_nums) {
        auto perm = getPerm2(tree, num);
      }
      end = std::chrono::high_resolution_clock::now();
      perm2_time = static_cast<double>(
          std::chrono::duration_cast<std::chrono::microseconds>(
              end - start).count()) / test_nums.size();
    }

    std::cout << n << "\t"
              << all_time << "\t\t"
              << perm1_time << "\t\t"
              << perm2_time << "\t\t"
              << total_perms << std::endl;
  }
}

int main() {
  demonstratePermutations();
  runPerformanceTest();
  return 0;
}
