#include "Inventory.hpp"

template <class Comparator, class Container>
Inventory<Comparator, Container>::Inventory()
    : items_ {}
    , equipped_ { nullptr }
    , weight_ { 0.0 }
{
}

template <class Comparator, class Container>
Item* Inventory<Comparator, Container>::getEquipped() const
{
    return equipped_;
}

template <class Comparator, class Container>
void Inventory<Comparator, Container>::equip(Item* itemToEquip)
{
    equipped_ = itemToEquip;
}

template <class Comparator, class Container>
void Inventory<Comparator, Container>::discardEquipped()
{
    if (!equipped_) return;
    delete equipped_;
    equipped_ = nullptr;
}

template <class Comparator, class Container>
float Inventory<Comparator, Container>::getWeight() const
{
    return weight_;
}

template <class Comparator, class Container>
size_t Inventory<Comparator, Container>::size() const
{
    return items_.size();
}

template <class Comparator, class Container>
Container Inventory<Comparator, Container>::getItems() const
{
    return items_;
}

template <class Comparator, class Container>
bool Inventory<Comparator, Container>::pickup(const Item& target)
{
    // Check for duplicate name
    for (const Item& item : items_) {
        if (item.name_ == target.name_) return false;
    }
    items_.push_back(target);
    weight_ += target.weight_;
    return true;
}

template <class Comparator, class Container>
bool Inventory<Comparator, Container>::discard(const std::string& itemName)
{
    for (auto it = items_.begin(); it != items_.end(); ++it) {
        if (it->name_ == itemName) {
            weight_ -= it->weight_;
            items_.erase(it);
            return true;
        }
    }
    return false;
}

template <class Comparator, class Container>
bool Inventory<Comparator, Container>::contains(const std::string& itemName) const
{
    for (const Item& item : items_) {
        if (item.name_ == itemName) return true;
    }
    return false;
}

template <class Comparator, class Container>
std::unordered_set<Item> Inventory<Comparator, Container>::query(const Item& start, const Item& end) const
{
    std::unordered_set<Item> result;
    // If end < start, return empty
    if (Comparator::lessThan(end, start)) return result;

    for (const Item& item : items_) {
        if (Comparator::leq(start, item) && Comparator::leq(item, end)) {
            result.insert(item);
        }
    }
    return result;
}

template <class Comparator, class Container>
Inventory<Comparator, Container>::~Inventory()
{
    discardEquipped();
}
