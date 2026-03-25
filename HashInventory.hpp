#pragma once
#include "Inventory.hpp"
#include <unordered_set>

template <class Comparator>
class Inventory<Comparator, std::unordered_set<Item>> {
private:
    std::unordered_set<Item> items_;

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
    std::unordered_set<Item> getItems() const;
    bool pickup(const Item& target);
    bool discard(const std::string& itemName);
    bool contains(const std::string& itemName) const;
    std::unordered_set<Item> query(const Item& start, const Item& end) const;
    ~Inventory();
};

#include "HashInventory.cpp"
