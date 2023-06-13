//! Copyright [2023] <Augusto de Hollanda Vieira Guerner>

#include <stdexcept>
#include <cstdint>

namespace structures {


template<typename T>
class LinkedQueue {
 public:
    LinkedQueue() {
        head = nullptr;
        tail = nullptr;
        size_ = 0;
    }

    ~LinkedQueue() {
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
    }  // limpar

    void enqueue(const T& data) {
        Node *new_node = new Node(data, nullptr);
        if (empty()) {
            tail = new_node;
            head = new_node;
        } else {
            tail->next(new_node);
            tail = tail->next();
        }
        size_++;
    }  // enfilerar

    T dequeue() {
        if (empty())
            throw std::out_of_range("Pilha vazia!");
        T data = head->data();
        Node *new_head = head->next();
        delete head;
        head = new_head;
        size_--;
        return data;
    }  // desenfilerar

    T& front() const {
        if (empty())
            throw std::out_of_range("Pilha vazia!");
        return head->data();
    }  // primeiro dado

    T& back() const {
        if (empty())
            throw std::out_of_range("Pilha vazia!");
        return tail->data();
    }  // último dado

    bool empty() const {
        return size() == 0;
    }  // fila vazia

    std::size_t size() const {
        return size_;
    }  // tamanho

 private:
    class Node {
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
        }  // getter: info
        const T& data() const {
            return data_;
        }  // getter-constante: info

        Node* next() {
            return next_;
        }  // getter: próximo
        const Node* next() const {
            return next_;
        }  // getter-constante: próximo

        void next(Node* next) {
            next_ = next;
        }  // setter: próximo

     private:
        T data_;
        Node* next_;
    };

    Node* head;  // nodo-cabeça
    Node* tail;  // nodo-fim
    std::size_t size_;  // tamanho
};
}  // namespace structures