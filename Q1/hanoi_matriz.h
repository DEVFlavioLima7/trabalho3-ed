#ifndef HANOI_MATRIZ_H
#define HANOI_MATRIZ_H
#define NUM_DISCOS  5
#define NUM_PINOS   3
#define NUM_VERTICES 243   /* 3^5 */
#define INF          99999

typedef struct {
    int adj[NUM_VERTICES][NUM_VERTICES]; /* 1 se ha aresta, 0 caso contrario */
    int num_vertices;
    int num_arestas;
} GrafoMatriz;


typedef struct {
    int pino[NUM_DISCOS];
} Configuracao;

/* ------------------------------------------------------------------
 * Resultado do Dijkstra
 * ------------------------------------------------------------------ */
typedef struct {
    int dist[NUM_VERTICES];   /* distancia minima a partir da origem  */
    int prev[NUM_VERTICES];   /* vertice anterior no caminho minimo   */
    int origem;
    int destino;
} ResultadoDijkstra;


int config_para_indice(const Configuracao *c);
void indice_para_config(int indice, Configuracao *c);

/* ------------------------------------------------------------------
 * Verifica se e legal mover o disco 'disco' do pino 'origem' para
 * o pino 'destino' dada a configuracao atual.
 * Regras:
 *   - So move o disco de topo de cada pino (menor disco no pino).
 *   - O disco so pode ser colocado sobre um disco maior ou pino vazio.
 * ------------------------------------------------------------------ */
int movimento_valido(const Configuracao *c, int disco, int pino_dest);

/* ------------------------------------------------------------------
 * Retorna o disco de topo (menor indice) presente em 'pino'.
 * Retorna -1 se o pino estiver vazio.
 * ------------------------------------------------------------------ */
int topo_do_pino(const Configuracao *c, int pino);

/* ------------------------------------------------------------------
 * Constroi o grafo completo de movimentos do Hanoi com 5 discos.
 * ------------------------------------------------------------------ */
void construir_grafo(GrafoMatriz *g);

/* ------------------------------------------------------------------
 * Algoritmo de Dijkstra sobre a matriz de adjacencia.
 * Todos os pesos sao 1 (arestas nao ponderadas).
 * ------------------------------------------------------------------ */
void dijkstra_matriz(const GrafoMatriz *g, int origem, ResultadoDijkstra *res);

/* ------------------------------------------------------------------
 * Imprime o caminho minimo encontrado pelo Dijkstra.
 * ------------------------------------------------------------------ */
void imprimir_caminho(const ResultadoDijkstra *res);

/* ------------------------------------------------------------------
 * Imprime uma configuracao no formato [p0, p1, p2, p3, p4]
 * ------------------------------------------------------------------ */
void imprimir_config(const Configuracao *c);

/* ------------------------------------------------------------------
 * Retorna a configuracao inicial (todos os discos no pino 0)
 * e a configuracao final (todos os discos no pino 2).
 * ------------------------------------------------------------------ */
void config_inicial(Configuracao *c);
void config_final(Configuracao *c);

#endif /* HANOI_MATRIZ_H */
