// Copyright [2023] <Augusto>
#ifndef STRUCTURES_AVL_TREE_H
#define STRUCTURES_AVL_TREE_H

#include <algorithm>
#include "array_list.h"

namespace structures {


template<typename T>
class AVLTree {
public:
    AVLTree() {
        root = nullptr;
        size_ = 0;
    }
    ~AVLTree() {}

    void insert(const T& data) {}

    void remove(const T& data) {}

    bool contains(const T& data) const {
        if (root) return root->contains(data);
        return false;
    }

    bool empty() const {
        return !size_;
    }

    std::size_t size() const {
        return size_;
    }

    int height() const {
        return root->height(root);
    }

    ArrayList<T> pre_order() const {
        ArrayList<T> a;
        if (!root) return a;
        root->pre_order(a);
        return a;
    }

    ArrayList<T> in_order() const {
        ArrayList<T> a;
        if (!root) return a;
        root->in_order(a);
        return a;
    }

    ArrayList<T> post_order() const {
        ArrayList<T> a;
        if (!root) return a;
        root->post_order(a);
        return a;
    }

private:
    struct Node {
        explicit Node(const T& data_) {
            data = data_;
            height_ = 0;
            left = nullptr;
            right = nullptr;
        }

        T data;
        int height_;
        Node* left;
        Node* right;

        void insert(const T& data_) {}

        bool remove(const T& data_) {return false;}

        bool contains(const T& data_) const {
            if (data_ < data) {
                if (left) return left->contains(data_);
                return false;
            } else if (data < data_) {
                if (right) return right->contains(data_);
                return false;
            }
            return true;
        }

        void updateHeight() {
            std::max(height(left), height(right)) + 1;
        }

        Node* simpleLeft() {
            Node *k1 = this->left;

            this->left = k1->right;
            k1->right = this;

            updateHeight();
            k1->updateHeight();

            return k1;
        }

        Node* simpleRight() {
            Node *k1 = this->right;

            this->right = k1->left;
            k1->left = this;

            updateHeight();
            k1->updateHeight();

            return k1;
        }

        Node* doubleLeft() {
            left = left->simpleRight();
            return simpleLeft();
        }

        Node* doubleRight() {
            right = right->simpleLeft();
            return simpleRight();
        }

        void pre_order(ArrayList<T>& v) const {
            v.push_back(data);
            if (left) pre_order(v);
            if (right) pre_order(v);
        }

        void in_order(ArrayList<T>& v) const {
            if (left) in_order(v);
            v.push_back(data);
            if (right) in_order(v);
        }

        void post_order(ArrayList<T>& v) const {
            if (left) post_order(v);
            if (right) post_order(v);
            v.push_back(data);
        }

        int height(Node *x) {
            if (!x) return -1;
            return x->height_;
        }
    };

    Node* root;
    std::size_t size_;
};

}  // namespace structures

#endif
