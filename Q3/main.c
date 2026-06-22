#include <stdio.h>
#include "grafo_probabilidade.h"

int main()
{
    Grafo g;

    inicializarGrafo(&g, 50);

    /* =========================
       CAMADA PRINCIPAL (0 -> 49)
       ========================= */

    for(int i = 0; i < 49; i++)
    {
        inserirAresta(&g, i, i + 1, 0.95f);
    }

    /* atalhos principais */
    inserirAresta(&g, 0, 5, 0.80f);
    inserirAresta(&g, 0, 10, 0.85f);
    inserirAresta(&g, 5, 15, 0.88f);
    inserirAresta(&g, 10, 20, 0.90f);
    inserirAresta(&g, 15, 25, 0.87f);
    inserirAresta(&g, 20, 30, 0.86f);
    inserirAresta(&g, 25, 35, 0.88f);
    inserirAresta(&g, 30, 40, 0.89f);
    inserirAresta(&g, 35, 45, 0.90f);

    /* conexões finais múltiplas */
    inserirAresta(&g, 40, 49, 0.92f);
    inserirAresta(&g, 45, 49, 0.93f);

    /* conexões alternativas */
    inserirAresta(&g, 0, 12, 0.78f);
    inserirAresta(&g, 12, 24, 0.84f);
    inserirAresta(&g, 24, 36, 0.86f);
    inserirAresta(&g, 36, 49, 0.91f);

    inserirAresta(&g, 5, 20, 0.82f);
    inserirAresta(&g, 20, 35, 0.85f);
    inserirAresta(&g, 35, 49, 0.90f);

    /* ========================= */

    int origem, destino;

    printf("=====================================\n");
    printf(" CAMINHO MAIS CONFIAVEL (50 VERTICES)\n");
    printf("=====================================\n");

    exibirGrafo(&g);

    printf("Digite origem (0-49): ");
    scanf("%d", &origem);

    printf("Digite destino (0-49): ");
    scanf("%d", &destino);

    if(origem < 0 || origem >= 50 || destino < 0 || destino >= 50)
    {
        printf("Vertice invalido!\n");
        return 1;
    }

    caminhoMaisConfiavel(&g, origem, destino);

    return 0;
}