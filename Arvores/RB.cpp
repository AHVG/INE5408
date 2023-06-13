#ifndef NO_RB_HPP
#define NO_RB_HPP

#include <vector>

#define RB_RUBRO true
#define RB_NEGRO false

template <typename T>
class NoRB {
 private:
    T* dado;  //!< Atributo onde é guardado o valor representado pelo nó
    NoRB<T>* esquerda;  //!< Atributo que aponta para menor nó descendente
    NoRB<T>* direita;  //!< Atributo que aponta para maior nó descendente
    NoRB<T>* pai;  //!< Atributo que aponta para nó ascendente
    bool cor;  //!< Atributo que indica a cor do nó

 public:
    explicit NoRB(const T& dado);
    virtual ~NoRB();
    NoRB<T>* getPai();
    bool getCor(NoRB<T>*);
    std::vector<NoRB<T>* > getElementos();
    NoRB<T>* getEsquerda();
    NoRB<T>* getDireita();
    NoRB<T>* inserir(const T& dado, NoRB<T>* arv);
    NoRB<T>* remover(NoRB<T>* arv, const T& dado);
    NoRB<T>* minimo(NoRB<T>* nodo);
    T* getDado();
    T* busca(const T& dado, NoRB<T>* arv);
    void preOrdem(NoRB<T>* nodo);
    void emOrdem(NoRB<T>* nodo);
    void posOrdem(NoRB<T>* nodo);
};

#endif /* NO_RB_HPP */