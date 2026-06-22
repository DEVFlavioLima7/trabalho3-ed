#include <stdio.h>
#include <string.h>
#include "hanoi_matriz.h"

/* ------------------------------------------------------------------ */
/* Potencias de 3 pre-calculadas para a conversao base-3              */
/* pot3[i] = 3^i                                                       */
const int pot3[NUM_DISCOS + 1] = {1, 3, 9, 27, 81, 243};

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
/* Logica de movimentos validos                                        */
/* ================================================================== */

int topo_do_pino(const Configuracao *c, int pino)
{
    int topo = -1; /* Inicializa com o valor padrão (pino vazio) */

    /*
     * O disco "de topo" em um pino e o menor disco presente nele,
     * pois discos menores ficam sobre os maiores.
     * Discos sao numerados 0 (menor) a N-1 (maior).
     */
    for (int d = 0; d < NUM_DISCOS; d++) {
        if (c->pino[d] == pino) {
            topo = d; /* Armazena o disco encontrado */
            break;    /* Interrompe o laço, pois já achou o menor disco */
        }
    }

    return topo; /* Único return da função */
}

int movimento_valido(const Configuracao *c, int disco, int pino_dest)
{
    int valido = 0; /* Por padrao, assumimos que o movimento e invalido (0) */
    
    /* 1) O disco deve ser o topo do pino onde esta */
    int topo = topo_do_pino(c, c->pino[disco]);
    
    if (topo == disco) {
        /* Se ele for o topo, passamos para a segunda validacao */
        
        /* 2) O destino deve estar vazio ou ter disco maior no topo */
        int topo_dest = topo_do_pino(c, pino_dest);
        
        if (topo_dest == -1) {
            valido = 1; /* pino destino vazio, movimento super valido */
        } else {
            valido = (topo_dest > disco); /* valido apenas se o disco de destino for maior */
        }
    }
    /* Se (topo != disco), o bloco if nem executa e a variavel 'valido' continua sendo 0 */

    return valido; /* Unico return da funcao */
}

/* ================================================================== */
/* Construcao do grafo                                                 */
/* ================================================================== */

void construir_grafo(GrafoMatriz *g)
{
    g->num_vertices = NUM_VERTICES;
    g->num_arestas  = 0;

    /* Zera a matriz */
    memset(g->adj, 0, sizeof(g->adj));

    Configuracao c_orig, c_dest;

    for (int v = 0; v < NUM_VERTICES; v++) {
        indice_para_config(v, &c_orig);

        /* Tenta mover cada disco para cada pino diferente do atual */
        for (int disco = 0; disco < NUM_DISCOS; disco++) {
            int pino_atual = c_orig.pino[disco];
            for (int pino_dest = 0; pino_dest < NUM_PINOS; pino_dest++) {
                if (pino_dest == pino_atual)
                    continue;

                if (movimento_valido(&c_orig, disco, pino_dest)) {
                    /* Gera a configuracao vizinha */
                    c_dest = c_orig;
                    c_dest.pino[disco] = pino_dest;

                    int u = v;
                    int w = config_para_indice(&c_dest);

                    if (!g->adj[u][w]) {
                        g->adj[u][w] = 1;
                        g->adj[w][u] = 1; /* grafo nao-orientado */
                        g->num_arestas++;
                    }
                }
            }
        }
    }
}

/* ================================================================== */
/* Dijkstra sobre matriz de adjacencia (pesos = 1)                    */
/* ================================================================== */

void dijkstra_matriz(const GrafoMatriz *g, int origem, ResultadoDijkstra *res)
{
    int visitado[NUM_VERTICES] = {0};

    res->origem = origem;
    res->destino = -1;

    /* Inicializacao */
    for (int i = 0; i < NUM_VERTICES; i++) {
        res->dist[i] = INF;
        res->prev[i] = -1;
    }
    res->dist[origem] = 0;

    for (int iter = 0; iter < NUM_VERTICES; iter++) {
        /* Seleciona o vertice nao visitado com menor distancia */
        int u = -1;
        for (int i = 0; i < NUM_VERTICES; i++) {
            if (!visitado[i] && (u == -1 || res->dist[i] < res->dist[u]))
                u = i;
        }
        if (u == -1 || res->dist[u] == INF)
            break;

        visitado[u] = 1;

        /* Relaxa arestas saindo de u */
        for (int v = 0; v < NUM_VERTICES; v++) {
            if (g->adj[u][v] && !visitado[v]) {
                int nova_dist = res->dist[u] + 1; /* peso = 1 */
                if (nova_dist < res->dist[v]) {
                    res->dist[v] = nova_dist;
                    res->prev[v] = u;
                }
            }
        }
    }
}

/* ================================================================== */
/* Utilitarios de impressao                                            */
/* ================================================================== */

void imprimir_config(const Configuracao *c)
{
    printf("[");
    for (int i = 0; i < NUM_DISCOS; i++) {
        printf("%d", c->pino[i] + 1); /* exibe pinos como 1, 2, 3 */
        if (i < NUM_DISCOS - 1) printf(", ");
    }
    printf("]");
}

void imprimir_caminho(const ResultadoDijkstra *res)
{
    int destino = res->destino;

    if (res->dist[destino] == INF) {
        printf("Nao existe caminho entre origem e destino.\n");
        return;
    }

    /* Reconstroi o caminho de tras para frente */
    int caminho[NUM_VERTICES];
    int tam = 0;
    for (int v = destino; v != -1; v = res->prev[v])
        caminho[tam++] = v;

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

/* ================================================================== */
/* Configuracoes padrao                                                */
/* ================================================================== */

void config_inicial(Configuracao *c)
{
    /* Todos os discos no pino 0 */
    for (int i = 0; i < NUM_DISCOS; i++)
        c->pino[i] = 0;
}

void config_final(Configuracao *c)
{
    /* Todos os discos no pino 2 */
    for (int i = 0; i < NUM_DISCOS; i++)
        c->pino[i] = 2;
}
