#include "graph.h"

/********************************************************************************/
/* Gestione dell'arborescenza dei cammini minimi:                               */
/********************************************************************************/

/* Estrae l'elemento u dalla lista (definita a partire dall'elemento indirizzato dal puntatore primo) tale che d[u] sia minima. */
int estrai_min(graph *g,node **primo, int d[]) {
    
  node *p, *pmin, *prec=NULL, *precmin=NULL;
  int u;
  pmin = *primo;
  p = *primo;
  
  while (p != NULL) { 
    
    if ( (d[p->v-1]>1 && d[p->v-1] < d[pmin->v-1]) || d[pmin->v-1] == INFINITO) { 
                     
      pmin = p;
      precmin = prec;
    }
       
    prec = p;
    p = p->next;
  }
  u = pmin->v-1;

  if (precmin == NULL)
    *primo = (*primo)->next;
  else
    precmin->next = pmin->next;
  free(pmin);

  return(u);
}

/*Inserisce un elemento nella lista .*/
void accoda(node **primo, int v) {
  
  node *p;
  
  p = malloc(sizeof(struct node));
  p->v = v+1;
  p->next = *primo;
  *primo = p;
  
  return;
}

void Dijkstra(graph *g,int *pi,int sorg,int tpy)
{
          /*dichiarazione e inizializzazione var per Dijkstra*/
          int u,v;
          int *d = malloc( g->ne * sizeof(int));
          
          node *q, *primo=NULL;
          node *p;
           
          for (v=0; v<g->nv; v++) {
            d[v] = INFINITO;
            pi[v] = -1;
          }
        
          d[sorg] = 0;
        
          for (v=0; v<g->nv; v++)
             if ( !is_pericolosa(g,g->nodi[v]->conf))
               accoda(&primo, v);
        
          while (primo != NULL ) { 
            u = estrai_min(g,&primo, d);
            p = g->nodi[u];
            
            while (p != NULL ) { 
                           
                  /*considero la possibilità di poter aggirare i nodi che hanno una configurazione pericolosa*/
                  if ( !is_pericolosa(g,p->conf) && (d[p->v-1] == INFINITO || d[p->v-1] > d[u] + p->costo)) {
                    pi[p->v-1] = u; 
                    d[p->v-1] = d[u] + p->costo;
                  }
                  
              p = p->next;
            }
          }
          
          /*stampa intermedi (debug)
          for (v=0; v<g->nv; v++) printf("d[%d] = %3d, pi[%d] = %3d\n", v, d[v], v, pi[v]);*/         
}

int cammino_valido(graph *g,int sorg,int dest,int min_prec,char tpy,int *mst,int *pi,int *MST)
{
      int i,v,fail=FALSE;
      int tmp = 0;   
      
      /*scrivo il cammino fino a raggiungere -1 oppure fino alla lunghezza minima trovata fin'ora*/      
      for(v=0,tmp=dest; tmp!=-1 && v <= min_prec; v++){
               
             mst[v]=tmp;                     
             tmp = pi[tmp];                      
      }  
                         
      /*adesso che conosco la lunghezza del cammino lo riscrivo nel giusto ordine*/
      reverse(mst,v);
          
      /*se gli svuotamenti sono disattivati, controllo che non sia stato effettuato nessuno svuotamento */
      if(g->sv_attivo==0)
          for(i=0;i <= min_prec && !fail && mst[i]!=-1;i++) 
          {
            if(raggiungibile_azione(g,mst[i+1],mst[i],'s')) fail=TRUE;      
          }
      
      /*Se il cammino è lungo uguale o è più corto del precedente, allora sovrascrivi mst */ 
      if(v<= min_prec && !fail){   
          
          for(i=0; i<v ;i++)
              
             MST[i]=mst[i];                   
                                        
          MST[v] = -1; 
      }
      else v = min_prec;
      
      return v;   
}

void esiste_ragg_mosse_critica (graph *g, int *livelli, char tpy)
{ 
   int *sorg,*dest,*pi,*mst,*MST ,i=0,j=0,a=0,b=0;                          /*Inizializzazione variabili*/
   int min=g->nv,num_camm=0,parametri=0,pesomin=0;
   
   if(  !(sorg = malloc  (min * sizeof(int))) ||
        !(dest = malloc  (min * sizeof(int))) ||
        !(pi   = malloc  (min * sizeof(int))) ||
        !(mst  = malloc  (min * sizeof(int))) ||
        !(MST  = malloc  (min * sizeof(int))) 
     ) {
		fprintf(stderr,"Errore di Allocazione\n");
		exit(-2);
     }
     
   sorg[0]=-1; dest[0]=-1;                                                  /*fine inizializzazione variabili*/
   
   for (i=0;livelli[i]!=-1;i++)  parametri ++;                              /*Verifico il numero di parametri inseriti dall'utente .*/     
   
   verifica_sorgente_destinazione(g,livelli,tpy,sorg,dest,parametri);       /*memorizzo tutte le possibili chiavi sorgente e destinazione.       */
   
   for(a=0;sorg[a]!=-1;a++){                                                /*Per ogni sorgente trovata (o per l'unica consentita) e per ogni    */
                                                                                             
       for(i=0;dest[i]!= -1;i++){                                           /*destinazione trovata (o per l'unica consentita) ..                 */
                                                                        
          num_camm++;                                                       /*incremento contatore di cammini generati*/
          
          if(g->sv_attivo==0 && (tpy=='e' || tpy=='k')) min = g->ne;        /*riinizializza pesomin del cammino per disattiva-esiste/critica      */                    
                           
          Dijkstra(g,pi,sorg[a],tpy);                                       /*Applica algoritmo Dijkstra per la costruzione del cammino minimo.   */
                                                                        
          pesomin =cammino_valido(g,sorg[a],dest[i],min,tpy,mst,pi,MST);    /*Controlla il cammino generato e ritorna il costo del cammino minimo.*/
            
          if ((pesomin < min || g->sv_attivo==0) && tpy!='k'){              /*Se il cammino ha prodotto un costo inferiore a quello precedente ,  */
                 
             min = pesomin ;                                                /*allora assegna il nuovo valore al costo minimo e aggiorna gli indici*/
             j = i;
             b = a;                                                         /*della sorgente e della destinazione.*/
             
             if(tpy=='e' || tpy=='r')                                       /*Se l'azione scelta è stata 'esiste' ed è stato trovato un cammino per*/
             
                 dest[i+1] = -1;                                            /*cui è possibile raggiungere la configurazione con il livello inserito*/
                                                              
          }                                                                 /*allora esci dal ciclo.*/  
      }                                                                     
  }                                                 /*printf("\n totale cammini calcolati variando la destinazione e la sorgente: %d\n",num_camm);*/
  
  if((num_camm > 1 && tpy=='k') || (num_camm && tpy!='k' && min<g->nv)){    /*Se è stato trovato almeno un cammino valido [ n.archi > costo > 1] ,*/
              
     if(tpy=='m' ){ node *t;                                                /*e l'azione scelta dall'utente è stata 'mosse'                       */
     
       for(i=0;i<pesomin;i++){                                              /*percorri il cammino minimo generato stampandolo a video             */
       
           j = MST[i];
           t = g->nodi[j];
           stampa_configurazione(g,t,NULL,tpy); 
       }
     }   
     if(tpy=='e' || tpy=='r')                                               /*Se l'azione scelta è stata 'esiste' oppure 'raggiungibile' stampa a */ 
     
       printf("\nSI'\n");                                                   /*video la stringa SI' .*/
     
     if(tpy=='k')                                                           /*Per il caso critica, numerosi dubbi sono sorti. Nonostante il chiarimento*/
     
       ;                                                                    /*iniziale avuto discutendo con il professore, rimangono ancora delle*/
                                                        
  }                                                                         /*problematiche che desidererei se possibile chiarire. */
  else { 
        
     if(tpy=='m' && parametri==1)                                           /*Se non esiste nessun cammino, per quanto minimo lo si voglia, stampa*/
     
        printf("\nIRRAGGIUNGIBILE!\n");                                     /*un messaggio negativo, corrispondente all'azione intrapresa . */
        
     if(tpy=='m' && parametri==2)
     
        printf("\nNON E' POSSIBILE\n"); 
        
     if(tpy=='e' || tpy=='r')
     
        printf("\nNO\n"); 
        
     if(tpy=='k'){
     
       printf("\nNON ESISTE CONFIGURAZIONE CRITICA\n");             
     }
        
  }
  printf("\n\n");   
  
  free(sorg);
  free(dest);
  free(pi);
  free(mst);
  free(MST);    
}  
 
void verifica_sorgente_destinazione(graph *g, int *livelli, char tpy, int *sorg,int *dest,int parametri)
{                
   int i; 
         
   if(tpy=='m' || tpy=='e'){
                
       if(parametri==1){                            /*************CASO MOSSE/ESISTE con un  parametro***********************/
           
          sorg[0]= chiave(g,g->attuale)-1;          /*la sorgente: è la chiave del nodo attuale*/ 
          sorg[1]= -1;           
          verifica_dest_m_e(g,livelli[0],dest);     /*la destinazione: è composta da un contenitore di livello *h */  
          
                                                    /*possibilità di avere più possibili destinazioni di livello h*/              
       }          
       
       if(parametri==2){                            /*************CASO MOSSE con due parametri*****************************/
                       
          verifica_sorg_m(g,sorg,livelli[0]);       /*la sorgente: è composta da un contenitore di livello k */
          
                                                    /*possibilità di avere più possibili sorgenti di livello k*/        
          
          verifica_dest_m_e(g,livelli[1],dest);     /*la destinazione: è composta da un contenitore di livello *h */
          
                                                    /*possibilità di avere più possibili destinazioni di livello h*/                                                   
       } 
   }                                           
   else if(tpy=='r' || tpy=='k' ){                  /*************CASO RAGGIUNGIBILE*******************************/
        
         sorg[0]= chiave(g,g->attuale)-1;           /*la sorgente: è la chiave del nodo attuale*/ 
         sorg[1]= -1;         
         dest[0]= chiave(g,livelli)-1;              /*la destinazione: è la chiave del nodo che ha come configurazione i livelli */
         dest[1]= -1;         
        }
}

/*Poiché si ha la possibilità di avere più sorgenti di livello k, memorizzo le chiavi di queste possibili sorgenti*/
void verifica_sorg_m(graph *g,int *sorg, int k)
{
      int f,s,indice_sorg=0;                  
          
      for(f=0; f < g->nv; f++){            
      
         if(!is_pericolosa(g,g->nodi[f]->conf) )      
         
             for(s=0;s<g->nc;s++)
                                  
                 if(g->nodi[f]->conf[s]==k){ 
                                                                      
                     sorg[indice_sorg++]= g->nodi[f]->v-1;  
                     s=g->nc;                        
                 }                                  
      }
      
      sorg[indice_sorg]= -1;      
}

/*controllo esistenza della destinazione per i casi mosse ed esiste e, poiché potrebbe esistere più di una destinazione, memorizzo in un array*/
void verifica_dest_m_e(graph *g,int h,int *dest)
{
      int i,j,indice_dest=0;      
      
      for(i=0;i<g->nv;i++)
      {              
            if(!is_pericolosa(g,g->nodi[i]->conf)  )
                for (j=0;j<g->nc;j++){
                    
                         /*controllo che esista una destinazione con un contenitore che abbia livello h */
                         if(g->nodi[i]->conf[j]==h){   
                                                           
                                 /*ok è presente, ma potrebbero esserci più vertici con livello k , quindi li memorizzo tutti*/
                                 dest[indice_dest++] = g->nodi[i]->v-1;   j=g->nc;                                                                        
                         }             
                 }                
      } 
      
      dest[indice_dest]= -1;   
}

/*mi dice se il nodo con chiave pred è raggiungibile dal nodo con chiave tmp attraverso un azione precisata (s,r,t) */
int raggiungibile_azione(graph *g,int tmp,int pred, char tpy)
{
	node *t;
	  
	for(t=g->nodi[pred]->next; t; t=t->next){
                                                       
	    if(t->v-1 == tmp && t->azione == tpy){
	         
            return 1;       
        }
          
    }
    return 0;	
}


void reverse(int *mst,int dim)
{
    unsigned short int a=0,b=dim;
    int swap;
    
    for(a;a<--b;a++)  
    {
        swap=mst[a];        
        mst[a]=mst[b];     
        mst[b]=swap;        
    }
    
    mst[dim] = -1;  
   
}

/********************************************************************************/
/*funzioni di gestione per la coda di priorità                                  */
/********************************************************************************/

intqueue *createqueue(void)
{
    intqueue *q = malloc(sizeof(intqueue));

    if(!q) {
        fprintf(stderr,"Errore di allocazione nella creazione della coda\n");
        exit(-1);
    }
    q->head = createlist();
    q->count = 0;
    return q;
}

void destroyqueue(intqueue *q)
{
	destroylist(q->head);
	free(q);
}

void enqueue(intqueue *q, int elem)
{
    insertatend(q->head, elem);
    q->count++;
}

int dequeue(intqueue *q)
{
    int e;

    if(!q->count){
        fprintf(stderr,"Errore: dequeue su coda vuota\n");
        exit(-2);
    };
    e = q->head->next->dato;
    delete(q->head->next);
    q->count--;
    return e;
}

char emptyq(intqueue *q)
{
    return !q->count;
}

/********************************************************************************/
/*funzioni di gestione per le liste                                             */
/********************************************************************************/

/* crea una lista vuota e ne restituisce il puntatore radice */
intlist *createlist(void)
{
    intlist *q = malloc(sizeof(intlist));

    if(!q) {
        fprintf(stderr,"Errore di allocazione nella creazione della lista\n");
        exit(-1);
    }
    q->next = q->prev = q;
    return q;
}

/* restituisce il dato nel primo elemento effettivo della lista */
/* o un valore non determinato (quello della sentinella) se tale elemento non esiste */
int head(intlist *p)
{
	return p->next->dato; 
}


/* visita una lista e esegue su ogni elemento la funzione op */
void traverse(intlist *p, void (*op)(intlist *, intlist *))
{
	intlist *q;

	if(!p) return;
	for(q = p->next; q != p; q = q->next)
		(*op)(p,q);
}

/* stampa l'elemento puntato */
void printelem(intlist *p, intlist *q)
{
	printf("\t-------\n\t|%5d|\n\t-------\n\t|  %c  |\n\t---%c---\n\t",
			q->dato, q->next != p ? '.' : 'X', q->next != p ? '|' : '-');
	if(q->next != p)
		printf("   |   \n\t   V\n");
	else
		printf("\n");
}

/* ritorna il numero di elementi nella lista */
int countlist(intlist *p)
{
	int i;
	intlist *q;
	
	if(!p || p == p->next)
		return 0;
	for(i = 1, q = p->next; q->next != p; q = q->next, i++);
	return i;
}

/* inserisce un elemento in testa alla lista */
void insert(intlist *p, int elem)
{
	intlist *q = malloc(sizeof(intlist));

	if(!q) {
		fprintf(stderr,"Errore nell'allocazione del nuovo elemento\n");
		exit(-1);	
	}
	q->dato = elem;
	q->next = p->next;
	p->next->prev = q;
	p->next = q;
	q->prev = p;
}

/* inserisce un elemento in coda alla lista */
void insertatend(intlist *p, int elem)
{
	intlist *q = malloc(sizeof(intlist));

	if(!q) {
		fprintf(stderr,"Errore nell'allocazione del nuovo elemento\n");
		exit(-1);	
	}
	q->dato = elem;
	q->prev = p->prev;
	p->prev->next = q;
	p->prev = q;
	q->next = p;
}


/* cancella l'elemento puntato da q dalla lista */
void delete(intlist *q)
{
	q->prev->next = q->next;
	q->next->prev = q->prev;
	free(q);	
}

/* distrugge la lista */
void destroylist(intlist *p)
{
	while(p->next != p)
		delete(p->next);
	free(p);
}

/* concatena la lista con radice q alla lista con radice p */
void listcat(intlist *p, intlist *q)
{
	if(q != q->next) {
		if(p == p->next) {
			p->next = q->next;
			q->next->prev = p;
			
		} else {
			p->prev->next = q->next;
			q->next->prev = p->prev;
		}
		p->prev = q->prev;
		q->prev->next = p;
	}
	free(q);
}

/* restituisce vero se la funzione check e' vera su almeno un elemento */
char traverseandcheck(intlist *p, char (*check)(intlist *, int), int a)
{
	intlist *q;

	for(q = p->next; q != p; q = q->next)
		if((*check)(q,a))
			return 1;
	return 0;
}

/* restituisce il primo elemento per cui check e' vera oppure NULL*/
intlist *getcheckelem(intlist *p, char (*check)(intlist *, int), int a)
{
	intlist *q;

	for(q = p->next; q != p; q = q->next)
		if((*check)(q,a))
			return q;
	return NULL;
}

/* restituisce vero se p->dato == elem */ 
char checkexist(intlist *p, int elem)
{
	return p->dato == elem;
}

/* restituisce vero se l'elemento e' presente nella lista */
char existsinlist(intlist *p, int elem)
{
	return traverseandcheck(p,checkexist,elem);
}

/* restituisce il primo elemento q per cui q->dato == elem */
intlist *geteleminlist(intlist *p, int elem)
{
	return getcheckelem(p,checkexist,elem);
}

/* distrugge il primo elemento effettivo della lista, */
/* se tale elemento non esiste lascia la lista inalterata */
void deletehead(intlist *p)
{
	if(p != p->next)
		delete(p->next);
}



