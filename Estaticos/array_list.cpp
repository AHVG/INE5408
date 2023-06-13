// Copyright [2023] <Augusto de Hollanda Vieira Guerner>

#ifndef STRUCTURES_ARRAY_LIST_H
#define STRUCTURES_ARRAY_LIST_H

#include <stdexcept>
#include <cstdint>


namespace structures {

template<typename T>
class ArrayList {
 public:
    ArrayList() : ArrayList(DEFAULT_MAX) {}
    explicit ArrayList(std::size_t max_size) {
        max_size_ = max_size;
        size_ = 0;
        contents = new T[max_size_];
    }
    ArrayList(const ArrayList &other) {
        *this = other;
    }
    ~ArrayList() {
        delete [] contents;
    }

    void clear() {
        size_ = 0;
    }
    void push_back(const T& data) {
        if (full())
            throw std::out_of_range("Lista cheia!");
        contents[size_++] = data;
    }
    void push_front(const T& data) {
        if (full())
            throw std::out_of_range("Lista cheia!");
        for (std::size_t i = size_; i > 0; i--)
            contents[i] = contents[i - 1];
        contents[0] = data;
        size_++;
    }
    void insert(const T& data, std::size_t index)  {
        if (index > size_ || index < 0)
            throw std::out_of_range("Index inválido!");
        for (std::size_t i = size_; i > index; i--)
            contents[i] = contents[i - 1];
        contents[index] = data;
        size_++;
    }
    void insert_sorted(const T& data) {
        if (full())
            throw std::out_of_range("Lista cheia!");
        std::size_t index = 0;
        for (std::size_t i = 0; i < size_; i++)
            if (contents[i] <= data)
                index = i + 1;
        insert(data, index);
    }
    T pop(std::size_t index)  {
        if (index >= size_ || index < 0)
            throw std::out_of_range("Index inválido!");
        T aux = contents[index];
        size_--;
        for (std::size_t i = index; i < size_; i++)
            contents[i] = contents[i + 1];
        return aux;
    }
    T pop_back() {
        if (empty())
            throw std::out_of_range("Lista vazia!");
        return contents[--size_];
    }
    T pop_front() {
        if (empty())
            throw std::out_of_range("Lista vazia!");
        return pop(0);
    }
    void remove(const T& data)  {
        for (std::size_t i = 0; i < size_; i++)
            if (contents[i] == data)
                pop(i);
    }
    bool full() const {
        return size_ >= max_size_;
    }
    bool empty() const {
        return size_ <= 0;
    }
    bool contains(const T& data) const {
        for (std::size_t i = 0; i < size_; i++)
            if (contents[i] == data)
                return true;
        return false;
    }

    std::size_t find(const T& data) const {
        for (std::size_t i = 0; i < size_; i++)
            if (contents[i] == data)
                return i;
        return size_;
    }
    std::size_t size() const {
        return size_;
    }
    std::size_t max_size() const {
        return max_size_;
    }
    T& at(std::size_t index) {
        if (index > size_ || index < 0)
            throw std::out_of_range("Index inválido!");
        return contents[index];
    }
    T& operator[](std::size_t index) {
        return at(index);
    }
    const T& at(std::size_t index) const {
        if (index > size_ || index < 0)
            throw std::out_of_range("Index inválido!");
        return contents[index];
    }
    const T& operator[](std::size_t index) const {
        return at(index);
    }

 private:
    T* contents;
    std::size_t size_;
    std::size_t max_size_;

    static const auto DEFAULT_MAX = 10u;
};

}