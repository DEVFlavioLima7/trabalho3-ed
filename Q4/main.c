

#include <stdio.h>
#include <stdlib.h>
#include "tad.h"

int main() {
    int tamanhoVetor, opcao, chave;
    int statusRetorno = 0; 

    printf("========= EXERCICIO 4 - TABELA HASHING =========\n");
    printf("Digite o tamanho desejado para o vetor de encaixe: ");
    
    if (scanf("%d", &tamanhoVetor) != 1 || tamanhoVetor <= 0) {
        printf("Tamanho invalido! O programa sera encerrado.\n");
        statusRetorno = 1; // Define o status de erro
    } else {
        TabelaHash *tabela = criarTabela(tamanhoVetor);

        do {
            printf("\n------------- MENU -------------\n");
            printf("1. Inserir nova chave (6 digitos)\n");
            printf("2. Imprimir tabela hashing\n");
            printf("0. Sair do programa\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao);

            switch (opcao) {
                case 1:
                    printf("\nDigite a chave (exemplo: 274562): ");
                    scanf("%d", &chave);
                    inserirChave(tabela, chave);
                    break;
                case 2:
                    imprimirTabela(tabela);
                    break;
                case 0:
                    printf("\nLiberando memoria e encerrando o programa...\n");
                    break;
                default:
                    printf("\nOpcao invalida. Tente novamente.\n");
            }
        } while (opcao != 0);

        liberarTabela(tabela);
    }

    return statusRetorno; 
}