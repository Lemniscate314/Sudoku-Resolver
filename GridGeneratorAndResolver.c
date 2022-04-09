#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 3  // Le 3 ici correspond à 3*3=9 qui veut dire un tableau de dimension 9*9. Si on veut que l'ordinateur résolve un tableau de dimension a*a, il faudra remplacer le 3 par sqrt(a)
typedef int tableau[MAX*MAX][MAX*MAX];
typedef int cube [MAX*MAX][MAX*MAX][MAX*MAX];


/**INITIALISATION DU TABLEAU*/
int VerifierColonne(tableau Tab,int i,int j)    //fonction qui vérifie si le chiffre entré ne se trouve pas déjà sur la colonne
{
    int k;
     for(k=0;k<MAX*MAX;k++)
     {
         if (Tab[i][j]==Tab[i][k]&& k!=j)
         {
             return 1;      //retourne 1 s'il y a déjà le chiffre sur la colonne et 0 sinon
         }
     }
     return 0;

}


int VerifierLigne(tableau Tab,int i,int j)      //fonction qui vérifie si le chiffre entré ne se trouve pas déjà sur la ligne
{
    int k;
     for(k=0;k<MAX*MAX;k++)
     {

         if (Tab[i][j]==Tab[k][j]&& k!=i)
         {
             return 1;      //retourne 1 s'il y a déjà le chiffre sur la ligne et 0 sinon
         }
     }
     return 0;
}


int VerifierCarre(tableau Tab,int i,int j)      //fonction qui vérifie si le chiffre entré ne se trouve pas déjà sur le carré
{
    int p=i-(i%MAX);
    int q=j-(j%MAX);
    int m;
    int n;
    for (m=p;m<p+MAX; m++)
    {
        for (n=q; n<q+MAX; n++)
        {
            if (Tab[i][j] == Tab[m][n] && (i!=m || j!=n))
            {
                return 1;      //retourne 1 s'il y a déjà le chiffre sur le carré et 0 sinon
            }
        }
    }
    return 0;
}

/**Ce sous-programme permet de générer une grille de départ qui respecte les contraintes du jeu*/
void InitialiserGrille(tableau Tab,int niveau)  //crée et initialise la grille de départ en la préremplissant adequatement
{
    int i;
    int j;
    int Nb=0;
    for(i=0;i<MAX*MAX;i++)
    {
        for(j=0;j<MAX*MAX;j++)
        {
            Tab[i][j]=0; //initialisation du tableau avec des zéros partout
        }
    }
do
{
   i=rand()%(MAX*MAX);
   j=rand()%(MAX*MAX);
   if(Tab[i][j]==0)   //si la case est vide
   {
       while(VerifierLigne(Tab,i,j) || VerifierColonne(Tab,i,j) || VerifierCarre(Tab,i,j)) // tant que le chiffre est déjà présent sur la ligne, la colonne ou le carré
       {
           Tab[i][j]=rand()%(MAX*MAX)+1;                                                   // le programme est repete jusqu'a ce qu'on ait la bonne valeur
       }
       Nb++;

    }


}
while(Nb<niveau);
}



/**AFFICHAGE DE LA GRILLE*/
void afficherGrille(tableau Tab)
{
    int i,j;
    for(i=0;i<MAX*MAX; i++){
        for(j=0;j<MAX*MAX;j++){

            printf(" | ");
            printf("%d", Tab[i][j]);
        }

        printf(" | ");
        printf("\n");
    }
    printf("\n");
}



/**PROGRAMME DE RESOLUTION DU SUDOKU*/
void StockerValeursPotentielles(cube cube,tableau Tab,int i,int j)
{
    for(int k=0;k<MAX*MAX;k++)
    {

        Tab[i][j]=k+1;

        if(VerifierColonne(Tab,i,j) || VerifierLigne(Tab,i,j) || VerifierCarre(Tab,i,j))    // tant que le chiffre est déjà présent sur la ligne, la colonne ou le carré
        {
            Tab[i][j]=0;
        }
        cube[i][j][k]=Tab[i][j];
    }
}

void ConfondreGrilleCube(cube cube,tableau Tab)
{
    int i,j;
    for(i=0;i<MAX*MAX;i++)
    {
        for(j=0;j<MAX*MAX;j++)
        {
            cube[i][j][0]=Tab[i][j];
        }
    }
}

void ValeurPotentielleCase(cube cube,tableau Tab,int i,int j)
{
    for(int k=0;k<MAX*MAX;k++)
    {
        Tab[i][j]=k+1;

        if(VerifierColonne(Tab,i,j) || VerifierLigne(Tab,i,j) || VerifierCarre(Tab,i,j))    // tant que le chiffre est déjà présent sur la ligne, la colonne ou le carré
        {
            Tab[i][j]=0;
        }
        cube[i][j][k]=Tab[i][j];
    }
}

void AfficherCubeDimension3(cube cube,int i,int j)
{
    int k;
    for(k=1;k<MAX*MAX;k++)
    {
        printf("%d ",cube[i][j][k]);
    }

}
/*Nous allons maintenant coder quelques méthodes pour pour éliminer le plus le nombre de cases vides ainsi, optimiser notre méthode de résolution finale*/

/**1ere METHODE:Conduite Inclusion*/
int CompteurNombresPotentiels(cube cube, int i, int j)
{
    int Nb=0;
    int k;
    for(k=0;k<MAX*MAX;k++)
    {
        if(cube[i][j][k]!=0)
        {
           Nb++;
        }
    }
    return Nb;
}

void conduiteInclusion(cube cube,tableau Tab,int i, int j)
{
    int Nb,k;

    Nb=CompteurNombresPotentiels(cube,i,j);
    if(Nb==1)
    {
        for(k=0;k<MAX*MAX;k++)
        {
            if(cube[i][j][k]!=0)
            {
                Tab[i][j]=cube[i][j][k];
            }break;
        }
    }
}


/**2e METHODE:Méthode exclusion*/
int VerifierNombreExclusionLigne(cube cube,int i,int j,int k)
{
    int nombre=cube[i][j][k];
    int p;
    int q;
    int Nb=0;
    for(p=0;p<MAX*MAX;p++)
    {
        if(p==i) continue;
        for(q=0;q<MAX*MAX;q++)
        {
            if(cube[p][j][q]==nombre)
            {
                Nb++;
            }
        }
    }
    return Nb;
}

int VerifierNombreExclusionColonne(cube cube,int i,int j,int k) //On compte le nombre de fois qu'un nombre dans la liste des valeurs potentielles d'une case est présente dans la liste d'une autre case
{
    int nombre=cube[i][j][k];
    int p;
    int q;
    int Nb=0;
    for(p=0;p<MAX*MAX;p++)
    {
        if(p==j) continue;
        for(q=0;q<MAX*MAX;q++)
        {
            if(cube[i][p][q]==nombre)
            {
                Nb++;
            }
        }
    }
    return Nb;

}

int VerifierNombreExclusionCarre(cube cube,int i,int j,int k)
{
    int p=i-(i%MAX);
    int q=j-(j%MAX);
    int m;
    int n;
    int l;
    int Nb=0;
    int nombre=cube[i][j][k];
    for (m=p;m<p+MAX; m++)
    {
        for (n=q; n<q+MAX; n++)
        {
            if(m==i && n==j) continue;
            for(l=0;l<MAX*MAX;l++)
            {
                if(cube[m][n][l]==nombre)
                {
                    Nb++;
                }
            }
        }
    }
    return Nb;
}

void ConduiteExclusive(cube cube, tableau Tab, int i,int j)
{
    int k;
    for(k=0;k<MAX*MAX;k++)
    {
        if(cube[i][j][k]!=0)
        {
            int Nb0=VerifierNombreExclusionLigne(cube,i,j,k);
            int Nb1=VerifierNombreExclusionColonne(cube,i,j,k);
            int Nb2=VerifierNombreExclusionCarre(cube,i,j,k);

            if(Nb0==0 || Nb1==0 || Nb2==0)  // Dans le cas ou l'un des compteurs est nul, cela veut dire que la valeur est exclue des autres cases du secteur(la ligne, la colonne ou la région) afférent.
            {
                Tab[i][j]=cube[i][j][k];
            }
        }
    }
}
/*Apres avoir optimisé au mieux notre grille, nous allons pouvoir tester toutes les valeurs possibles pour cela, on va devoir utiliser le backtracking */

void ReorganiserValeur(cube cube,int i,int j)
{
    int k;
    int p;
    for(k=0;k<MAX*MAX;k++)
    {
        for(p=k+1;p<MAX*MAX;p++)
        {
            if(cube[i][j][k]==0 && cube[i][j][p]!=0)
            {
                cube[i][j][k]=cube[i][j][p];
                cube[i][j][p]=0;
            }
        }
    }
}

int SolutionGrille(cube cube,tableau Tab,int i,int j)       //Apres avoir optimisé au mieux notre grille, nous allons pouvoir tester toutes les valeurs possibles
                                                            //Pour cela, on va devoir utiliser le backtracking
{
    int k;
    if(i==MAX*MAX-1 && j==MAX*MAX-1)
    {
        ValeurPotentielleCase(cube,Tab,MAX*MAX-1,MAX*MAX-1);
        ReorganiserValeur(cube,MAX*MAX-1,MAX*MAX-1);
        Tab[MAX*MAX-1][MAX*MAX-1]=cube[MAX*MAX-1][MAX*MAX-1][0];


        return 1;
    }
    if(Tab[i][j]!=0)
    {
        if(j<MAX*MAX-1 ) return SolutionGrille(cube,Tab,i,j+1);
        else return SolutionGrille(cube,Tab,i+1,0);
    }
    int Nb=CompteurNombresPotentiels(cube,i,j);
    for(k=0;k<Nb;k++)
    {
        Tab[i][j]=cube[i][j][k];
        if(VerifierLigne(Tab,i,j)==0 && VerifierColonne(Tab,i,j)==0 && VerifierCarre(Tab,i,j)==0)
        {

            if(j<MAX*MAX-1 )
            {
                if(SolutionGrille(cube,Tab,i,j+1)) return 1;
            }

            else if(SolutionGrille(cube,Tab,i+1,0)) return 1;


        }

    }
    Tab[i][j]=0;
    return 0;
}




/**Programme Principal*/

int main()
{
    cube cube;
    int i;
    int j;
    int k;
    for(i=0;i<MAX*MAX;i++)
    {
        for(j=0;j<MAX*MAX;j++)
        {
             for(k=0;k<MAX*MAX;k++)
             {
                 cube[i][j][k]=0;
             }
        }
    }


    tableau Tab={
        {9,0,0,1,0,0,0,0,5},
        {0,0,5,0,9,0,2,0,1},
        {8,0,0,0,4,0,0,0,0},
        {0,0,0,0,8,0,0,0,0},                //Ce tableau de dim 9*9 est résolu par l'ordinateur grâce à nos différentes méthodes de résolution utilisées
        {0,0,0,7,0,0,0,0,0},                //Il pourra être remplacé par un tableau de dimension différente en changeant le #define max
        {0,0,0,0,2,6,0,0,9},
        {2,0,0,3,0,0,0,0,6},
        {0,0,0,2,0,0,9,0,0},
        {0,0,1,9,0,4,5,7,0}
    };

    afficherGrille(Tab);
    ConfondreGrilleCube(cube,Tab);

    for(i=0;i<MAX*MAX;i++)
    {
        for(j=0;j<MAX*MAX;j++)
        {
            if(Tab[i][j]==0)
            StockerValeursPotentielles(cube,Tab,i,j);
        }
    }
    for(i=0;i<MAX*MAX;i++)
    {
        for(j=0;j<MAX*MAX;j++)
        {
            ReorganiserValeur(cube,i,j);
        }
    }
   SolutionGrille(cube,Tab,0,0);
   printf("     Voici le tableau resolu\n\n");
   afficherGrille(Tab);

    return 0;
}
