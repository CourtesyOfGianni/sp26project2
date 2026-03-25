#include "ItemAVL.hpp"

template <class Comparator>
const Node* ItemAVL<Comparator>::root() const { return root_; }

template <class Comparator>
int ItemAVL<Comparator>::height(Node* n) const
{
    return (n == nullptr) ? -1 : n->height_;
}

template <class Comparator>
int ItemAVL<Comparator>::size() const { return size_; }

template <class Comparator>
void ItemAVL<Comparator>::displayLevelOrder(Node* t) const
{
    if (!t) return;
    std::queue<Node*> current, next;
    current.push(t);
    while (!current.empty()) {
        while (!current.empty()) {
            Node*& front = current.front();
            if (front->left_) next.push(front->left_);
            if (front->right_) next.push(front->right_);
            std::cout << front->value_ << std::endl;
            current.pop();
        }
        std::cout << std::endl;
        current = std::move(next);
    }
}

template <class Comparator>
void ItemAVL<Comparator>::displayLevelOrder() const { displayLevelOrder(root_); }

template <class Comparator>
void ItemAVL<Comparator>::displayInOrder() const
{
    if (!root_) return;
    displayInOrder(root_);
    std::cout << std::endl;
}

template <class Comparator>
void ItemAVL<Comparator>::displayInOrder(Node* root) const
{
    if (!root) return;
    displayInOrder(root->left_);
    std::cout << root->value_ << std::endl;
    displayInOrder(root->right_);
}

template <class Comparator>
ItemAVL<Comparator>::ItemAVL()
    : root_ { nullptr }
    , size_ { 0 }
{
}

template <class Comparator>
void ItemAVL<Comparator>::deleteTree(Node*& t)
{
    if (t == nullptr) return;
    deleteTree(t->left_);
    deleteTree(t->right_);
    delete t;
    t = nullptr;
}

template <class Comparator>
ItemAVL<Comparator>::~ItemAVL() { deleteTree(root_); }

/**
 * Public wrapper: delegates to the private recursive helper starting at root_.
 */
template <class Comparator>
bool ItemAVL<Comparator>::contains(const std::string& target) const
{
    return contains(target, root_);
}

/**
 * Private helper: checks every node in the subtree for a name match.
 * We CANNOT use the ordering property here because ordering is by Comparator
 * (could be weight or type), not by name. So we do a full traversal.
 */
template <class Comparator>
bool ItemAVL<Comparator>::contains(const std::string& target, const Node* subroot) const
{
    if (subroot == nullptr) return false;
    if (subroot->value_.name_ == target) return true;
    // Check both subtrees regardless of ordering
    return contains(target, subroot->left_) || contains(target, subroot->right_);
}

template <class Comparator>
bool ItemAVL<Comparator>::insert(const Item& target)
{
    if (contains(target.name_)) return false;
    insert(target, root_);
    size_++;
    return true;
}

template <class Comparator>
void ItemAVL<Comparator>::insert(const Item& target, Node*& subroot)
{
    if (subroot == nullptr) {
        subroot = new Node({ target });
        return;
    }
    if (Comparator::lessThan(target, subroot->value_)) {
        insert(target, subroot->left_);
    } else {
        insert(target, subroot->right_);
    }
    balance(subroot);
}

template <class Comparator>
float ItemAVL<Comparator>::erase(const std::string& target)
{
    float erased_weight = erase(target, root_);
    if (erased_weight) size_--;
    return erased_weight;
}

template <class Comparator>
float ItemAVL<Comparator>::erase(const std::string& target, Node*& subroot)
{
    if (subroot == nullptr) return 0;

    if (subroot->value_.name_ != target) {
        float erased_weight = erase(target, subroot->left_);
        if (erased_weight) return erased_weight;
        return erase(target, subroot->right_);
    }

    auto findMin = [](Node* subroot) {
        while (subroot && subroot->left_) subroot = subroot->left_;
        return subroot;
    };

    if (subroot->left_ && subroot->right_) {
        std::swap(subroot->value_, findMin(subroot->right_)->value_);
        return erase(target, subroot->right_);
    }

    Node* toDelete = subroot;
    float erased_weight = subroot->value_.weight_;
    subroot = (subroot->left_) ? subroot->left_ : subroot->right_;
    delete toDelete;
    toDelete = nullptr;
    balance(subroot);
    return erased_weight;
}

template <class Comparator>
void ItemAVL<Comparator>::balance(Node*& t)
{
    if (t == nullptr) return;

    if (height(t->left_) - height(t->right_) > ALLOWED_IMBALANCE) {
        if (height(t->left_->left_) >= height(t->left_->right_))
            rotateWithLeftChild(t);
        else
            doubleWithLeftChlid(t);
    } else {
        if (height(t->right_) - height(t->left_) > ALLOWED_IMBALANCE) {
            if (height(t->right_->right_) >= height(t->right_->left_))
                rotateWithRightChild(t);
            else
                doubleWithRightChild(t);
        }
    }
    t->height_ = std::max(height(t->left_), height(t->right_)) + 1;
}

template <class Comparator>
void ItemAVL<Comparator>::rotateWithLeftChild(Node*& k2)
{
    Node* k1 = k2->left_;
    k2->left_ = k1->right_;
    k1->right_ = k2;
    k2->height_ = std::max(height(k2->left_), height(k2->right_)) + 1;
    k1->height_ = std::max(height(k1->left_), k2->height_) + 1;
    k2 = k1;
}

template <class Comparator>
void ItemAVL<Comparator>::rotateWithRightChild(Node*& k1)
{
    Node* k2 = k1->right_;
    k1->right_ = k2->left_;
    k2->left_ = k1;
    k1->height_ = 1 + std::max(height(k1->left_), height(k1->right_));
    k2->height_ = 1 + std::max(k1->height_, height(k2->right_));
    k1 = k2;
}

template <class Comparator>
void ItemAVL<Comparator>::doubleWithLeftChlid(Node*& k3)
{
    rotateWithRightChild(k3->left_);
    rotateWithLeftChild(k3);
}

template <class Comparator>
void ItemAVL<Comparator>::doubleWithRightChild(Node*& k3)
{
    rotateWithLeftChild(k3->right_);
    rotateWithRightChild(k3);
}
