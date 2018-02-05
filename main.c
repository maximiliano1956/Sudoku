/********************************************/
/*				SUDOKU SOLVER				*/
/********************************************/

/* By Lelli Massimo on August 2007 */



#include <stdio.h>
#ifndef D_LINUX
#include <windows.h>
#endif

#define MAXN	5			/* Massima dimensione */
#define DEFN	3			/* Dimensione di default */

#define DESTRA		1
#define	SINISTRA	0		/* Servono all'algoritmo della permutazione */

#define YES	1
#define NOT 0

#define OPZ1	"-n="
#define OPZ2	"-f="
#define OPZ3	"-h"
#define OPZ4	"-?"
#define OPZ5	"h"
#define OPZ6	"?"
#define OPZ7	"-clear"
#define OPZ8	"-ss"
#define OPZ9	"-d"		/* opzioni di lancio */

#ifndef D_LINUX
#define ASPETTA(x)	Sleep(x)
#else
#define ASPETTA(x)	sleep((x)/1000)
#endif

typedef struct
		{
		int value;					/* Numero contenuto nella casella */
		int dir;					/* direzione corrente di permutazione */
		} Casella;

typedef struct
		{
		int numcas;					/* Numero caselle nel sottoquadrato */
		Casella sq[MAXN*MAXN];		/* Sottoquadrato una riga per volta */
		} Square;					/* Definisce un sottoquadrato */

int sqinit_e[MAXN*MAXN][MAXN*MAXN] = 
	{
		{6,0,0, 0,9,0, 0,0,8},
		{0,9,0, 6,0,7, 0,5,0},
		{0,0,4, 0,0,0, 1,0,0},

		{0,7,0, 0,0,0, 0,2,0},
		{9,0,0, 0,8,0, 0,0,5},
		{0,4,0, 0,0,0, 0,6,0},

		{0,0,3, 0,0,0, 9,0,0},
		{0,2,0, 4,0,8, 0,7,0},
		{5,0,0, 0,3,0, 0,0,2},
	};								/* Schema di default "scolpito" */

Square squares[MAXN*MAXN];			/* Lista dei sottoquadrati (da sinistra a destra dall'alto al basso) */
int field[MAXN*MAXN][MAXN*MAXN];	/* Configurazione attuale a matrice */
int sqinit[MAXN*MAXN][MAXN*MAXN];	/* Schema di default corrente */
int dim;							/* Dimensione del sottoquadrato */
int super_sudoku;					/* Attivazione super sudoku */
int clear;							/* Attivazione ricerca esaustiva */
int debug;							/* Attivazione debug */

void initsq(int ns);
int permuta(int ns);
void stampa(int ns);
int check(void);
void mostra(int nsol);
void genField(void);
void welcome(void);
void bye(int nsol);
int getOptions(int argc,char **argv);
void carDefault(void);


/********************/
/*					*/
/*		Main		*/
/*					*/
/********************/

/* Main program */

int main(int argc,char **argv)
{
int ns=0;		/* Init numero sottoquadro corrente */
int nsol=0;		/* Init numero soluzioni trovate */
int go=YES;		/* Attiva i loop */
int count=0;	/* per debug */

carDefault();						/* Carica i valori di default */

if (getOptions(argc,argv)==YES)		/* Ricava le opzioni di lancio */
	return;

memset(squares,0,sizeof(squares));	/* Azzera il campo di lavoro */

welcome();			/* Greetings.. */

initsq(ns);	/* Init sottoquadro corrente */

while (go)
		{
		if (check()==YES)			/* Controllo se lo schema allo stato attuale va' bene */
			{
			if (debug)
				{
				printf("Check superato nr.=%d\tNr. pezzi in gioco=%d\n",count++,ns);
				}

			if (ns<dim*dim-1)		/* Se va' bene verifica se e' una soluzione */
				{
				initsq(++ns);		/* Non e' una soluzione: passa al sottoquadro successivo e lo inizializza */
				continue;
				}
			else
				mostra(++nsol);		/* E' una soluzione: incrementa il numero di soluzioni e la visualizza */
			}

		while (go && permuta(ns)==NOT)	/* Permuta il sottoquadro se possibile */
			{
			memset(&squares[ns],0,sizeof(squares[ns]));		/* Essendo gia' all'ultima permutazione */
															/* azzera il sottoquadro corrente */
			if (ns==0)										/* Se era il primo sottoquadro dichiara la fine */
				{
				bye(nsol);				/* Saluti */
				go=NOT;					/* Disattiva i loop */
				}
			else
				--ns;					/* Altrimenti torna al sottoquadro precedente */
			}
		}
return(0);
}


/********************/
/*					*/
/*	    Initsq		*/
/*					*/
/********************/

/* Init sottoquadro */

/* INPUT:	ns		numero del sottoquadro */

void initsq(int ns)
{
int ind;
int nr;
int nc;
int esito;

squares[ns].numcas=0;			/* Default numero caselle del sottoquadro */

for (ind=0;ind<dim*dim;ind++)	/* Loop sui valori da inserire in ogni casella del sottoquadro */
	{
	esito=NOT;

	for (nr=0;nr<dim;nr++)		/* Loop sulle righe dello schema da risolvere */
		{
		if (esito==YES)
			break;
		for (nc=0;nc<dim;nc++)	/* Loop sulle colonne dello schema da risolvere */
			if (ind+1==sqinit[ns/dim*dim+nr][(ns%dim)*dim+nc])	/* Controllo se il valore da inserire */
																/* e' presente nello schema da risolvere */ 
				{
				esito=YES;
				break;
				}
		}

	if (esito==NOT)		/* Solo se il valore non e' presente lo considera come elemento del sottoquadro */
		{
		squares[ns].sq[squares[ns].numcas].value=ind+1;		/* Init valore */
		squares[ns].sq[squares[ns].numcas].dir=SINISTRA;	/* Init direzione di permutazione */
		squares[ns].numcas++;								/* Aggiorna numero caselle del sottoquadro */
		}
	}
}


/********************/
/*					*/
/*	   Permuta		*/
/*					*/
/********************/

/* Permutazione sottoquadro */

/* INPUT:	ns		numero del sottoquadro */
/* OUTPUT:	YES/NOT	se la permutazione e' stata effettivamente effettuata */

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

	if (((dir==SINISTRA && ind>0 && value>squares[ns].sq[ind-1].value) ||
		(dir==DESTRA && ind<squares[ns].numcas-1 && value>squares[ns].sq[ind+1].value))
		&& value>max_v)
		{
		max_v=value;
		max_i=ind;
		}
	}

if (max_v==0)									/* Test se esiste almeno un casella mobile */
	return(NOT);								/* Se no le permutazioni sono finite */

for (ind=0;ind<squares[ns].numcas;ind++)		/* Loop su ogni casella */
	{
	if (ind==max_i)								/* Per la massima casella mobile viene effettuato uno swap */
												/* con la casella adiacente nella direzione opportuna */
		{
		dir=squares[ns].sq[max_i].dir;
		value=squares[ns].sq[max_i].value;

		indv = (dir==SINISTRA) ? max_i-1 : max_i+1;	/* Ricava la casella da scambiare in base alla direzione */

		squares[ns].sq[max_i].dir=squares[ns].sq[indv].dir;
		squares[ns].sq[max_i].value=squares[ns].sq[indv].value;
		squares[ns].sq[indv].dir=dir;
		squares[ns].sq[indv].value=value;
		}
	else
		if (squares[ns].sq[ind].value>max_v)	/* Per tutte le altre ne viene commutata la direzione */
			squares[ns].sq[ind].dir^=DESTRA;
	}

return(YES);
}


/********************/
/*					*/
/*		Stampa		*/
/*					*/
/********************/

/* Visualizza sottoquadro (attualmente non chiamata: e' servita in fase di debug)*/

/* INPUT:	ns		numero del sottoquadro */

void stampa(int ns)
{
int ind;

for (ind=0;ind<squares[ns].numcas;ind++)
	{
	if (squares[ns].sq[ind].dir==SINISTRA)
		{
		printf("<");
		printf("%d",squares[ns].sq[ind].value);
		}
	else
		{
		printf("%d",squares[ns].sq[ind].value);
		printf(">");
		}
	printf(" ");
	}

printf("\n");
}


/*********************/
/*					 */
/*		Mostra		 */
/*					 */
/*********************/

/* Visualizza schema attuale */

/* INPUT:	se ns>0	lo schema attuale rappresenta una soluzione e ns ne e' il numero */

void mostra(int nsol)
{
int nr;
int nc;
int ind;

genField();				/* Genera matrice field dai vari sottoquadri e dallo schema da risolvere */

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
			printf("%3d",field[nr][nc]);
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
/*					 */
/*		Check		 */
/*					 */
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

genField();						/* Genera matrice field dai vari sottoquadri e dallo schema da risolvere */

for (nr=0;nr<dim*dim;nr++)					/* Controllo righe */
	for (ind1=0;ind1<dim*dim-1;ind1++)
		{
		if (field[nr][ind1]==0)				/* controlla il punto di inizio di riga vuota */
			continue;
		for (ind2=(1+ind1/dim)*dim;ind2<dim*dim;ind2++)
			if (field[nr][ind1]==field[nr][ind2])
				return(NOT);
		}

for (nc=0;nc<dim*dim;nc++)					/* Controllo colonne */
	for (ind1=0;ind1<dim*dim-1;ind1++)
		{
		if (field[ind1][nc]==0)				/* controlla il punto di inizio di colonna vuota */
			continue;
		for (ind2=(1+ind1/dim)*dim;ind2<dim*dim;ind2++)
			if (field[ind1][nc]==field[ind2][nc])
				return(NOT);
		}

if (super_sudoku)
	{
	for (ind=0;ind<dim*dim-1;ind++)					/* Controllo prima diagonale */
		{
		if (field[ind][ind]==0)				/* controlla il punto di inizio di riga vuota */
			continue;
		for (ind1=(1+ind/dim)*dim;ind1<dim*dim;ind1++)
			if (field[ind][ind]==field[ind1][ind1])
				{
				if (debug)
					printf("Errore in prima diagonale\n");
				return(NOT);
				}
		}
	for (ind=0;ind<dim*dim-1;ind++)					/* Controllo seconda diagonale */
		{
		if (field[ind][dim*dim-ind-1]==0)				/* controlla il punto di inizio di riga vuota */
			continue;
		for (ind1=(1+ind/dim)*dim;ind1<dim*dim;ind1++)
			if (field[ind][dim*dim-ind-1]==field[ind1][dim*dim-ind1-1])
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
/*					 */
/*		GenField	 */
/*					 */
/*********************/

/* Genera matrice field dai vari sottoquadri e dallo schema da risolvere */

void genField(void)
{
int nr;
int nc;
int ns;
int ind;
int ind1;
int val;
int sq[MAXN*MAXN][MAXN*MAXN];	/* elenco sottoquadri corrente */
								/* (merge tra schema da risolvere e sottoquadri permutati) */

for (ns=0;ns<dim*dim;ns++)		/* Genera lista sq d'appoggio */
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
		field[nr][nc]=sq[nr/dim*dim+nc/dim][(nr%dim)*dim+nc%dim];
}


/*********************/
/*					 */
/*		Welcome		 */
/*					 */
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

if (super_sudoku)
	printf("\nSuper sudoku abilitato\n\n");

printf("\n\n");

printf("Premere un tasto\n");
getchar();

printf("\nLet's go!\n\n\n");
}


/*********************/
/*					 */
/*		 Bye		 */
/*					 */
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
ASPETTA(1000);
}


/*********************/
/*					 */
/*	   GetOptions	 */
/*					 */
/*********************/

/* Ricava le opzioni di lancio */

/* OUTPUT:	YES/NOT		se uscire o meno */

int getOptions(int argc,char **argv)
{
int ind;
int nr;
int nc;
char strf[(MAXN+2)*(MAXN+2)*(MAXN+2)*(MAXN+2)];

for (ind=1;ind<argc;ind++)
	{
	if (!strncmp(argv[ind],OPZ1,strlen(OPZ1)))	/* opzione -n= per settare la dimensione */
		{
		sscanf(argv[ind]+strlen(OPZ1),"%d",&dim);
		if (dim<=0 || dim>MAXN)
			dim=DEFN;
		}
	if (!strncmp(argv[ind],OPZ2,strlen(OPZ2)))	/* opzione -f= per settare lo schema da risolvere */
		{
		sscanf(argv[ind]+strlen(OPZ2),"%s",strf);
		for (nr=0;nr<dim*dim;nr++)
			for (nc=0;nc<dim*dim;nc++)
				sqinit[nr][nc]=strf[nr*dim*dim+nc]-'0';
		}
	if (!strncmp(argv[ind],OPZ7,strlen(OPZ7)))	/* opzioni -clear per sbiancare lo schema da risolvere */
		{
		memset(sqinit,0,sizeof(sqinit));
		clear=YES;
		}
	if (!strncmp(argv[ind],OPZ3,strlen(OPZ3)) || !strncmp(argv[ind],OPZ4,strlen(OPZ4)) ||
		!strncmp(argv[ind],OPZ4,strlen(OPZ4)))	/* opzioni di help */
		{
		printf("Commands availables:\n\n");
		printf("-n=Array rank (default=3)\n");
		printf("-f=Field to solve (format: d000ddd... d=digit 0=blank  one row after another)\n");
		printf("-clear to search all the solutions\n");
		printf("-ss enable super-sudoku\n");
		printf("-d enable debug mode\n");
		printf("-h=This help\n");
		printf("-?=This help\n");
		printf("h =This help\n");
		printf("? =This help\n");
		return(YES);
		}
	if (!strncmp(argv[ind],OPZ8,strlen(OPZ8)))
		super_sudoku=YES;
	if (!strncmp(argv[ind],OPZ9,strlen(OPZ9)))
		debug=YES;
	}

return(NOT);
}


/*********************/
/*					 */
/*	  CarDefault	 */
/*					 */
/*********************/

/* Carica i valori di default */

void carDefault(void)
{
int nr;
int nc;

dim=DEFN;				/* default dimensione */
super_sudoku=NOT;		/* no super sudoku */
clear=NOT;				/* c'e' uno schema da risolvere */
debug=NOT;				/* no debug */

for (nr=0;nr<dim*dim;nr++)
	for (nc=0;nc<dim*dim;nc++)
		sqinit[nr][nc]=sqinit_e[nr][nc];		/* Default schema da risolvere */
}