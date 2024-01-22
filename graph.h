#include <stdio.h>
#include <stdlib.h>

#define error1 "\nOPERAZIONE NON VALIDA\n"
#define error2 ", INDICE PRIMO CONTENITORE NON VALIDO\n"
#define error3 ", INDICE SECONDO CONTENITORE NON VALIDO\n"
#define error4 ", I CONTENITORI NON SONO STATI CREATI\n"
#define INFINITO  -1
#define FALSE              0
#define TRUE               1
#define BIANCO  0   /*non visitato*/
#define GRIGIO  1   /*visitato*/
#define NERO   	2   /*soluzione*/ 

/*struttura grafo orientato*/
typedef struct node {

	int v;                  /* chiave del nodo                      */
	
	                        /* seguono i dati satellite:            */
    char azione;            /* passo eseguito per ottenere il nodo  */
	int pericolosa;         /* 1: pericolosa ;  0: innocua          */
	int costo;			    /* peso per ogni percorso di arrivo     */
	int colore;  		    /* contrassegno per la visita dei nodi  */
	int *conf;				/* configurazione dei contenitori       */
	struct node *next;	    /* puntatore al nodo adiacente          */
	
} node;

typedef struct graph {

	int ne;				    /* numero di archi                                        */
	int nv; 			    /* numero di vertici                                      */	
	int nc;     		    /* numero di contenitori                                  */
	int *capacita;  	    /* array di tipo int per capacita contenitori             */
	int *attuale;			/* contiene la configurazione attuale                     */
	int sv_attivo;          /* 1: Svuotamento Abilitato ; 0: Svuotamento disabilitato */
	int pesato;             /* 1: grafo pesato          ; 0: grafo non pesato         */
	struct node **nodi;	    /* array di puntatori a liste                             */
	
} graph; 

/*prototipi funzioni di gestione del grafo*/
graph *crea_contenitori(graph *g,int *capacita,int n_cont) ;
graph *vertex_insert(graph *g, int *conf);
node  *edge_insert(graph *g,int x,int k, int *conf, int size,char tpy);
graph *passo(graph *g, int i1,int i2, char p);
graph *pericolosa_innocua_visitato(graph *g,int *livelli,char tpy);
void   stampa_configurazione(graph *g,node *t,int *attuale,char tpy);
void   percorri_lista_p_i_v(node *p,int v,char tpy);
int    is_empty(graph *g);
int    is_pericolosa(graph *g,int *conf);
int    is_new_edge(graph *g, int *conf, int a);
int   *contenenti(graph *g,int i,int *capacita,int *dim,int livello);
int   *configurazioni(graph *g,int k,int numeroPasso,int mosse,int *capacita,int *dim);  
graph *grafo_pesato(graph *g);
graph *assegna_peso(graph *g, int i, int *aux);
graph *DestroyGraph (graph *g);

/*prototipi algoritmo Dijkstra*/
int  estrai_min(graph *g,node **primo, int d[]);
void accoda(node **primo, int v); 
int  raggiungibile_azione(graph *g,int tmp,int pred, char tpy);
void Dijkstra(graph *g,int *pi,int sorg,int tpy);
int  cammino_valido(graph *g,int sorg,int dest,int min_prec,char tpy,int *mst,int *pi,int *MST);
void esiste_ragg_mosse_critica (graph *g, int *livelli, char tpy);
void verifica_sorgente_destinazione(graph *g, int *livelli,char tpy, int *sorg, int *dest,int parametri);
void verifica_sorg_m(graph *g,int *sorg, int k);
void verifica_dest_m_e(graph *g,int h,int *dest);
void reverse(int *mst,int dim); 

/*struttura per la gestione della lista*/
struct intlist {
    int dato;
    struct intlist *next, *prev;
};

typedef struct intlist intlist;

/*prototipi lista*/
intlist *createlist(void);
int head(intlist *p);
void traverse(intlist *p, void (*op)(intlist *, intlist *));
void printelem(intlist *p, intlist *q);
int countlist(intlist *p);
void insert(intlist *p, int elem);
void insertatend(intlist *p, int elem);
void delete(intlist *q);
void destroylist(intlist *p);
void listcat(intlist *p, intlist *q);
char traverseandcheck(intlist *p, char (*check)(intlist *, int), int a);
intlist *getcheckelem(intlist *p, char (*check)(intlist *, int), int a);
char checkexist(intlist *p, int elem);
char existsinlist(intlist *p, int elem);
intlist *geteleminlist(intlist *p, int elem);
void deletehead(intlist *p);

/*struttura per la gestione della coda di priorità*/
struct intqueue {
    intlist *head;
    int count;
};

typedef struct intqueue intqueue;

/*prototipi coda di priorità*/
intqueue *createqueue(void);
void destroyqueue(intqueue *q);
void enqueue(intqueue *q, int elem);
int dequeue(intqueue *q);
char emptyq(intqueue *q);
