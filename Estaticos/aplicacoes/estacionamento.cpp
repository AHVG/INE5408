// Copyright [2023] <Augusto de Hollanda Vieira Guerner>

/*
    *** Importante ***

    O código de fila em vetor está disponível internamente (não precisa de implementação aqui)

*/



void retira_veiculo(structures::ArrayQueue<int> &f, int k) {
    for (int i = 0; i < k - 1; i++) {
        int aux = f.dequeue();
        f.enqueue(aux);
    }
    f.dequeue();
}


void mantenha_veiculo(structures::ArrayQueue<int> &f, int k) {
    for (int i = 0; i < k - 1; i++)
        f.dequeue();
    int carroK = f.dequeue();
    f.clear();
    f.enqueue(carroK);
}



/*
    *** Importante ***

    A função 'main()' não deve ser escrita aqui, pois é parte do código dos testes e já está implementada

*/
