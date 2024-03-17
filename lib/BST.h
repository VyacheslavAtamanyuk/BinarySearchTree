#pragma once

#include "Nodes.h"
#include <cstdint>
#include <memory>

const uint8_t kEmptyTree = 0;
const uint8_t kOnlyOneNode = 1;

template <typename Key, typename Compare, typename Allocator>
class BST {
public:
    using node_alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node<Key>>;
    using node_alloc_traits = std::allocator_traits<node_alloc>;

    BST(const Compare& cmp, const Allocator& alloc);
    ~BST();

    void RecursiveDestructor(Node<Key>* current_node);
    void clear();

    bool IsSameTrees(Node<Key>* node_from_first_tree, Node<Key>* node_from_second_tree) const;

    BST& operator=(const BST& other);
    bool operator==(const BST& other) const;
    bool operator!=(const BST& other) const;

    Node<Key>* GetInOrderBegin() const;
    Node<Key>* GetInOrderBeforeEnd() const;

    Node<Key>* GetPreOrderBegin() const;
    Node<Key>* GetPreOrderBeforeEnd() const;

    Node<Key>* GetPostOrderBegin() const;
    Node<Key>* GetPostOrderBeforeEnd() const;

    Node<Key>* Find(Node<Key>* current_node, const Key& value) const;

    Node<Key>* Min(Node<Key>* current_node) const;
    Node<Key>* Max(Node<Key>* current_node) const;

    Node<Key>* Next(const Key& value) const;
    Node<Key>* Prev(const Key& value) const;

    void Insert(const Allocator& alloc, const Key& new_value);
    void InsertNodesFromOtherTree(const Allocator& other_alloc, Node<Key>* other_current_node);

    void DestroyAndDeallocateNode(const Allocator& alloc, Node<Key>* deletable_node);
    void Delete(const Allocator& alloc, const Key& deletable_value);
    void DeleteRoot(const Allocator& alloc);
    void DeleteNotRoot(const Allocator& alloc, Node<Key>* deletable_node);

    Node<Key>* GetRoot() const;
    size_t GetSize() const;
    size_t max_size() const;
private:
    Node<Key>* root;
    size_t size;

    Compare cmp_;
    Allocator alloc_;
};

template <typename Key, typename Compare, typename Allocator>
BST<Key, Compare, Allocator>::BST(const Compare& cmp, const Allocator& alloc): root(nullptr), size(kEmptyTree), cmp_(cmp), alloc_(alloc) {}

template <typename Key, typename Compare, typename Allocator>
BST<Key, Compare, Allocator>::~BST() {
    clear();
}

template <typename Key, typename Compare, typename Allocator>
void BST<Key, Compare, Allocator>::RecursiveDestructor(Node<Key>* current_node) {
    if (current_node) {
        RecursiveDestructor(current_node->GetLeft());
        RecursiveDestructor(current_node->GetRight());
        DestroyAndDeallocateNode(alloc_, current_node);
    }
}

template <typename Key, typename Compare, typename Allocator>
void BST<Key, Compare, Allocator>::clear() {
    RecursiveDestructor(root);
    root = nullptr;
    size = kEmptyTree;
}

template <typename Key, typename Compare, typename Allocator>
bool BST<Key, Compare, Allocator>::IsSameTrees(Node<Key>* node_from_first_tree, Node<Key>* node_from_second_tree) const {
    if (!node_from_first_tree && !node_from_second_tree) {
        return true;
    } else if (!node_from_first_tree || !node_from_second_tree) {
        return false;
    } else if (node_from_first_tree->GetKey() == node_from_second_tree->GetKey() && IsSameTrees(node_from_first_tree->GetLeft(), node_from_second_tree->GetLeft())
                                                                                 && IsSameTrees(node_from_first_tree->GetRight(), node_from_second_tree->GetRight())) {
        return true;
    } else {
        return false;
    }
}


template <typename Key, typename Compare, typename Allocator>
BST<Key, Compare, Allocator>& BST<Key, Compare, Allocator>::operator=(const BST& other) {
    if (this != &other) {
        Allocator new_alloc = std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value ? (other.alloc_): alloc_;

        clear();
        InsertNodesFromOtherTree(other.alloc_, other.GetRoot());

        cmp_ = other.cmp_;
        alloc_ = new_alloc;
        size = other.size;
    }

    return *this;
}

template <typename Key, typename Compare, typename Allocator>
bool BST<Key, Compare, Allocator>::operator==(const BST& other) const {
    if (!size && !other.size) {
        return true;
    } else if (!size || !other.size) {
        return false;
    } else if (root->GetKey() == other.root->GetKey() && IsSameTrees(root->GetLeft(), other.root->GetLeft()) && IsSameTrees(root->GetRight(), other.root->GetRight())) {
        return true;
    } else {
        return false;
    }
}

template <typename Key, typename Compare, typename Allocator>
bool BST<Key, Compare, Allocator>::operator!=(const BST& other) const {
    return !(*this == other);
}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::GetInOrderBegin() const { return Min(root);}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::GetInOrderBeforeEnd() const { return Max(root);}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::GetPreOrderBegin() const { return root;}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::GetPreOrderBeforeEnd() const {
    Node<Key>* last_node = root;
    while (last_node->GetLeft() || last_node->GetRight()) {
        if (last_node->GetRight()) {
            last_node = last_node->GetRight();
        } else {
            last_node = last_node->GetLeft();
        }
    }
    return last_node;
}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::GetPostOrderBegin() const {
    Node<Key>* first_node = root;
    while (first_node->GetLeft() || first_node->GetRight()) {
        if (first_node->GetLeft()) {
            first_node = first_node->GetLeft();
        } else {
            first_node = first_node->GetRight();
        }
    }
    return first_node;
}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::GetPostOrderBeforeEnd() const { return root;}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::Find(Node<Key>* current_node, const Key& value) const{
    if (!current_node || (!cmp_(current_node->GetKey(), value) && !cmp_(value, current_node->GetKey()))) {
        return current_node;
    }
    if (cmp_(value, current_node->GetKey())) {
        return Find(current_node->GetLeft(), value);
    } else {
        return Find(current_node->GetRight(), value);
    }
}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::Min(Node<Key>* current_node) const {
    if (!current_node->GetLeft()) {
        return current_node;
    }
    return Min(current_node->GetLeft());
}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::Max(Node<Key>* current_node) const {
    if (!current_node->GetRight()) {
        return current_node;
    }
    return Max(current_node->GetRight());
}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::Next(const Key& value) const{
    Node<Key>* current_node = root;
    Node<Key>* lowest_upper_bound = nullptr;
    while (current_node) {
        if (cmp_(value, current_node->GetKey())) {
            lowest_upper_bound = current_node;
            current_node = current_node->GetLeft();
        } else {
            current_node = current_node->GetRight();
        }
    }
    return lowest_upper_bound;
}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::Prev(const Key& value) const{
    Node<Key>* current_node = root;
    Node<Key>* greatest_lower_bound = nullptr;
    while (current_node) {
        if (cmp_(current_node->GetKey(), value)) {
            greatest_lower_bound = current_node;
            current_node = current_node->GetRight();
        } else {
            current_node = current_node->GetLeft();
        }
    }
    return greatest_lower_bound;
}

template <typename Key, typename Compare, typename Allocator>
void BST<Key, Compare, Allocator>::Insert(const Allocator& alloc, const Key& value) {
    if (!size) {
        node_alloc node_alloc(alloc);
        root = node_alloc_traits::allocate(node_alloc, kOnlyOneNode);
        root->SetNodePtr(nullptr, nullptr, nullptr, value);
        ++size;
    } else if (!Find(root, value)) {
        node_alloc node_alloc(alloc);
        Node<Key>* new_node = node_alloc_traits::allocate(node_alloc, kOnlyOneNode);

        Node<Key>* current_node = root;
        while (current_node) {
            if (cmp_(current_node->GetKey(), value)) {
                if (current_node->GetRight()) {
                    current_node = current_node->GetRight();
                } else {
                    new_node->SetNodePtr(nullptr, nullptr, current_node, value);
                    current_node->SetNodePtr(current_node->GetLeft(), new_node, current_node->GetParent(), current_node->GetKey());
                    break;
                }
            } else {
                if (current_node->GetLeft()) {
                    current_node = current_node->GetLeft();
                } else {
                    new_node->SetNodePtr(nullptr, nullptr, current_node, value);
                    current_node->SetNodePtr(new_node, current_node->GetRight(), current_node->GetParent(), current_node->GetKey());
                    break;
                }
            }
        }
        ++size;
    }
}

template <typename Key, typename Compare, typename Allocator>
void BST<Key, Compare, Allocator>::InsertNodesFromOtherTree(const Allocator& other_allocator, Node<Key>* other_current_node) {
    if (other_current_node) {
        Insert(other_allocator, other_current_node->GetKey());
        InsertNodesFromOtherTree(other_allocator, other_current_node->GetLeft());
        InsertNodesFromOtherTree(other_allocator, other_current_node->GetRight());
    }
}

template <typename Key, typename Compare, typename Allocator>
void BST<Key, Compare, Allocator>::DestroyAndDeallocateNode(const Allocator& alloc, Node<Key>* deletable_node) {
    node_alloc node_alloc(alloc);
    node_alloc_traits::destroy(node_alloc, deletable_node);
    node_alloc_traits::deallocate(node_alloc, deletable_node, kOnlyOneNode);
}

template <typename Key, typename Compare, typename Allocator>
void BST<Key, Compare, Allocator>::Delete(const Allocator& alloc, const Key& value) {
    if (root) {
        Node<Key>* check_exist = Find(root, value);
        if (check_exist == root) {
            DeleteRoot(alloc);
            --size;
        } else if (check_exist) {
            DeleteNotRoot(alloc, check_exist);
            --size;
        }
    }
}

template <typename Key, typename Compare, typename Allocator>
void BST<Key, Compare, Allocator>::DeleteRoot(const Allocator& alloc) {
    if (!root->GetLeft() && !root->GetRight()) {
        DestroyAndDeallocateNode(alloc, root);
        root = nullptr;
    } else if (!root->GetLeft() || !root->GetRight()) {
        if (!root->GetLeft()) {
            root->GetRight()->SetNodePtr(root->GetRight()->GetLeft(), root->GetRight()->GetRight(), nullptr, root->GetRight()->GetKey());
            Node<Key>* copy_of_right_son = root->GetRight();
            DestroyAndDeallocateNode(alloc, root);
            root = copy_of_right_son;
        } else {
            root->GetLeft()->SetNodePtr(root->GetLeft()->GetLeft(), root->GetLeft()->GetRight(), nullptr, root->GetLeft()->GetKey());
            Node<Key>* copy_of_left_son = root->GetLeft();
            DestroyAndDeallocateNode(alloc, root);
            root = copy_of_left_son;
        }
    } else {
        Node<Key>* next_after_root = Next(root->GetKey());
        root->SetNodePtr(root->GetLeft(), root->GetRight(), nullptr, next_after_root->GetKey());
        DeleteNotRoot(alloc, next_after_root);
    }
}

template <typename Key, typename Compare, typename Allocator>
void BST<Key, Compare, Allocator>::DeleteNotRoot(const Allocator& alloc, Node<Key>* deletable_node) {
    Node<Key>* parent_of_deletable_node = deletable_node->GetParent();
    if (!deletable_node->GetLeft() && !deletable_node->GetRight()) {
        if (parent_of_deletable_node->GetLeft() == deletable_node) {
            parent_of_deletable_node->SetNodePtr(nullptr, parent_of_deletable_node->GetRight(), parent_of_deletable_node->GetParent(), parent_of_deletable_node->GetKey());
        }
        if (parent_of_deletable_node->GetRight() == deletable_node) {
            parent_of_deletable_node->SetNodePtr(parent_of_deletable_node->GetLeft(), nullptr, parent_of_deletable_node->GetParent(), parent_of_deletable_node->GetKey());
        }
        DestroyAndDeallocateNode(alloc, deletable_node);
    } else if (!deletable_node->GetLeft() || !deletable_node->GetRight()) {
        if (!deletable_node->GetLeft()) {
            if (parent_of_deletable_node->GetLeft() == deletable_node) {
                parent_of_deletable_node->SetNodePtr(deletable_node->GetRight(), parent_of_deletable_node->GetRight(),
                                                     parent_of_deletable_node->GetParent(), parent_of_deletable_node->GetKey());
            } else {
                parent_of_deletable_node->SetNodePtr(parent_of_deletable_node->GetLeft(), deletable_node->GetRight(),
                                                     parent_of_deletable_node->GetParent(), parent_of_deletable_node->GetKey());
            }
            deletable_node->GetRight()->SetNodePtr(deletable_node->GetRight()->GetLeft(), deletable_node->GetRight()->GetRight(),
                                                   parent_of_deletable_node, deletable_node->GetRight()->GetKey());
        }
        if (!deletable_node->GetRight()) {
            if (parent_of_deletable_node->GetLeft() == deletable_node) {
                parent_of_deletable_node->SetNodePtr(deletable_node->GetLeft(), parent_of_deletable_node->GetRight(),
                                                     parent_of_deletable_node->GetParent(), parent_of_deletable_node->GetKey());
            } else {
                parent_of_deletable_node->SetNodePtr(parent_of_deletable_node->GetLeft(), deletable_node->GetLeft(),
                                                     parent_of_deletable_node->GetParent(), parent_of_deletable_node->GetKey());
            }
            deletable_node->GetLeft()->SetNodePtr(deletable_node->GetLeft()->GetLeft(), deletable_node->GetLeft()->GetRight(),
                                                   parent_of_deletable_node, deletable_node->GetLeft()->GetKey());
        }
        DestroyAndDeallocateNode(alloc, deletable_node);
    } else {
        Node<Key>* next_node_after_deletable = Next(deletable_node->GetKey());
        deletable_node->SetNodePtr(deletable_node->GetLeft(), deletable_node->GetRight(), deletable_node->GetParent(), next_node_after_deletable->GetKey());
        DeleteNotRoot(alloc, next_node_after_deletable);
    }
}

template <typename Key, typename Compare, typename Allocator>
Node<Key>* BST<Key, Compare, Allocator>::GetRoot() const { return root;}

template <typename Key, typename Compare, typename Allocator>
size_t BST<Key, Compare, Allocator>::GetSize() const { return size;}

template <typename Key, typename Compare, typename Allocator>
size_t BST<Key, Compare, Allocator>::max_size() const {
    node_alloc nodeAlloc(alloc_);
    return node_alloc_traits::max_size(nodeAlloc);
}
