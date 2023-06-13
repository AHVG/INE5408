#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

#define LETRAS_ALFABETO 26

class NoArvore {
    public:

    NoArvore() {
        for (int i = 0; i < LETRAS_ALFABETO; i++) filhos[i] = nullptr;
        letra = '*';
        posicao = 0;
        comprimento = 0;
        prefixos_ = 0;
    }
    NoArvore(char letra_) : NoArvore() {letra = letra_;}
    NoArvore(char letra_, unsigned long posicao_) : NoArvore(letra_) {posicao = posicao_;}
    NoArvore(char letra_, unsigned long posicao_, unsigned long comprimento_) : NoArvore(letra_, posicao_) {comprimento = comprimento_;}
    ~NoArvore() { }

    unsigned long prefixos() {return prefixos_;}

    char letra;
    NoArvore *filhos[LETRAS_ALFABETO];
    unsigned long posicao;
    unsigned long comprimento;
    unsigned long prefixos_;
};

class Arvore {

 public:

    Arvore() {raiz = new NoArvore();}
    ~Arvore() {deletar(raiz); raiz = nullptr;}

    void paraMinusculo(std::string &texto) {std::transform(texto.begin(), texto.end(), texto.begin(), [](unsigned char c) {return std::tolower(c);});}

    void deletar(NoArvore *raiz) {
        for (int i = 0; i < LETRAS_ALFABETO; i++) if (raiz->filhos[i]) deletar(raiz->filhos[i]);
        delete raiz;
    }

    void inserir(std::string palavra, int posicao, int comprimento) {
        NoArvore *atual = raiz;
        paraMinusculo(palavra);
        for (int i = 0; i < palavra.length(); i++) {
            if (!atual->filhos[palavra[i] - 'a']) atual->filhos[palavra[i] - 'a'] = new NoArvore(palavra[i]);
            atual = atual->filhos[palavra[i] - 'a'];
            atual->prefixos_++;
        }
        atual->comprimento = comprimento;
        atual->posicao = posicao;
    }

    NoArvore *prefixos(std::string palavra) {
        NoArvore *atual = raiz;
        for (int i = 0; i < palavra.length(); i++) {
            if (!atual->filhos[palavra[i] - 'a']) return atual->filhos[palavra[i] - 'a'];
            atual = atual->filhos[palavra[i] - 'a'];
        }
        return atual;
    }

    NoArvore *raiz;

};

int main() {

    Arvore arvore;
    std::string file_name;

    // Leitura do dic
    std::cin >> file_name;
    std::ifstream file;
    std::string dic = "";
    file.open(file_name);
    std::string line;
    while (getline(file, line)) dic += line + "\n"; // Sera que precisa do "\n"?
    file.close();

    // Pegar cada plavra do dic e suas informacoes como posicao e comprimento
    std::vector<std::string> palavrasDic;
    std::vector<unsigned long> posicoes;
    std::vector<unsigned long> comprimentos;
    std::size_t off_set = 0;
    while (1) {
        std::size_t inicio = dic.find('[', off_set);
        std::size_t fim = dic.find(']', off_set);
        if (inicio == std::string::npos) break;
        std::string palavra = dic.substr(inicio + 1, fim - inicio - 1);
        unsigned long posicao = inicio;
        unsigned long comprimento = dic.find('\n', off_set) == std::string::npos ? dic.length() : dic.find('\n', off_set);
        off_set = comprimento + 1;
        comprimento -= inicio;
        palavrasDic.push_back(palavra);
        posicoes.push_back(posicao);
        comprimentos.push_back(comprimento);
    }

    // Construcao da arvore
    for (int i = 0; i < palavrasDic.size(); i++) arvore.inserir(palavrasDic[i], posicoes[i], comprimentos[i]);

    // Leitura das palavras (armazenar as palavras num vector)
    std::vector<std::string> palavras;
    while (1) {
        std::string palavra;
        std::cin >> palavra;
        if (palavra.compare("0") == 0) break;
        palavras.push_back(palavra);
    }

    // Gerar as entradas desejadas
    for (auto palavra : palavras) {
        NoArvore *aux = arvore.prefixos(palavra);
        if (!aux) {
            std::cout << palavra << " is not prefix" << std::endl;
        } else {
            std::cout << palavra << " is prefix of " << aux->prefixos() << " words" << std::endl;
            if (aux->comprimento) std::cout << palavra << " is at (" << aux->posicao << "," << aux->comprimento << ")" << std::endl;
        }
    }

    return 0;
}