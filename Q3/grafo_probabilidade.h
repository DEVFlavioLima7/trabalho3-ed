#ifndef GRAFO_H
#define GRAFO_H

#define MAX_VERTICES 50

typedef struct
{
    int numVertices;
    float mat[MAX_VERTICES][MAX_VERTICES];
} Grafo;

void inicializarGrafo(Grafo *g, int n);

void inserirAresta(Grafo *g, int origem, int destino, float confiabilidade);

void exibirGrafo(const Grafo *g);

void caminhoMaisConfiavel(Grafo *g, int origem, int destino);

#endif