 

#ifndef TAD_H
#define TAD_H

// Estrutura que representa a Tabela Hashing
typedef struct {
    int *vetor;
    int tamanho;
    int ocupados;
} TabelaHash;

// Funções da TAD
TabelaHash* criarTabela(int tamanho);
void inserirChave(TabelaHash *tabela, int chave);
void imprimirTabela(TabelaHash *tabela);
void liberarTabela(TabelaHash *tabela);

#endif