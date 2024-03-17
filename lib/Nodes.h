#pragma once

#include <cstdint>

template <typename Key>
class Node {
public:
    Node(const Key& key, Node* left, Node* right, Node* parent): key(key), left(left), parent(parent) {}
    ~Node() {}

    void SetNodePtr(Node* node_left, Node* node_right, Node* node_parent, const Key& node_key) {
        left = node_left;
        right = node_right;
        parent = node_parent;
        key = node_key;
    }

    const Key& GetKey() const {return key;}
    Node* GetLeft() const {return left;}
    Node* GetRight() const {return right;}
    Node* GetParent() const {return parent;}
private:
    Key key;
    Node* left;
    Node* right;
    Node* parent;
};