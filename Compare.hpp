#pragma once
#include "Item.hpp"

class CompareItemName {
public:
    static bool lessThan(const Item& a, const Item& b);
    static bool equal(const Item& a, const Item& b);
    static bool leq(const Item& a, const Item& b);
};

class CompareItemWeight {
public:
    static bool lessThan(const Item& a, const Item& b);
    static bool equal(const Item& a, const Item& b);
    static bool leq(const Item& a, const Item& b);
};

class CompareItemType {
public:
    static bool lessThan(const Item& a, const Item& b);
    static bool equal(const Item& a, const Item& b);
    static bool leq(const Item& a, const Item& b);
};
