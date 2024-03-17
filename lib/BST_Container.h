#pragma once

#include "BST_BidirectionalIterator.h"

#include <functional>
#include <memory>

const uint8_t kOnlyOneTree = 1;

template <typename Key, typename Travel, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
class BST_Container {
public:
    using key_type = Key;
    using value_type = Key;
    
    using travel_type = Travel;
    
    using key_compare = Compare;
    using value_compare = Compare;
    
    using allocator_type = Allocator;

    using node_type = Node<key_type>;
    using node_type_ptr = Node<key_type>*;

    using tree = BST<Key, Compare, Allocator>;
    using tree_ptr = tree*;
    using tree_alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<BST<key_type, key_compare, allocator_type>>;
    using tree_alloc_traits = std::allocator_traits<tree_alloc>;

    using not_reversed_iterator = BST_Iterator<BST_Container<key_type, travel_type, key_compare, allocator_type>>;
    using reversed_iterator = BST_Iterator<BST_Container<key_type, travel_type, key_compare, allocator_type>, true>;

    not_reversed_iterator begin() const {
        if (!tree_->GetRoot()) {
            not_reversed_iterator begin_iterator(nullptr, tree_);
            return begin_iterator;
        }

        if (std::is_same<travel_type, InOrder>::value) {
            not_reversed_iterator begin_iterator(tree_->GetInOrderBegin(), tree_);
            return begin_iterator;
        } else if (std::is_same<travel_type, PreOrder>::value) {
            not_reversed_iterator begin_iterator(tree_->GetPreOrderBegin(), tree_);
            return begin_iterator;
        } else if (std::is_same<travel_type, PostOrder>::value) {
            not_reversed_iterator begin_iterator(tree_->GetPostOrderBegin(), tree_);
            return begin_iterator;
        }
    }

    not_reversed_iterator cbegin() const {
        return begin();
    }

    reversed_iterator rbegin() const {
        if (!tree_->GetRoot()) {
            reversed_iterator begin_iterator(nullptr, tree_);
            return begin_iterator;
        }

        if (std::is_same<travel_type, InOrder>::value) {
            reversed_iterator begin_iterator(tree_->GetInOrderBeforeEnd(), tree_);
            return begin_iterator;
        } else if (std::is_same<travel_type, PreOrder>::value) {
            reversed_iterator begin_iterator(tree_->GetPreOrderBeforeEnd(), tree_);
            return begin_iterator;
        } else if (std::is_same<travel_type, PostOrder>::value) {
            reversed_iterator begin_iterator(tree_->GetPostOrderBeforeEnd(), tree_);
            return begin_iterator;
        }
    }

    reversed_iterator crbegin() const {
        return rbegin();
    }

    not_reversed_iterator end() const {
        BST_Iterator<BST_Container> end_iterator(nullptr, tree_);
        return end_iterator;
    }

    not_reversed_iterator cend() const {
        return end();
    }

    reversed_iterator rend() const {
        BST_Iterator<BST_Container, true> end_iterator(nullptr, tree_);
        return end_iterator;
    }

    reversed_iterator crend() const {
        return rend();
    }


    void CreateTree(const allocator_type& alloc, const key_compare& cmp) {
        tree_alloc tree_alloc(alloc);
        tree_ = tree_alloc_traits::allocate(tree_alloc, kOnlyOneTree);
        tree_alloc_traits::construct(tree_alloc, tree_, cmp, alloc);
    }

    void DeleteTree(const allocator_type& alloc) {
        tree_alloc tree_alloc(alloc);
        tree_alloc_traits::destroy(tree_alloc, tree_);
        tree_alloc_traits::deallocate(tree_alloc, tree_, kOnlyOneTree);
    }

    BST_Container() {
        CreateTree(alloc_, cmp_);
    };

    BST_Container(const allocator_type& alloc) {
        alloc_ = alloc;

        CreateTree(alloc_, cmp_);
    }

    BST_Container(const BST_Container& other) {
        alloc_ = std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.alloc_);
        cmp_ = other.cmp_;

        CreateTree(alloc_, cmp_);
        *tree_ = *other.tree_;
    }

    BST_Container(const BST_Container& other, const allocator_type& alloc) {
        alloc_ = alloc;
        cmp_ = other.cmp_;

        CreateTree(alloc_, cmp_)
        *tree_ = *other.tree_;
    }

    BST_Container(const key_compare& compare_object): cmp_(compare_object) {
        CreateTree(alloc_, cmp_);
    };

    BST_Container(key_type value_initialized_key): BST_Container() {
        tree_->Insert(alloc_, value_initialized_key);
    }

    template <typename Iterator>
    BST_Container (const Iterator& begin, const Iterator& end, const key_compare& compare_object) {
        CreateTree(alloc_, compare_object);
        for (Iterator it = begin; it != end; ++it) {
            tree_->Insert(alloc_, *it);
        }
    }

    template <typename Iterator>
    BST_Container (const Iterator& begin, const Iterator& end): BST_Container() {
        for (Iterator it = begin; it != end; ++it) {
            tree_->Insert(alloc_, *it);
        }
    }

    BST_Container(std::initializer_list<key_type> il): BST_Container() {
        for (const key_type& key: il) {
            tree_->Insert(alloc_, key);
        }
    }

    ~BST_Container() {
        tree_alloc tree_alloc(alloc_);
        DeleteTree(tree_alloc);
    }

    BST_Container& operator=(const BST_Container& other) {
        if (this != &other) {
            allocator_type new_alloc = std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value ? other.alloc_: alloc_;

            DeleteTree(alloc_);
            CreateTree(other.alloc_, other.cmp_);

            *tree_ = *other.tree_;

            alloc_ = new_alloc;
            cmp_ = other.cmp_;
        }

        return *this;
    }

    void swap(BST_Container& other) {
        if (this != &other) {
            BST<key_type, key_compare, allocator_type> temp(cmp_, alloc_);
            temp = *tree_;

            DeleteTree(alloc_);
            CreateTree(other.alloc_, other.cmp_);
            *tree_ = *other.tree_;

            other.DeleteTree(other.alloc_);
            other.CreateTree(alloc_, cmp_);
            *other.tree_ = temp;

            std::swap(cmp_, other.cmp_);

            if (alloc_ != other.alloc_ && std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
                std::swap(alloc_, other.alloc_);
            }
        }
    }

    allocator_type get_allocator() const { return alloc_;}

    bool operator==(const BST_Container& other) const { return *tree_ == *other.tree_;}

    bool operator!=(const BST_Container& other) const { return *tree_ != *other.tree_;}

    size_t size() const { return tree_->GetSize();}

    size_t max_size() const { return tree_->max_size();}

    bool empty() const { return !tree_->GetSize();}

    void clear() {
        tree_->clear();
    }

    BST_Container& operator=(std::initializer_list<key_type> il) {
        clear();

        for (const key_type& key: il) {
            tree_->Insert(alloc_, key);
        }

        return *this;
    }

    key_compare key_comp() const{
        return cmp_;
    }

    key_compare value_comp() const {
        return cmp_;
    }

    std::pair<not_reversed_iterator, bool> insert(const key_type& new_key) {
        size_t current_size = tree_->GetSize();

        tree_->Insert(alloc_, new_key);
        not_reversed_iterator it(tree_->Find(tree_->GetRoot(), new_key), tree_);

        return std::make_pair(it, current_size != tree_->GetSize());
    }

    not_reversed_iterator insert(const not_reversed_iterator& prior_it, const key_type& new_key) {
        tree_->Insert(alloc_, new_key);
        not_reversed_iterator it(tree_->Find(tree_->GetRoot(), new_key), tree_);

        return it;
    }

    void insert(const not_reversed_iterator& left, const not_reversed_iterator& right) {
        for (not_reversed_iterator it = left; it != right; ++it) {
            tree_->Insert(alloc_, *it);
        }
    }

    void insert(std::initializer_list<key_type> il) {
        for (const key_type& key: il) {
            tree_->Insert(alloc_, key);
        }
    }

    size_t erase(const key_type& value) {
        size_t current_size = tree_->GetSize();

        tree_->Delete(alloc_, value);

        return current_size != tree_->GetSize();
    }

   not_reversed_iterator erase(const not_reversed_iterator& it) {
        tree_->Delete(alloc_, *it++);
        return it;
    }

    not_reversed_iterator erase(const not_reversed_iterator& left, const not_reversed_iterator& right) {
        for (not_reversed_iterator it = left; it != right; ++it) {
            tree_->Delete(*it);
        }
        return right;
    }


    not_reversed_iterator find(const key_type& key) const {
        not_reversed_iterator it(tree_->Find(tree_->GetRoot(), key), tree_);
        return it;
    }

    template <typename T>
    not_reversed_iterator find(const T& user_type_obj) const {
        not_reversed_iterator it(tree_->Find(tree_->GetRoot(), user_type_obj), tree_);
        return it;
    }

    size_t count(const key_type& key) const {
        return tree_->Find(tree_->GetRoot(), key) ? 1: 0;
    }

    template <typename T>
    size_t count(const T& user_type_obj) const {
        return tree_->Find(tree_->GetRoot(), user_type_obj) ? 1: 0;
    }

    bool contains(const key_type& key) const {
        return tree_->Find(tree_->GetRoot(), key);
    }

    template <typename T>
    bool contains(const T& user_type_obj) const {
        return tree_->Find(tree_->GetRoot(), user_type_obj);
    }

    not_reversed_iterator lower_bound(const Key& key) const {
        if (!contains(key)) {
            not_reversed_iterator it(tree_->Next(key), tree_);
            return it;
        } else {
            not_reversed_iterator it(tree_->Find(tree_->GetRoot(), key), tree_);
            return it;
        }
    }

    template <typename T>
    not_reversed_iterator lower_bound(const T& user_type_obj) const {
        if (!contains(user_type_obj)) {
            not_reversed_iterator it(tree_->Next(user_type_obj), tree_);
            return it;
        } else {
            not_reversed_iterator it(tree_->Find(tree_->GetRoot(), user_type_obj), tree_);
            return it;
        }
    }

    not_reversed_iterator upper_bound(const Key& key) const {
        if (!contains(key)) {
            not_reversed_iterator it(tree_->Prev(key), tree_);
            return it;
        } else {
            not_reversed_iterator it(tree_->Find(tree_->GetRoot(), key), tree_);
            return it;
        }
    }

    template <typename T>
    not_reversed_iterator upper_bound(const T& user_type_obj) const {
        if (!contains(user_type_obj)) {
            not_reversed_iterator it(tree_->Prev(user_type_obj), tree_);
            return it;
        } else {
            not_reversed_iterator it(tree_->Find(tree_->GetRoot(), user_type_obj), tree_);
            return it;
        }
    }

    std::pair<not_reversed_iterator, not_reversed_iterator> equal_range(const Key& key) const {
        return std::make_pair<lower_bound(key), upper_bound(key)>;
    };

    template <typename T>
    std::pair<not_reversed_iterator, not_reversed_iterator> equal_range(const T& user_type_obj) const {
        return std::make_pair<lower_bound(user_type_obj), upper_bound(user_type_obj)>;
    }

    tree_ptr GetTree() const { return tree_;}

private:
    tree_ptr tree_;

    key_compare cmp_;
    allocator_type alloc_;
};

template <typename key_type, typename Travel, typename key_compare = std::less<key_type>, typename allocator_type = std::allocator<key_type>>
void swap(BST_Container<key_type, Travel, key_compare, allocator_type>& first, BST_Container<key_type, Travel, key_compare, allocator_type>& second) {
    return first.swap(second);
}