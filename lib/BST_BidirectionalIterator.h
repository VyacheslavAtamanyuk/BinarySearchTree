#pragma once

#include "BST.h"

struct InOrder{};
struct PostOrder{};
struct PreOrder{};

template <typename Key, typename Travel, typename Compare, typename Allocator>
class BST_Container;

template <typename Container , bool is_reversed = false>
class BST_Iterator {
public:
    friend class BST_Container<typename Container::key_type, typename Container::travel_type, typename Container::key_compare, typename Container::allocator_type>;

    using tree = typename Container::tree;
    using tree_ptr = tree*;

    using travel_type = typename Container::travel_type;

    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = ptrdiff_t;

    using value_type = typename Container::key_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using node_type = Node<value_type>;
    using node_type_ptr = Node<value_type>*;

    BST_Iterator(node_type_ptr other_ptr, tree_ptr tree): ptr(other_ptr), tree_(tree) {}

    const_reference operator*() const { return ptr->GetKey();}

    BST_Iterator<Container, is_reversed>& IncrementInOrder() {
        if (is_reversed && !ptr) {
            ptr = tree_->GetInOrderBegin();
            return *this;
        }

        if (ptr->GetRight()) {
            ptr = tree_->Min(ptr->GetRight());
        } else {
            node_type_ptr parent = ptr->GetParent();
            while (parent && parent->GetRight() == ptr) {
                ptr = parent;
                parent = parent->GetParent();
            }
            ptr = parent;
        }
        return *this;
    }

    BST_Iterator<Container, is_reversed>& DecrementInOrder() {
        if (!ptr) {
            ptr = tree_->GetInOrderBeforeEnd();
        } else if (ptr->GetLeft()) {
            ptr = tree_->Max(ptr->GetLeft());
        } else {
            node_type_ptr parent = ptr->GetParent();

            while (parent && (parent->GetRight() != ptr)) {
                ptr = parent;
                parent = parent->GetParent();
            }

            ptr = parent;
        }
        return *this;
    }

    BST_Iterator<Container, is_reversed>& IncrementPreOrder() {
        if (is_reversed && !ptr) {
            ptr = tree_->GetPreOrderBegin();
            return *this;
        }

        if (ptr->GetLeft()) {
            ptr = ptr->GetLeft();
        } else if (ptr->GetRight()) {
            ptr = ptr->GetRight();
        } else {
            node_type_ptr parent = ptr->GetParent();

            if (parent && parent->GetLeft() == ptr && parent->GetRight()) {
                ptr = parent->GetRight();
                return *this;
            }

            while (parent && parent->GetParent() && (!parent->GetParent()->GetRight() || parent->GetParent()->GetRight() == parent)) {
                parent = parent->GetParent();
            }

            if (!parent || !parent->GetParent()) {
                ptr = nullptr;
            } else {
                ptr = parent->GetParent()->GetRight();
            }
        }
        return *this;
    }

    BST_Iterator<Container, is_reversed>& DecrementPreOrder() {
        if (!ptr) {
            ptr = tree_->GetPreOrderBeforeEnd();
        } else if (ptr == tree_->GetRoot()) {
            ptr = nullptr;
        } else if (ptr->GetParent()->GetRight() == ptr && ptr->GetParent()->GetLeft()) {
            node_type_ptr left_sibling = ptr->GetParent()->GetLeft();
            while (left_sibling->GetLeft() || left_sibling->GetRight()) {
                if (left_sibling->GetRight()) {
                    left_sibling = left_sibling->GetRight();
                } else {
                    left_sibling = left_sibling->GetLeft();
                }
            }
            ptr = left_sibling;
        } else {
            ptr = ptr->GetParent();
        }
        return *this;
    }

    BST_Iterator<Container, is_reversed>& IncrementPostOrder() {
        if (is_reversed && !ptr) {
            ptr = tree_->GetPostOrderBegin();
            return *this;
        }

        if (ptr == tree_->GetRoot()) {
            ptr = nullptr;
            return *this;
        }
        if (ptr->GetParent()->GetRight() == ptr) {
            ptr = ptr->GetParent();
        } else {
            if (ptr->GetParent()->GetRight()) {
                node_type_ptr right_sibling = ptr->GetParent()->GetRight();

                while (right_sibling->GetLeft() || right_sibling->GetRight()) {
                    if (right_sibling->GetLeft()) {
                        right_sibling = right_sibling->GetLeft();
                    } else {
                        right_sibling = right_sibling->GetRight();
                    }
                }

                ptr = right_sibling;
            } else {
                ptr = ptr->GetParent();
            }
        }
        return *this;
    }

    BST_Iterator<Container, is_reversed>& DecrementPostOrder() {
        if (!ptr) {
            ptr = tree_->GetPostOrderBeforeEnd();
        } else if (ptr->GetRight()) {
            ptr = ptr->GetRight();
        } else if (ptr->GetLeft()) {
            ptr = ptr->GetLeft();
        } else if (ptr->GetParent() && (ptr->GetParent()->GetRight() == ptr) && ptr->GetParent()->GetLeft()) {
            ptr = ptr->GetParent()->GetLeft();
        } else {
            node_type_ptr parent = ptr->GetParent();

            while (parent && parent->GetParent() && (parent->GetParent()->GetLeft() == parent || !parent->GetParent()->GetLeft())) {
                parent = parent->GetParent();
            }

            if (!parent || !parent->GetParent()) {
                ptr = nullptr;
            } else {
                ptr = parent->GetParent()->GetLeft();
            }
        }
        return *this;
    }

    BST_Iterator<Container, is_reversed>& operator++() {
        if (!ptr) {
            return *this;
        }

        if (std::is_same<travel_type, InOrder>::value && !is_reversed) {
            return IncrementInOrder();
        } else if (std::is_same<travel_type, PreOrder>::value && !is_reversed) {
            return IncrementPreOrder();
        } else if (std::is_same<travel_type, PostOrder>::value && !is_reversed) {
            return IncrementPostOrder();
        } else if (std::is_same<travel_type, InOrder>::value && is_reversed) {
            return DecrementInOrder();
        } else if (std::is_same<travel_type, PreOrder>::value && is_reversed) {
            return DecrementPreOrder();
        } else if (std::is_same<travel_type, PostOrder>::value && is_reversed) {
            return DecrementPostOrder();
        }
    };

    BST_Iterator<Container, is_reversed>& operator--() {
        if (std::is_same<travel_type, InOrder>::value && !is_reversed) {
            return DecrementInOrder();
        } else if (std::is_same<travel_type, PreOrder>::value && !is_reversed) {
            return DecrementPreOrder();
        } else if (std::is_same<travel_type, PostOrder>::value && !is_reversed) {
            return DecrementPostOrder();
        } else if (std::is_same<travel_type, InOrder>::value && is_reversed) {
            return IncrementInOrder();
        } else if (std::is_same<travel_type, PreOrder>::value && is_reversed) {
            return IncrementPreOrder();
        } else if (std::is_same<travel_type, PostOrder>::value && is_reversed) {
            return IncrementPostOrder();
        }
    };

    BST_Iterator<Container, is_reversed> operator++(int) {
        BST_Iterator<Container, is_reversed> temp = *this;
        ++(*this);
        return temp;
    }

    BST_Iterator<Container, is_reversed> operator--(int) {
        BST_Iterator<Container, is_reversed> temp = *this;
        --(*this);
        return temp;
    }

    friend bool operator==(const BST_Iterator<Container, is_reversed>& first, const BST_Iterator<Container, is_reversed>& second) {
        return first.ptr == second.ptr;
    }

    friend bool operator!=(const BST_Iterator<Container, is_reversed>& first, const BST_Iterator<Container, is_reversed>& second) {
        return first.ptr != second.ptr;
    }

private:
    node_type_ptr ptr;
    tree_ptr tree_;
};