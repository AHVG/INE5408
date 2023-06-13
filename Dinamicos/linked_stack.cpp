//! Copyright [2023] <Augusto de Hollanda Vieira Guerner>

#include <stdexcept>
#include <cstdint>


namespace structures {

template<typename T>
class LinkedStack {
 public:
    LinkedStack() {
        top_ = nullptr;
        size_ = 0;
    }

    ~LinkedStack() {
        clear();
    }

    void clear() {
        Node *next;
        Node *current = top_;
        for (std::size_t i = 0; i < size(); i++) {
            next = current->next();
            delete current;
            current = next;
        }
        size_ = 0;
    }  // limpa pilha

    void push(const T& data) {
        Node *new_top = new Node(data, top_);
        top_ = new_top;
        size_++;
    }  // empilha

    T pop() {
        if (empty())
            throw std::out_of_range("Pilha vazia!");
        T data = top_->data();
        Node *next = top_->next();
        delete top_;
        top_ = next;
        size_--;
        return data;
    }  // desempilha

    T& top() const {
        if (empty())
            throw std::out_of_range("Pilha vazia!");
        return top_->data();
    }  // dado no topo

    bool empty() const {
        return size_ == 0;
    }  // pilha vazia

    std::size_t size() const {
        return size_;
    }  // tamanho da pilha

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

    Node* top_;  // nodo-topo
    std::size_t size_;  // tamanho
};

}  // namespace structures

