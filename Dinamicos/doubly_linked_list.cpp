//! Copyright [year] <Augusto de Hollanda Vieira Guerner>

#include <stdexcept>
#include <cstdint>

namespace structures {

template<typename T>
class DoublyLinkedList {
 public:
    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
        size_ = 0;
    }
    ~DoublyLinkedList() {
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
    }

    void push_back(const T& data) {
        Node *new_node = new Node(data);
        if (empty()) {
            tail = new_node;
            head = new_node;
            tail->prev(head);
            head->next(tail);
        } else {
            tail->next(new_node);
            new_node->prev(tail);
            tail = tail->next();
        }
        size_++;
    }  // insere no fim
    void push_front(const T& data) {
        Node *new_node = new Node(data);
        if (empty()) {
            tail = new_node;
            head = new_node;
            tail->prev(head);
            head->next(tail);
        } else {
            new_node->next(head);
            head->prev(new_node);
            head = new_node;
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
            push_back(data);
        } else if (size() == 1) {
            if (head->data() <= data)
                push_back(data);
            else
                push_front(data);
        } else {
            if (data < head->data()) {
                push_front(data);
                return;
            }
            std::size_t i = 1;
            Node *current = head->next();
            for (i = 1; i < size(); i++) {
                if (current->prev()->data() <= data &&
                    data <= current->data()) {
                    insert(data, i);
                    return;
                }
            }
            push_back(data);
        }
    }  // insere em ordem

    T pop(std::size_t index) {
        if (index < 0 || index >= size())
            throw std::out_of_range("Index inválido!");
        Node *current = head;
        for (std::size_t i = 0; i < index; i++)
            current = current->next();
        Node *prev = current->prev();
        Node *next = current->next();
        T data = current->data();
        delete current;
        if (prev != nullptr)
            prev->next(next);
        if (next != nullptr)
            next->prev(prev);
        size_--;
        return data;
    }  // retira da posição

    T pop_back() {
        if (empty())
            throw std::out_of_range("Lista vazia!");
        T data = tail->data();
        Node *new_tail = tail->prev();
        delete tail;
        if (size() == 1) {
            head = nullptr;
            tail = nullptr;
        } else {
            tail = new_tail;
            tail->next(nullptr);
        }
        size_--;
        return data;
    }  // retira do fim
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
            head->prev(nullptr);
        }
        size_--;
        return data;
    }  // retira do início
    void remove(const T& data) {
        if (empty())
            throw std::out_of_range("Lista vazia!");
        Node *current = head;
        for (std::size_t i = 0; i < size(); i++) {
            if (current->data() == data)
                break;
            current = current->next();
        }
        Node *prev = current->prev();
        Node *next = current->next();
        if (prev != nullptr)
            prev->next(next);
        if (next != nullptr)
            next->prev(prev);
        delete current;
        size_--;
    }  // retira específico

    bool empty() const {
        return size() == 0;
    }  // lista vazia
    bool contains(const T& data) const {
        if (empty())
            throw std::out_of_range("Lista vazia!");
        Node *current = head;
        for (std::size_t i = 0; i < size(); i++) {
            if (current->data() == data)
                return true;
            current = current->next();
        }
        return false;
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
        if (empty())
            throw std::out_of_range("Lista vazia!");
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

    Node* head;  // primeiro da lista
    Node* tail;  // ultimo da lista
    std::size_t size_;
};
}  // namespace structures
