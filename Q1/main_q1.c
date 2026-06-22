#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hanoi_matriz.h"

/* ------------------------------------------------------------------ */
/* Exibe o menu para o usuario digitar uma configuracao               */
/* ------------------------------------------------------------------ */
void ler_configuracao(Configuracao *c, const char *nome)
{
    printf("\n=== Configuracao %s ===\n", nome);
    printf("Informe o pino (1, 2 ou 3) de cada disco (disco 1 = menor):\n");
    for (int i = 0; i < NUM_DISCOS; i++) {
        int p;
        do {
            printf("  Disco %d: ", i + 1);
            scanf("%d", &p);
            if (p < 1 || p > 3)
                printf("  [ERRO] Pino invalido. Digite 1, 2 ou 3.\n");
        } while (p < 1 || p > 3);
        c->pino[i] = p - 1; /* converte para 0-based */
    }
}

/* ------------------------------------------------------------------ */
/* Verifica se uma configuracao e alcancavel (estado valido)           */
/* ------------------------------------------------------------------ */
void exibir_info_grafo(const GrafoMatriz *g)
{
    printf("\n=== Informacoes do Grafo (Matriz de Adjacencia) ===\n");
    printf("  Numero de vertices : %d  (3^%d configuracoes possiveis)\n",
           g->num_vertices, NUM_DISCOS);
    printf("  Numero de arestas  : %d\n", g->num_arestas);
}

/* ================================================================== */
/* main                                                                */
/* ================================================================== */
int main(void)
{
    printf("=======================================================\n");
    printf(" Torre de Hanoi - 5 Discos - Grafo / Matriz Adjacencia \n");
    printf(" Disciplina: Estruturas de Dados II - UFPI/CSHNB       \n");
    printf("=======================================================\n");

    /* ---- 1. Construcao do grafo (feita apenas uma vez) ----------- */
    printf("\n[1] Construindo o grafo de movimentos...\n");

    clock_t t_ini_construcao = clock();
    GrafoMatriz grafo;
    construir_grafo(&grafo);
    clock_t t_fim_construcao = clock();

    double tempo_construcao =
        (double)(t_fim_construcao - t_ini_construcao) / CLOCKS_PER_SEC;

    exibir_info_grafo(&grafo);
    printf("  Tempo de construcao: %.6f segundos\n", tempo_construcao);

    /* ---- 2. Menu Interativo (Loop) ------------------------------- */
    int opcao;
    do {
        printf("\n=======================================================\n");
        printf("[2] MENU DE CONFIGURACOES:\n");
        printf("    1 - Usar configuracao PADRAO (todos discos pino 1 -> pino 3)\n");
        printf("    2 - Informar configuracoes manualmente\n");
        printf("    3 - Sair do programa\n");
        printf("=======================================================\n");
        printf("Opcao: ");
        fflush(stdout); /* Garante que o texto apareça no terminal antes de ler */
        
        /* Proteção contra entrada de letras */
        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n'); 
            opcao = 0; 
        }

        if (opcao == 3) {
            printf("\nEncerrando o programa...\n");
            break;
        }

        if (opcao < 1 || opcao > 3) {
            printf("\n[ERRO] Opcao invalida. Tente novamente.\n");
            continue;
        }

        Configuracao c_orig, c_dest;

        if (opcao == 2) {
            ler_configuracao(&c_orig, "ORIGEM");
            ler_configuracao(&c_dest, "DESTINO");
        } else if (opcao == 1) {
            config_inicial(&c_orig);
            config_final(&c_dest);
        }

        int idx_orig = config_para_indice(&c_orig);
        int idx_dest = config_para_indice(&c_dest);

        printf("\nConfiguracao de origem  : ");
        imprimir_config(&c_orig);
        printf("  (indice %d)\n", idx_orig);

        printf("Configuracao de destino : ");
        imprimir_config(&c_dest);
        printf("  (indice %d)\n", idx_dest);

        if (idx_orig == idx_dest) {
            printf("\nOrigem e destino sao iguais. Nenhum movimento necessario.\n");
            continue; /* Volta pro menu em vez de fechar o programa */
        }

        /* ---- 3. Dijkstra ----------------------------------------- */
        printf("\n[3] Executando Dijkstra (matriz de adjacencia)...\n");

        ResultadoDijkstra resultado;

        clock_t t_ini_dijkstra = clock();
        dijkstra_matriz(&grafo, idx_orig, &resultado);
        clock_t t_fim_dijkstra = clock();

        double tempo_dijkstra =
            (double)(t_fim_dijkstra - t_ini_dijkstra) / CLOCKS_PER_SEC;

        resultado.destino = idx_dest;

        /* ---- 4. Exibicao dos resultados -------------------------- */
        printf("\n=======================================================\n");
        printf(" RESULTADO DA BUSCA\n");
        printf("=======================================================\n");
        imprimir_caminho(&resultado);
        printf("\n  Tempo de Dijkstra   : %.6f segundos\n", tempo_dijkstra);
        printf("  Tempo total (constr + Dijkstra): %.6f segundos\n",
               tempo_construcao + tempo_dijkstra);
        
    } while (opcao != 3);

    return 0;
}