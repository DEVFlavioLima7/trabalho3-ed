#ifndef HANOI_LISTA_H
#define HANOI_LISTA_H

/*
 * Torre de Hanoi - Grafo com Lista de Adjacencia
 * Disciplina: Estruturas de Dados II - UFPI/CSHNB
 *
 * Mesma logica da Questao 1, mas armazena o grafo em lista encadeada.
 * Total de vertices: 3^5 = 243
 */

#define NUM_DISCOS   5
#define NUM_PINOS    3
#define NUM_VERTICES 243
#define INF          99999

/* ------------------------------------------------------------------
 * No da lista encadeada de adjacencia
 * ------------------------------------------------------------------ */
typedef struct No {
    int vertice;      /* indice do vizinho */
    int peso;         /* peso da aresta (sempre 1) */
    struct No *prox;
} No;

/* ------------------------------------------------------------------
 * Grafo representado por lista de adjacencia
 * ------------------------------------------------------------------ */
typedef struct {
    No *lista[NUM_VERTICES]; /* cabeca da lista de cada vertice */
    int num_vertices;
    int num_arestas;
} GrafoLista;

/* ------------------------------------------------------------------
 * Configuracao de discos (mesmo modelo da Q1)
 * ------------------------------------------------------------------ */
typedef struct {
    int pino[NUM_DISCOS];
} Configuracao;

/* ------------------------------------------------------------------
 * Resultado do Dijkstra
 * ------------------------------------------------------------------ */
typedef struct {
    int dist[NUM_VERTICES];
    int prev[NUM_VERTICES];
    int origem;
    int destino;
} ResultadoDijkstra;

/* ------------------------------------------------------------------
 * Inicializa o grafo (listas vazias)
 * ------------------------------------------------------------------ */
void grafo_inicializar(GrafoLista *g);

/* ------------------------------------------------------------------
 * Libera toda a memoria alocada para as listas
 * ------------------------------------------------------------------ */
void grafo_liberar(GrafoLista *g);

/* ------------------------------------------------------------------
 * Conversoes configuracao <-> indice (base 3)
 * ------------------------------------------------------------------ */
int config_para_indice(const Configuracao *c);
void indice_para_config(int indice, Configuracao *c);

/* ------------------------------------------------------------------
 * Retorna o disco de topo (menor indice) presente em 'pino'.
 * Retorna -1 se vazio.
 * ------------------------------------------------------------------ */
int topo_do_pino(const Configuracao *c, int pino);

/* ------------------------------------------------------------------
 * Verifica se e valido mover 'disco' para 'pino_dest'
 * ------------------------------------------------------------------ */
int movimento_valido(const Configuracao *c, int disco, int pino_dest);

/* ------------------------------------------------------------------
 * Constroi o grafo de movimentos usando lista de adjacencia
 * ------------------------------------------------------------------ */
void construir_grafo(GrafoLista *g);

/* ------------------------------------------------------------------
 * Dijkstra com fila de prioridade minima (min-heap binario)
 * Complexidade: O((V + E) log V) -- melhor que a versao com matriz
 * ------------------------------------------------------------------ */
void dijkstra_lista(const GrafoLista *g, int origem, ResultadoDijkstra *res);

/* ------------------------------------------------------------------
 * Impressao
 * ------------------------------------------------------------------ */
void imprimir_caminho(const ResultadoDijkstra *res);
void imprimir_config(const Configuracao *c);

/* ------------------------------------------------------------------
 * Configuracoes padrao
 * ------------------------------------------------------------------ */
void config_inicial(Configuracao *c);
void config_final(Configuracao *c);

#endif /* HANOI_LISTA_H */
