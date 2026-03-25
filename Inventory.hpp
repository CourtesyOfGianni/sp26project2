#pragma once
#include "Compare.hpp"
#include "Item.hpp"
#include <algorithm>
#include <type_traits>
#include <unordered_set>
#include <vector>

template <class Comparator, class Container = std::vector<Item>>
class Inventory {
private:
    Container items_;

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
    Container getItems() const;
    bool pickup(const Item& target);
    bool discard(const std::string& itemName);
    bool contains(const std::string& itemName) const;
    std::unordered_set<Item> query(const Item& start, const Item& end) const;
    ~Inventory();
};

#include "Inventory.cpp"
