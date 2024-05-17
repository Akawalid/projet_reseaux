#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

/* Port local du serveur */
#define MAX_TAILLE_RECEPTION 2048//la taille maximale des données qu'on peut réceptionner
//port et @IP du serveur
#define PORT 9600
#define SERVEUR_ADRESSE "127.0.0.1"
int main (int argc, char *argv[]){
/*
* Variables du serveur
*
* Déclarer ici les variables suivantes :
*- sockfd le descripteur de socket
*- structure d'adresse locale du serveur
*- structure d'adresse du client
*- taille de l'adresse du client
*/
//==============================================================================================================================+
     //déclaration des variables:
    char buff_reception[MAX_TAILLE_RECEPTION];
    int sockfd;
    struct sockaddr_in server_add, client_add;
    //taille de l'@ du client déclaré avec le type <socklen_t>
    socklen_t  client_add_length=sizeof(client_add);//Entier non signé, utilisé spécialement pour les stuctures des sockets pour assurer le bon fonctionnement sur n'importe quelle plateform.
    
//==============================================================================================================================  
/*

* Code du serveur
*
* - Ouvrir le socket du serveur
* - Remplir la structure d'adresse locale du serveur :
* - la famille d'adresse
* - l'adresse IP
* - le port
* - Sp�cifier l'adresse locale du socket du serveur
*/

//Affichage des données du serveur    
    printf("%s", KRED);printf("\n\t> ");printf("%s", KYEL);printf("Informations serveur");
    printf("%s", KYEL);printf("\n\t\t@IP: ");printf("%s", KGRN);printf("%s", SERVEUR_ADRESSE);
    printf("%s", KYEL);printf("\n\t\tPORT: ");printf("%s", KGRN);printf("%d", PORT);
    printf("%s", KRED);printf("\n\t> ");printf("%s", KYEL);printf("Protocol: ");printf("%s", KRED);printf("UDP\n");

//==================================================+
    
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    //traitemet des erreurs potentiels
    if (sockfd == -1) {
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la creation de la socket.");
        return -1;
    }else
        printf("%s", KYEL);printf("\n\tSocket TCP cree avec");printf("%s", KGRN);printf(" succes");
    
//===================================================+
//===================================================+

    memset(&server_add, 0, sizeof(server_add));//initialisation des champs d'@ du serveur à 0, pour eviter les problems si la structure se remplie partiellement dans la suite du programme
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(PORT);//htons pour convertir la valeur du port d'un entier vers un type compatible avec le modèle TCP/IP
    server_add.sin_addr.s_addr = inet_addr(SERVEUR_ADRESSE);//de meme pour la fonciton inet_addr, conversion de types

    if(bind(sockfd, (struct sockaddr *)&server_add, sizeof(server_add)) == -1){
        printf("\n\t");printf("%s", KRED);printf("Erreur");printf("%s", KBLU);printf(" BIND ");
        close(sockfd);
        return -1;
    }
    printf("%s", KYEL);printf("\n\tSocket associee avec");printf("%s", KGRN);printf(" succes");printf("%s", KRED);printf("(");printf("%s", KBLU);printf("BIND");printf("%s", KRED);printf(")\n");

//===================================================+
/*
* Boucle générale du serveur (infinie)
*/

    //numero requete pour afficher le numéro de la requete reçu à partire du moment du lancement du serveur, il change à chaque fois qu'un autre client envoie des données au serveur
    int numero_req = 0;
    while (1) {
            //===============================================================================================================+
            
            printf("%s", KRED);printf("\n> ");printf("%s", KCYN);printf("...En attente de reception.\n");
            fflush(stdout);
            //la taille à lire est MAX_TAILLE_RECEPTION-1 est pour toujours donner la possibilité d'ajouter '\0' à la fin, ça nous permet d'éviter les problèmes d'affichage sur la sortie standard (STDOUT).
            //ret_of_receive pour compter le nombre d'octets par la fonction recvfrom
            ssize_t ret_of_receive = recvfrom(sockfd, buff_reception, MAX_TAILLE_RECEPTION-1, 0,(struct sockaddr *)&client_add, &client_add_length);
            if(ret_of_receive == -1){
                printf("\n\tEchec lors de receprtion des données.");
                close(sockfd);
                return -1;
            }
            buff_reception[ret_of_receive]='\0';



            //pour afficher l'@ ip client
            char ip_client[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(client_add.sin_addr), ip_client, INET_ADDRSTRLEN);//conversions de types pour affichage
            int client_port = ntohs(client_add.sin_port);// de meme, c'est pour p'affichage
            
            printf("%s", KRED);printf("\n+");printf("%s", KCYN);printf("--------");printf("%s", KBLU);printf("Requete N° ");printf("%s", KRED);printf("%d", numero_req);printf("%s", KCYN);printf("--------------------------------------------------------------------------------------------------------------");printf("%s", KRED);printf("+");printf("%s\n\n", KWHT);   
            printf("%s", KYEL);printf("\n\tINFO:");
            printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" Numero de requete: ");printf("%s", KRED);printf("%d.", numero_req);
            printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" Nombre s\'octets reçu: ");printf("%d.", ret_of_receive);
            printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" Fonciton: ");printf("%s", KRED);printf("(");printf("%s", KBLU);printf("RECVFROM");printf("%s", KRED);printf(")");
            printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" @IP client et port: ");printf("%s", KRED);printf("[");printf("%s", KBLU);printf("%s", ip_client);printf("%s", KRED);printf("; ");printf("%s", KBLU);printf("%d", client_port);printf("%s", KRED);printf("]");
            printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" @IP serveur et port: ");printf("%s", KRED);printf("[");printf("%s", KBLU);printf("%s", SERVEUR_ADRESSE);printf("%s", KRED);printf("; ");printf("%s", KBLU);printf("%d", PORT);printf("%s", KRED);printf("]");
            printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" Contenu:");printf("%s\n", KWHT);
            //================================================================================================================+
        
            //afficage des données
            ssize_t ret_of_write = write(STDOUT_FILENO, buff_reception, ret_of_receive);
            if(ret_of_write == -1){
                printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KBLU);printf(" lors de la reception des donnees.");
                close(sockfd);
                return -1;
            }
            printf("%s", KRED);printf("\n\n+");printf("%s", KCYN);printf("----------------------------------------------------------------------------------------------------------------------------------");printf("%s", KRED);printf("+");printf("%s\n\n", KWHT);
        
            //================================================================================================================+
            numero_req+=1;//passer à la requete suivante (au client suivant)

            //re initialiser le buffeur de reception à vide pour eviter les problems d'affichage dans les lectures suivantes
            memset(buff_reception, '\0', MAX_TAILLE_RECEPTION);
    }

    //=====================================+
    
    if(close(sockfd) == -1){
        printf("%s", KYEL);printf("\n\tFermeture de socket ");printf("%s", KRED);printf("(");printf("%s", KRED);printf("%s", SERVEUR_ADRESSE);printf("%s", KRED);printf(",");printf("%s", KRED);printf(" %d", PORT);printf("%s", KRED);printf(") ");printf("%s", KRED);printf("echouee");
        printf("\n\t");printf("%s", KRED);printf("Erreur");printf("%s", KBLU);printf("(");printf("%s", KBLU);printf("LISTEN");printf("%s", KRED);printf(")");
        return -1;
    } else
        printf("%s", KYEL);printf("\n\tFermeture de socket ");printf("%s", KRED);printf("(");printf("%s", KBLU);printf("%s", SERVEUR_ADRESSE);printf("%s", KRED);printf(", ");printf("%s", KBLU);printf("%d", PORT);printf("%s", KRED);printf(")");printf("%s", KYEL);printf(" avec ");printf("%s", KGRN);printf("succes");printf("%s", KRED);printf("(");printf("%s", KBLU);printf("CLOSE");printf("%s", KRED);printf(")");
    
    //=====================================+

return 0;
}
