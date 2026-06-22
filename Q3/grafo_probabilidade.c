#include <stdio.h>
#include "grafo_probabilidade.h"

void inicializarGrafo(Grafo *g, int n)
{
    g->numVertices = n;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            g->mat[i][j] = 0.0f;
        }
    }
}

void inserirAresta(Grafo *g, int origem, int destino, float confiabilidade)
{
    g->mat[origem][destino] = confiabilidade;
}

void exibirGrafo(const Grafo *g)
{
    printf("\n=== ARESTAS DO GRAFO ===\n");

    for(int i = 0; i < g->numVertices; i++)
    {
        for(int j = 0; j < g->numVertices; j++)
        {
            if(g->mat[i][j] > 0)
            {
                printf("%d -> %d (%.2f)\n", i, j, g->mat[i][j]);
            }
        }
    }
    printf("\n");
}

int escolherMaior(float conf[], int vis[], int n)
{
    float maior = -1;
    int idx = -1;

    for(int i = 0; i < n; i++)
    {
        if(!vis[i] && conf[i] > maior)
        {
            maior = conf[i];
            idx = i;
        }
    }
    return idx;
}

void caminhoMaisConfiavel(Grafo *g, int origem, int destino)
{
    float confiabilidade[MAX_VERTICES];
    int visitado[MAX_VERTICES];
    int anterior[MAX_VERTICES];

    int n = g->numVertices;

    for(int i = 0; i < n; i++)
    {
        confiabilidade[i] = 0.0f;
        visitado[i] = 0;
        anterior[i] = -1;
    }

    confiabilidade[origem] = 1.0f;

    for(int i = 0; i < n; i++)
    {
        int u = escolherMaior(confiabilidade, visitado, n);
        if(u == -1) break;

        visitado[u] = 1;

        for(int v = 0; v < n; v++)
        {
            if(g->mat[u][v] > 0)
            {
                float nova = confiabilidade[u] * g->mat[u][v];

                if(nova > confiabilidade[v])
                {
                    confiabilidade[v] = nova;
                    anterior[v] = u;
                }
            }
        }
    }

    if(confiabilidade[destino] == 0)
    {
        printf("\nNao existe caminho entre %d e %d.\n", origem, destino);
        return;
    }

    printf("\n=== RESULTADO ===\n");
    printf("Confiabilidade maxima: %.6f\n", confiabilidade[destino]);

    int caminho[MAX_VERTICES];
    int tam = 0;

    for(int at = destino; at != -1; at = anterior[at])
    {
        caminho[tam++] = at;
    }

    printf("Caminho: ");

    for(int i = tam - 1; i >= 0; i--)
    {
        printf("%d", caminho[i]);
        if(i > 0) printf(" -> ");
    }

    printf("\n");
}