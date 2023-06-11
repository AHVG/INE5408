//! Copyright [2023] <Augusto de Hollanda Vieira Guerner>

#ifndef STRUCTURES_CIRCULAR_LIST_H
#define STRUCTURES_CIRCULAR_LIST_H

#include <stdexcept>
#include <cstdint>


namespace structures {

template<typename T>
class CircularList {
 public:
    CircularList() {
        head = nullptr;
        tail = nullptr;
        size_ = 0;
    }
    ~CircularList() {
        clear();
    }

    void clear() {
        Node *current = head;
        for (std::size_t i = 0; i < size(); i++) {
            Node *next = current->next();
            delete current;
            current = next;
        }
        size_ = 0;
        head = nullptr;
        tail = nullptr;
    }  // limpar lista

    void push_back(const T& data) {
        Node *new_node = new Node(data, head);
        if (empty()) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next(new_node);
            tail = new_node;
        }
        size_++;
    }  // inserir no fim
    void push_front(const T& data) {
        Node *new_node = new Node(data, head);
        if (empty()) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next(new_node);
            head = new_node;
        }
        size_++;
    }  // inserir no início
    void insert(const T& data, std::size_t index) {
        if (index <= 0 || index >= size())
            throw std::out_of_range("Index inválido!");
        Node *new_node = new Node(data, nullptr);
        Node *next = head->next();
        Node *prev = head;
        for (std::size_t i = 1; i < index; i++) {
            prev = next;
            next = next->next();
        }
        new_node->next(next);
        prev->next(new_node);
        size_++;
    }  // inserir na posição
    void insert_sorted(const T& data) {
        if  (empty()) {
            push_back(data);
        } else if (size() == 1) {
            if (head->data() < data)
                push_back(data);
            else
                push_front(data);
        } else  {
            if (data < head->data()) {
                push_front(data);
            } else {
                Node *prev = head;
                Node *next = head->next();
                for (std::size_t i = 1; i < size(); i++) {
                    if (prev->data() <= data && data <= next->data()) {
                        insert(data, i);
                        return;
                    }
                }
                push_back(data);
            }
        }
    }  // inserir em ordem

    T& at(std::size_t index) {
        if (index < 0 || index >= size())
            throw std::out_of_range("Index inválido!");
        Node *current = head;
        for (std::size_t i = 0; i < index; i++)
            current = current->next();
        return current->data();
    }  // acessar em um indice (com checagem de limites)
    const T& at(std::size_t index) const {
        return at(index);
    }  // versão const do acesso ao indice

    T pop(std::size_t index) {
        if (index < 0 || index >= size())
            throw std::out_of_range("Index inválido!");
        if (size() == 1)
            return pop_front();
        if (index == size() - 1)
            return pop_back();
        Node *prev = head;
        Node *current = head->next();
        for (std::size_t i = 1; i < index; i++) {
            prev = current;
            current = current->next();
        }
        prev->next(current->next());
        T data = current->data();
        size_--;
        delete current;
        return data;
    }  // retirar da posição
    T pop_back() {
        if (empty())
            throw std::out_of_range("Lista vazia!");
        T data = tail->data();
        delete tail;
        if (size() == 1) {
            head = nullptr;
            tail = nullptr;
        } else {
            Node *new_tail = head;
            for (std::size_t i = 1; i < size() - 1; i++)
                new_tail = new_tail->next();
            tail = new_tail;
            tail->next(head);
        }
        size_--;
        return data;
    }  // retirar do fim
    T pop_front() {
        if (empty())
            throw std::out_of_range("Lista vazia!");
        T data = head->data();
        Node *new_head = head->next();
        delete head;
        if (size() == 1) {
            head = nullptr;
            tail = nullptr;
        } else {
            head = new_head;
            tail->next(head);
        }
        size_--;
        return data;
    }  // retirar do início
    void remove(const T& data) {
        if (empty())
            throw std::out_of_range("Lista vazia!");
        Node *current = head;
        for (std::size_t i = 0; i < size(); i++) {
            if (current->data() == data) {
                pop(i);
                return;
            }
            current = current->next();
        }
    }  // remover dado específico

    bool empty() const {
        return size_ == 0;
    }  // lista vazia
    bool contains(const T& data) const {
        return find(data) != size();
    }  // lista contém determinado dado?
    std::size_t find(const T& data) const {
        Node *current = head;
        for (std::size_t i = 0; i < size(); i++) {
            if (current->data() == data)
                return i;
            current = current->next();
        }
        return size();
    }  // posição de um item na lista

    std::size_t size() const {
        return size_;
    }  // tamanho da lista

 private:
    class Node {  // implementar cada um dos métodos de Node
     public:
        explicit Node(const T& data) {
            data_ = data;
            next_ = nullptr;
        }
        Node(const T& data, Node* next) {
            data_ = data;
            next_ = next;
        }

        T& data() {
            return data_;
        }
        const T& data() const {
            return data_;
        }

        Node* next() {
            return next_;
        }
        const Node* next() const {
            return next_;
        }

        void next(Node* node) {
            next_ = node;
        }

     private:
        T data_;
        Node* prev_;
        Node* next_;
    };

    Node* head;  // primeiro da lista
    Node* tail;  // ultimo da lista
    std::size_t size_;
};
}  // namespace structures

#endif
