//! Copyright [2023] <Augusto de Hollanda Vieira Guerner>

#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

#include <stdexcept>
#include <cstdint>


namespace structures {

template<typename T>
class LinkedList {
 public:
    LinkedList() {}  // construtor padrão

    ~LinkedList() {
        clear();
    }  // destrutor

    void clear() {
        Node *current = head;
        for (std::size_t i = 0; i < size(); i++) {
            Node *next = current->next();
            delete current;
            current = next;
        }
        head = nullptr;
        size_ = 0;
    }  // limpar lista

    void push_back(const T& data) {
        Node *node = new Node(data, nullptr);
        if (empty())
            head = node;
        else
            end()->next(node);
        size_++;
    }  // inserir no fim

    void push_front(const T& data) {
        Node *node = new Node(data, head);
        head = node;
        size_++;
    }  // inserir no início

    void insert(const T& data, std::size_t index) {
        if (index <= 0 || size() <= index)
            throw std::out_of_range("Index inválido!");

        Node *previous = head;
        Node *current = head->next();
        for (std::size_t i = 1; i < index; i++) {
            previous = current;
            current = current->next();
        }
        Node *node = new Node(data, current);
        previous->next(node);

        size_++;
    }  // inserir na posição

    void insert_sorted(const T& data) {
        if (empty()) {
            push_front(data);
            return;
        }
        if (data < head->data()) {
            push_front(data);
            return;
        }
        if (size() == 1) {
            push_back(data);
            return;
        }
        Node *previous = head;
        Node *current = head->next();
        for (std::size_t i = 1; i < size(); i++) {
            if (previous->data() <= data && data <= current->data()) {
                insert(data, i);
                return;
            }
            previous = current;
            current = current->next();
        }
        push_back(data);
    }  // inserir em ordem

    T& at(std::size_t index) const {
        if (index < 0 || size() <= index || empty())
            throw std::out_of_range("Index inválido!");
        Node *current = head;
        for (std::size_t i = 0; i < index; i++)
            current = current->next();

        return current->data();
    }  // acessar um elemento na posição index

    const T &operator[](const int &index) const {
        return at(index);
    }

    T pop(std::size_t index) {
        if (index < 0 || size() <= index || empty())
            throw std::out_of_range("Index inválido!");
        T data = at(index);
        remove(data);
        return data;
    }  // retirar da posição

    T pop_back() {
        if (empty())
            throw std::out_of_range("Lista vazia!");
        Node *end_ = end();
        T data = end_->data();
        delete end_;
        size_--;
        return data;
    }  // retirar do fim

    T pop_front() {
        if (empty())
            throw std::out_of_range("Lista vazia!");

        Node *head_ = head;
        Node *new_head = head->next();
        head = new_head;

        T data = head_->data();
        delete head_;
        size_--;

        return data;
    }  // retirar do início

    void remove(const T& data) {
        std::size_t index = find(data);
        if (index == size())
            return;
        Node *previous = nullptr;
        Node *current = head;
        for (std::size_t i = 0; i < index; i++) {
            previous = current;
            current = current->next();
        }
        if (previous == nullptr)
            head = current->next();
        else
            previous->next(current->next());
        delete current;
        size_--;
    }  // remover específico

    bool empty()  const {
        return size_ == 0;
    }  // lista vazia

    bool contains(const T& data) const {
        return find(data) != size();
    }  // contém

    std::size_t find(const T& data) const {
        Node *current = head;
        for (std::size_t i = 0; i < size(); i++) {
            if (current->data() == data)
                return i;
            current = current->next();
        }
        return size();
    }  // posição do dado

    std::size_t size() const {
        return size_;
    }  // tamanho da lista

    const LinkedList<T> &operator=(const LinkedList<T> &other) {
        clear();
        for (std::size_t i = 0; i < other.size(); i++)
            push_back(other[i]);
        return *this;
    }

 private:
    class Node {  // Elemento
     public:
        explicit Node(const T& data):
            data_{data}
        {}

        Node(const T& data, Node* next):
            data_{data},
            next_{next}
        {}

        T& data() {  // getter: dado
            return data_;
        }

        const T& data() const {  // getter const: dado
            return data_;
        }

        Node* next() {  // getter: próximo
            return next_;
        }

        const Node* next() const {  // getter const: próximo
            return next_;
        }

        void next(Node* node) {  // setter: próximo
            next_ = node;
        }

     private:
        T data_;
        Node* next_{nullptr};
    };

    Node* end() {  // último nodo da lista
        auto it = head;
        for (auto i = 1u; i < size(); ++i) {
            it = it->next();
        }
        return it;
    }

    Node* head{nullptr};
    std::size_t size_{0u};
};
}  // namespace structures
#endif  // LINKED_LIST_H_INCLUDED