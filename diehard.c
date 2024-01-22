/***********************************************************************************/
/* matricola: 747112                                                               */
/* studente : Galasi Massimo                                                       */
/* progetto : diehard                                                              */
/***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/*funzione ausiliaria per acquisire valori inseriti dall'utente*/
char *leggi_stringa(char carattere)
{  
   int i=2;                                           
   char *numero, c;
	
   numero=malloc(i*sizeof(char));	                
   *numero=carattere;
   
   c = getchar();                                    
	   
	   while(c!='\n' && !isspace(c))                 /*un numero termina prima di invio oppure prima di un altro spazio*/
	   {
		 i++;
		 numero = realloc(numero, i*sizeof(char));  
		 numero[i-2]=c;							     /*inserisco l'ultimo carattere e..*/
		 c = getchar();
	   }
	   
   numero[i-1]='\0';                                 /*inserisco il carattere di fine stringa*/
	   
   return numero;
}

#include "graph.h"

int main(void)
{
   char car,p;		    /*memorizza il carattere di input*/
   int n_cont;		    /*num.nodi e numero di contenitori correttamente inseriti*/	
   int i,j,k,d,c,dim; 	/*variabili ausiliarie*/	
   int *capacita,*aux;  /*array che contiene le capacita/livelli dei contenitori creati*/
   char *numero;        /*array di carattere per acquisizione capacita/livello di un contenitore*/
   graph *g=NULL;       /*inizializzazione del grafo orientato con liste di adiacenza*/
   
   /* Fino a quando non leggo il carattere f il programma non termina*/
   while((p = car = getchar()) != 'f')
   {
		switch(car)
        {
			/*crea contenitori: non so a priori il numero di contenitori che inserirà l'utente*/
            case 'N': 
					if(!is_empty(g)){
                        g = DestroyGraph(g);        
                    }
                      
                        i = 1;
						n_cont=0;		  
						
						/*inizializzo ed espando gli array dinamicamente nel while*/ 
						numero=malloc(sizeof(char));	
						capacita=malloc(sizeof(int)); 
						
						while(1)
						 {
							car=getchar();
						 
							if(!isspace(car)){						 
							   
							   numero = realloc(numero, ++i*sizeof(char)); 
							   numero[i-2]=car;
							}
							else {
							
								numero[i-1]='\0';								
								if((c = atoi(numero)) > 0){
								
									if(n_cont++>0)
										capacita = realloc(capacita, n_cont*sizeof(int)); 
									capacita[n_cont-1]=c;									
								
								}
								i=1;
								numero=malloc(i*sizeof(char));
							}
							
							if(car=='\n') break;
							
						 }
						 
						if(n_cont>0){
						
						    if(!is_empty(g)) free(g);
                        	
							g=crea_contenitori(g,capacita,n_cont);
                        
						}else 
							printf("nessun contenitore correttamente inserito");
							
						free(capacita);
					 
					break;

			/*riempi contenitore*/
			case 'R': case 'S': case 'T':
                   {
                      if(!is_empty(g))
                      {			             
                         while(isspace(car=getchar()));	 
                         i=atoi(leggi_stringa(car));		 
    					 
                         if(i<1 || i>g->nc){
                                
    						printf(error1);
                            printf(error2);
    						break;
    						
                         }
         
			             if(p!='T'){
                             if(p=='S' && !(g->sv_attivo))
                                break;
                                
                             g=passo(g,--i,0,p);							
                         }
                         else
                         {
                             /*travasa contenitore i in j*/
                              while(isspace(car=getchar()));	 
                              j =atoi(leggi_stringa(car));     					 
						 
						      if(i<1 || j>g->nc){
                                     
							     printf(error1);
                                 printf(error3);
                                 break;
                                 
                              }
                              
							  if(p == 'T'){
                                g=passo(g,--i,--j,p);
								
                              }
                         }
                      }
                      else{
                            printf(error1);
                            printf(error4);
                      }
                    }	 
                    break;
					  
			/*visualizza la configurazione attuale*/
			case 'v': if(!is_empty(g)){ 
			
                            stampa_configurazione(g,NULL,g->attuale,car);
                            printf("\n");
                      }
                      else{ 
                            printf(error1);
                            printf(error4);     
                      }
                      break;	
						
			/*esiste*/
			case 'e':  if(!is_empty(g)){
                 
                           while(isspace(car=getchar()));	 
                           k =atoi(leggi_stringa(car));
    						 
    					   if(k<0)
    					   
    						   printf("valore non corretto");
    						 
    					   else{
                               capacita=malloc(2*sizeof(int)); 
                               capacita[0] = k; 
                               capacita[1] = -1; 
                                  
    					  	   esiste_ragg_mosse_critica(g,capacita,p);
    					  	    
                           }
                       }
                       else {
                            
                            printf(error1);
                            printf(error4);     
                       } 
                             	 	
			           break;
						 
		    /*raggiungibile*/
			case 'r':  if(!is_empty(g)) {  
                  	         
                            i = 1;	  	
    						n_cont=0;		  
    						
    						/*inizializzo ed espando gli array dinamicamente nel while*/ 
    						numero=malloc(sizeof(char));	
    						capacita=malloc(sizeof(int)); 
    						
    						while(1)
   						    {
    							car=getchar();
    						 
    							if(!isspace(car)){						 
    							   
    							   numero = realloc(numero, ++i*sizeof(char)); 
    							   numero[i-2]=car;
    							}
    							else if(i>1) {
    							
    								numero[i-1]='\0';								
    								if((c = atoi(numero)) >= 0){
    								
    									if(n_cont++>0)
    										capacita = realloc(capacita, n_cont*sizeof(int)); 
    									capacita[n_cont-1]=c;									
    								
    								}
    								i=1;
    								numero=malloc(i*sizeof(char));
    							}
    							
    							if(car=='\n') break;
    							
    						 }
   						    
   						    capacita = realloc(capacita,(n_cont+1) * sizeof(int));
   						    capacita[n_cont] = -1;
    						 
    						if(n_cont==g->nc){ /*controllo che sono stati inseriti tutti i livelli*/
        						    
                               n_cont = 0;
        						    
        					   /*controllo se la configurazione attuale corrisponde a quella che si vuole raggiungere*/	    
        						    
                               for(i=0;i<g->nc;i++) 
                                   if(capacita[i]==g->attuale[i]){
                                                                  
                                     n_cont++;                                                       
                                   }
                                   
                                   if(n_cont==g->nc) { 
                                    
                                       printf("configurazione da raggiungere uguale alla configurazione attuale\n");   
                                       break;               
                                   }
                                   
    					  	       esiste_ragg_mosse_critica(g,capacita,p);
                                   
                            } 
  						    else 
   							    printf("il numero di contenitori inseriti non corrisponde al numero di contenitori creati\n");
 					       
						    free(capacita); 							     
        						 
  						  
			 		   }
        	           else{ 
                            printf(error1);
                            printf(error4);     
                       }
                       
                       break;	 
		    
		    /*dichiara pericolosa/innocua la configurazione in cui il contenitore i ha livello ai*/
            case 'p': case 'i':
                 
                      if(!is_empty(g)) { 
                    	
    						    i = 1;
        						n_cont=0;		  
        						
        						/*inizializzo ed espando gli array dinamicamente nel while*/ 
        						numero=malloc(sizeof(char));	
        						capacita=malloc(sizeof(int)); 
        						
        						while(1)
        						 {
        							car=getchar();
        						 
        							if(!isspace(car)){						 
        							   
        							   numero = realloc(numero, ++i*sizeof(char)); 
        							   numero[i-2]=car;
        							}
        							else if(i>1){
        							
        								numero[i-1]='\0';								
        								if((c = atoi(numero)) >= 0){
        								
        									if(n_cont++>0)
        										capacita = realloc(capacita, n_cont*sizeof(int)); 
        									capacita[n_cont-1]=c;									
        								
        								}
        								i=1;
        								numero=malloc(i*sizeof(char));
        							}
        							
        							if(car=='\n') break;
        							
        						 }
        						 
        						if(n_cont>0){
                                             
                                    for(n_cont=0,i=0;i<g->nc;i++)
                                       if(g->attuale[i] == capacita[i])
                                           n_cont++;
                                	
                                    if(n_cont!=g->nc)
        			                   g=pericolosa_innocua_visitato(g,capacita,p);
        			                   
                                    else
                                       printf("impossibile impostare come configurazione pericolosa la configurazione attuale\n"); 
                                } 
        						else 
        							printf("nessun contenitore correttamente inserito\n");
        							
        						free(capacita);		
    					    
					   }
					   else{ 
                            printf(error1);
                            printf(error4);     
                       }
					
					   break;
                       
             /*configurazioni*/
			case 'c':  if(!is_empty(g)){
                 
                          while(isspace(car=getchar()));	 
                          d =atoi(leggi_stringa(car));
    					 
    					   if(d<0)
    						  printf("valori non corretti");
    					   else{
                                capacita = malloc(sizeof(int));
                                if(!capacita)
                                   printf("errore di allocazione");
		
                                dim = 0;
                                
    						    capacita=configurazioni(g,0,0,d,capacita,&dim); 
    						    
    						    for(j=0; j < dim; j++)
                                {
                                    i=capacita[j]; 
                                    
        						    stampa_configurazione(g,g->nodi[i],NULL,'c');                                    
                                }
                                
                                /*riconfigura il colore BIANCO*/
                                pericolosa_innocua_visitato(g,g->attuale,'B');
                                
                                printf("\n"); 					     
                           }	
                       }		  
					   else {
                            printf(error1);
                            printf(error4);     
                       } 	
			           break;
                       
            /*contenenti(k)*/
			case 'w':  if(!is_empty(g)){
                 
                           while(isspace(car=getchar()));	 
                           k =atoi(leggi_stringa(car));
    						 
    					   if(k<0)
    					   
    						   printf("valore non corretto");
    						 
    					   else{
                                g=pericolosa_innocua_visitato(g,g->attuale,'N');
                                
                                aux = calloc(g->nv , sizeof(int));
    					  	    capacita=malloc(sizeof(int));
                                dim = 0;
                            
                            	if(!aux || !capacita) {
                            		fprintf(stderr,"Errore di Allocazione\n");
                            		exit(-4);
                            	}
                            	
                            	capacita=contenenti(g,chiave(g,g->attuale)-1,capacita,&dim,k);
                            		 
                            	free(aux);
    						    
                                /*for(j=0; j < dim[0]; j++) printf("%d,",capacita[j]); */
                                
                                if(dim==0){ printf("NON PRESENTI\n"); }
                                
                                else {
                                    
                                    for(j=dim-1; j >=0 ; j--)
                                    {
                                        i=capacita[j];
                                        
            						    /* per ogni elemento di capacita.. stampa conf*/
           						        stampa_configurazione(g,g->nodi[i],NULL,'w'); 
                                        
                                    }
                                    printf("\n\n");
                                }
                                /*riconfigura il colore BIANCO*/
                                g=pericolosa_innocua_visitato(g,g->attuale,'B');
                           }
                       }
                       else {
                            
                            printf(error1);
                            printf(error4);     
                       } 
                             	 	
			           break;	
           				 
			 /*'m': mosse(k) stampa una sequenza ammissibile di lunghezza minima tra quelle che partono dalla configurazione 
               attuale ad una configurazione contenente il valore k come livello di almeno un contenitore.*/
               
             /*'k': critica(a) stampa, se esiste, una configurazione a' (distinta da a e dalla configurazione attuale) tale che
               ogni sequenza di operazioni elementari dalla configurazione attuale alla configurazione a contiene a'.*/
               
			case 'm': case 'k':  
                            if(!is_empty(g)){
                 
    						    i = 1;
        						n_cont=0;		  
        						
        						/*inizializzo ed espando gli array dinamicamente nel while*/ 
        						numero=malloc(sizeof(char));	
        						capacita=malloc(sizeof(int)); 
        						
        						while(1)
        						 {
        							car=getchar();
        						 
        							if(!isspace(car)){						 
        							   
        							   numero = realloc(numero, ++i*sizeof(char)); 
        							   numero[i-2]=car;
        							}
        							else if(i>1){
        							
        								numero[i-1]='\0';								
        								if((c = atoi(numero)) >= 0){
        								
        									if(n_cont++>0)
        										capacita = realloc(capacita, n_cont*sizeof(int)); 
        									capacita[n_cont-1]=c;									
        								
        								}
        								i=1;
        								numero=malloc(i*sizeof(char));
        							}
        							
        							if(car=='\n') break;
        							
        						 }                                
        						 
        						 if(p=='m'){
            						if(n_cont>0 && n_cont<=2){ 
                                                
                                       capacita = realloc(capacita, (n_cont+1) * sizeof(int));
                                       capacita[n_cont] = -1;  
                                       
                                       /*Per il caso mosse, assegno i pesi ad ogni nodo del grafo, utilizzando una visita in ampiezza */
                                       
                                       if(g->pesato==0)
                                          g=grafo_pesato(g);
                                       
        					  	       /*chiama Dijkstra per la creazione dell'albero dei cammini minimi*/
        					  	       
        					  	       esiste_ragg_mosse_critica (g, capacita, p);   
        					  	       
                                    } 
            						else 
            							printf("nessun livello di contenitore correttamente inserito\n");
                                 }
                                 else if(p=='k'){
                                 
                                         if(n_cont>0){
                                            int cont = n_cont; 
                                            for(n_cont=0,i=0;i<g->nc;i++)
                                               if(g->attuale[i] == capacita[i])
                                                   n_cont++;
                                        	
                                            if(n_cont==g->nc){
                			                   printf("impossibile impostare come configurazione di arrivo la configurazione attuale\n"); 
                			                   break;   
                                            }
                                            
                                            if(cont==g->nc){
                                                            
                                                            capacita = realloc(capacita, (cont+1) * sizeof(int));
                                                            capacita[cont] = -1;  
                            					  	       
                            					  	        esiste_ragg_mosse_critica (g, capacita, p); 
                                            }                
                                            else   
                                               printf("Il numero di livelli inseriti per la configurazione di arrivo non e' valido. Riprovare.");
                                         } 
                						 else 
                							 printf("nessun contenitore correttamente inserito\n");
                                 }     
        							
        						free(capacita);
                       }
                       else {
                            
                            printf(error1);
                            printf(error4);     
                       } 
                             	 	
			           break;
			           
	         case 'A': g->sv_attivo = 1; break;
             
             case 'D': g->sv_attivo = 0; break;   
                       
             case 'V': /*debugging*/
                      if(!is_empty(g)) {
                                       
                          printf("\n\nQuesta e' la rappresentazione del grafo orientato con liste di adiacenza\n");
                          printf("-------------------------------------------------------------------------\n");
                          printf("\nLegenda:\n\n");
                          printf("- Ogni configurazione e' preceduta dal campo chiave\n");
                          printf("- Colore: B = Bianco, G = Grigio, N = Nero\n");
                          printf("- Peso: inizialmente il grafo ha tutti i pesi a 1\n");
                          printf("- Azione: s=svuotamento, r=riempimento, t=travaso\n");
                          printf("- %c indica i nodi con configurazione pericolosa  \n",1);
                          printf("\nGrafo:\n");
                          for(i = 0; i < g->nv; i++) {
    			                if(g->nodi[i]->colore==1) p='G'; else if(g->nodi[i]->colore==0) p='B'; else if(g->nodi[i]->colore==2) p='N';
    							printf("\n%c%2d %c %c%2d [",g->nodi[i]->azione,g->nodi[i]->costo,p,g->nodi[i]->pericolosa,
                                                   g->nodi[i]->v);
    							
    							for(j=0;j<g->nc;j++){
    								printf("%d",g->nodi[i]->conf[j]);
    								if(j!=g->nc-1) printf(",");
    						    }
    							printf("]");
    							
    							stampalista(g->nodi[i]->next,g->nc);
    					   }
    					   printf("\n"); 
                       }
					   break;
			 
		}/*fine switch*/
   }
   /*fine while*/
   
   return 0;

}

