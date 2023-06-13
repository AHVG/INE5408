//! Copyright [2023] <Augusto de Hollanda Vieira Guerner>

#include <stdexcept>
#include <cstdint>

namespace structures {

template<typename T>
class DoublyCircularList {
 public:
    DoublyCircularList() {
        head = nullptr;
        size_ = 0;
    }
    ~DoublyCircularList() {
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
    }

    void push_back(const T& data) {
        Node *new_tail = new Node(data);
        if (empty()) {
            head = new_tail;
            head->next(head);
            head->prev(head);
        } else {
            head->prev()->next(new_tail);
            new_tail->next(head);
            new_tail->prev(head->prev());
            head->prev(new_tail);
        }
        size_++;
    }  // insere no fim
    void push_front(const T& data) {
        Node *new_head = new Node(data);
        if (empty()) {
            head = new_head;
            head->next(head);
            head->prev(head);
        } else {
            head->prev()->next(new_head);
            new_head->next(head);
            new_head->prev(head->prev());
            head->prev(new_head);
            head = new_head;
        }
        size_++;
    }  // insere no início
    void insert(const T& data, std::size_t index) {
        if (index <= 0 || index >= size())
            throw std::out_of_range("Index inválido!");
        Node *next = head;
        for (std::size_t i = 0; i < index; i++)
            next = next->next();
        Node *prev = next->prev();
        Node *new_node = new Node(data, prev, next);
        prev->next(new_node);
        next->prev(new_node);
        size_++;
    }  // insere na posição
    void insert_sorted(const T& data) {
        if (empty()) {
            push_front(data);
        } else if (size() == 1) {
            if (head->data() < data)
                push_back(data);
            else
                push_front(data);
        } else {
            if (data <= head->data()) {
                push_front(data);
            } else {
                Node *current = head->next();
                for (std::size_t i = 1; i < size(); i++) {
                    if (current->prev()->data() <= data &&
                        data <= current->data()) {
                        insert(data, i);
                        return;
                    }
                }
                push_back(data);
            }
        }
    }  // insere em ordem

    T pop(std::size_t index) {
        if (index < 0 || index >= size())
            throw std::out_of_range("Index inválido!");
        if (size() == 1)
            return pop_front();
        if (index == size() - 1)
            return pop_back();
        Node *current = head->next();
        for (std::size_t i = 1; i < index; i++)
            current = current->next();
        current->prev()->next(current->next());
        current->next()->prev(current->prev());
        T data = current->data();
        delete current;
        size_--;
        return data;
    }  // retira da posição
    T pop_back() {
        if (empty())
            throw std::out_of_range("Lista vazia!");
        T data = head->prev()->data();
        if (size() == 1) {
            delete head;
            head = nullptr;
        } else {
            Node *new_tail = head->prev()->prev();
            new_tail->next(head);
            delete head->prev();
            head->prev(new_tail);
        }
        size_--;
        return data;
    }  // retira do fim
    T pop_front() {
        if (empty())
            throw std::out_of_range("Lista vazia!");
        T data = head->data();
        if (size() == 1) {
            delete head;
            head = nullptr;
        } else {
            Node *new_head = head->next();
            new_head->prev(head->prev());
            delete head;
            head = new_head;
        }
        size_--;
        return data;
    }  // retira do início
    void remove(const T& data) {
        Node *current = head;
        for (std::size_t i = 0; i < size(); i++) {
            if (current->data() == data) {
                pop(i);
                return;
            }
            current = current->next();
        }
    }  // retira específico

    bool empty() const {
        return size_ == 0;
    }  // lista vazia
    bool contains(const T& data) const {
        return find(data) != size();
    }  // contém

    T& at(std::size_t index) {
        if (index < 0 || index >= size())
            throw std::out_of_range("Index inválido!");
        Node *current = head;
        for (std::size_t i = 0; i < index; i++)
            current = current->next();
        return current->data();
    }  // acesso a um elemento (checando limites)
    const T& at(std::size_t index) const {
        return at(index);
    }  // getter constante a um elemento

    std::size_t find(const T& data) const {
        Node *current = head;
        for (std::size_t i = 0; i < size(); i++) {
            if (current->data() == data)
                return i;
            current = current->next();
        }
        return size();
    }  // posição de um dado
    std::size_t size() const {
        return size_;
    }  // tamanho

 private:
    class Node {  // implementar cada um dos métodos de Node
     public:
        explicit Node(const T& data) {
            data_ = data;
            next_ = nullptr;
            prev_ = nullptr;
        }
        Node(const T& data, Node* next) {
            data_ = data;
            next_ = next;
            prev_ = nullptr;
        }
        Node(const T& data, Node* prev, Node* next) {
            data_ = data;
            next_ = next;
            prev_ = prev;
        }

        T& data() {
            return data_;
        }
        const T& data() const {
            return data_;
        }

        Node* prev() {
            return prev_;
        }
        const Node* prev() const {
            return prev_;
        }

        void prev(Node* node) {
            prev_ = node;
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

    Node* head;
    std::size_t size_;
};
}  // namespace structures
