/********************************/
/*	SUDOKU SOLVER		*/
/********************************/

/* By Lelli Massimo on August 2007 */



#include <stdio.h>
#include <unistd.h>

#define	A	10
#define	B	11
#define	C	12
#define	D	13
#define	E	14
#define	F	15
#define	G	16

#define MAXN	4	/* Massima dimensione */
#define DEFN	3	/* Dimensione di default */

#define DESTRA	  1
#define	SINISTRA  0	/* Per algoritmo permutazione */

#define YES	1
#define NOT	0

#define OPZ1	"-n="
#define OPZ2	"-f="
#define OPZ3	"-h"
#define OPZ4	"-?"
#define OPZ5	"h"
#define OPZ6	"?"
#define OPZ7	"-clear"
#define OPZ8	"-sd"
#define OPZ9	"-d"	/* opzioni di lancio */

typedef struct
	{
	int value;  /* Numero contenuto nella casella */
	int dir;    /* Direzione di permutazione */
	} Casella;

typedef struct
	{
	int numcas; /* Num. caselle nel sottoquadrato */
	Casella sq[MAXN*MAXN];	/* Sottoquadrato */
				/* una riga per volta */
	} Square;	/* Definisce un sottoquadrato */

int sqinit3_e[MAXN*MAXN][MAXN*MAXN] = 
	{
		{6,0,0, 0,9,0, 0,0,8},
		{0,9,0, 6,0,7, 0,5,0},
		{0,0,4, 0,0,0, 1,0,0},

		{0,7,0, 0,0,0, 0,2,0},
		{9,0,0, 0,8,0, 0,0,5},
		{0,4,0, 0,0,0, 0,6,0},

		{0,0,3, 0,0,0, 9,0,0},
		{0,2,0, 4,0,8, 0,7,0},
		{5,0,0, 0,3,0, 0,0,2}
	}; /* Schema 3x3x3 di default "scolpito" */

int sqinit3_ss_e[MAXN*MAXN][MAXN*MAXN] = 
	{
		{0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 6,0,0, 4,0,0},
		{0,5,9, 1,0,0, 2,0,0},

		{0,0,0, 8,0,3, 7,9,0},
		{0,0,0, 0,7,0, 0,0,0},
		{0,4,8, 9,0,5, 0,0,0},

		{0,0,1, 0,0,4, 3,2,0},
		{0,0,7, 0,0,8, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0}
	}; /* Schema 3x3x3 sd di default "scolpito" */

int sqinit4_e[MAXN*MAXN][MAXN*MAXN] = 
	{
		{7,1,0,8, 0,C,4,0, 0,2,6,0, D,0,G,9},
		{9,0,0,5, 0,3,0,A, G,0,4,0, E,0,0,8},
		{0,0,0,0, 0,0,0,B, C,0,0,0, 0,0,0,0},
		{A,4,0,0, 7,D,0,0, 0,0,5,E, 0,0,1,F},

		{0,0,0,A, 0,0,8,0, 0,1,0,0, C,0,0,0},
		{8,9,0,B, 0,0,D,C, F,G,0,0, 7,0,3,A},
		{2,0,0,0, G,E,0,0, 0,0,9,B, 0,0,0,1},
		{0,G,1,0, 0,F,0,5, 4,0,E,0, 0,8,9,0},

		{0,3,C,0, 0,A,0,8, 2,0,B,0, 0,D,E,0},
		{5,0,0,0, 3,B,0,0, 0,0,D,A, 0,0,0,C},
		{G,2,0,D, 0,0,7,E, 1,3,0,0, F,0,B,6},
		{0,0,0,7, 0,0,G,0, 0,5,0,0, 9,0,0,0},

		{F,B,0,0, A,2,0,0, 0,0,C,5, 0,0,D,G},
		{0,0,0,0, 0,0,0,G, 9,0,0,0, 0,0,0,0},
		{6,0,0,1, 0,4,0,7, D,0,G,0, A,0,0,5},
		{C,5,0,G, 0,9,E,0, 0,B,A,0, 1,0,F,2}
	}; /* Schema 4x4x4 di default "scolpito" */

int sqinit4_ss_e[MAXN*MAXN][MAXN*MAXN] = 
	{
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},

		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},

		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},

		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
		{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}
	}; /* Schema 4x4x4 sd di default "scolpito" */

Square squares[MAXN*MAXN];  /* Lista sottoquadrati   */
			    /* (da sinistra a destra */
			    /* dall'alto al basso)   */

/* Configurazione attuale a matrice */
int field[MAXN*MAXN][MAXN*MAXN];

/* Schema di default corrente */
int sqinit[MAXN*MAXN][MAXN*MAXN];

int dim;	/* Dimensione del sottoquadrato */
int d_sudoku;	/* Attivazione diagonal sudoku */
int clear;	/* Attivazione ricerca esaustiva */
int debug;	/* Attivazione debug */

void initsq(int ns);
int permuta(int ns);
int check(void);
void mostra(int nsol);
void genField(void);
void welcome(void);
void bye(int nsol);
int getOpt(int argc,char **argv);
int getOptions(int argc,char **argv);
void carDefault(void);
void carDefSq(void);


/********************/
/*		    */
/*	Main	    */
/*		    */
/********************/

/* Main program */

int main(int argc,char **argv)
{
int ns=0;	/* Init numero sottoquadro corrente */
int nsol=0;	/* Init numero soluzioni trovate */
int go=YES;	/* Attiva i loop */
int count=0;	/* per debug */

carDefault();	/* Carica i valori di default */

/* Ricava le opzioni di lancio essenziali */
if (getOpt(argc,argv)==YES)
	return(0);

carDefSq();	/* Carica quadro di default */

/* Ricava le opzioni di lancio */
if (getOptions(argc,argv)==YES)
	return(0);

/* Azzera il campo di lavoro */
memset(squares,0,sizeof(squares));

welcome();	/* Greetings.. */

initsq(ns);	/* Init sottoquadro corrente */

while (go)
	{
/* Controllo se lo schema allo stato attuale va' bene */
	if (check()==YES)
		{
		if (debug)
printf("Check superato nr.=%d\tNr. pezzi in gioco=%d\n",
count++,ns);

/* Se va' bene verifica se e' una soluzione */
		if (ns<dim*dim-1)
			{
/* Non e' una soluzione: */
/* passa al sottoquadro successivoe lo inizializza */
			initsq(++ns);
			continue;
			}
		else
/* E' una soluzione: */
/* incrementa il numero di soluzioni e la visualizza */
			mostra(++nsol);
		}

/* Permuta il sottoquadro se possibile */
	while (go && permuta(ns)==NOT)
	  {
/* Essendo gia' all'ultima permutazione */
/* azzera il sottoquadro corrente */
	  memset(&squares[ns],0,sizeof(squares[ns]));

/* Se era il primo sottoquadro dichiara la fine */
		if (ns==0)
			{
			bye(nsol);	/* Saluti */
			go=NOT;	/* Disattiva i loop */
			}
		else
/* Altrimenti torna al sottoquadro precedente */
			--ns;
	  }
	}
return(0);
}


/********************/
/*		    */
/*	 Initsq	    */
/*		    */
/********************/

/* Init sottoquadro */

/* INPUT:	ns	numero del sottoquadro */

void initsq(int ns)
{
int ind;
int nr;
int nc;
int esito;

/* Default numero caselle del sottoquadro */
squares[ns].numcas=0;

/* Loop sui valori da inserire */
/* in ogni casella del sottoquadro */
for (ind=0;ind<dim*dim;ind++)
	{
	esito=NOT;

/* Loop sulle righe dello schema da risolvere */
	for (nr=0;nr<dim;nr++)
	  {
	  if (esito==YES)
		break;
/* Loop sulle colonne dello schema da risolvere */
	  for (nc=0;nc<dim;nc++)
/* Controllo se il valore da inserire */
/* e' presente nello schema da risolvere */
      if (ind+1==sqinit[ns/dim*dim+nr][(ns%dim)*dim+nc])
		{
		esito=YES;
		break;
		}
	  }

/* Solo se il valore non e' presente */
/* lo considera come elemento del sottoquadro */
	if (esito==NOT)
	 {
/* Init valore */
	 squares[ns].sq[squares[ns].numcas].value=ind+1;
/* Init direzione di permutazione */
	squares[ns].sq[squares[ns].numcas].dir=SINISTRA;
/* Aggiorna numero caselle del sottoquadro */
	 squares[ns].numcas++;
	 }
	}
}


/********************/
/*		    */
/* 	Permuta	    */
/*		    */
/********************/

/* Permutazione sottoquadro */

/* INPUT:	ns	numero del sottoquadro */
/* OUTPUT:	YES/NOT se la permutazione */
/*              e' stata effettivamente effettuata */

int permuta(int ns)
{
int ind;
int max_v=0;
int max_i;
int dir;
int value;
int indv;

for (ind=0;ind<squares[ns].numcas;ind++)		/* Ricava il massimo mobile */
	{
	dir=squares[ns].sq[ind].dir;
	value=squares[ns].sq[ind].value;

	if (((dir==SINISTRA && ind>0 &&
	      value>squares[ns].sq[ind-1].value) ||
	     (dir==DESTRA && ind<squares[ns].numcas-1 &&
	      value>squares[ns].sq[ind+1].value)) &&
		value>max_v)
		{
		max_v=value;
		max_i=ind;
		}
	}

/* Test se esiste almeno un casella mobile */
/* Se no le permutazioni sono finite */
if (max_v==0)
	return(NOT);

/* Loop su ogni casella */
for (ind=0;ind<squares[ns].numcas;ind++)
	{
/* Per la massima casella mobile */
/* viene effettuato uno swap */
/* con la casella adiacente nella direzione opportuna */
	if (ind==max_i)
		{
		dir=squares[ns].sq[max_i].dir;
		value=squares[ns].sq[max_i].value;
/* Ricava casella da scambiare in base alla direzione */
	     indv = (dir==SINISTRA) ? max_i-1 : max_i+1;
 squares[ns].sq[max_i].dir=squares[ns].sq[indv].dir;
 squares[ns].sq[max_i].value=squares[ns].sq[indv].value;
		squares[ns].sq[indv].dir=dir;
		squares[ns].sq[indv].value=value;
		}
	else
/* Per tutte le altre ne viene commutata la direzione */
		if (squares[ns].sq[ind].value>max_v)
			squares[ns].sq[ind].dir^=DESTRA;
	}

return(YES);
}


/*********************/
/*		     */
/*	Mostra	     */
/*		     */
/*********************/

/* Visualizza schema attuale */

/* INPUT: se nsol>0 lo schema attuale rappresenta una */
/*                  soluzione e ns ne e' il numero */

void mostra(int nsol)
{
int nr;
int nc;
int ind;
int val;

/* Genera matrice field dai vari sottoquadri */
/* e dallo schema da risolvere */
genField();

if (nsol>0)
	printf("\nSoluzione nr.%d\n",nsol);

for (nr=0;nr<dim*dim;nr++)
	{
	if (nr%dim==0)
		{
		for (ind=0;ind<dim*(dim+1);ind++)
			if (ind%(dim+1)==0)
				printf(" ");
			else
				printf("---");
		printf("\n");
		}


	for (nc=0;nc<dim*dim;nc++)
		{
		if (nc%dim==0)
			printf("|");

		if (field[nr][nc]==0)
			printf("  .");
		else
			{
			val=field[nr][nc];
			if (val<10)
			  printf("%3d",val);
			else
			  printf("  %c",val-10+'A');
			}
		}
	printf("|\n");
	}

for (ind=0;ind<dim*(dim+1);ind++)
	if (ind%(dim+1)==0)
		printf(" ");
	else
		printf("---");
printf("\n");
}


/*********************/
/*		     */
/*	Check	     */
/*		     */
/*********************/

/* Controllo se lo schema corrente va' bene */

/* OUTPUT:	YES/NOT	 */


int check(void)
{
int nr;
int nc;
int ind;
int ind1;
int ind2;

/* Genera matrice field dai vari sottoquadri */
/* e dallo schema da risolvere */
genField();

/* Controllo righe */
for (nr=0;nr<dim*dim;nr++)
	for (ind1=0;ind1<dim*dim-1;ind1++)
		{
/* controlla il punto di inizio di riga vuota */
		if (field[nr][ind1]==0)
			continue;
	for (ind2=(1+ind1/dim)*dim;ind2<dim*dim;ind2++)
		if (field[nr][ind1]==field[nr][ind2])
				return(NOT);
		}

/* Controllo colonne */
for (nc=0;nc<dim*dim;nc++)
	for (ind1=0;ind1<dim*dim-1;ind1++)
		{
/* controlla il punto di inizio di colonna vuota */
		if (field[ind1][nc]==0)
			continue;
	for (ind2=(1+ind1/dim)*dim;ind2<dim*dim;ind2++)
		if (field[ind1][nc]==field[ind2][nc])
			return(NOT);
		}

if (d_sudoku)
	{
/* Controllo prima diagonale */
	for (ind=0;ind<dim*dim-1;ind++)
		{
/* controlla il punto di inizio di riga vuota */
		if (field[ind][ind]==0)
			continue;
	for (ind1=(1+ind/dim)*dim;ind1<dim*dim;ind1++)
		if (field[ind][ind]==field[ind1][ind1])
			{
			if (debug)
		  printf("Errore in prima diagonale\n");
			return(NOT);
			}
		}
/* Controllo seconda diagonale */
	for (ind=0;ind<dim*dim-1;ind++)
		{
/* controlla il punto di inizio di riga vuota */
		if (field[ind][dim*dim-ind-1]==0)
			continue;
	for (ind1=(1+ind/dim)*dim;ind1<dim*dim;ind1++)
		if (field[ind][dim*dim-ind-1]==
			field[ind1][dim*dim-ind1-1])
				{
				if (debug)
		printf("Errore in seconda diagonale\n");
				return(NOT);
				}
		}
	}

return(YES);
}


/*********************/
/*		     */
/*	GenField     */
/*		     */
/*********************/

/* Genera matrice field dai vari */
/* sottoquadri e dallo schema da risolvere */

void genField(void)
{
int nr;
int nc;
int ns;
int ind;
int ind1;
int val;
/* elenco sottoquadri corrente */
/* (merge tra schema da risolvere */
/* e sottoquadri permutati) */
int sq[MAXN*MAXN][MAXN*MAXN];

/* Genera lista sq d'appoggio */
for (ns=0;ns<dim*dim;ns++)
	{
	ind1=0;
	for (ind=0;ind<dim*dim;ind++)
		{
val=sqinit[ns/dim*dim+ind/dim][(ns%dim)*dim+ind%dim];
		if (val!=0)
			sq[ns][ind]=val;
		else
			{
		sq[ns][ind]=squares[ns].sq[ind1].value;
			ind1++;
			}
		}
	}

	/* Ricava la matrice di output */
for (nr=0;nr<dim*dim;nr++)
  for (nc=0;nc<dim*dim;nc++)
    field[nr][nc]=
	sq[nr/dim*dim+nc/dim][(nr%dim)*dim+nc%dim];
}


/*********************/
/*		     */
/*	Welcome	     */
/*		     */
/*********************/

/* Alcune smancerie... */ 

void welcome(void)
{
printf("\n");
printf("========== Sudoku solver =============\n");
printf("\n");
printf("By Lelli Massimo on August 2007\n");
printf("\n");
printf("======================================\n");
printf("\n");

if (clear==NOT)
	{
	printf("Schema da risolvere:\n");
	printf("\n");
	mostra(-1);
	}
else
	printf("\nRicercare tutte le soluzioni\n\n");

if (d_sudoku)
	printf("\nDiagonal sudoku abilitato\n\n");

if (debug)
	printf("\nDebug abilitato\n\n");

printf("\n\n");

printf("Premere un tasto\n");
getchar();

printf("\nLet's go!\n\n\n");
}


/*********************/
/*		     */
/*	 Bye	     */
/*		     */
/*********************/

/* Saluti di rito... */

/* INPUT:	nsol	numero soluzioni trovate */

void bye(int nsol)
{
printf("\n\nFine della ricerca!\n\n\n");
printf("Numero soluzioni totali=%d\n\n",nsol);
printf("\n\nPremere un tasto\n");
getchar();
printf("\nBye!\n");
sleep(1);
}


/*****************/
/*		 */
/*   getOpt	 */
/*		 */
/*****************/

/* Ricava le opzioni di lancio essenziali */

/* OUTPUT:	YES/NOT		se uscire o meno */

int getOpt(int argc,char **argv)
{
int ind;

for (ind=1;ind<argc;ind++)
	{
/* opzione -n= per settare la dimensione */
	if (!strncmp(argv[ind],OPZ1,strlen(OPZ1)))
		{
	sscanf(argv[ind]+strlen(OPZ1),"%d",&dim);
		if (dim<=0 || dim>MAXN)
			dim=DEFN;
		}
	if (!strncmp(argv[ind],OPZ8,strlen(OPZ8)))
		d_sudoku=YES;
	}

return(NOT);
}


/*********************/
/*		     */
/*	GetOptions   */
/*		     */
/*********************/

/* Ricava le opzioni di lancio */

/* OUTPUT:	YES/NOT		se uscire o meno */

int getOptions(int argc,char **argv)
{
int ind;
int nr;
int nc;
char c;
int posc;
char strf[(MAXN+2)*(MAXN+2)*(MAXN+2)*(MAXN+2)];

for (ind=1;ind<argc;ind++)
	{
/* opzione -f= per settare lo schema da risolvere */
	if (!strncmp(argv[ind],OPZ2,strlen(OPZ2)))
	  {
	  for (nr=0;nr<dim*dim;nr++)
	    for (nc=0;nc<dim*dim;nc++)
	      sqinit[nr][nc]=0;

	  sscanf(argv[ind]+strlen(OPZ2),"%s",strf);
	  posc=0;
	  for (nr=0;nr<dim*dim;nr++)
	    for (nc=0;nc<dim*dim;nc++)
	      {
		if (posc<strlen(strf))
		      {
			c=strf[posc];
			if (c>='A')
		          sqinit[nr][nc]=c-'A'+10;
			else
		          sqinit[nr][nc]=c-'0';
		      }
	        posc++;
	      }
	  }
/* opzioni -clear per sbiancare schema da risolvere */
	if (!strncmp(argv[ind],OPZ7,strlen(OPZ7)))
		{
		memset(sqinit,0,sizeof(sqinit));
		clear=YES;
		}
/* opzioni di help */
	if (!strncmp(argv[ind],OPZ3,strlen(OPZ3)) ||
	    !strncmp(argv[ind],OPZ4,strlen(OPZ4)) ||
	    !strncmp(argv[ind],OPZ5,strlen(OPZ5)) ||
	    !strncmp(argv[ind],OPZ6,strlen(OPZ6)))
		{
		printf("Commands availables:\n\n");
		printf("-n=Array rank (default=3)\n");
		printf("-f=Field to solve (format: d000ddd... d=digit 0=blank  one row after another)\n");
	printf("-clear to search all the solutions\n");
		printf("-sd enable diagonal-sudoku\n");
		printf("-d enable debug mode\n");
		printf("-h=This help\n");
		printf("-?=This help\n");
		printf("h =This help\n");
		printf("? =This help\n");
		return(YES);
		}
	if (!strncmp(argv[ind],OPZ9,strlen(OPZ9)))
		debug=YES;
	}

return(NOT);
}


/*********************/
/*		     */
/*    CarDefault     */
/*		     */
/*********************/

/* Carica i valori di default */

void carDefault(void)
{
int nr;
int nc;

dim=DEFN;		/* default dimensione */
d_sudoku=NOT;		/* no diagonal sudoku */
clear=NOT;		/* vi e' schema da risolvere */
debug=NOT;		/* no debug */
}





/*********************/
/*		     */
/*      carDefSq     */
/*		     */
/*********************/

/* Carica quadro di default */

void carDefSq(void)
{
int nr;
int nc;

switch(dim)
	{
	case 2:
	break;
	case 3:
	if (d_sudoku==NOT)
		for (nr=0;nr<dim*dim;nr++)
		  for (nc=0;nc<dim*dim;nc++)
		    sqinit[nr][nc]=sqinit3_e[nr][nc];
	else
		for (nr=0;nr<dim*dim;nr++)
		  for (nc=0;nc<dim*dim;nc++)
		    sqinit[nr][nc]=sqinit3_ss_e[nr][nc];
	break;
	case 4:
	if (d_sudoku==NOT)
		for (nr=0;nr<dim*dim;nr++)
		  for (nc=0;nc<dim*dim;nc++)
		    sqinit[nr][nc]=sqinit4_e[nr][nc];
	else
		for (nr=0;nr<dim*dim;nr++)
		  for (nc=0;nc<dim*dim;nc++)
		    sqinit[nr][nc]=sqinit4_ss_e[nr][nc];
	break;
	}
}
