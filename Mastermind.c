// Jeu Mastermind fait par Damien MARQUET, Thomas FROGER, Raphael BIDAULT et Yvan NASSO

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Définition des constantes
#define MAX_NAME_LENGTH 50
#define MAX_CODE_LENGTH 6
#define MAX_EASY_ATTEMPTS 12
#define MAX_MEDIUM_ATTEMPTS 10
#define MAX_HARD_ATTEMPTS 8
#define MAX_PREVIOUS_GUESSES 10
#define NUM_DIGIT 10

// Définition de codes de couleur ANSI pour l'affichage des chiffres bien/mal placés
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Structure de données pour stocker les informations du joueur
struct Player {
    char name[MAX_NAME_LENGTH];
    int easy_attempts;
    int medium_attempts;
    int hard_attempts;
};

// Fonction pour générer un code secret aléatoire
void generateSecretCode(int secretCode[], int codeLength) {
    srand(time(NULL));
    for (int i = 0; i < codeLength; i++) {
        secretCode[i] = rand() % NUM_DIGIT; // Génère un chiffre aléatoire entre 0 et 9
    }
}

// Fonction pour évaluer la proposition de l'utilisateur
void evaluateGuess(int secretCode[], int guess[], int codeLength) {
    int bienPlace = 0;
    int malPlace = 0;
    for (int i = 0; i < codeLength; i++) {
        if (guess[i] == secretCode[i]) {
            bienPlace++; // Compte les chiffres correctement placés
        } else {
            for (int j = 0; j < codeLength; j++) {
                if (guess[i] == secretCode[j]) {
                    malPlace++; // Compte les chiffres corrects, mais mal placés
                    break;
                }
            }
        }
    }
    printf("Chiffre bien placé: %d\n", bienPlace);
    printf("Chiffre correct, mais mal placé: %d\n", malPlace);
}

// Fonction pour enregistrer les détails de la partie dans un fichier
void recordGame(struct Player *player, int difficulty, int attempts, char result[]) {
    // Ouvrir le fichier en mode append (ajout à la fin)
    FILE *historiqueFile = fopen("historique.txt", "a");

    if (historiqueFile == NULL) {
        printf("Impossible d'ouvrir le fichier historique.txt.\n");
        return;
    }

    // Obtenir la date et l'heure actuelles
    time_t currentTime;
    time(&currentTime);
    struct tm *timeInfo = localtime(&currentTime);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%b %d %Y %H:%M:%S", timeInfo);

    // Écrire les détails de la partie dans le fichier
    fprintf(historiqueFile, "[%s] Joueur: %s, Difficulté: %d, Tentatives: %d, Résultat: %s\n",
            timestamp, player->name, difficulty, attempts, result);

    // Fermer le fichier
    fclose(historiqueFile);
}

// Fonction pour valider si une chaîne de caractères ne contient que des chiffres
int isNumeric(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

// Fonction pour afficher une tentative avec les chiffres bien/mal placés en couleurs
void displayGuess(int secretCode[], int guess[], int codeLength) {
    for (int i = 0; i < codeLength; i++) {
        if (guess[i] == secretCode[i]) {
            printf(ANSI_COLOR_GREEN "%d " ANSI_COLOR_RESET, guess[i]); // Affiche en vert les chiffres correctement placés
        } else {
            printf(ANSI_COLOR_RED "%d " ANSI_COLOR_RESET, guess[i]); // Affiche en rouge les chiffres corrects, mais mal placés
        }
    }
    printf("\n");
}

// Fonction pour afficher le menu du jeu
void displayMenu() {
    printf("Menu:\n");
    printf("1. Nouvelle Partie\n");
    printf("2. Afficher l'historique\n");
    printf("3. Quitter le jeu\n");
    printf("Votre choix: ");
}

int main() {
    char playerName[MAX_NAME_LENGTH];
    printf("Bienvenue dans le jeu Mastermind!\n");

    int choice;
    do {
        displayMenu();
        int scanfResult = scanf("%d", &choice);

        if (scanfResult != 1) {
            printf("Choix invalide. Veuillez choisir une option valide.\n");
            while (getchar() != '\n'); // Nettoie le tampon d'entrée
            continue;
        }

        switch (choice) {
            case 1: // Nouvelle Partie
                printf("Entrez votre nom: ");
                scanf("%s", playerName);

                struct Player player;
                strcpy(player.name, playerName);
                player.easy_attempts = 0;
                player.medium_attempts = 0;
                player.hard_attempts = 0;

                int playAgain = 1;

                while (playAgain) {
                    int difficulty;
                    printf("Choisissez le niveau de difficulté:\n");
                    printf("1. Facile (4 chiffres, %d essais)\n", MAX_EASY_ATTEMPTS);
                    printf("2. Moyen (5 chiffres, %d essais)\n", MAX_MEDIUM_ATTEMPTS);
                    printf("3. Difficile (6 chiffres, %d essais)\n", MAX_HARD_ATTEMPTS);
                    printf("Votre choix: ");
                    scanf("%d", &difficulty);

                    int codeLength, maxAttempts;
                    switch (difficulty) {
                        case 1:
                            codeLength = 4;
                            maxAttempts = MAX_EASY_ATTEMPTS;
                            break;
                        case 2:
                            codeLength = 5;
                            maxAttempts = MAX_MEDIUM_ATTEMPTS;
                            break;
                        case 3:
                            codeLength = 6;
                            maxAttempts = MAX_HARD_ATTEMPTS;
                            break;
                        default:
                            printf("Niveau de difficulté invalide. Choisissez entre 1, 2 ou 3.\n");
                            continue;
                    }

                    int secretCode[MAX_CODE_LENGTH];
                    generateSecretCode(secretCode, codeLength);

                    int attempts = 0;
                    int previousGuesses[MAX_PREVIOUS_GUESSES][MAX_CODE_LENGTH];

                    printf("Devinez la séquence de chiffres de %d chiffres (de 0 à 9).\n", codeLength);

                    while (attempts < maxAttempts) {
                        int guess[MAX_CODE_LENGTH];
                        printf("Essai %d/%d : ", attempts + 1, maxAttempts);
                        char input[MAX_CODE_LENGTH + 1];
                        scanf("%s", input);

                        if (strlen(input) != codeLength || !isNumeric(input)) {
                            printf("Votre proposition doit contenir exactement %d chiffres (de 0 à 9).\n", codeLength);
                            continue;
                        }

                        for (int i = 0; i < codeLength; i++) {
                            guess[i] = input[i] - '0';
                        }

                        memcpy(previousGuesses[attempts % MAX_PREVIOUS_GUESSES], guess, sizeof(int) * codeLength);

                        printf("Précédentes tentatives :\n");
                        int start = (attempts >= MAX_PREVIOUS_GUESSES) ? (attempts - MAX_PREVIOUS_GUESSES + 1) : 0;
                        for (int i = start; i <= attempts; i++) {
                            printf("Tentative %d : ", i + 1);
                            displayGuess(secretCode, previousGuesses[i % MAX_PREVIOUS_GUESSES], codeLength);
                        }

                        evaluateGuess(secretCode, guess, codeLength);

                        int correct = 1;
                        for (int i = 0; i < codeLength; i++) {
                            if (guess[i] != secretCode[i]) {
                                correct = 0;
                                break;
                            }
                        }

                        if (correct) {
                            printf("Félicitations, %s! Vous avez deviné la séquence correcte en %d essais.\n", player.name, attempts + 1);
                            recordGame(&player, difficulty, attempts + 1, "Gagné");
                            break;
                        }

                        attempts++;
                    }

                    if (attempts == maxAttempts) {
                        printf("Désolé, %s, vous avez épuisé tous vos essais. La séquence correcte était : ", player.name);
                        for (int i = 0; i < codeLength; i++) {
                            printf("%d ", secretCode[i]);
                        }
                        printf("\n");
                        recordGame(&player, difficulty, maxAttempts, "Perdu");
                    }

                    if (difficulty == 1) {
                        player.easy_attempts += attempts + 1;
                    } else if (difficulty == 2) {
                        player.medium_attempts += attempts + 1;
                    } else if (difficulty == 3) {
                        player.hard_attempts += attempts + 1;
                    }

                    // Demander à l'utilisateur s'il veut rejouer
                    while (true) {
                        printf("Voulez-vous jouer à nouveau ? (1 pour oui, 0 pour non) : ");
                        int replayChoice;
                        int scanfResult = scanf("%d", &replayChoice);
                        if (scanfResult != 1) {
                            printf("Choix invalide. Entrez 1 pour rejouer ou 0 pour quitter.\n");
                            while (getchar() != '\n'); // Nettoie le tampon d'entrée
                        } else {
                            if (replayChoice == 1 || replayChoice == 0) {
                                playAgain = replayChoice;
                                break;
                            } else {
                                printf("Choix invalide. Entrez 1 pour rejouer ou 0 pour quitter.\n");
                            }
                        }
                    }
                }
                break;
            case 2: // Afficher l'historique.txt
                // Code pour afficher l'historique depuis un fichier "historique.txt"
                FILE *historiqueFile;
                historiqueFile = fopen("historique.txt", "r");
                if (historiqueFile == NULL) {
                    printf("Impossible d'ouvrir le fichier historique.txt.\n");
                } else {
                    char line[100];
                    printf("Historique des parties :\n");
                    while (fgets(line, sizeof(line), historiqueFile) != NULL) {
                        printf("%s", line);
                    }
                    fclose(historiqueFile);
                }
                break;
            case 3: // Quitter le jeu
                printf("Au revoir!\n");
                break;
            default:
                printf("Choix invalide. Veuillez choisir une option valide.\n");
        }
    } while (choice != 3);

    // Affichez les statistiques du joueur ici, enregistrez-les dans un fichier, etc.

    return 0;
}
