#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*************  Constantes   *****************************/
#define NbPriorite 10    //nombre total de priorités
#define Nmax       8    //nombre de requêtes générées


/**************  Définition des structures ***************/

int nb = 0; // compteur du nombre d'opérations

struct requete{
  int numero;    // numéro de la requête
  int priorite;  // priorité de la requête
};
typedef struct requete Requete;




/* Définir ici la structure Fap1 et Fap2 */
/* Fap1 */
struct element{
  Requete val;
  struct element* suiv;
};
typedef struct element Element;

typedef Element* Fap1 ;


/* Fap2 */
typedef Fap1* Fap2;

/**************  Fonctions de manipulation des structures *******/
// Fonction random

// file initialisée vide
Fap1 initialiseFile1(){
 Fap1 f1;
 f1 = NULL;
 return f1;
}

// file initialisée vide

Fap2 initialiseFile2(){
	Fap2 f2;
	int  i = 0;
	f2 = malloc( NbPriorite * sizeof(Fap1));
	for(i = 0; i < NbPriorite;i++)
	{
		f2[i] = initialiseFile1();
	}
	return f2;
}


void afficheFile1(Fap1 f){
  int n =1;
  if(f == NULL)printf("VIDE");
  while(f != NULL)
  {
	  printf("|[%d][%d]|",f->val.numero,f->val.priorite);
	  if(n%10==0)printf("\n");
	  f = f->suiv;
	  n++;
  }
  printf("\n");
}

void afficheFile2(Fap2 f)
{
	int i =0;
	for(i=0;i<NbPriorite;i++)
	{
		afficheFile1(f[i]);
	}
	printf("\n");
}



Fap1 ajoutFile1(Fap1 f, Requete r){
    if(f == NULL){
    Element* tmp = malloc(sizeof(Element));
    tmp->val = r;
    tmp->suiv = f;
    f = tmp;
  }
  else
    f->suiv = ajoutFile1(f->suiv, r);
   //nb++;//décommenter pour le nombre d'opérations
  return f;
}

Fap2 ajoutFile2(Fap2 f, Requete r)
{
  f[r.priorite] = ajoutFile1(f[r.priorite],r);
  return f;
}

// Supprime la première requête de priorité m

Fap1 supprime(Fap1 f, int m){
  Fap1 tmp;
  if(f != NULL){
    if(f->val.priorite == m){
      tmp = f;
      f = f->suiv;
      free(tmp);
      m = -1;// m prend une valeur de requete impossible afin de bien traiter 1 requete à la fois
      f = supprime(f, m);
    }
    else
      f->suiv = supprime(f->suiv, m);
  }
  return f;
}


Fap1 traiteRequete1(Fap1 f){

     Fap1 tmp;
     tmp = f;
     int max;
     max = 0;
     while(f != NULL)
     {
		 if (max < f->val.priorite)
		     max = f->val.priorite;		 
		 f = f->suiv;
	 }
	 f = tmp;	 
	 f = supprime(f, max);
	 //nb++; //décommenter pour test le nombre d'opérations
	 return f;		
	}

Fap2 traiteRequete2(Fap2 f){
    int max,i;
    max = 0;
    i = 0;
    for(i=0;i<NbPriorite;i++)
    {
		if(f[i] != NULL)
		max = i ;
	}
	f[max]=traiteRequete1(f[max]);
	return f;
}





void libereMemoire1(Fap1 f){
  if(f != NULL)
 {
 libereMemoire1(f->suiv);
 free(f);
 }
}

void libereMemoire2(Fap2 f){
  int i;
  for(i=0;i<NbPriorite;i++)
  {
	  libereMemoire1(f[i]);
  }
  free(f);
}





/************** Génération des requêtes pour simulation *****************/

//retourne un entier aléatoire entre 0 et n-1
int alea(int n){
  return rand()%n;
}

Requete generationRequete(){
  static int numero = 0;
  Requete r;
  numero++;
  r.numero = numero;
  r.priorite = alea(NbPriorite);
  return r;
}



/************ Simulation file *****************************/


//structure 1
void simulationFile1(){
  Fap1 f1 = initialiseFile1();
  int compteur = 0;
  Requete r;
  while(compteur < Nmax){
    if(alea(2) == 0){ //traitement requête avec proba 0.5
      f1 = traiteRequete1(f1);
    }
    else{ //génération d'une nouvelle requête avec proba 0.5
      compteur++;
      r = generationRequete();
      f1 = ajoutFile1(f1, r);
    }
    afficheFile1(f1);
  }
  libereMemoire1(f1);
  printf("\n #######################################\n");
}


//structure 2
void simulationFile2(){
  Fap2 f2 = initialiseFile2();
  int compteur = 0;
  Requete r;
  while(compteur < Nmax){
    if(alea(2) == 0){ //traitement requête avec proba 0.5
      f2 = traiteRequete2(f2);
    }
    else{ //génération d'une nouvelle requête avec proba 0.5
      compteur++;
      r = generationRequete();
      f2 = ajoutFile2(f2, r);
    }
    afficheFile2(f2);
  }
  libereMemoire2(f2);
  printf("\n########################################\n");
}


/*************  Fonction main *******************************/

int main(){
  srand(time(NULL));

  /********  à décommenter pour lancer une simulation *********/
  //simulationFile1();
    
  //simulationFile2();
  
  //printf("\n nmbre : %d", nb); //affichagenmbre opérations

  
  return 0;
}

/*Sur 10 essais on obtient avec Fap2 des valeurs entre 20 et 32 
avec une moyenne de 24(24.8) et une médiane à 23
 test avec la commande valgrind */

/*Sur 10 essais on obtient avec Fap2 des valeurs entre 11 et 20 
avec une moyenne de 15(15.8) et une médiane à 16
 test avec la commande valgrind */




