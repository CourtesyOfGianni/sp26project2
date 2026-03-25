#pragma once
#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <unordered_set>
#include <vector>

#include "Compare.hpp"
#include "Item.hpp"

struct Node {
    Item value_;
    int height_;
    Node* left_;
    Node* right_;

    Node(Item i, Node* lt = nullptr, Node* rt = nullptr)
        : value_ { i }
        , height_ { 0 }
        , left_ { lt }
        , right_ { rt }
    {
    }
};

template <class Comparator = CompareItemName>
class ItemAVL {
public:
    ItemAVL();
    ~ItemAVL();

    const Node* root() const;
    bool insert(const Item& target);
    float erase(const std::string& name);
    bool contains(const std::string& name) const;
    int height(Node* n) const;
    void displayLevelOrder() const;
    void displayInOrder() const;
    int size() const;

private:
    static const int ALLOWED_IMBALANCE = 1;
    Node* root_;
    int size_;

    bool contains(const std::string& name, const Node* subroot) const;
    void insert(const Item& target, Node*& subroot);
    float erase(const std::string& itemName, Node*& subroot);
    void balance(Node*& t);
    void displayLevelOrder(Node* t) const;
    void displayInOrder(Node* t) const;
    void rotateWithLeftChild(Node*& k2);
    void rotateWithRightChild(Node*& k1);
    void doubleWithLeftChlid(Node*& k3);
    void doubleWithRightChild(Node*& k3);
    void deleteTree(Node*& t);
};

#include "ItemAVL.cpp"
