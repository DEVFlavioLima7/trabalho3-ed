#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hanoi_lista.h"
const int pot3[NUM_DISCOS + 1] = {1, 3, 9, 27, 81, 243};


void grafo_inicializar(GrafoLista *g)
{
    g->num_vertices = NUM_VERTICES;
    g->num_arestas = 0;
    for (int i = 0; i < NUM_VERTICES; i++)
        g->lista[i] = NULL;
}

void grafo_liberar(GrafoLista *g)
{
    for (int i = 0; i < NUM_VERTICES; i++) {
        No *atual = g->lista[i];
        while (atual) {
            No *prox = atual->prox;
            free(atual);
            atual = prox;
        }
        g->lista[i] = NULL;
    }
}

/* ================================================================== */
/* Conversoes configuracao <-> indice                                  */
/* ================================================================== */

int config_para_indice(const Configuracao *c)
{
    int idx = 0;
    for (int i = 0; i < NUM_DISCOS; i++)
        idx += c->pino[i] * pot3[i];
    return idx;
}

void indice_para_config(int indice, Configuracao *c)
{
    for (int i = 0; i < NUM_DISCOS; i++) {
        c->pino[i] = indice % 3;
        indice    /= 3;
    }
}
/* ================================================================== */
/* Logica de movimentos                                               */
/* ================================================================== */

int topo_do_pino(const Configuracao *c, int pino)
{
    int topo = -1; /* Inicializa como pino vazio */

    for (int d = 0; d < NUM_DISCOS; d++) {
        if (c->pino[d] == pino) {
            topo = d;
            break; /* Achou o menor disco, interrompe a busca */
        }
    }
    
    return topo; 
}

int movimento_valido(const Configuracao *c, int disco, int pino_dest)
{
    int valido = 0; /* Por padrao, e invalido (0) */
    
    int topo = topo_do_pino(c, c->pino[disco]);
    
    if (topo == disco) {
        int topo_dest = topo_do_pino(c, pino_dest);
        
        if (topo_dest == -1) {
            valido = 1; /* Pino destino vazio */
        } else {
            valido = (topo_dest > disco); /* Valido se disco destino for maior */
        }
    }

    return valido; /* Unico return */
}



int inserir_aresta(GrafoLista *g, int u, int v) 
{
    int inseriu = 0; /* Retorno da funcao: assume que nao inseriu (0) */
    int existe = 0;  /* Flag para saber se a aresta ja existe */
    No *a = g->lista[u];
    
    /* 1. Busca para ver se o vertice ja esta na lista de adjacencia */
    while (a) {
        if (a->vertice == v) {
            existe = 1; 
            break; /* Curto-circuito: ja encontrou, para o loop */
        }
        a = a->prox;
    }
    
    /* 2. Só faz a alocacao e insere se a aresta NAO existir */
    if (!existe) {
        No *n = (No*)malloc(sizeof(No));
        n->vertice = v; 
        n->peso = 1; 
        n->prox = g->lista[u]; 
        g->lista[u] = n;
        
        inseriu = 1; /* Insercao concluida com sucesso */
    }
    
    return inseriu; /* Unico return */
}


void construir_grafo(GrafoLista *g)
{
    grafo_inicializar(g);

    Configuracao c_orig, c_dest;

    for (int v = 0; v < NUM_VERTICES; v++) {
        indice_para_config(v, &c_orig);

        for (int disco = 0; disco < NUM_DISCOS; disco++) {
            int pino_atual = c_orig.pino[disco];
            for (int pino_dest = 0; pino_dest < NUM_PINOS; pino_dest++) {
                if (pino_dest == pino_atual) continue;

                if (movimento_valido(&c_orig, disco, pino_dest)) {
                    c_dest = c_orig;
                    c_dest.pino[disco] = pino_dest;

                    int w = config_para_indice(&c_dest);
                    
                    /* CORREÇÃO AQUI: 
                       Tenta inserir v->w. Se conseguir (retornar 1), insere a 
                       volta w->v e soma 1 aresta. Quando o loop chegar em w e 
                       tentar inserir w->v, a função retornará 0 e não contará duplo. */
                    if (inserir_aresta(g, v, w)) {
                        inserir_aresta(g, w, v);
                        g->num_arestas++;
                    }
                }
            }
        }
    }
}



typedef struct {
    int vertice;
    int dist;
} HeapNo;

typedef struct {
    HeapNo dados[NUM_VERTICES];
    int    pos[NUM_VERTICES];  
    int    tamanho;
} MinHeap;

 void heap_inicializar(MinHeap *h)
{
    h->tamanho = 0;
    for (int i = 0; i < NUM_VERTICES; i++)
        h->pos[i] = -1;
}

void heap_trocar(MinHeap *h, int i, int j)
{
    HeapNo tmp   = h->dados[i];
    h->dados[i]  = h->dados[j];
    h->dados[j]  = tmp;
    h->pos[h->dados[i].vertice] = i;
    h->pos[h->dados[j].vertice] = j;
}

void heap_subir(MinHeap *h, int i)
{
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[pai].dist > h->dados[i].dist) {
            heap_trocar(h, pai, i);
            i = pai;
        } else break;
    }
}

void heap_descer(MinHeap *h, int i)
{
    int menor = i;
    int esq   = 2 * i + 1;
    int dir   = 2 * i + 2;

    if (esq < h->tamanho && h->dados[esq].dist < h->dados[menor].dist)
        menor = esq;
    if (dir < h->tamanho && h->dados[dir].dist < h->dados[menor].dist)
        menor = dir;

    if (menor != i) {
        heap_trocar(h, i, menor);
        heap_descer(h, menor);
    }
}

void heap_inserir(MinHeap *h, int vertice, int dist)
{
    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].dist    = dist;
    h->pos[vertice]     = i;
    heap_subir(h, i);
}

HeapNo heap_extrair_min(MinHeap *h)
{
    HeapNo min   = h->dados[0];
    int ultimo   = --h->tamanho;
    h->dados[0]  = h->dados[ultimo];
    h->pos[h->dados[0].vertice] = 0;
    h->pos[min.vertice] = -1;
    if (h->tamanho > 0)
        heap_descer(h, 0);
    return min;
}

void heap_diminuir_chave(MinHeap *h, int vertice, int nova_dist)
{
    int i = h->pos[vertice];
    
    /* Só atualiza e sobe no heap se o vertice realmente existir na fila */
    if (i != -1) {
        h->dados[i].dist = nova_dist;
        heap_subir(h, i);
    }
    
    /* Fim natural da função void, sem necessidade de return */
}

/* ================================================================== */
/* Dijkstra com lista de adjacencia + min-heap                        */
/* ================================================================== */

void dijkstra_lista(const GrafoLista *g, int origem, ResultadoDijkstra *res)
{
    MinHeap heap;
    heap_inicializar(&heap);

    res->origem  = origem;
    res->destino = -1;

    for (int i = 0; i < NUM_VERTICES; i++) {
        res->dist[i] = INF;
        res->prev[i] = -1;
    }
    res->dist[origem] = 0;

    for (int i = 0; i < NUM_VERTICES; i++)
        heap_inserir(&heap, i, res->dist[i]);

    while (heap.tamanho > 0) {
        HeapNo atual = heap_extrair_min(&heap);
        int u = atual.vertice;

        if (res->dist[u] == INF) break;

        No *viz = g->lista[u];
        while (viz) {
            int v         = viz->vertice;
            int nova_dist = res->dist[u] + viz->peso;

            if (nova_dist < res->dist[v]) {
                res->dist[v] = nova_dist;
                res->prev[v] = u;
                heap_diminuir_chave(&heap, v, nova_dist);
            }
            viz = viz->prox;
        }
    }
}

/* ================================================================== */
/* Impressao                                                           */
/* ================================================================== */

void imprimir_config(const Configuracao *c)
{
    printf("[");
    for (int i = 0; i < NUM_DISCOS; i++) {
        printf("%d", c->pino[i] + 1);
        if (i < NUM_DISCOS - 1) printf(", ");
    }
    printf("]");
}
void imprimir_caminho(const ResultadoDijkstra *res)
{
    int destino = res->destino;

    if (res->dist[destino] == INF) {
        printf("Nao existe caminho entre origem e destino.\n");
    } else {
        /* Se existir caminho, executa a reconstrução e impressão */
        int caminho[NUM_VERTICES];
        int tam = 0;
        
        for (int v = destino; v != -1; v = res->prev[v]) {
            caminho[tam++] = v;
        }

        printf("Numero de movimentos: %d\n", res->dist[destino]);
        printf("Caminho (configuracoes):\n");

        Configuracao c;
        for (int i = tam - 1; i >= 0; i--) {
            indice_para_config(caminho[i], &c);
            printf("  Passo %3d -> indice %3d  ", tam - 1 - i, caminho[i]);
            imprimir_config(&c);
            printf("\n");
        }
    }
    /* Fim natural da função void, sem necessidade de return no meio */
}

void config_inicial(Configuracao *c)
{
    for (int i = 0; i < NUM_DISCOS; i++)
        c->pino[i] = 0;
}

void config_final(Configuracao *c)
{
    for (int i = 0; i < NUM_DISCOS; i++)
        c->pino[i] = 2;
}