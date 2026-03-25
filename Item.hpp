#pragma once
#include <iostream>
#include <string>

enum ItemType {
    NONE = 0,
    WEAPON = 1,
    ACCESSORY = 2,
    ARMOR = 3
};

struct Item {
    std::string name_;
    float weight_;
    ItemType type_;

    Item(const std::string& name = "", const float& weight = 0, const ItemType& type = NONE);
    bool operator==(const Item& rhs) const;
    friend std::ostream& operator<<(std::ostream& os, const Item& item);
};

template <>
struct std::hash<Item> {
    size_t operator()(const Item& i) const;
};
