#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hanoi_lista.h"


static void ler_configuracao(Configuracao *c, const char *nome)
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
        c->pino[i] = p - 1; /* Transforma pino 1,2,3 em indices 0,1,2 */
    }
}

static void exibir_info_grafo(const GrafoLista *g)
{
    printf("\n=== Informacoes do Grafo (Lista de Adjacencia) ===\n");
    printf("  Numero de vertices : %d  (3^%d configuracoes possiveis)\n", g->num_vertices, NUM_DISCOS);
    printf("  Numero de arestas  : %d\n", g->num_arestas);

    long mem_lista = (long)g->num_arestas * 2 * (long)sizeof(No) + (long)NUM_VERTICES * (long)sizeof(No *);
    long mem_matriz = (long)NUM_VERTICES * (long)NUM_VERTICES * (long)sizeof(int);

    printf("\n  Memoria estimada (lista)    : %ld bytes (~%ld KB)\n", mem_lista, mem_lista / 1024);
    printf("  Memoria equivalente (matriz): %ld bytes (~%ld KB)\n", mem_matriz, mem_matriz / 1024);
    printf("  Economia de memoria         : %ld bytes\n", mem_matriz - mem_lista);
}


int main(void)
{
    printf("=======================================================\n");
    printf(" Torre de Hanoi - 5 Discos - Grafo / Lista Adjacencia  \n");
    printf(" Disciplina: Estruturas de Dados II - UFPI/CSHNB       \n");
    printf("=======================================================\n");


    printf("\n[1] Construindo o grafo de movimentos...\n");

    GrafoLista grafo;
    clock_t t_ini_construcao = clock();
    construir_grafo(&grafo);
    clock_t t_fim_construcao = clock();

    double tempo_construcao = (double)(t_fim_construcao - t_ini_construcao) / CLOCKS_PER_SEC;

    exibir_info_grafo(&grafo);
    printf("  Tempo de construcao: %.6f segundos\n", tempo_construcao);

    int opcao;
    do {
        printf("\n=======================================================\n");
        printf("[2] MENU DE CONFIGURACOES:\n");
        printf("    1 - Usar configuracao PADRAO (todos discos pino 1 -> pino 3)\n");
        printf("    2 - Informar configuracoes manualmente\n");
        printf("    3 - Sair do programa\n");
        printf("=======================================================\n");
        printf("Opcao: ");
        fflush(stdout); /* Força a exibição imediata no terminal */
        
        if (scanf("%d", &opcao) != 1) {
            /* Limpa o buffer caso o usuario digite uma letra por engano */
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
            continue; /* Volta pro menu sem rodar o Dijkstra */
        }

        /* ---- 3. Dijkstra ----------------------------------------- */
        printf("\n[3] Executando Dijkstra (lista de adjacencia + min-heap)...\n");

        ResultadoDijkstra resultado;

        clock_t t_ini_dijkstra = clock();
        dijkstra_lista(&grafo, idx_orig, &resultado);
        clock_t t_fim_dijkstra = clock();

        double tempo_dijkstra = (double)(t_fim_dijkstra - t_ini_dijkstra) / CLOCKS_PER_SEC;

        resultado.destino = idx_dest;

        /* ---- 4. Resultados --------------------------------------- */
        printf("\n=======================================================\n");
        printf(" RESULTADO DA BUSCA\n");
        printf("=======================================================\n");
        imprimir_caminho(&resultado);
        printf("\n  Tempo de Dijkstra   : %.6f segundos\n", tempo_dijkstra);
        printf("  Tempo total (constr + Dijkstra): %.6f segundos\n", tempo_construcao + tempo_dijkstra);
        
        /* Imprime a comparação pedida na Q2 apenas após o resultado */
        printf("\n--- Comparacao Q1 vs Q2 ---\n");
        printf("  Dijkstra em Matriz : O(V^2)\n");
        printf("  Dijkstra em Lista  : O((V+E)logV) -> Mais rapido com poucos movimentos (arestas)\n");
        
    } while (opcao != 3);

    /* ---- 5. Fim do Programa -------------------------------------- */
    grafo_liberar(&grafo);
    return 0;
}