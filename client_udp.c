
/*
* Code du client
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <termios.h>
#define SIZE 100
#define PORT 9600
#define CLIENT_PORT 8080

// ANSI escape codes for terminal control
#define KBLU  "\x1B[94m"
#define KGRN  "\x1B[92m"
#define KYEL  "\x1B[96m"
#define KRED  "\x1B[91m"
#define KCYN  "\x1B[96m"
#define KWHT  "\x1B[97m"
#define KNRM  "\x1B[0m"

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
        printf("%s", KRED);printf("\n\tNom du serveur manquant.");printf("\n\t%s", KBLU);printf("Essayez la commande: ");;printf("%s", KBLU);printf("gcc ");;printf("%s", KGRN);printf("client_udp.c ");printf("%s", KBLU);printf("-o");printf("%s", KGRN);printf(" clientUdp ");printf("%s", KRED);printf(";");printf("%s", KBLU);printf(" ./clientUdp ");printf("%s", KGRN);printf("localhost\n");
        return -1;
    }

    //declaration des variables:
    //==================================================================

    int sockfd;
    struct sockaddr_in add_serv;
    //struct sockaddr_in add_client;
    struct hostent * host_details;
    char read_buff[SIZE];
    size_t  read_buff_length=0;
    
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

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la creation de la socket.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tSocket cree avec");printf("%s", KGRN);printf(" succes");

//==================================================================
//==================================================================

    host_details = gethostbyname(argv[1]);
    if (host_details == NULL) {
        printf("%s", KRED);printf("\n\tEchec");printf("%s", KBLU);printf("lors de la recherche du serveur.");
        close(sockfd);
        return -1;
    }

    memset(&add_serv, 0, sizeof(add_serv));
    add_serv.sin_family = AF_INET;
    add_serv.sin_port = htons(PORT);
    memcpy(&add_serv.sin_addr.s_addr, host_details->h_addr, host_details->h_length);
    
    // memset(&add_client, 0, sizeof(add_client));
    // add_client.sin_family = AF_INET;
    // add_client.sin_port = htons(CLIENT_PORT);
    // add_serv.sin_addr.s_addr=INADDR_ANY;

//==================================================================
//==================================================================
    
    printf("%s", KBLU);
    printf("%s", KRED);printf("\n\n\n+");printf("%s", KBLU);printf("----");printf("%s", KRED);printf("+"); printf("%s", KYEL);printf("Veuillez vous saisir le contenu du fichier à envoyer au serveur :");printf("%s", KRED);printf("+");printf("%s", KBLU);printf("--------------------------------------------------------------------");printf("%s", KRED);printf("+\n");
    printf("%s\n", KWHT);

    //lecture des données à partire de l'entrée standard avec read.
    fflush(stdin);
    read_buff_length = read(STDIN_FILENO, read_buff, sizeof(read_buff));
    
    printf("%s", KRED);
    printf("\n  +");printf("%s", KBLU);printf("-------------------------------------------------------------------");printf("%s", KRED);printf("+");printf("%s", KRED);printf(" ");printf("%s", KYEL);printf("TEXT");printf("%s", KRED);printf(" ");printf("%s", KRED);printf("+");printf("%s", KBLU);printf("-------------------------------------------------------------");printf("%s", KRED);
    printf("+\n\n");

    if(read_buff_length == 0){
        printf("\n\tMessage vide.");
        return 0;
    }

//==================================================================
//==================================================================

    //envoie des données au serveur en mode UDP, pas besoin de définr l'ip et le port du client par ce que ça se fait implicitement apres l'aéxecution de la fonciton socket()
    if(sendto(sockfd, read_buff, read_buff_length-1, 0, (struct sockaddr *)&add_serv, sizeof(add_serv)) == -1){
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de l'envoie du message'.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tMessage envoye avec");printf("%s", KGRN);printf(" succes");

//==================================================================
//==================================================================

    if(close(sockfd) == -1){
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la fermeture de la socket.");
        return -1;
    } else
        printf("%s", KYEL);printf("\n\tSocket fermee avec");printf("%s", KGRN);printf(" succes\n%s", KWHT);

//==================================================================
    return 0;
}
