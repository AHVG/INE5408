#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <cstdint>


#include <iostream>
#include <stdexcept>
#include <cstdint>


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


#define ListString  ArrayList<std::string>
#define ListInt     ArrayList<int>
#define ListList    ArrayList<ListInt*>


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
    }

    void push(const T& data) {
        Node *new_top = new Node(data, top_);
        top_ = new_top;
        size_++;
    }

    T pop() {
        if (empty())
            throw std::out_of_range("Pilha vazia!");
        T data = top_->data();
        Node *next = top_->next();
        delete top_;
        top_ = next;
        size_--;
        return data;
    }

    T& top() const {
        if (empty())
            throw std::out_of_range("Pilha vazia!");
        return top_->data();
    }

    bool empty() const {
        return size_ == 0;
    }

    std::size_t size() const {
        return size_;
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

class XMLParser {
 private:

    ListString *opening_tags;
    ListString *closing_tags;

 public:
    
    XMLParser(ListString &tags) {
        opening_tags = new ListString(tags.size());
        closing_tags = new ListString(tags.size());
        for (std::size_t i = 0; i < tags.size(); i++) {
            opening_tags->push_back("<" + tags[i] + ">");
            closing_tags->push_back("</" + tags[i] + ">");
        }
    }
    ~XMLParser() {
        delete opening_tags;
        delete closing_tags;
    }

    bool isValid(std::string xml) {
        size_t xml_len = xml.length();
        LinkedStack<std::string> tags_stack;
        for (size_t i = 0; i < xml_len; i++) {
            for (std::size_t j = 0; j < opening_tags->size(); j++)
                if (opening_tags->at(j).length() <= xml_len - i)
                    if (xml.substr(i, opening_tags->at(j).length()) == opening_tags->at(j))
                        tags_stack.push(opening_tags->at(j));

            for (std::size_t j = 0; j < closing_tags->size(); j++)
                if (closing_tags->at(j).length() <= xml_len - i)
                    if (xml.substr(i, closing_tags->at(j).length()) == closing_tags->at(j)) {
                        if (tags_stack.empty())
                            return false;
                        else if (tags_stack.top() == opening_tags->at(j))
                            tags_stack.pop();
                        else
                            return false;
                    }
        }
        return tags_stack.empty();
    }

    ListString *get_tags_contents(std::string xml, std::string tag) {
        ListString *tag_contents = new ListString(200);
        tag = "<" + tag + ">";
        std::size_t index = -1;
        for (std::size_t i = 0; i < opening_tags->size(); i++)
            if (opening_tags->at(i) == tag)
                index = i;
        if (index == -1u)
            return tag_contents;
        while(true) {
            std::size_t opening_tag_index = xml.find(tag);
            if (opening_tag_index == std::string::npos)
                break;
            opening_tag_index += tag.length();
            std::size_t closing_tag_index = xml.find(closing_tags->at(index));
            std::size_t size = closing_tag_index - opening_tag_index;
            tag_contents->push_back(xml.substr(opening_tag_index, size));
            size = xml.length() - (closing_tag_index + closing_tags->at(index).length());
            xml = xml.substr(closing_tag_index + closing_tags->at(index).length(), size);
        }
        return tag_contents;
    }

    ListString *get_tags_contents(std::string xml, ListString &tag_hierarchy) {
        ListString *contents;
        for (std::size_t i = 0; i < tag_hierarchy.size(); i++) {
            std::string aux = "";
            contents = get_tags_contents(xml, tag_hierarchy[i]);
            for (std::size_t i = 0; i < contents->size(); i++)
                aux += contents->at(i);
            if (i + 1 != tag_hierarchy.size())
                delete contents;
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

    int analyze(Point robot, Point dimension, ListList &matrix) {
        LinkedStack<Point> way;
        ListList *R = new ListList(dimension.y);
        int sum = 0;
        for (int i = 0; i < dimension.y; i++) {
            R->push_back(new ListInt(dimension.x));
            for (int j = 0; j < dimension.x; j++)
                R->at(i)->push_back(0);
        }
        way.push(Point(robot.y, robot.x));
        Point aux = way.top();
        if (aux.x < dimension.x && aux.x >= 0 &&
            aux.y < dimension.y && aux.y >= 0)
            if (!matrix.at(aux.y)->at(aux.x))
                return sum;

        while (!way.empty()) {
            Point p = way.pop();
            R->at(p.y)->at(p.x) = 1;

            for (int i = -1; i < 2; i += 2)
                if (p.y + i < dimension.y && p.y + i > -1 &&
                    p.x < dimension.x && p.x > -1)
                    if (matrix.at(p.y + i)->at(p.x) && !R->at(p.y + i)->at(p.x))
                        way.push(Point(p.x, p.y + i));

            for (int i = -1; i < 2; i += 2)
                if (p.x + i < dimension.x && p.x + i > -1 &&
                    p.y < dimension.y && p.y > -1)
                    if (matrix.at(p.y)->at(p.x + i) && !R->at(p.y)->at(p.x + i))
                        way.push(Point(p.x + i, p.y));
        }
        for (std::size_t i = 0; i < R->size(); i++)
            for (std::size_t j = 0; j < R->at(i)->size(); j++)
                sum += R->at(i)->at(j);
        for (std::size_t i = 0; i < R->size(); i++)
            delete R->at(i);
        delete R;
        return sum;
    }


};

class Solver {

 private:

    XMLParser *parser;

    ListList *convert(std::string matrix, Point dimension) {
        ListList *new_matrix = new ListList(dimension.y);
        matrix.erase(remove(matrix.begin(), matrix.end(), '\n'), matrix.end());
        int x = 0;
        int y = 0;
        for (int i = 0; i < dimension.y; i++)
            new_matrix->push_back(new ListInt(dimension.x));

        for (auto c : matrix) {
            new_matrix->at(y)->push_back(int(c) - int('0'));
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
        ListString tags(10);
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
        ListString *names = parser->get_tags_contents(xml, "nome");
        ListString *width = parser->get_tags_contents(xml, "largura");
        ListString *height = parser->get_tags_contents(xml, "altura");
        ListString *robot_xs = parser->get_tags_contents(xml, "x");
        ListString *robot_ys = parser->get_tags_contents(xml, "y");
        ListString *matrices = parser->get_tags_contents(xml, "matriz");

        Analyzer analyzer;
        for (std::size_t i = 0; i < names->size(); i++) {
            Point robot = Point(std::stoi(robot_xs->at(i)), std::stoi(robot_ys->at(i)));
            Point dimension = Point(std::stoi(width->at(i)), std::stoi(height->at(i)));
            ListList *matrix = convert(matrices->at(i), dimension);
            std::cout << names->at(i) << " " << analyzer.analyze(robot, dimension, *matrix) << std::endl;

            for (std::size_t i = 0; i < matrix->size(); i++)
                delete matrix->at(i);
            delete matrix;
        }
        std::cout << std::endl;
        delete names;
        delete width;
        delete height;
        delete robot_xs;
        delete robot_ys;
        delete matrices;
    }

};

int main()
{
    Solver *solver = new Solver();

    solver->solve();

    return 0;
}
