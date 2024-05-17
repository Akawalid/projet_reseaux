#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <termios.h>

#define KBLU  "\x1B[94m"
#define KGRN  "\x1B[92m"
#define KYEL  "\x1B[96m"
#define KRED  "\x1B[91m"
#define KCYN  "\x1B[96m"
#define KWHT  "\x1B[97m"
#define KNRM  "\x1B[0m"

#define SIZE 100
#define PORT 9600
int main (int argc, char *argv[])
{
    /*
    * Variables du client
    *
    * Déclarer ici les variables suivantes :
    * - sockfd le descripteur de socket
    * - structure d’adresse du serveur
    * - pointeur vers la structure descriptive de machine (hostent)
    * - zone de mémoire destinée à accueillir la chaîne
    * entrée au clavier
    * - taille de la chaîne à envoyer
    */

    //si la commande du lancement du client ne contient pas le nom du serveur, onrenvoie une erreur.
    if(argc < 2){
        printf("%s", KRED);printf("\n\tNom du serveur manquant.");printf("\n\t%s", KBLU);printf("Essayez la commande: ");;printf("%s", KBLU);printf("gcc ");;printf("%s", KGRN);printf("client_tcp.c ");printf("%s", KBLU);printf("-o");printf("%s", KGRN);printf(" clientTcp ");printf("%s", KRED);printf(";");printf("%s", KBLU);printf(" ./clientTcp ");printf("%s", KGRN);printf("localhost\n");
        return -1;
    }
    
    //declaration des variables:
    //==================================================================

    int sockfd, newSockfd;
    struct sockaddr_in server_add;
    struct hostent * host_details;
    char read_buff[SIZE];
    size_t read_buff_length=0;

    //==================================================================
    /*
    * Code du client
    *
    * - Ouvrir le socket du client
    * - Récupérer l’adresse IP du serveur à partir de son nom
    * donné en ligne de commande
    * - Remplir la structure d’adresse du serveur
    * - Lire une ligne de l’entrée standard
    * - Envoyer la chaîne lue au serveur
    */
//==================================================================

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la creation de la socket.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tSocket cree avec");printf("%s", KGRN);printf(" succes");

//==================================================================
//==================================================================

    host_details = gethostbyname(argv[1]);
    if (host_details == NULL) {
        printf("\nEchec lors de la recherche du serveur.");
        close(sockfd);
        return -1;
    }

    //initialisation de tout les champ de l'@ du serveur à 0 avant de la remplire, cela, pour eviter des problemes des champs non initialisés
    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(PORT);
    memcpy(&server_add.sin_addr.s_addr, host_details->h_addr, host_details->h_length);//assigner l'adresse de lala machine à son champ correspondant dans la structure du serveur

//==================================================================
//==================================================================

    if(connect(sockfd, (struct sockaddr *)&server_add, sizeof(server_add)) == -1){
        printf("\n\t");printf("%s", KYEL);printf("Connexion");printf("%s", KRED);printf(" refusee.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tConnexion");printf("%s", KGRN);printf(" acceptee");

//==================================================================
//==================================================================
    // read_buff_length = read(sockfd, read_buff, sizeof(read_buff));
    // printf("\n%s; %d", read_buff, read_buff_length);


    //a pour ffichage
    printf("%s", KBLU);
    printf("%s", KRED);printf("\n\n\n+");printf("%s", KBLU);printf("----");printf("%s", KRED);printf("+"); printf("%s", KYEL);printf("Veuillez vous saisir le contenu du fichier à envoyer au serveur :");printf("%s", KRED);printf("+");printf("%s", KBLU);printf("--------------------------------------------------------------------");printf("%s", KRED);printf("+\n");
    printf("%s\n", KWHT);

    // la lecture des données à partire de l'entrée standard
    fflush(stdin);
    read_buff_length = read(STDIN_FILENO, read_buff, sizeof(read_buff));
    
    //affichage
    printf("%s", KRED);
    printf("\n  +");printf("%s", KBLU);printf("-------------------------------------------------------------------");printf("%s", KRED);printf("+");printf("%s", KRED);printf(" ");printf("%s", KYEL);printf("TEXT");printf("%s", KRED);printf(" ");printf("%s", KRED);printf("+");printf("%s", KBLU);printf("-------------------------------------------------------------");printf("%s", KRED);
    printf("+\n\n");
    
    //on affiche un commentaire et on arrete le programme si le contenu à envoyer est vide (pas la peine de l'envoyer mais comme meme on renvoie 0)
    if(read_buff_length == 0){
        printf("\n\tMessage vide.");
        return 0;
    }

//==================================================================
//==================================================================
    //ecriture des données dans la socket du serveur
    if(write(sockfd, read_buff, read_buff_length-1) == -1){
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KYEL);printf(" lors de l\'ecriture des données.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tMessage envoye avec");printf("%s", KGRN);printf(" succes");

//==================================================================
//==================================================================

    //fermeture de la connexion
    if(close(sockfd) == -1){
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la fermeture de la socket.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tSocket fermee avec");printf("%s", KGRN);printf(" succes\n");printf("%s", KWHT);

//==================================================================
    return 0;
}
        return -1;
    }
    printf("%s", KYEL);printf("\n\tConnexion");printf("%s", KGRN);printf(" acceptee");

//==================================================================
//==================================================================
    // read_buff_length = read(sockfd, read_buff, sizeof(read_buff));
    // printf("\n%s; %d", read_buff, read_buff_length);


    //a pour ffichage
    printf("%s", KBLU);
    printf("%s", KRED);printf("\n\n\n+");printf("%s", KBLU);printf("----");printf("%s", KRED);printf("+"); printf("%s", KYEL);printf("Veuillez vous saisir le contenu du fichier � envoyer au serveur :");printf("%s", KRED);printf("+");printf("%s", KBLU);printf("--------------------------------------------------------------------");printf("%s", KRED);printf("+\n");
    printf("%s\n", KWHT);

    // la lecture des donn�es � partire de l'entr�e standard
    fflush(stdin);
    read_buff_length = read(STDIN_FILENO, read_buff, sizeof(read_buff));

    //affichage
    printf("%s", KRED);
    printf("\n  +");printf("%s", KBLU);printf("-------------------------------------------------------------------");printf("%s", KRED);printf("+");printf("%s", KRED);printf(" ");printf("%s", KYEL);printf("TEXT");printf("%s", KRED);printf(" ");printf("%s", KRED);printf("+");printf("%s", KBLU);printf("-------------------------------------------------------------");printf("%s", KRED);
    printf("+\n\n");

    //on affiche un commentaire et on arrete le programme si le contenu � envoyer est vide (pas la peine de l'envoyer mais comme meme on renvoie 0)
    if(read_buff_length == 0){
        printf("\n\tMessage vide.");
        return 0;
    }

//==================================================================
//==================================================================
    //ecriture des donn�es dans la socket du serveur
    if(write(sockfd, read_buff, read_buff_length-1) == -1){
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KYEL);printf(" lors de l\'ecriture des donn�es.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tMessage envoye avec");printf("%s", KGRN);printf(" succes");

//==================================================================
//==================================================================

    //fermeture de la connexion
    if(close(sockfd) == -1){
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la fermeture de la socket.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tSocket fermee avec");printf("%s", KGRN);printf(" succes\n");printf("%s", KWHT);

//==================================================================
    return 0;
}
