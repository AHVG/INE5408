// Copyright [2023] <Augusto de Hollanda Vieira Guerner>
#include <string>
#include "./array_queue.h"

structures::ArrayQueue<char> editaTexto(std::string texto) {
    structures::ArrayQueue<char> fila(500);
    for (auto c : texto)
        if (c == '<')
            fila.dequeue();
        else if (c == '>')
            fila.enqueue(fila.back());
        else
            fila.enqueue(c);
    return fila;
}