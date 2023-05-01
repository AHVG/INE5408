#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <cstdint>


// Classe feita nos VPL´s com algumas melhorias
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
// #defines para facilitar a instanciação do ArrayList
#define ListString  ArrayList<std::string>
#define ListInt     ArrayList<int>
#define ListList    ArrayList<ListInt*>
// Observer que a ListList é uma lista de ponteiros para ListInt.
// Se fosse uma lista de ListInt, seria necessário implementar
// métodos de atribuição, de copia e de movimentação.


// Classe implementada nos VPL´s com algumas melhorias
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

        void next(Node* next) {
            next_ = next;
        }

     private:
        T data_;
        Node* next_;
    };

    Node* top_;
    std::size_t size_;
};

// Classe para analisar o XML
class XMLParser {
 private:

    // Duas listas usadas para armazenar as tags de abertura e fechamento
    // A ideia é que nos mesmos índices as listas opening e closing tenham
    // itens correspondentes.
    // Ex.:
    // Suponhamos que temos a tag de abertura chamada <cenario>, sua
    // contraparte seria </cenario>, e que ela está no índice i da lista
    // opening_tags. Quando formos acessar o índice i da lista closing_tags,
    // obteremos a contraparte de <cenario>, ou seja, </cenario>.
    ListString *opening_tags;
    ListString *closing_tags;

 public:
    
    // Recebe as tags apenas para ser o mais generalista possível
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

    // Método utilizado para verificar se o xml é válido
    bool isValid(std::string xml) {
        size_t xml_len = xml.length();

        // Instanciação da pilha de tags, à principio vazia.
        LinkedStack<std::string> tags_stack;

        // O for vai iteirar por todos o xml (xml_len).
        // Isto é, ele vai de caracter por caracter
        for (size_t i = 0; i < xml_len; i++) {
            
            // Este primeiro for é para verificar se os próximos caracteres
            // são uma tag de abertura.
            for (std::size_t j = 0; j < opening_tags->size(); j++)
                if (opening_tags->at(j).length() <= xml_len - i)
                    if (xml.substr(i, opening_tags->at(j).length()) == opening_tags->at(j))
                        tags_stack.push(opening_tags->at(j));

            // O segundo for é para verificar se os mesmos próximos caracteres
            // são uma tag de fechamento.
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

        // Se no fim estiver vazia a pilha de tag é por que o xml é válido
        return tags_stack.empty();
    }

    // Função para retornar a próxima tag de fechamento
    // Off set é para não empilhar a tag de abertura já
    // inserida no início do algoritmo
    std::size_t get_next_closing_tag(std::string xml, std::size_t index, std::size_t off_set) {
        std::size_t xml_len = xml.length();

        // A tag de abertura e fechamento que vai ser analisada
        std::string opening_tag = opening_tags->at(index);
        std::string closing_tag = closing_tags->at(index);

        // Instanciando uma pilha de tags com uma tag de abertura já inserida
        LinkedStack<std::string> tags;
        tags.push(opening_tag);

        // Índice da última tag de fechamento encontrada (vai ser a tag que queremos)
        std::size_t last_closing_tag_index = 0;
        // For que vai percorrer o xml até encontrar a tag de fechamento
        for (size_t i = off_set; i < xml_len; i++) {
            
            // Se tiver uma tag de abertura vai inserir na pilha
            if (opening_tag.length() <= xml_len - i)
                if (xml.substr(i, opening_tag.length()) == opening_tag)
                    tags.push(opening_tag);

            // Se tiver uma tag de fechamento vai retirar da pilha a tag de abertura e
            // armazenar o índice da tag de fechamento
            if (closing_tag.length() <= xml_len - i)
                if (xml.substr(i, closing_tag.length()) == closing_tag) {
                        tags.pop();
                        last_closing_tag_index = i;
                }
            
            // Quando estiver vazia a pilha, quer dizer que já encontramos a tag de fechamento
            // que queriamos
            if (tags.empty())
                break;
        }

        return last_closing_tag_index;
    }

    // Função que retorna a lista de conteudo da tag escolhida
    ListString *get_tags_contents(std::string xml, std::string tag) {
        // Instanciando os conteúdos da tag com 200, apenas porque nenhum
        // arquivo passa de 102
        ListString *tag_contents = new ListString(200);

        // Formatando a tag
        tag = "<" + tag + ">";

        // Verificando se a tag
        std::size_t index = -1;
        for (std::size_t i = 0; i < opening_tags->size(); i++)
            if (opening_tags->at(i) == tag)
                index = i;
        // Se não tiver retorna
        if (index == -1u)
            return tag_contents;
        
        // Se tiver entra no while para analisar o xml e obter os conteúdos
        while(true) {
            // Encontra a próxima tag de abertura
            std::size_t opening_tag_index = xml.find(tag);
            // Se não encontrar, sai do while
            if (opening_tag_index == std::string::npos)
                break;
            
            // Calcula o indice do início do conteúdo
            opening_tag_index += tag.length();

            // Encontra a tag de fechamento correspondente
            std::size_t closing_tag_index = get_next_closing_tag(xml, index, opening_tag_index);

            // Calcula o tamanho do conteúdo
            std::size_t size = closing_tag_index - opening_tag_index;

            // Adiciona o bloco de conteúdo encontrado na lista de conteúdo
            tag_contents->push_back(xml.substr(opening_tag_index, size));
            
            // Calcula o novo tamanho do xml
            size = xml.length() - (closing_tag_index + closing_tags->at(index).length());

            // Atualiza o xml com o bloco depois da tag obtida antes
            xml = xml.substr(closing_tag_index + closing_tags->at(index).length(), size);
        }
        return tag_contents;
    }

    // Uma função que retorna o conteudo de uma hierarquia de tag.
    // Não utilizada no projeto.
    ListString *get_tags_contents(std::string xml, ListString &tag_hierarchy) {
        ListString *contents;
        for (std::size_t i = 0; i < tag_hierarchy.size(); i++) {
            std::string aux = "";
            contents = get_tags_contents(xml, tag_hierarchy[i]);
            for (std::size_t i = 0; i < contents->size(); i++)
                aux += contents->at(i);
            
            // Deletando a conteudo obtido caso não seja a última iteração
            if (i + 1 != tag_hierarchy.size())
                delete contents;
            xml = aux;
        }
        return contents;
    }
};

// Classe criada para facilitar a analise de cada cenário.
// Ela seria a abstração de um ponto ou coordenada na matriz.
// Observe que tudo é público. Foi feito assim, apenas para
// facilitar o acesso as informações do objeto.
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


// Classe que análise cada cenário
class Analyzer {

 public:

    int analyze(Point robot, Point dimension, ListList &matrix) {
        // Caminho
        LinkedStack<Point> way;
        // Area que o robô limpará
        ListList *R = new ListList(dimension.y);

        // O tanto que será limpo (retorno)
        int sum = 0;

        // Criando a área a ser limpa vazia
        for (int i = 0; i < dimension.y; i++) {
            R->push_back(new ListInt(dimension.x));
            for (int j = 0; j < dimension.x; j++)
                R->at(i)->push_back(0);
        }

        // Colocando o robô no way
        way.push(Point(robot.y, robot.x));
        Point aux = way.top();

        // Verificando se o robô está dentro de uma área suja
        // Se não tiver retorna 0
        if (aux.x < dimension.x && aux.x >= 0 &&
            aux.y < dimension.y && aux.y >= 0)
            if (!matrix.at(aux.y)->at(aux.x))
                return sum;

        // Se tiver, começa a anlisar a vizinhança até acabar
        while (!way.empty()) {
            // Retira a vizinhaça
            Point p = way.pop();

            // Define 1 para não analisar novamente a área
            R->at(p.y)->at(p.x) = 1;

            // Analisa as casas de cima e de baixo da atual.
            // Se tiver suja e dentro da matriz, insere no way.
            for (int i = -1; i < 2; i += 2)
                if (p.y + i < dimension.y && p.y + i > -1 &&
                    p.x < dimension.x && p.x > -1)
                    if (matrix.at(p.y + i)->at(p.x) && !R->at(p.y + i)->at(p.x))
                        way.push(Point(p.x, p.y + i));

            // Analisa as casas do lado e do outro da atual.
            // Se tiver suja e dentro da matriz, insere no way.
            for (int i = -1; i < 2; i += 2)
                if (p.x + i < dimension.x && p.x + i > -1 &&
                    p.y < dimension.y && p.y > -1)
                    if (matrix.at(p.y)->at(p.x + i) && !R->at(p.y)->at(p.x + i))
                        way.push(Point(p.x + i, p.y));
        }

        // Faz a contagem do número de casas que serão limpas
        for (std::size_t i = 0; i < R->size(); i++)
            for (std::size_t j = 0; j < R->at(i)->size(); j++)
                sum += R->at(i)->at(j);

        // Deletando a matriz R auxiliar
        for (std::size_t i = 0; i < R->size(); i++)
            delete R->at(i);
        delete R;

        return sum;
    }


};

// Classe que soluciona o problema.
class Solver {

 private:

    XMLParser *parser;

    // Método para converter uma matriz feita em string para uma matriz de inteiros
    ListList *convert(std::string matrix, Point dimension) {
        // Nova matriz de inteiros
        ListList *new_matrix = new ListList(dimension.y);

        // Limpa as quebras de linhas
        matrix.erase(remove(matrix.begin(), matrix.end(), '\n'), matrix.end());
        int x = 0;
        int y = 0;
        // Inicializa a nova matriz
        for (int i = 0; i < dimension.y; i++)
            new_matrix->push_back(new ListInt(dimension.x));

        // Percorrer cada caracter da matriz em string
        for (auto c : matrix) {
            // Faz a conversão de ascii para inteiro
            new_matrix->at(y)->push_back(int(c) - int('0'));
            // Atualiza a coluna
            x++;
            // Se a coluna for maior que a dimensão
            if (x >= dimension.x) {
                // Reseta a coluna
                x = 0;
                // Incrementa a linha
                y++;
            }
        }

        return new_matrix;
     }

 public:

    Solver() {
        // Criando as tags do exercicio
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

        // Entrando com o nome do arquivo
        char file_name[100];
        std::cin >> file_name;

        // Obtendo o conteúdo do arquivo
        std::ifstream file;
        std::string xml = "";
        file.open(file_name);
        std::string line;
        while (getline(file, line)) xml += line + "\n";
        file.close();

        // Verificando se o xml é válido
        if (!parser->isValid(xml)) {
            std::cout << "erro" << std::endl << std::endl;
            return;
        }

        // Obtendo as informações de cada cenário
        ListString *names = parser->get_tags_contents(xml, "nome");
        ListString *width = parser->get_tags_contents(xml, "largura");
        ListString *height = parser->get_tags_contents(xml, "altura");
        ListString *robot_xs = parser->get_tags_contents(xml, "x");
        ListString *robot_ys = parser->get_tags_contents(xml, "y");
        ListString *matrices = parser->get_tags_contents(xml, "matriz");


        // For que passará por cada cenário analisando
        Analyzer analyzer;
        for (std::size_t i = 0; i < names->size(); i++) {

            // Analisando o cenário
            Point robot = Point(std::stoi(robot_xs->at(i)), std::stoi(robot_ys->at(i)));
            Point dimension = Point(std::stoi(width->at(i)), std::stoi(height->at(i)));
            ListList *matrix = convert(matrices->at(i), dimension);
            std::cout << names->at(i) << " " << analyzer.analyze(robot, dimension, *matrix) << std::endl;

            // Destruindo a metriz criada
            for (std::size_t i = 0; i < matrix->size(); i++)
                delete matrix->at(i);
            delete matrix;
        }
        std::cout << std::endl;

        // Deletando as informações obtidas
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
    // Instanciando um solucionador para resolver o problema
    Solver *solver = new Solver();

    // Resolvendo o problema
    solver->solve();

    return 0;
}
