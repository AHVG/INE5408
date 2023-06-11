// Copyright [2023] <Augusto>
#ifndef STRUCTURES_BINARY_TREE_H
#define STRUCTURES_BINARY_TREE_H

#include <cstdint>
#include "array_list.h"


namespace structures {

template<typename T>
class BinaryTree {
public:
    BinaryTree() {root_ = nullptr; size_ = 0;}

    ~BinaryTree() {
        ArrayList<T> datas = pre_order();
        while (!empty()) this->remove(datas.pop_back());
    }

    void insert(const T& data) {
        insert_(data, &root_);
        size_++;
    }

    void remove(const T& data) {
        if (contains(data)) size_--;
        root_ = remove_(data, &root_);
    }

    bool contains(const T& data) const {
        return contains_(data, root_);
    }

    bool empty() const {
        return !size_;
    }

    std::size_t size() const {
        return size_;
    }

    ArrayList<T> pre_order() const {
        ArrayList<T> v;
        pre_order_(v, root_);
        return v;
    }

    ArrayList<T> in_order() const {
        ArrayList<T> v;
        in_order_(v, root_);
        return v;
    }

    ArrayList<T> post_order() const {
        ArrayList<T> v;
        post_order_(v, root_);
        return v;
    }

private:
    struct Node {
        explicit Node(const T& data_) {
            data = data_;
            right = nullptr;
            left = nullptr;
        }
        T data;
        Node* left;
        Node* right;
    };

    Node *min_node(Node *root) {
        if (!root) return nullptr;
        if (root->left) return min_node(root->left);
        else return root;
    }

    void insert_(const T& data, Node **root) {
        if (*root == nullptr) *root = new Node(data);
        else if (data < (*root)->data) insert_(data, &((*root)->left));
        else insert_(data, &((*root)->right));
    }

    Node *remove_(const T& data, Node **root) {
        if (!(*root)) return *root;
        if (data < (*root)->data) {
            (*root)->left = remove_(data, &((*root)->left));
            return *root;
        } else if (data > (*root)->data) {
            (*root)->right = remove_(data, &((*root)->right));
            return *root;
        } else if ((*root)->left && (*root)->right) {
            Node *tmp = min_node((*root)->right);
            (*root)->data = tmp->data;
            (*root)->right = remove_(tmp->data, &((*root)->right));
            return *root;
        } else if ((*root)->left) {
            Node *tmp = (*root)->left;
            delete *root;
            return tmp;
        } else if ((*root)->right) {
            Node *tmp = (*root)->right;
            delete *root;
            return tmp;
        } else {
            delete *root;
            return nullptr;
        }
    }

    bool contains_(const T& data, Node *root) const {
        if (!root) return false;
        if (data < root->data) return contains_(data, root->left);
        else if (data > root->data) return contains_(data, root->right);
        else return true;
    }

    void pre_order_(ArrayList<T>& v, Node *root) const {
        if (!root) return;
        v.push_back(root->data);
        pre_order_(v, root->left);
        pre_order_(v, root->right);
    }

    void in_order_(ArrayList<T>& v, Node *root) const {
        if (!root) return;
        in_order_(v, root->left);
        v.push_back(root->data);
        in_order_(v, root->right);
    }

    void post_order_(ArrayList<T>& v, Node *root) const {
        if (!root) return;
        post_order_(v, root->left);
        post_order_(v, root->right);
        v.push_back(root->data);
    }

    Node* root_;
    std::size_t size_;
};

}  // namespace structures

#endif