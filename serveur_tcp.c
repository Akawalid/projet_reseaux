/*
* Code du serveur
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <termios.h>
/* Port local du serveur */
#define PORT 9600// le port de déployemet du serveur
#define BACKLOG 128// le nombre de connexions maximale qui peuvent etre en attente
#define MAX_TAILLE_RECEPTION 5// la taille du buffeur de récépition
#define SERVEUR_ADRESSE "127.0.0.1"

#define KBLU  "\x1B[94m"
#define KGRN  "\x1B[92m"
#define KYEL  "\x1B[96m"
#define KRED  "\x1B[91m"
#define KCYN  "\x1B[96m"
#define KWHT  "\x1B[97m"
#define KNRM  "\x1B[0m"

int main(int argc, char *argv[])
{
/*
* Variables du serveur
*
* D�clarer ici les variables suivantes :
* - sockfd le descripteur de socket
* - structure d�adresse locale du serveur
* - structure d�adresse du client
* - taille de l�adresse du client
*/

//Affichage des données du serveur et le protocole utilisé
    printf("%s", KRED);printf("\n\t> ");printf("%s", KYEL);printf("Informations serveur");
    printf("%s", KYEL);printf("\n\t\t@IP: ");printf("%s", KGRN);printf("%s", SERVEUR_ADRESSE);
    printf("%s", KYEL);printf("\n\t\tPORT: ");printf("%s", KGRN);printf("%d", PORT);
    printf("%s", KRED);printf("\n\t> ");printf("%s", KYEL);printf("Protocol: ");printf("%s", KRED);printf("TCP\n");
//======================================================+
    
    //structure d'adresse du client, serveur
    char buff_reception[MAX_TAILLE_RECEPTION];
    //déclaration des variables:
    int sockfd;
    struct sockaddr_in server_add, client_add;
    socklen_t  client_add_length=sizeof(client_add);

//======================================================+
/*
* Code du serveur
*
* - Ouvrir le socket du serveur
* - Remplir la structure d�adresse locale du serveur :
* - la famille d�adresse
7
Universit� Paris-Sud � L3 Informatique et M1 MIAGE R�seaux
* - l�adresse IP
* - le port
* - Sp�cifier l�adresse locale du socket du serveur
*/
//===============================================================+

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    //traitemet des erreurs potentiels
    if (sockfd == -1) {
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la creation de la socket.");
        return -1;
    }else
        printf("%s", KYEL);printf("\n\tSocket TCP cree avec");printf("%s", KGRN);printf(" succes");
    
//================================================================+
//================================================================+
    
    memset(&server_add, 0, sizeof(server_add));//initialisation des champs d'@ du serveur à 0, pour eviter les problems si la structure se remplie partiellement dans la suite du programme
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(PORT);//htons pour convertir la valeur du port d'un entier vers un type compatible avec le modèle TCP/IP
    server_add.sin_addr.s_addr = inet_addr(SERVEUR_ADRESSE);//On evite d'utiliser INADDR_ANY parce quelle elle peut causer des  
                                                            //vulnerabilités contre les hakeurs.
                                                            //de meme pour la fonciton inet_addr, conversion de types

    if(bind(sockfd, (struct sockaddr *)&server_add, sizeof(server_add)) == -1){
        printf("\n\t");printf("%s", KRED);printf("Erreur");printf("%s", KBLU);printf(" BIND ");
        close(sockfd);
        return -1;
    } else
      printf("%s", KYEL);printf("\n\tSocket associee avec");printf("%s", KGRN);printf(" succes");printf("%s", KRED);printf("(");printf("%s", KBLU);printf("BIND");printf("%s", KRED);printf(")");

//================================================================+
//================================================================+
    
    if(listen(sockfd, BACKLOG)==-1){
        printf("\n\t");printf("%s", KRED);printf("Erreur");printf("%s", KRED);printf(" (");printf("%s", KBLU);printf("LISTEN");printf("%s", KRED);printf(")");

        return 1;
    }
    printf("%s", KYEL);printf("\n\tEn ecoute sur le port ");printf("%s", KBLU);printf("%d", PORT);printf("%s", KRED);printf("(");printf("%s", KBLU);printf("LISTEN");printf("%s", KRED);printf(")");
    
//================================================================+
    /*
    * Boucle générale du serveur (infinie)
    */

    //communicaiton_socket c'est pour la nouvelle socket de collunicaiton rendu par accept()
    //numero requete pour afficher le numéro de la requete reçu à partire du moment du lancement du serveur, il change à chaque fois qu'un autre client envoie des données au serveur
    int communication_socket, numero_req=0;
    //pour contenir l'@ ip du client pour l'afficher
    char ip_client[INET_ADDRSTRLEN];
    while(1){
        fflush(stdout);
        // Calculer le nombre de connexions en attente
        //la ofnciton getsockopt renvoie les options de la socket, selon les parametres passé en arguments, dans ce cas
        //le parametre SO_RCVBUF c'est pour compter le nombre de connexions en attente dans le buffeur de connexions
        //les variables conneciotn_on_hold et len et pour recupérer ce nombre
        int  connections_on_hold = 0, len=sizeof(connections_on_hold);
        if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&connections_on_hold, &len) == -1) {
            perror("getsockopt");
            return 1;
        }
        printf("%s", KCYN);printf("\n\tTCP INFO:");
        printf("%s", KYEL);printf("\n\t\tNombre de connexions en attente: ");printf("%s", KRED);printf("%d/%d\n",-connections_on_hold/682+BACKLOG, BACKLOG);
        printf("%s", KCYN);printf("\n\t...En attente de demande de connexion.\n");
        
        //==================================================================================================

        communication_socket = accept(sockfd, (struct sockaddr *) &client_add, &client_add_length);
        
        //==================================================================================================
        // Convertir la reprsentation binaire de l'ip vers Stirng, c'est pour afficher l'ip du client
        inet_ntop(AF_INET, &(client_add.sin_addr), ip_client, INET_ADDRSTRLEN);
        //converstion du port du client à un entier pur l'afficher
        int client_port = ntohs(client_add.sin_port);;
        if(communication_socket!=-1){
            printf("\n\t");printf("%s", KYEL);printf("Connexion");printf("%s", KGRN);printf(" acceptee");printf("%s", KRED);printf("%s", KBLU);printf(", Socket hote: ");printf("[");printf("%s", KBLU);printf("%s", ip_client);printf("%s", KRED);printf(", ");printf("%s", KBLU);printf("%d", client_port);printf("%s", KRED);printf("]");printf("%s", KRED);printf("(");printf("%s", KBLU);printf("ACCEPT");printf("%s", KRED);printf(")");
            //numero_seg c'est opour compter le nombre de segments d'une requete, par exemple si un client envoie 4000 octets de données et que le buffeur de reception dans ce cas peut contenir au max 2048 octets, ces données du client vont entre envoyé dans deux segments, le premier contiendra 2048 octets et le deuxiem va contenir 4000-2048 octets
            int  numero_seg = 0;
            //ret_of_recv (resp ret_of_write) c'est pour calculer le nombre d'octets reçu par read() (resp write())
            ssize_t ret_of_write, ret_of_receive;
            do {
                //===================================================================================================================+
                
                //la taille à lire est MAX_TAILLE_RECEPTION-1 est pour toujours donner la possibilité d'ajouter '\0' à la fin, ça nous permet d'éviter les problèmes d'affichage sur la sortie standard (STDOUT).
                ret_of_receive = read(communication_socket, buff_reception, MAX_TAILLE_RECEPTION-1);
                if(ret_of_receive > 0){
                    buff_reception[ret_of_receive]='\0';

                //Affichage des données sur le segment envoyé
                    printf("%s", KRED);printf("\n+");printf("%s", KCYN);printf("--------");printf("%s", KBLU);printf("Requete N° ");printf("%s", KRED);printf("%d", numero_req);printf("%s", KBLU);printf(".");printf("%s", KRED);printf("%d", numero_seg);printf("%s", KCYN);printf("----------------------------------------------------------------------------------------------------------");printf("%s", KRED);printf("+");printf("%s\n\n", KWHT);   
                    printf("%s", KYEL);printf("\n\tINFO:");
                    printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" Numero de requete: ");printf("%s", KRED);printf("%d.", numero_req);
                    printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" Nombre s\'octets reçu: ");printf("%s", KRED);printf("%d.", ret_of_receive);
                    printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" Fonciton: ");printf("%s", KRED);printf("(");printf("%s", KBLU);printf("RECVFROM");printf("%s", KRED);printf(")");
                    printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" @IP client et port: ");printf("%s", KRED);printf("[");printf("%s", KBLU);printf("%s", ip_client);printf("%s", KRED);printf("; ");printf("%s", KBLU);printf("%d", client_port);printf("%s", KRED);printf("]");
                    printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" @IP serveur et port: ");printf("%s", KRED);printf("[");printf("%s", KBLU);printf("%s", SERVEUR_ADRESSE);printf("%s", KRED);printf("; ");printf("%s", KBLU);printf("%d", PORT);printf("%s", KRED);printf("]");
                    printf("%s", KRED);printf("\n\t\t> ");printf("%s", KYEL);printf(" Contenu:");printf("\n%s", KWHT);
                //===================================================================================================================+
              
                    //Affichage des données sur la sortie standard
                    ret_of_write = write(STDOUT_FILENO, buff_reception, ret_of_receive);
                    if(ret_of_write == -1){
                        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KBLU);printf(" lors de la reception des donnees.");
                        close(sockfd);
                        return -1;
                    }
                    
                //===================================================================================================================+
                    printf("%s", KRED);printf("\n\n+");printf("%s", KCYN);printf("--------------------------------------------------------------------------------------------------------------------------------");printf("%s", KRED);printf("+");printf("%s\n\n", KWHT);
                    //à chaque opération de lecture on passe à lire le prochaines segment si il reste encore des données non reçu.
                    numero_seg += 1;

                    //re initialiser le buffeur de lecture à 0('\0' dans le cas des caracters) pur eviter les problemes d'affichage
                    memset(buff_reception, '\0', MAX_TAILLE_RECEPTION);
                }
            }
            while ( ret_of_receive > 0);
            
            //======================================================================+
            //======================================================================+
            if(close(communication_socket) == -1){
                printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KBLU);printf(" lors de la fermeture de la socket.");
                return -1;
            }
            printf("%s", KYEL);printf("\n\tFermeture de connexion avec");printf("%s", KGRN);printf(" succes, ");printf("%s", KGRN);printf("hote:");printf("%s", KRED);printf("[");printf("%s", KBLU);printf("%s", ip_client);printf("%s", KRED);printf(",");printf("%s", KBLU);printf(" %d", client_port);printf("%s", KRED);printf("].\n");
            //======================================================================+
        }
        numero_req++;// à chaque fin de phase de lecture on passe à la connexion suivante donc la requete suivante
    }

    //=============================================================================+
    
    if(close(sockfd) == -1){
        printf("%s", KYEL);printf("\nFermeture de socket ");printf("%s", KRED);printf("(");printf("%s", KRED);printf("%s", SERVEUR_ADRESSE);printf("%s", KRED);printf(",");printf("%s", KRED);printf(" %d", PORT);printf("%s", KRED);printf(") ");printf("%s", KRED);printf("echouee");
        printf("\n\t");printf("%s", KRED);printf("Erreur");printf("%s", KRED);printf(" (");printf("%s", KBLU);printf("LISTEN");printf("%s", KRED);printf(")");
        return -1;
    }
    printf("%s", KYEL);printf("\nFermeture de socket ");printf("%s", KRED);printf("(");printf("%s", KBLU);printf("%s", SERVEUR_ADRESSE);printf("%s", KRED);printf(", ");printf("%s", KBLU);printf("%d", PORT);printf("%s", KRED);printf(")");printf("%s", KYEL);printf(" avec ");printf("%s", KGRN);printf("succes");
    
    //=============================================================================+
    return 0;
}
