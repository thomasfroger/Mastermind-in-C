#include <stdio.h>
#include <stdlib.h>

// Fonction qui demande a l'utilisateur de rentrer une combinaison de cases
void add_score(int victoire,int essais)
{
   FILE *fptr;
    char filename[] = "score.txt";

    // Ouvrir le fichier en mode "append"
    fptr = fopen(filename, "a");
    if (fptr == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    // Ajouter une ligne de données au fichier
    fprintf(fptr, "Utilisateur | 5 victoires | Essais restants\n");

    // Fermer le fichier
    fclose(fptr);

    printf("Données ajoutées avec succès dans %s\n", filename);
     
}


int main() {
    add_score(4,5);
    return 0;
}