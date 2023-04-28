#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <cstdint>


#include <iostream>
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
        return at(index);
    }
    const T& operator[](std::size_t index) const {
        return at(index);
    }


    // descricao do 'operator []' na FAQ da disciplina
 private:
    T* contents;
    std::size_t size_;
    std::size_t max_size_;

    static const auto DEFAULT_MAX = 10u;
};

}  // namespace structures


namespace structures {

template<typename T>
class LinkedStack {
 public:
    LinkedStack() {
        top_ = nullptr;
        size_ = 0;
    }

    LinkedStack(const LinkedStack &other) {
        *this = other;
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

    // Sem const
    LinkedStack &operator=(LinkedStack &other) {
        LinkedStack aux;
        std::size_t other_size;
        while (!other.empty())
            aux.push(other.pop());
        clear();
        while (!aux.empty()) {
            T data = aux.pop();
            push(data);
            other.push(data);
        }
        return *this;
    }

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

namespace structures {

template<typename T>
class LinkedList {
 public:
    LinkedList() {
        head = nullptr;
        size_ = 0u;
    }  // construtor padrão

    LinkedList(const LinkedList &other) {
        *this = other;
    }

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
            throw std::out_of_range("Index inválido! " + std::to_string(index));
        Node *current = head;
        for (std::size_t i = 0; i < index; i++)
            current = current->next();
        return current->data();
    }  // acessar um elemento na posição index

    T &operator[](const int &index) const {
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

    const LinkedList &operator=(const LinkedList &other) {
        clear();
        for (long long int i = other.size() - 1; i >= 0; i--)
            push_front(other.at(i));
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

class XMLParser {
 private:

    structures::LinkedList<std::string> opening_tags;
    structures::LinkedList<std::string> closing_tags;

 public:
    // https://pt.stackoverflow.com/questions/410527/passar-objeto-de-classe-como-par%C3%A2metro-de-construtor-em-c
    XMLParser(structures::LinkedList<std::string> tags) {
        for (std::size_t i = 0; i < tags.size(); i++) {
            opening_tags.push_back("<" + tags[i] + ">");
            closing_tags.push_back("</" + tags[i] + ">");
        }
    }

    bool isValid(std::string xml) {
        size_t xml_len = xml.length();
        structures::LinkedStack<std::string> tags_stack;
        for (size_t i = 0; i < xml_len; i++) {
            for (std::size_t j = 0; j < opening_tags.size(); j++)
                if (opening_tags.at(j).length() <= xml_len - i)
                    if (xml.substr(i, opening_tags.at(j).length()) == opening_tags.at(j))
                        tags_stack.push(opening_tags.at(j));

            for (std::size_t j = 0; j < closing_tags.size(); j++)
                if (closing_tags.at(j).length() <= xml_len - i)
                    if (xml.substr(i, closing_tags.at(j).length()) == closing_tags.at(j)) {
                        if (tags_stack.empty())
                            return false;
                        else if (tags_stack.top() == opening_tags.at(j))
                            tags_stack.pop();
                        else
                            return false;
                    }
        }
        return tags_stack.empty();
    }

    structures::LinkedList<std::string> get_tags_contents(std::string xml, std::string tag) {
        structures::LinkedList<std::string> tag_contents;
        tag = "<" + tag + ">";
        std::size_t index = -1;
        for (std::size_t i = 0; i < opening_tags.size(); i++)
            if (opening_tags.at(i) == tag)
                index = i;
        if (index == -1u)
            return tag_contents;
        while(true) {
            std::size_t opening_tag_index = xml.find(tag);
            if (opening_tag_index == std::string::npos)
                break;
            opening_tag_index += tag.length();
            std::size_t closing_tag_index = xml.find(closing_tags[index]);
            std::size_t size = closing_tag_index - opening_tag_index;
            tag_contents.push_back(xml.substr(opening_tag_index, size));
            size = xml.length() - (closing_tag_index + closing_tags[index].length());
            xml = xml.substr(closing_tag_index + closing_tags[index].length(), size);
        }
        return tag_contents;
    }

    structures::LinkedList<std::string> get_tags_contents(std::string xml, structures::LinkedList<std::string> tag_hierarchy) {
        structures::LinkedList<std::string> contents;
        for (std::size_t i = 0; i < tag_hierarchy.size(); i++) {
            std::string aux = "";
            contents = get_tags_contents(xml, tag_hierarchy[i]);
            for (std::size_t i = 0; i < contents.size(); i++)
                aux += contents[i];
            xml = aux;
        }
        return contents;
    }
};

class Point {

 public:

    Point() {}

    Point(const Point &other) {
        x = other.x;
        y = other.y;
    }

    Point(int x_, int y_) {
        x = x_;
        y = y_;
    }

    const Point &operator=(const Point &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    int x;
    int y;

};

class Analyzer {


 public:

    int analyze(Point robot, Point dimension, structures::LinkedList<structures::LinkedList<int>> &matrix) {
        structures::LinkedStack<Point> way;
        structures::LinkedList<structures::LinkedList<int>> R;
        int sum = 0;
        for (int i = 0; i < dimension.y; i++) {
            R.push_front(structures::LinkedList<int>());
            for (int j = 0; j < dimension.x; j++) {
                R[i].push_front(0);
            }
            std::cout << R[i].size() << std::endl;
        }
        way.push(Point(robot.y, robot.x));
        Point aux = way.top();
        if (aux.x < dimension.x && aux.x >= 0 &&
            aux.y < dimension.y && aux.y >= 0)
            if (!matrix[aux.y][aux.x])
                return sum;
        while (!way.empty()) {
            Point p = way.pop();
            std::cout << "(" << dimension.x << "," << dimension.y << ")" << std::endl;
            std::cout << "(" << p.x << "," << p.y << ")" << std::endl;
            std::cout << "(" << R[0].size() << "," << R.size() << ")" << std::endl;
            R[p.y][p.x] = 1;

            for (int i = -1; i < 2; i += 2)
                if (p.y + i < dimension.y && p.y + i > -1 &&
                    p.x < dimension.x && p.x > -1)
                    if (matrix[p.y + i][p.x] && !R[p.y + i][p.x])
                        way.push(Point(p.x, p.y + i));

            for (int i = -1; i < 2; i += 2)
                if (p.x + i < dimension.x && p.x + i > -1 &&
                    p.y < dimension.y && p.y > -1)
                    if (matrix[p.y][p.x + i] && !R[p.y][p.x + i])
                        way.push(Point(p.x + i, p.y));
        }
        for (std::size_t i = 0; i < R.size(); i++)
            for (std::size_t j = 0; j < R[i].size(); j++)
                sum += R[i][j];
        return sum;
    }


};

class Solver {

 private:

    XMLParser *parser;

    structures::LinkedList<structures::LinkedList<int>> convert(std::string matrix, Point dimension) {
        structures::LinkedList<structures::LinkedList<int>> new_matrix;
        matrix.erase(remove(matrix.begin(), matrix.end(), '\n'), matrix.end());
        int x = 0;
        int y = 0;
        for (int i = 0; i < dimension.y; i++)
            new_matrix.push_back(structures::LinkedList<int>());

        for (auto c : matrix) {
            new_matrix[y].push_back(int(c) - int('0'));
            x++;
            if (x >= dimension.x) {
                x = 0;
                y++;
            }
        }

        return new_matrix;
     }

 public:

    Solver() {
        structures::LinkedList<std::string> tags;
        tags.push_back("cenarios");
        tags.push_back("cenario");
        tags.push_back("nome");
        tags.push_back("dimensoes");
        tags.push_back("altura");
        tags.push_back("largura");
        tags.push_back("robo");
        tags.push_back("x");
        tags.push_back("y");
        tags.push_back("matriz");
        parser = new XMLParser(tags);
        std::cout << "io" << std::endl;
    }

    ~Solver() {
        delete parser;
    }

    void solve() {
        char file_name[100];
        std::cin >> file_name;

        std::ifstream file;
        std::string xml = "";
        file.open(file_name);
        std::string line;
        while (getline(file, line)) xml += line + "\n";
        file.close();


        if (!parser->isValid(xml)) {
            std::cout << "erro" << std::endl << std::endl;
            return;
        }
        structures::LinkedList<std::string> names = parser->get_tags_contents(xml, "nome");
        structures::LinkedList<std::string> width = parser->get_tags_contents(xml, "largura");
        structures::LinkedList<std::string> height = parser->get_tags_contents(xml, "altura");
        structures::LinkedList<std::string> robot_xs = parser->get_tags_contents(xml, "x");
        structures::LinkedList<std::string> robot_ys = parser->get_tags_contents(xml, "y");
        structures::LinkedList<std::string> matrices = parser->get_tags_contents(xml, "matriz");

        Analyzer analyzer;
        for (std::size_t i = 0; i < names.size(); i++) {
            Point robot = Point(std::stoi(robot_xs[i]), std::stoi(robot_ys[i]));
            Point dimension = Point(std::stoi(width[i]), std::stoi(height[i]));
            structures::LinkedList<structures::LinkedList<int>> matrix = convert(matrices[i], dimension);
            std::cout << names[i] << " " << analyzer.analyze(robot, dimension, matrix) << std::endl;
        }
        std::cout << std::endl;
    }

};

int main()
{
    Solver solver;

    solver.solve();

    return 0;
}
