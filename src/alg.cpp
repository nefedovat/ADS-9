// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include "tree.h"
#include <algorithm>
#include <memory>

using namespace std;

// Конструктор дерева
PMTree::PMTree(const vector<char>& elements) {
    if (elements.empty()) {
        root = nullptr;
        total_permutations = 0;
        return;
    }

    // Вычисляем общее количество перестановок (n!)
    total_permutations = 1;
    for (size_t i = 2; i <= elements.size(); ++i) {
        total_permutations *= i;
    }

    root = make_unique<Node>('\0'); // Создаем корневой узел

    // Строим дерево для каждого элемента
    for (char elem : elements) {
        auto child = make_unique<Node>(elem);
        vector<char> remaining = elements;
        remaining.erase(remove(remaining.begin(), remaining.end(), elem), remaining.end());
        buildTree(child.get(), remaining);
        root->children.push_back(move(child));
    }
}

// Рекурсивное построение дерева
void PMTree::buildTree(Node* node, const vector<char>& remaining) {
    if (remaining.empty()) {
        return;
    }

    for (char elem : remaining) {
        auto child = make_unique<Node>(elem);
        vector<char> new_remaining = remaining;
        new_remaining.erase(remove(new_remaining.begin(), new_remaining.end(), elem), new_remaining.end());
        buildTree(child.get(), new_remaining);
        node->children.push_back(move(child));
    }
}

vector<vector<char>> PMTree::getAllPerms() const {
    vector<vector<char>> result;
    if (!root) return result;
    
    vector<char> current;
    for (const auto& child : root->children) {
        getAllPermsHelper(child.get(), current, result);
    }
    // Сортируем результат для соответствия тестам
    sort(result.begin(), result.end());
    return result;
}

// Вспомогательная функция для получения всех перестановок
void PMTree::getAllPermsHelper(const Node* node, vector<char>& current, vector<vector<char>>& result) const {
    current.push_back(node->value);

    if (node->children.empty()) {
        result.push_back(current);
    }
    else {
        for (const auto& child : node->children) {
            getAllPermsHelper(child.get(), current, result);
        }
    }

    current.pop_back();
}

vector<char> PMTree::getPerm1(int num) const {
    auto all = getAllPerms();
    if (num < 1 || num > static_cast<int>(all.size())) {
        return {};
    }
    return all[num-1];
}



// Вспомогательная функция для getPerm1
void PMTree::getPerm1Helper(const Node* node, int& remaining, vector<char>& result) const {
    result.push_back(node->value);
    remaining--;

    if (remaining == 0) {
        return;
    }

    for (const auto& child : node->children) {
        int before = remaining;
        getPerm1Helper(child.get(), remaining, result);
        if (remaining == 0) {
            return;
        }
    }

    result.pop_back();
}

vector<char> PMTree::getPerm2(int num) const {
    if (num < 1 || num > total_permutations || !root) {
        return {};
    }
    
    vector<char> result;
    vector<char> elements;
    for (const auto& child : root->children) {
        elements.push_back(child->value);
    }
    
    num--; // Переводим в 0-based индекс
    
    while (!elements.empty()) {
        int fact = factorial(elements.size() - 1);
        int index = num / fact;
        result.push_back(elements[index]);
        elements.erase(elements.begin() + index);
        num %= fact;
    }
    
    return result;
}

// Вспомогательная функция для getPerm2
bool PMTree::getPerm2Helper(const Node* node, int remaining, vector<char>& result, int& subtree_size) const {
    result.push_back(node->value);

    if (node->children.empty()) {
        subtree_size = 1;
        return subtree_size == remaining;
    }

    int total = 0;
    for (const auto& child : node->children) {
        int child_size = 0;
        if (getPerm2Helper(child.get(), remaining - total, result, child_size)) {
            subtree_size += child_size;
            return true;
        }
        total += child_size;
    }

    subtree_size = total;
    result.pop_back();
    return false;
}

// Внешние функции-обертки
vector<vector<char>> getAllPerms(const PMTree& tree) {
    return tree.getAllPerms();
}

vector<char> getPerm1(const PMTree& tree, int num) {
    return tree.getPerm1(num);
}

vector<char> getPerm2(const PMTree& tree, int num) {
    return tree.getPerm2(num);
}

