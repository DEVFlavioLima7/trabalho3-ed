
#include <stdio.h>
#include <stdlib.h>
#include "tad.h"


int extrairDigitos(int chave) {
    // Exemplo: 274562 -> 274562 / 100 = 2745 -> 2745 % 1000 = 745
    return (chave / 100) % 1000;
}

// Função de hash inicial baseada no método da divisão inteira (com o +1 do enunciado)
int hashInicial(int valorExtraido, int tamanho) {
    return (valorExtraido % tamanho) + 1;
}

// Função para recalcular o hash em caso de colisões (Gerador pseudo-aleatório)
int rehash(int enderecoAnterior, int tamanho) {
    int a = 3;
    int c = -1;
    
    int valor = (a * enderecoAnterior + c) % tamanho;
    
    // Tratamento para evitar módulo de número negativo no C
    if (valor < 0) {
        valor += tamanho; 
    }
    
    return valor + 1;
}

// Cria e inicializa a tabela hashing
TabelaHash* criarTabela(int tamanho) {
    TabelaHash *tabela = (TabelaHash*) malloc(sizeof(TabelaHash));
    tabela->tamanho = tamanho;
    tabela->ocupados = 0;
    
    // Aloca tamanho + 1 pois as posições requisitadas no problema vão de 1 até M
    tabela->vetor = (int*) malloc((tamanho + 1) * sizeof(int));
    
    // Inicializa todas as posições com -1 (indicando vazio)
    for (int i = 0; i <= tamanho; i++) {
        tabela->vetor[i] = -1; 
    }
    
    return tabela;
}

// Insere uma chave na tabela hashing tratando colisões
void inserirChave(TabelaHash *tabela, int chave) {
    int extraido = extrairDigitos(chave);
    int posInicial = hashInicial(extraido, tabela->tamanho);
    int posAtual = posInicial;
    int colisoes = 0;

    printf("\n>>> Inserindo chave %d...\n", chave);
    printf("    Digitos extraidos: %d | Hash Inicial (Tentativa 1): %d\n", extraido, posInicial);

    // Loop de colisão
    while (tabela->vetor[posAtual] != -1 && tabela->vetor[posAtual] != chave) {
        colisoes++;
        printf("    -> Colisao detectada na posicao %d.\n", posAtual);
        
        posAtual = rehash(posAtual, tabela->tamanho);
        printf("    -> Recalculando hash para a posicao: %d\n", posAtual);

        // Regra de parada: se retornar a primeira encontrada, sobrepõe.
        if (posAtual == posInicial) {
            printf("    [!] Ciclo completo detectado. Sobrescrevendo a posicao inicial %d.\n", posInicial);
            break;
        }
    }

    // Se estava vazio antes de inserir, aumenta o contador de ocupação
    if (tabela->vetor[posAtual] == -1) {
        tabela->ocupados++;
    }
    
    tabela->vetor[posAtual] = chave;
    printf("=> Sucesso! Chave %d salva na posicao %d com %d colisao(oes).\n", chave, posAtual, colisoes);
}

// Imprime o estado atual da tabela e sua densidade
void imprimirTabela(TabelaHash *tabela) {
    printf("\n============= TABELA HASH =============\n");
    printf("Tamanho: %d | Ocupacao: %d\n\n", tabela->tamanho, tabela->ocupados);
    
    // Imprime a partir do índice 1, baseando-se na regra do enunciado
    for (int i = 1; i <= tabela->tamanho; i++) {
        if (tabela->vetor[i] != -1) {
            printf("  [Posicao %02d] -> %d\n", i, tabela->vetor[i]);
        } else {
            printf("  [Posicao %02d] -> VAZIO\n", i);
        }
    }
    
    float densidade = ((float)tabela->ocupados / tabela->tamanho) * 100;
    printf("\nDensidade atual da lista: %.2f%%\n", densidade);
    printf("=======================================\n");
}

// Libera a memória alocada dinamicamente
void liberarTabela(TabelaHash *tabela) {
    if (tabela != NULL) {
        free(tabela->vetor);
        free(tabela);
    }
}