#include "HashInventory.hpp"

template <class Comparator>
Inventory<Comparator, std::unordered_set<Item>>::Inventory()
    : items_ {}
    , equipped_ { nullptr }
    , weight_ { 0.0 }
{
}

template <class Comparator>
Item* Inventory<Comparator, std::unordered_set<Item>>::getEquipped() const
{
    return equipped_;
}

template <class Comparator>
void Inventory<Comparator, std::unordered_set<Item>>::equip(Item* itemToEquip)
{
    equipped_ = itemToEquip;
}

template <class Comparator>
void Inventory<Comparator, std::unordered_set<Item>>::discardEquipped()
{
    if (!equipped_) return;
    delete equipped_;
    equipped_ = nullptr;
}

template <class Comparator>
float Inventory<Comparator, std::unordered_set<Item>>::getWeight() const
{
    return weight_;
}

template <class Comparator>
size_t Inventory<Comparator, std::unordered_set<Item>>::size() const
{
    return items_.size();
}

template <class Comparator>
std::unordered_set<Item> Inventory<Comparator, std::unordered_set<Item>>::getItems() const
{
    return items_;
}

template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::pickup(const Item& target)
{
    // unordered_set uses Item's hash (name-based) and operator==
    // insert returns a pair; second is true if inserted, false if duplicate
    auto [it, inserted] = items_.insert(target);
    if (inserted) {
        weight_ += target.weight_;
        return true;
    }
    return false;
}

template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::discard(const std::string& itemName)
{
    // We can't look up by name directly since the hash is name-based,
    // but we need a dummy Item to search with
    Item dummy(itemName, 0, NONE);
    auto it = items_.find(dummy);
    if (it == items_.end()) return false;
    weight_ -= it->weight_;
    items_.erase(it);
    return true;
}

template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::contains(const std::string& itemName) const
{
    Item dummy(itemName, 0, NONE);
    return items_.count(dummy) > 0;
}

template <class Comparator>
std::unordered_set<Item> Inventory<Comparator, std::unordered_set<Item>>::query(const Item& start, const Item& end) const
{
    std::unordered_set<Item> result;
    if (Comparator::lessThan(end, start)) return result;

    for (const Item& item : items_) {
        if (Comparator::leq(start, item) && Comparator::leq(item, end)) {
            result.insert(item);
        }
    }
    return result;
}

template <class Comparator>
Inventory<Comparator, std::unordered_set<Item>>::~Inventory()
{
    discardEquipped();
}
