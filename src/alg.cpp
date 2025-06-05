// Copyright 2022 NNTU-CS
#include <algorithm>
#include <memory>
#include <utility>
#include <vector>
#include "tree.h"

int PMTree::factorial(int n) const {
  int result = 1;
  for (int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

PMTree::PMTree(const std::vector<char>& elements) {
  if (elements.empty()) {
    root = nullptr;
    total_permutations = 0;
    return;
  }

  total_permutations = 1;
  for (size_t i = 2; i <= elements.size(); ++i) {
    total_permutations *= i;
  }

  root = std::make_unique<Node>('\0');

  for (char elem : elements) {
    auto child = std::make_unique<Node>(elem);
    std::vector<char> remaining = elements;
    remaining.erase(std::remove(remaining.begin(), remaining.end(), elem),
                   remaining.end());
    buildTree(child.get(), remaining);
    root->children.push_back(std::move(child));
  }
}

void PMTree::buildTree(Node* node, const std::vector<char>& remaining) {
  if (remaining.empty()) {
    return;
  }

  for (char elem : remaining) {
    auto child = std::make_unique<Node>(elem);
    std::vector<char> new_remaining = remaining;
    new_remaining.erase(std::remove(new_remaining.begin(), new_remaining.end(),
                                  elem),
                       new_remaining.end());
    buildTree(child.get(), new_remaining);
    node->children.push_back(std::move(child));
  }
}

std::vector<std::vector<char>> PMTree::getAllPerms() const {
  std::vector<std::vector<char>> result;
  if (!root) return result;

  std::vector<char> current;
  for (const auto& child : root->children) {
    getAllPermsHelper(child.get(), current, result);
  }
  std::sort(result.begin(), result.end());
  return result;
}

void PMTree::getAllPermsHelper(const Node* node, std::vector<char>& current,
                              std::vector<std::vector<char>>& result) const {
  current.push_back(node->value);

  if (node->children.empty()) {
    result.push_back(current);
  } else {
    for (const auto& child : node->children) {
      getAllPermsHelper(child.get(), current, result);
    }
  }

  current.pop_back();
}

std::vector<char> PMTree::getPerm1(int num) const {
  auto all = getAllPerms();
  if (num < 1 || num > static_cast<int>(all.size())) {
    return {};
  }
  return all[num-1];
}

std::vector<char> PMTree::getPerm2(int num) const {
  if (num < 1 || num > total_permutations || !root) {
    return {};
  }

  std::vector<char> result;
  std::vector<char> elements;
  for (const auto& child : root->children) {
    elements.push_back(child->value);
  }

  num--;

  while (!elements.empty()) {
    int fact = factorial(elements.size() - 1);
    int index = num / fact;
    result.push_back(elements[index]);
    elements.erase(elements.begin() + index);
    num %= fact;
  }

  return result;
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
  return tree.getAllPerms();
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
  return tree.getPerm1(num);
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
  return tree.getPerm2(num);
}
