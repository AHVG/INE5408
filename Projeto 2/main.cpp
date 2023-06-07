#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#define LETRAS_ALFABETO 27

class NoArvore {
    public:
    NoArvore() {
        for (int i = 0; i < LETRAS_ALFABETO; i++)
            filhos[i] = nullptr;
        letra = '*';
        posicao = 0;
        comprimento = 0;
    }
    NoArvore(char letra_) 
    : NoArvore() {
        letra = letra_;
    }
    NoArvore(char letra_, unsigned long posicao_) 
    : NoArvore(letra_) {
        posicao = posicao_;
    }
    NoArvore(char letra_, unsigned long posicao_, unsigned long comprimento_) 
    : NoArvore(letra_, posicao_) {
        comprimento = comprimento_;
    }
    ~NoArvore() {

    }

    int procurar_letra(char letra) {
        int i;
        for (i = 0; filhos[i]; i++)
            if (filhos[i]->letra == letra)
                break;
        return i;
    }

    void inserir(std::string palavra, int indice, unsigned long posicao_, unsigned long comprimento_) {
        int i = procurar_letra(palavra[indice]);
        if (filhos[i]) {
            if (palavra.length() - 1 == indice) {
                filhos[i]->posicao = posicao_;
                filhos[i]->comprimento = comprimento_;
            } else {
                filhos[i]->inserir(palavra, indice + 1, posicao_, comprimento_);
            }
        } else {
            filhos[i] = new NoArvore(palavra[indice]);
            if (palavra.length() - 1 == indice) {
                filhos[i]->posicao = posicao_;
                filhos[i]->comprimento = comprimento_;
            } else {
                filhos[i]->inserir(palavra, indice + 1, posicao_, comprimento_);
            }
        }
    }

    void deletar() {
        for (int i = 0; filhos[i]; i++)
            filhos[i]->deletar();
        // std::cout << "Deletando ";
        // mostrar();
        delete this;
    }

    int contar_palavras() {
        int numero_de_nos = 0;
        if (comprimento)
            numero_de_nos = 1;  
        for (int i = 0; filhos[i]; i++)
            numero_de_nos += filhos[i]->contar_palavras();
        return numero_de_nos;
    }

    NoArvore *prefix(std::string palavra, int indice) {
        int i = procurar_letra(palavra[indice]);
        if (palavra.length() - 1 == indice)
            return filhos[i];
        if (filhos[i])
            return filhos[i]->prefix(palavra, indice + 1);
        return nullptr;
    }

    void mostrar() {
        std::cout << "No Arvore" << std::endl;
        std::cout << "  Letra:       " << letra << std::endl;
        std::cout << "  Posicao:     " << posicao << std::endl;
        std::cout << "  Comprimento: " << comprimento << std::endl;
        std::cout << std::endl;
    }

    char letra;
    NoArvore *filhos[LETRAS_ALFABETO];
    unsigned long posicao;
    unsigned long comprimento;
};

class Arvore {

 public:

    Arvore() {
        raiz = new NoArvore();
    }
    ~Arvore() {
        raiz->deletar();
    }

    void inserir(std::string palavra, int posicao, int comprimento) {
        raiz->inserir(palavra, 0, posicao, comprimento);
    }

    void mostrar_arvore(NoArvore *raiz, int nivel) {
        if (!raiz)
            return;
        
        for (int i = 0; raiz->filhos[i]; i++) {
            std::cout << "Nivel: " << nivel << std::endl;
            std::cout << "Letra pai: " << raiz->letra << std::endl; 
            raiz->filhos[i]->mostrar();
            mostrar_arvore(raiz->filhos[i], nivel + 1);
        }
    }

    NoArvore *prefix(std::string palavra) {
        return raiz->prefix(palavra, 0);
    }

    void mostrar() {
        mostrar_arvore(raiz, 0);
    }

    NoArvore *raiz;

 private:


};



int main() {

    Arvore arvore;

    std::string file_name("dicionarios/dicionario2.dic");

    // std::cin >> file_name;  // entrada

    // Leitura do dic
    std::ifstream file;
    std::string dic = "";
    file.open(file_name);
    std::string line;
    while (getline(file, line)) dic += line + "\n"; // Sera que precisa do "\n"?
    file.close();

    // Pegar cada plavra do dic e suas informacoes como posicao e comprimento
    std::vector<std::string> palavras_dic;
    std::vector<unsigned long> posicoes;
    std::vector<unsigned long> comprimentos;
    
    std::size_t off_set = 0;
    std::size_t maximo_de_interacoes = 30000, interacao = 0;
    while (1) {
        std::size_t inicio = dic.find('[', off_set);
        std::size_t fim = dic.find(']', off_set);
        off_set = fim + 1;
        if (inicio == std::string::npos)
            break;
        std::string palavra = dic.substr(inicio + 1, fim - inicio - 1);
        unsigned long posicao = inicio;
        unsigned long comprimento = dic.find('[', off_set) == std::string::npos ? dic.length() : dic.find('[', off_set);
        comprimento += - inicio - 1;
        palavras_dic.push_back(palavra);
        posicoes.push_back(posicao);
        comprimentos.push_back(comprimento);
        if (interacao > maximo_de_interacoes) {
            std::cout << "Erro no loop!" << std::endl;
            return 1;
        }
    }
    std::cout << "Aqui" << std::endl;

    // Construcao da arvore
    for (int i = 0; i < palavras_dic.size(); i++) {
        std::cout << palavras_dic[i] << std::endl;
        arvore.inserir(palavras_dic[i], posicoes[i], comprimentos[i]);
    }
    
    for (int i = 0; i < LETRAS_ALFABETO; i++) {
        if (arvore.raiz->filhos[i])
            std::cout << arvore.raiz->filhos[i]->letra << std::endl;
    }
    // arvore.mostrar();

    // Leitura das palavras (armazenar as palavras num vector)
    std::vector<std::string> palavras;
    while (1) {
        std::string palavra;
        std::cin >> palavra;
        if (palavra.compare("0") == 0) {
            break;
        }
        palavras.push_back(palavra);
    }

    // Gerar as entradas desejadas
    for (auto palavra : palavras) {
        NoArvore *aux = arvore.prefix(palavra);
        if (aux == nullptr) {
            std::cout << palavra << " is not prefix" << std::endl;
        } else {
            std::cout << palavra << " is prefix of " << aux->contar_palavras() << " words" << std::endl;
            if (aux->comprimento)
                std::cout << palavra << " is at (" << aux->posicao << "," << aux->comprimento << ")" << std::endl;
        }
    }

    return 0;
}