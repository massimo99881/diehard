
#include "graph.h"

graph *crea_contenitori(graph *g,int *capacita,int n_cont) 
{		
    /*Inizializzazione variabili*/
    int v=0;
    int a=0;
    int j=0;
	int cx,x,cy,y; 
    int i, *conf_tmp, *conf; node *q;
	
	if( !(g = malloc(sizeof(struct graph))) ||
        !(g->capacita = malloc (n_cont * sizeof(int))) ||
        !(g->attuale  = malloc (n_cont * sizeof(int))) ||
        !(g->nodi = malloc (sizeof(node *))) ||
        !(q = malloc(sizeof(node))) ||
        !(q->conf = malloc(n_cont * sizeof(int))) ||
        !(conf_tmp = malloc(n_cont * sizeof(int))) ||
        !(conf = malloc(n_cont * sizeof(int)))
       ) {
		fprintf(stderr,"Errore di Allocazione\n");
		exit(-2);
     }
	 
	for(i=0;i<n_cont;i++){
  
	    conf[i] = q->conf[i] = conf_tmp[i] = g->attuale[i] = 0;
        g->capacita[i] = capacita[i]; 
      } 
      
    /*crea e inizializza il primo nodo: tutti i contenitori sono vuoti*/
	
	g->nodi[0] = q;
    g->nodi[0]->v = 1;
    g->nodi[0]->pericolosa = 0;
    g->nodi[0]->colore = BIANCO; 
    g->nodi[0]->costo = 1;
    g->nodi[0]->next=NULL;
	g->nodi[0]->azione = 'x';
	
	g->ne = 0;
    g->nv = 1;
	g->nc = n_cont;
	g->sv_attivo = 1;
	
	
	
	/*generazione del grafo con liste di adiacenza*/
    do{
	    /*imposta conf_tmp alla configurazione del vertice corrente*/
        for(j=0;j<g->nc;j++)
            conf_tmp[j] = g->nodi[a]->conf[j]; 
            
        /**********************************************************************/
	
	    /*ciclo per ogni contenitore del vertice corrente*/
        for(j=0;j<g->nc;j++)
		   for(i=0;i<g->nc;i++){   
                                   
                /*T: prova facendo un travaso dal contenitore i-esimo*/  
			     
				if(i!=j){  
    				    cx = g->capacita[j];
    					x  = conf_tmp[j];
    					cy = g->capacita[i];
    					y = conf_tmp[i];
    					if(x>0 && y!=cy) do{ --x; ++y; } while(x!=0 && y !=cy);
    					else continue; 
    				
    					conf_tmp[j] = x;
    				    conf_tmp[i] = y;		
    					
    					if(is_new_edge(g,conf_tmp,a)) { 
    							
    							   g=vertex_insert(g,conf_tmp);
    							   g->nodi[a]->next = edge_insert(g,a,chiave(g,conf_tmp),conf_tmp,g->nc,'t'); 
    							   g->ne = g->ne +1;
    							   
    					}
    					
    					/*ripristino configurazione*/
    					conf_tmp[j] = g->nodi[a]->conf[j];		
    					conf_tmp[i] = g->nodi[a]->conf[i];    
				}
               
           }
	
	    /**********************************************************************/
        
        /*ciclo per ogni contenitore del vertice corrente*/
		for(i=0;i<g->nc;i++){ 
		
		    /*R: prova facendo un riempimento sul contenitore i-esimo*/
			conf_tmp[i] = g->capacita[i];      
			
			if(is_new_edge(g,conf_tmp,a)) {
				  
				   g=vertex_insert(g,conf_tmp);
				   g->nodi[a]->next = edge_insert(g,a,chiave(g,conf_tmp),conf_tmp,g->nc,'r'); 
				   g->ne = g->ne +1;				   
				   
			}
			
			/*ripristina il contenitore alla configurazione del vertice corrente*/
			conf_tmp[i] = g->nodi[a]->conf[i]; 
        }
		
        /**********************************************************************/
	
	    for(i=0;i<g->nc;i++){                              
                
            /*S: prova facendo uno svuotamento sul contenitore i-esimo*/                  	
			conf_tmp[i] = 0;                  
			
			if(is_new_edge(g,conf_tmp,a)) {
				  
                   g=vertex_insert(g,conf_tmp);
				   g->nodi[a]->next = edge_insert(g,a,chiave(g,conf_tmp),conf_tmp,g->nc,'s'); 
				   g->ne = g->ne +1;
				   
			}
			
			conf_tmp[i] = g->nodi[a]->conf[i]; 
		}
		
        
        /**********************************************************************/
	
		a++;
		
	}while(a!=g->nv);
	
	return g;
} 

/*inserisce un vertice*/
graph *vertex_insert(graph *g, int *conf)
{
  if(is_new_vertex(g,conf)){
                              
      int i,key;  node *q;
      
      /*espansione della memoria necessaria a contenere tutti i vertici del grafo*/
      if(!(g->nodi = realloc(g->nodi, (g->nv+1) * sizeof(struct node *))))  {
		fprintf(stderr,"Errore di Allocazione\n");
		exit(-1);
	  }
	  
      /*controllo corretta allocazione in memoria per contenere il nuovo vertice*/
	  if(!(q = malloc(sizeof(node)))) {
		  fprintf(stderr,"Errore di Allocazione\n");
		  exit(-2);
	  }
	  
	  /*controllo corretta allocazione in memoria per contenere la configurazione del vertice*/
	  if(!(q->conf = malloc (g->nc * sizeof(int)))){
          fprintf(stderr,"Errore di Allocazione\n");
		  exit(-3);      
      }
      
	  for(i=0;i<g->nc;i++)
	     q->conf[i] = conf[i]; 
	  
      /*inizializza nuovo vertice*/   
	  g->nodi[g->nv] = q;
	  q->next = NULL;
	  key = g->nv +1;
	  g->nv = key;
	  q->v = key;
	  q->pericolosa = 0;
	  q->colore = BIANCO;
	  q->costo = 1;
	  q->azione = 'x';
  }
    
  return g;
}

/*inserisce un arco*/
node *edge_insert(graph *g,int x,int k, int *conf, int size,char tpy)
{
    int i;
	node *q = malloc(sizeof(node));

	if(!q) {
		fprintf(stderr,"Errore di Allocazione\n");
		exit(-3);
	}
	
	q->conf = malloc (size * sizeof(int));
	for(i=0;i<size;i++)
	   q->conf[i]=conf[i];
	
	/*inizializza nuovo arco*/
	q->v = k;
	q->pericolosa = 0;
	q->azione = tpy;
	q->colore = BIANCO;
	q->costo = 1;
	q->next = g->nodi[x]->next; 
	return q;
}


/*controlla che l'arco che stiamo per inserire nella lista di adiacenza del vertice di indice a non sia già presente*/
int is_new_edge(graph *g, int *conf, int a)
{
    int i,j, arco_gia_presente;
    node *p=g->nodi[a]/*->next*/ ;
     
     while(p){

        arco_gia_presente=0;                            
        
        for(j=0;j<g->nc;j++){      

    	     if(/*g->nodi[a]->next && */conf[j] == p->conf[j]){

                 arco_gia_presente++; 
             }
        }
        
        if(arco_gia_presente==g->nc) { 
 
            return 0;                       
        }
    
      p=p->next;    
    }   
   
   return 1;
} 

/*controlla che il vertice che stiamo per inserire nel grafo non sia già presente*/
int is_new_vertex(graph *g, int *conf)
{
    int i,j, vertice_gia_presente;
    
   	for(i=0;i<g->nv;i++) {

        vertice_gia_presente=0;                            
        
        for(j=0;j<g->nc;j++){      
		
		     if(conf[j] == g->nodi[i]->conf[j]){

                 vertice_gia_presente++; 
             }
        }
        
        if(vertice_gia_presente==g->nc) { 
   
            return 0;                       
        }   
   }
   
   return 1;
}   

/*aggiorna la configurazione attuale secondo le tre principali azioni di riempimento, svuotamento e travaso di un contenitore*/
graph *passo(graph *g, int i1,int i2, char p)
{   
  int i, *conf = malloc (g->nc * sizeof(int));
  int nessuna_modifica = 0;  
    
  if(p=='R'){
      if(g->attuale[i1]==g->capacita[i1]){ nessuna_modifica=1; printf(error1); printf("\n"); }
      else {
           for(i=0;i<g->nc;i++){
                                
              if(i==i1)                  
                  conf[i1] = g->capacita[i1];
              else 
                  conf[i] = g->attuale[i];
           }
      }
  }    
  else if(p=='S'){ 
                            
                if(g->attuale[i1] == 0){ nessuna_modifica=1; printf(error1); printf("\n"); }
                else {
                     
                  for(i=0;i<g->nc;i++){
                                    
                      if(i==i1)                  
                          conf[i1] = 0;
                      else 
                          conf[i] = g->attuale[i];
                  }
                }
          
       }
       else if(p=='T'){
       
            /*prova travaso*/
			  int cx,x, cy,y;
             
              cx = g->capacita[i1];
              x = g->attuale[i1];
              cy = g->capacita[i2];
              y = g->attuale[i2];
            
              if(x>0 && y!=cy) do{ --x; ++y; } while(x!=0 && y !=cy);
              else { printf(error1); printf("\n"); nessuna_modifica=1; } /*non travasabile*/
            
              for(i=0;i<g->nc;i++){
                                
                  if(i==i1)                  
                      conf[i1] = x;
                      
                  else if(i==i2)
                           conf[i2] = y;  
                       else 
                           conf[i] = g->attuale[i];
              }	        
       } 
   
   if(!is_pericolosa(g,conf)){
      
      if(nessuna_modifica==0) {                         
          for(i=0;i<g->nc;i++)
      
             g->attuale[i] = conf[i];                     
      
      }
   }
   else
       printf("OPERAZIONE PERICOLOSA\n");    
      
  return g;
}  

/*dichiara pericolosa/innocua oppure imposta come visitato la configurazione in cui il contenitore i ha livello ai*/
graph *pericolosa_innocua_visitato(graph *g,int *livelli,char tpy)
{  
    int v;
    
   /*scorro prima l'array dei vertici per verificare che esista una config di livelli uguale a quella inserita dall'utente usando chiave()*/
        
   if(v = chiave(g,livelli)) { 
                           
        int i; 
        
        for(i = 0; i < g->nv; i++) {
    			
			if( v == g->nodi[i]->v){
                
               if(tpy=='G') g->nodi[i]->colore = GRIGIO;
               if(tpy=='i') g->nodi[i]->pericolosa = 0;    
               if(tpy=='p') g->nodi[i]->pericolosa = 1; 
            }
            
            if(tpy=='B' && g->nodi[i]->colore==GRIGIO) g->nodi[i]->colore = BIANCO;
			
			if(tpy=='N') g->nodi[i]->colore = BIANCO;
			
			percorri_lista_p_i_v(g->nodi[i]->next,v,tpy);
			
	    }
   }
   else 
        printf("la configurazione inserita non e' presente\n"); 
   
   return g;     
}

/*scorre la lista di adiacenza del vertice passato come parametro e controlla lo stato di ciascun nodo adiacente*/
void percorri_lista_p_i_v(node *p,int v,char tpy)
{
     while(p){
		
		if(p->v == v){
        
           if(tpy=='i') p->pericolosa = 0;    
           if(tpy=='p') p->pericolosa = 1; 
           if(tpy=='G') p->colore =GRIGIO;  
        }
        
         if(tpy=='B' && p->colore==GRIGIO) p->colore =BIANCO;
         if(tpy=='N') p->colore = BIANCO; 
		 if(tpy=='c') p->costo=1;
		
		p = p->next;
	}
} 

/*stampa tutte le configurazioni raggiungibili dalla conf attuale con d passi elementari*/
int *configurazioni(graph *g,int k,int numeroPasso,int mosse,int *capacita,int *dim)
{  
    int i,j; node *t;  
    
    if(numeroPasso==0){
                       
         i= chiave(g,g->attuale)-1;
    }
    else
         i= k;
	
	for(t = g->nodi[i]->next; t; t = t->next){
	
		if( mosse ) {
                     
            if(mosse-1==0 && t->colore==BIANCO){        
			   /*printf("%d e dim: %d\n",t->v,dim[0]);*/
               j = dim[0];
               capacita = realloc(capacita,(j+1) * sizeof(int));
               capacita[j] = t->v-1;
               dim[0]+= 1;
               
               pericolosa_innocua_visitato(g,t->conf,'G');
            }
               
			capacita=configurazioni(g,t->v-1,numeroPasso+1,mosse-1,capacita,dim);
		}
		
    }
    
    return capacita;
} 

/*stampa tutte le configurazioni raggiungibili dalla configurazione attuale e contenenti il valore k come livello di
  almeno un contenitore. Se non esistono configurazioni soddisfacenti la condizione, stampa NON PRESENTI! .*/
int *contenenti(graph *g,int i,int *capacita,int *dim,int livello)
{  
    node *t; int j;
	
	for(t = g->nodi[i]; t; t = t->next){
          
		if( t->colore==BIANCO && !is_pericolosa(g,t->conf)) {
			
			pericolosa_innocua_visitato(g,t->conf,'G');
			
			for(j=0;j<g->nc;j++)
                  if(t->conf[j]==livello){
                           /*printf("%d,",t->v);*/
                           j = dim[0];
                           capacita = realloc(capacita,(j+1) * sizeof(int));
                           capacita[j] = t->v-1;
                           dim[0]+= 1; 
                           j=g->nc; 
                           t->colore = NERO; 
                           g->nodi[t->v-1]->colore = NERO;             
                  }      
            
			capacita=contenenti(g,t->v-1,capacita,dim,livello);
		}
		
    }
    
    return capacita;
}  

/*funzione ausialiaria che fornisce la chiave del vertice con la configurazione uguale a quella passata come argomento*/
int chiave(graph *g, int *conf2)
{
    int i,j,uguali;
    
    for(j=0;j<g->nv;j++){                     /*cicla per tutto l'array di vertici*/
    
        uguali = 0;                           /*imposta variabile di controllo*/    
    
        for(i=0;i<g->nc;i++) {                /*cicla per ogni contenitore del vertice corrente*/
                             
           if(g->nodi[j]->conf[i]==conf2[i]){ /*confronta la configurazione del vertice corrente*/
           
              uguali++;
              
              if(uguali == g->nc)             /*se trovato il vertice,*/
    			
                return g->nodi[j]->v;         /*ritorna la chiave del vertice con stessa configurazione*/
    	   }
        }
    }    
    return 0;
} 

/*funzione che controlla se il grafo è vuoto*/
int is_empty(graph *g)
{
	return (g==NULL);
}

/*verifica se la configurazione passata è pericolosa*/
int is_pericolosa(graph *g,int *conf)
{
     int i,j,check=0;
     
     for(i = 0; i < g->nv; i++) {
        		
	     for(j=0;j<g->nc;j++){
             
             if(g->nodi[i]->conf[j]==conf[j]) check++;
                                                            
         }
         
         if(check==g->nc && g->nodi[i]->pericolosa) return 1;
         
         check = 0;
     }
     
     return 0;
} 

void stampa_configurazione(graph *g,node *t,int *attuale,char tpy)
{  
   int s;
   printf("\n(");        		
   for(s=0;s<g->nc;s++){
                        
        if(tpy=='v')                
		   printf("%d[",attuale[s]);
		   
		if(tpy=='m' || tpy=='k' || tpy=='c' || tpy=='w')
		   printf("%d[",t->conf[s]);
		
		printf("%d]",g->capacita[s]);
		
		if(s!=g->nc-1) 
           printf(",");
   }
   printf(")");      
} 

/*stampa le liste di adiacenza di ogni vertice del grafo (debug)*/
void stampalista(node *p,int n_cont)
{
	int j; char v;
	while(p){
        if(p->colore==1) v='G'; else if(p->colore==0) v='B'; else if(p->colore==2) v='N';
		printf("-> %c%2d %c %c%2d (",p->azione,p->costo,v,p->pericolosa,
                            p->v);
		
		for(j=0;j<n_cont;j++){
			printf("%d",p->conf[j]);
			if(j!=n_cont-1) printf(",");
		}
		printf(")");
		p = p->next;
	}
}

/*dato un grafo orientato non pesato (ogni peso ha valore 1) e un vertice di partenza (quello della configurazione attuale),
 assegna i pesi (in crescendo) ai nodi del grafo utilizzando l'algoritmo di visita in ampiezza bfs*/
graph *grafo_pesato(graph *g)
{
  	int i, *aux=calloc(g->nv, sizeof(int));
	
	if(!aux)
		printf("errore di allocazione");
		
	else
	{   
		/*for(i=0;i<g->nv;i++) serve solo per grafi sparsi..
			
			if(!aux[i]){*/
			 
				g=assegna_peso(g,i=chiave(g,g->attuale)-1,aux);
			/*}*/
			
		free(aux);
	}
	
	return g;
}

graph *assegna_peso(graph *g, int i, int *aux)
{
    node *e; 
    int y,peso;
	intqueue *q = createqueue();
	enqueue(q,i);
	/*il vertice attuale viene segnato come GRIGIO e il suo costo è pari a 1*/
	g->nodi[i]->costo=1;
	g->nodi[i]->colore=GRIGIO;
	
	while(!emptyq(q)){
	
		i=dequeue(q);
		aux[i]=1;
		
		for(y=0,e=g->nodi[i];e;e=e->next,y++){
		
		    if(y==0) {
                     /*preleva il peso di questo vertice e assegnalo a tutti i nodi della lista corrispondente*/
                     peso=e->costo; 
                     /*stampa: printf("[%2d] ",e->v);*/
            }
		    else{
                  /*stampa: printf(",%d",e->v); */  
                  e->costo=peso;
                  /*se il corrispondente vertice di questo nodo non è GRIGIO, assegna il peso attuale increm di 1*/
                  if(g->nodi[e->v-1]->colore==BIANCO){
                     
                     g->nodi[e->v-1]->costo=peso+1;
                     g->nodi[e->v-1]->colore=GRIGIO;
                  }
            }                                           
		
			if(!aux[e->v-1]){
			
				enqueue(q,e->v-1);
				aux[e->v-1]=1;
			}			
			
        }
        /*stampa: printf(" (p: %2d)\n",peso); */
	}
	
	destroyqueue(q);
	return g;
} 

graph *DestroyGraph (graph *g)
{
  free(g->nodi);
  g->nodi = NULL;
  free(g->attuale);
  g->attuale=NULL;
  free(g->capacita);
  g->capacita=NULL;
  g->nc = 0;
  g->nv = 0;
  g->ne = 0;
  return NULL;
}

