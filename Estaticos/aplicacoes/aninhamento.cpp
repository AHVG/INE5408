// Copyright [2023] <Augusto de Hollanda Vieira Guerner>
#include <string>
#include "./array_stack.h"


bool verificaChaves(std::string trecho_programa) {
    int  tamanho  = trecho_programa.length();
    structures::ArrayStack<char> pilha(500);
    char begin = '{';
    char end = '}';
    for (int i = 0; i < tamanho; i++) {
        if (trecho_programa.at(i) == begin)
            pilha.push(begin);
        else if (trecho_programa.at(i) == end &&
                 !pilha.empty())
            pilha.pop();
        else if (trecho_programa.at(i) == end &&
                 pilha.empty())
            return false;
    }
    if (!pilha.empty())
        return false;
    return true;
}