#include "TreeInventory.hpp"

template <class Comparator>
Inventory<Comparator, Tree>::Inventory()
    : items_ { ItemAVL<Comparator>() }
    , equipped_ { nullptr }
    , weight_ { 0.0 }
{
}

template <class Comparator>
Item* Inventory<Comparator, Tree>::getEquipped() const { return equipped_; }

template <class Comparator>
void Inventory<Comparator, Tree>::equip(Item* itemToEquip) { equipped_ = itemToEquip; }

template <class Comparator>
void Inventory<Comparator, Tree>::discardEquipped()
{
    if (!equipped_) return;
    delete equipped_;
    equipped_ = nullptr;
}

template <class Comparator>
float Inventory<Comparator, Tree>::getWeight() const { return weight_; }

template <class Comparator>
size_t Inventory<Comparator, Tree>::size() const { return items_.size(); }

template <class Comparator>
ItemAVL<Comparator> Inventory<Comparator, Tree>::getItems() const { return items_; }

template <class Comparator>
bool Inventory<Comparator, Tree>::pickup(const Item& target)
{
    if (items_.insert(target)) {
        weight_ += target.weight_;
        return true;
    }
    return false;
}

template <class Comparator>
bool Inventory<Comparator, Tree>::discard(const std::string& itemName)
{
    float erased_weight = items_.erase(itemName);
    if (erased_weight) {
        weight_ -= erased_weight;
        return true;
    }
    return false;
}

template <class Comparator>
bool Inventory<Comparator, Tree>::contains(const std::string& itemName) const
{
    return items_.contains(itemName);
}

/**
 * Public query: validates range, then kicks off the recursive helper.
 */
template <class Comparator>
std::unordered_set<Item> Inventory<Comparator, Tree>::query(const Item& start, const Item& end) const
{
    std::unordered_set<Item> result;
    if (Comparator::lessThan(end, start)) return result;
    queryHelper(start, end, items_.root(), result);
    return result;
}

/**
 * queryHelper: efficient BST range search using the Comparator ordering.
 *
 * Key insight: the AVL tree is ordered by the Comparator property.
 * - If current node is BELOW the range (node < start), everything in the
 *   LEFT subtree is also below the range, so we only recurse RIGHT.
 * - If current node is ABOVE the range (node > end), everything in the
 *   RIGHT subtree is also above the range, so we only recurse LEFT.
 * - If the current node is IN the range, add it and recurse BOTH sides.
 *
 * This prunes entire subtrees and gives us better than O(n) on average.
 */
template <class Comparator>
void Inventory<Comparator, Tree>::queryHelper(const Item& start, const Item& end,
    const Node* root, std::unordered_set<Item>& result) const
{
    if (root == nullptr) return;

    // Current node is below the start of the range — go right only
    if (Comparator::lessThan(root->value_, start)) {
        queryHelper(start, end, root->right_, result);
    }
    // Current node is above the end of the range — go left only
    else if (Comparator::lessThan(end, root->value_)) {
        queryHelper(start, end, root->left_, result);
    }
    // Current node is within range — add it and search both subtrees
    else {
        result.insert(root->value_);
        queryHelper(start, end, root->left_, result);
        queryHelper(start, end, root->right_, result);
    }
}

template <class Comparator>
Inventory<Comparator, Tree>::~Inventory() { discardEquipped(); }
