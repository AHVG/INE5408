// Copyright [2023] <Augusto>
#ifndef STRUCTURES_AVL_TREE_H
#define STRUCTURES_AVL_TREE_H

#include <algorithm>
#include <cstdlib>
#include "array_list.h"

namespace structures {


template<typename T>
class AVLTree {
public:
    AVLTree() {
        root_ = nullptr;
        size_ = 0;
    }
    ~AVLTree() {
        ArrayList<T> a = pre_order();
        while (!a.empty()) this->remove(a.pop_back());
    }

    void insert(const T& data) {
        root_ = insert_(data, root_);
        size_++;
    }

    void remove(const T& data) {
        if (contains(data)) {
            root_ = remove_(data, root_);
            size_--;
        }
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

    int height() const {
        return height_(root_);
    }

    ArrayList<T> pre_order() const {
        ArrayList<T> v;
        if (root_) pre_order_(v, root_);
        return v;
    }

    ArrayList<T> in_order() const {
        ArrayList<T> v;
        if (root_) in_order_(v, root_);
        return v;
    }

    ArrayList<T> post_order() const {
        ArrayList<T> v;
        if (root_) post_order_(v, root_);
        return v;
    }

private:
    struct Node {
        explicit Node(const T& data_) {
            data = data_;
            height = 0;
            left = nullptr;
            right = nullptr;
        }

        T data;
        int height;
        Node* left;
        Node* right;
    };

    Node *insert_(const T& data_, Node *root) {
        Node *tmp = root;
        if (root == nullptr) {
            return new Node(data_);
        } else if (data_ < root->data) {
            root->left = insert_(data_, root->left);
            if (std::abs(height_(root->left) - height_(root->right)) > 1) {
                if (data_ < root->left->data) tmp = simpleLeft_(root);
                else
                    tmp = doubleLeft_(root);
            } else {
                updateHeight_(tmp);
            }
        } else if (data_ > (root->data)) {
            root->right = insert_(data_, root->right);
            if (std::abs(height_(root->left) - height_(root->right)) > 1) {
                if (data_ > root->right->data) tmp = simpleRight_(root);
                else
                    tmp = doubleRight_(root);
            } else {
                updateHeight_(tmp);
            }
        }
        return tmp;
    }

    Node *remove_(const T& data_, Node *root) {
        return root;
    }

    bool contains_(const T& data_, Node *root) const {
        if (!root) return false;
        if (data_ < root->data) return contains_(data_, root->left);
        else if (data_ > root->data) return contains_(data_, root->right);
        else
            return true;
    }

    void updateHeight_(Node *root) {
        root->height = std::max(height_(root->left), height_(root->right)) + 1;
    }

    Node* simpleLeft_(Node *root) {
        Node *k1 = root->left;
        root->left = k1->right;
        k1->right = root;

        updateHeight_(root);
        updateHeight_(k1);

        return k1;
    }

    Node* simpleRight_(Node *root) {
        Node *k1 = root->right;
        root->right = k1->left;
        k1->left = root;

        updateHeight_(root);
        updateHeight_(k1);

        return k1;
    }

    Node* doubleLeft_(Node *root) {
        root->left = simpleRight_(root->left);
        return simpleLeft_(root);
    }

    Node* doubleRight_(Node *root) {
        root->right = simpleLeft_(root->right);
        return simpleRight_(root);
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

    int height_(Node *root) const {
        return root ? root->height : -1;
    }

    Node* root_;
    std::size_t size_;
};

}  // namespace structures

#endif
