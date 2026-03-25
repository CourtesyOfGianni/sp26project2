#pragma once
#include "Compare.hpp"
#include "Inventory.hpp"
#include "ItemAVL.hpp"

struct Tree { };

template <class Comparator>
class Inventory<Comparator, Tree> {
private:
    ItemAVL<Comparator> items_;
    void queryHelper(const Item& start, const Item& end, const Node* root, std::unordered_set<Item>& result) const;

protected:
    Item* equipped_;
    float weight_;

public:
    Inventory();
    Item* getEquipped() const;
    void equip(Item* itemToEquip);
    void discardEquipped();
    float getWeight() const;
    size_t size() const;
    ItemAVL<Comparator> getItems() const;
    bool pickup(const Item& target);
    bool discard(const std::string& itemName);
    bool contains(const std::string& itemName) const;
    std::unordered_set<Item> query(const Item& start, const Item& end) const;
    ~Inventory();
};

#include "TreeInventory.cpp"
