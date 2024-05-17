#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//client variables
#define SIZE 100
#define PORT 9600
#define CLIENT_PORT 8080
#define SERVER_NAME "localhost"
// ANSI escape codes for terminal control
#define CLEAR_SCREEN() printf("\e[1;1H\e[2J")
#define MOVE_CURSOR(col, row) printf("\e[%d;%dH", (row), (col))
#define MAX_DISPLAY_LIST_LENGTH 4
#define MAX_DISPLAY_LIST_ELEMENTS_LENGTH 70
#define KBLU  "\x1B[94m"
#define KGRN  "\x1B[92m"
#define KYEL  "\x1B[96m"
#define KRED  "\x1B[91m"
#define KCYN  "\x1B[96m"
#define KWHT  "\x1B[97m"
#define KNRM  "\x1B[0m"


void setup_terminal() {
    struct termios newt;
    tcgetattr(STDIN_FILENO, &newt);
    
    newt.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    newt.c_cc[VMIN] = 1;               // Read at least one character
    newt.c_cc[VTIME] = 0;              // No timeout (blocking)

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void restore_terminal() {
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    
    oldt.c_lflag |= (ICANON | ECHO);  // Restore canonical mode and echo

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int read_arrow_key() {
    setup_terminal();
    int c1 = getchar();

    if (c1 == 27) {  // 27 is the ASCII code for Escape key
        int c2 = getchar();  // Read the next character

        if (c2 == 91) {  // 91 is the ASCII code for '[' after the Escape key
            int c3 = getchar();  // Read the final character

            switch (c3) {
                case 'A':
                    restore_terminal();
                    return 1;  // Up arrow key
                case 'B':
                    restore_terminal();
                    return 2;  // Down arrow key
                case 'C':
                    restore_terminal();
                    return 3;  // Right arrow key
                case 'D':
                    restore_terminal();
                    return 4;  // Left arrow key
                default:
                    restore_terminal();
                    return 0;  // Not an arrow key
            }
        }
    } else if(c1 == 10){
        restore_terminal();
        return 5;
    }
    restore_terminal();
    return 0;  // Not an arrow key
}
// Function to get the current X-coordinate of the console cursor on Linux
int wherex() {
    setup_terminal();
    printf("\033[6n");
    int x;
    scanf("\033[%dR", &x);

    restore_terminal();
    return x;
}

// Function to get the current Y-coordinate of the console cursor on Linux
int wherey() {
    setup_terminal();
    printf("\033[6n");  // ANSI escape code to request cursor position

    int row, col;
    scanf("\033[%d;%dR", &row, &col);  // Read the cursor position

    restore_terminal();
    return row;
}


void FisrtScreen()
{
    printf("%s", KRED);
    printf("\n\n_______________________________________________________________________________________________________________________________________________________________________\n\n");
    // ASCII art for "TP Res"
    printf("%s", KBLU);
    printf("\n                              TTTTTTTTTTTTTTTTTTTTTTTPPPPPPPPPPPPPPPPP");printf("%s", KGRN);printf("        RRRRRRRRRRRRRRRRR   EEEEEEEEEEEEEEEEEEEEEE   SSSSSSSSSSSSSSS");
    printf("%s", KBLU);printf("\n                              T:::::::::::::::::::::TP::::::::::::::::P");printf("%s", KGRN);printf("       R::::::::::::::::R  E::::::::::::::::::::E SS:::::::::::::::S");
    printf("%s", KBLU);printf("\n                              T:::::::::::::::::::::TP::::::PPPPPP:::::P");printf("%s", KGRN);printf("      R::::::RRRRRR:::::R E::::::::::::::::::::ES:::::SSSSSS::::::S");
    printf("%s", KBLU);printf("\n                              T:::::TT:::::::TT:::::TPP:::::P     P:::::P");printf("%s", KGRN);printf("     RR:::::R     R:::::REE::::::EEEEEEEEE::::ES:::::S     SSSSSSS");
    printf("%s", KBLU);printf("\n                              TTTTTT  T:::::T  TTTTTT  P::::P     P:::::P");printf("%s", KGRN);printf("       R::::R     R:::::R  E:::::E       EEEEEES:::::S");
    printf("%s", KBLU);printf("\n                                      T:::::T          P::::P     P:::::P");printf("%s", KGRN);printf("       R::::R     R:::::R  E:::::E             S:::::S");
    printf("%s", KBLU);printf("\n                                      T:::::T          P::::PPPPPP:::::P");printf("%s", KGRN);printf("        R::::RRRRRR:::::R   E::::::EEEEEEEEEE    S::::SSSS");
    printf("%s", KBLU);printf("\n                                      T:::::T          P:::::::::::::PP");printf("%s", KGRN);printf("         R:::::::::::::RR    E:::::::::::::::E     SS::::::SSSSS");
    printf("%s", KBLU);printf("\n                                      T:::::T          P::::PPPPPPPPP");printf("%s", KGRN);printf("           R::::RRRRRR:::::R   E:::::::::::::::E       SSS::::::::SS");
    printf("%s", KBLU);printf("\n                                      T:::::T          P::::P");printf("%s", KGRN);printf("                   R::::R     R:::::R  E::::::EEEEEEEEEE          SSSSSS::::S");
    printf("%s", KBLU);printf("\n                                      T:::::T          P::::P");printf("%s", KGRN);printf("                   R::::R     R:::::R  E:::::E                         S:::::S");
    printf("%s", KBLU);printf("\n                                      T:::::T          P::::P");printf("%s", KGRN);printf("                   R::::R     R:::::R  E:::::E       EEEEEE            S:::::S");
    printf("%s", KBLU);printf("\n                                    TT:::::::TT      PP::::::PP");printf("%s", KGRN);printf("               RR:::::R     R:::::REE::::::EEEEEEEE:::::ESSSSSSS     S:::::S");
    printf("%s", KBLU);printf("\n                                    T:::::::::T      P::::::::P");printf("%s", KGRN);printf("               R::::::R     R:::::RE::::::::::::::::::::ES::::::SSSSSS:::::S");
    printf("%s", KBLU);printf("\n                                    T:::::::::T      P::::::::P");printf("%s", KGRN);printf("               R::::::R     R:::::RE::::::::::::::::::::ES:::::::::::::::SS");
    printf("%s", KBLU);printf("\n                                    TTTTTTTTTTT      PPPPPPPPPP");printf("%s", KGRN);printf("               RRRRRRRR     RRRRRRREEEEEEEEEEEEEEEEEEEEEE SSSSSSSSSSSSSSS");

    printf("%s", KRED);
    printf("%s", KBLU);
    printf("\t\t\t\t                                                                                                   \n"
    "\t\t\t\t                                     TRAVAIL REALISE PAR :                                       `\n"
    "\t\t\t\t                                                                                                   \n"
    "\t\t\t\t                    ");printf("%s", KGRN);printf("</>");printf("%s", KBLU);printf(" WANE");printf("%s", KRED);printf(" Ismael                      ");printf("%s", KGRN);printf("</>");
    printf("%s", KBLU);printf(" CHABANE");printf("%s", KRED);printf(" Oualid\n\n"
    "\t\t\t\t\t    Theme"); printf("%s", KGRN); printf(" : Realisaiton dun serveur TCP et UDP en utilisant le langage C\n\n");printf("%s", KBLU);
    printf("\t\t\t\t                                           GROUPE :");printf("%s", KGRN);printf(" 01");printf("\n\n");
    printf("%s", KRED);
    printf("\n\n_____________________________________________________________________________");printf("%s", KGRN);printf(" FIN");printf("%s", KRED);printf(" ______________________________________________________________________________________");
    printf("%s", KRED);

    printf("%s", KGRN);printf("\n\t                                             ----->>"); printf("%s", KRED);printf(" APPUYEZ SUR N'IMPORTE QUELLE TOUCHE AFIN DE COMMENCER... ");
    printf("%s", KWHT);
    getchar();
}

int pointer(int x, int y, int Nb, char tabch[MAX_DISPLAY_LIST_LENGTH][MAX_DISPLAY_LIST_ELEMENTS_LENGTH], char * color1,  char * color2, char * color3, char * color4)
{
    //Fonction d'affichage des listes  de d�cisions
     printf("%s", color2);
     for(int i= 0; i < Nb; i++){
        MOVE_CURSOR(x, y+i);
        printf("%s", tabch[i]);
     }
     int z = 0, y1 = y;
     int key;
     do
     {
         MOVE_CURSOR(x, y);
         printf("%s",color4);
         printf(">");
         printf("%s",color1);
         printf(" %s", tabch[z]);
         key = read_arrow_key();
         MOVE_CURSOR(x, y);
         printf(" ");
         printf("%s",color2);printf("%s ",tabch[z]);
         if(key == 1){
            y--;
            z--;
            if(y < y1){
                y = y1 + Nb - 1;
                z = Nb - 1;
            }

         }
         else if(key == 2){
            y++;
            z++;
            if(y >= y1 + Nb){
                y = y1;
                z = 0;
            }
         }
     }
     while(key != 5);
    return z;
}

void server_adresse_display(int f1, int f2, int f3, int f4, const char *  machin_name){
    printf("%s", KRED);
    printf("\n\t> "); printf("%s", KYEL);printf("@IP du serveur: ");
    printf("%s", KCYN);printf("%d", f1);
    printf("%s", KRED);printf(".");
    printf("%s", KCYN);printf("%d", f2);
    printf("%s", KRED);printf(".");
    printf("%s", KCYN);printf("%d", f3);
    printf("%s", KRED);printf(".");
    printf("%s", KCYN);printf("%d", f4);
    printf("%s", KRED);printf("<");
    printf("%s", KBLU);printf("%s", machin_name);
    printf("%s", KRED);printf(">");

}

int client_udp(char * server_name){
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
    //declaration des variables:
    int sockfd;
    struct sockaddr_in add_serv, add_client;
    struct hostent * host_details;
    char read_buff[SIZE];
    size_t  read_buff_length=0;

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

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la creation de la socket.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tSocket cree avec");printf("%s", KGRN);printf(" succes");

    //const char * server_name = malloc(30 * sizeof(char));//nom du serveur
    host_details = gethostbyname(server_name);
    if (host_details == NULL) {
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la recherche du serveur.");

        close(sockfd);
        return -1;
    }
    memset(&add_serv, 0, sizeof(add_serv));
    add_serv.sin_family = AF_INET;
    add_serv.sin_port = htons(PORT);
    memcpy(&add_serv.sin_addr.s_addr, host_details->h_addr, host_details->h_length);
    
    memset(&add_client, 0, sizeof(add_client));
    add_client.sin_family = AF_INET;
    add_client.sin_port = htons(CLIENT_PORT);
    add_serv.sin_addr.s_addr=INADDR_ANY;

    // if(bind(sockfd, (struct sockaddr *)&add_serv, sizeof(add_serv)) == -1){
    //     perror("\n...Erreur BIND");
    //     close(sockfd);
    //     return -1;
    // }
    //printf("\n...Socket associee avec succes");

    printf("%s", KBLU);
    printf("\n\n\n----");printf("%s", KRED);printf("+"); printf("%s", KYEL);printf("Veuillez vous saisir le contenu du fichier à envoyer au serveur :");printf("%s", KRED);printf("+");printf("%s", KBLU);printf("------------------------------------------------------------------------------------------------\n");
    printf("%s\n", KWHT);
    
    read_buff_length = read(STDIN_FILENO, read_buff, sizeof(read_buff));

    printf("%s", KRED);
    printf("\n  +");printf("%s", KBLU);printf("---------------------------------------------------------------------------------");printf("%s", KRED);printf("+");printf("%s", KRED);printf(" ");printf("%s", KYEL);printf("TEXT");printf("%s", KRED);printf(" ");printf("%s", KRED);printf("+");printf("%s", KBLU);printf("--------------------------------------------------------------------------");printf("%s", KRED);
    printf("+\n\n");
    if(read_buff_length == 0){
        printf("\n\tMessage vide.");
        return 0;
    }

    if(sendto(sockfd, read_buff, read_buff_length, 0, (struct sockaddr *)&add_serv, sizeof(add_serv)) == -1){
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de l'envoie du message'.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tMessage envoye avec");printf("%s", KGRN);printf(" succes");

    if(close(sockfd) == -1){
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la fermeture de la socket.");
        return -1;
    } else
        printf("%s", KYEL);printf("\n\tSocket fermee avec");printf("%s", KGRN);printf(" succes");
}

int client_tcp(char * server_name){
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
    //declaration des variables:
    int sockfd, newSockfd;
    struct sockaddr_in server_add;
    struct hostent * host_details;
    char read_buff[SIZE];
    size_t read_buff_length=0;

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

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la creation de la socket.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tSocket cree avec");printf("%s", KGRN);printf(" succes");

    //char * server_name = malloc(30 * sizeof(char));//nom du serveur
    host_details = gethostbyname(server_name);
    if (host_details == NULL) {
        printf("\nEchec lors de la recherche du serveur.");
        close(sockfd);
        return -1;
    }
    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(PORT);
    memcpy(&server_add.sin_addr.s_addr, host_details->h_addr, host_details->h_length);

    if(connect(sockfd, (struct sockaddr *)&server_add, sizeof(server_add)) == -1){
        printf("\n\t");printf("%s", KYEL);printf("Connexion");printf("%s", KRED);printf(" refusee.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tConnexion");printf("%s", KGRN);printf(" acceptee");
    // read_buff_length = read(sockfd, read_buff, sizeof(read_buff));
    // printf("\n%s; %d", read_buff, read_buff_length);

    printf("%s", KBLU);
    printf("\n\n\n----");printf("%s", KRED);printf("+"); printf("%s", KYEL);printf("Veuillez vous saisir le contenu du fichier à envoyer au serveur :");printf("%s", KRED);printf("+");printf("%s", KBLU);printf("------------------------------------------------------------------------------------------------\n");
    printf("%s\n", KWHT);

    read_buff_length = read(STDIN_FILENO, read_buff, sizeof(read_buff));
    
    printf("%s", KRED);
    printf("\n  +");printf("%s", KBLU);printf("---------------------------------------------------------------------------------");printf("%s", KRED);printf("+");printf("%s", KRED);printf(" ");printf("%s", KYEL);printf("TEXT");printf("%s", KRED);printf(" ");printf("%s", KRED);printf("+");printf("%s", KBLU);printf("--------------------------------------------------------------------------");printf("%s", KRED);
    printf("+\n\n");

    if(read_buff_length == 0){
        printf("\n\tMessage vide.");
        return 0;
    }

    if(write(sockfd, read_buff, read_buff_length) == -1){
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KYEL);printf(" lors de l\'ecriture des données.");
        return -1;
    }
    printf("%s", KYEL);printf("\n\tMessage envoye avec");printf("%s", KGRN);printf(" succes");

    if(close(sockfd) == -1){
        printf("\n\t");printf("%s", KRED);printf("Echec");printf("%s", KRED);printf(" lors de la fermeture de la socket.");
        return -1;
    } else
        printf("%s", KYEL);printf("\n\tSocket fermee avec");printf("%s", KGRN);printf(" succes");
}

int main(int argc, char *argv[]){
    char tab[4][70] = {"Mode UDP.", "Mode TCP.", "", ""};
    FisrtScreen();
    while(1){
        CLEAR_SCREEN();
        printf("%s", KYEL);
        printf("\n\tChoisir un mode:");
        int choix = pointer(9, 4, 2, tab, KRED, KCYN, KRED, KGRN);
        char *file_to_send = malloc(sizeof(char) * 1024);
        CLEAR_SCREEN();
        switch(choix){
        case 0 :
            server_adresse_display(127, 0, 0 ,1 ,SERVER_NAME);
            printf("%s", KRED);
            printf("\n\t> "); printf("%s", KYEL);printf("Protocol utilise: ");
            printf("%s", KRED);
            printf("UDP");
            client_udp(SERVER_NAME);
            break;
        case 1:
            server_adresse_display(127, 0, 0 ,1 ,SERVER_NAME);
            printf("%s", KRED);
            printf("\n\t> "); printf("%s", KYEL);printf("Protocol utilise: ");
            printf("%s", KRED);
            printf("TCP");
            printf("%s", KBLU);
            client_tcp(SERVER_NAME);
            break;
        }
        printf("%s", KGRN);printf("\n\n\t\tAppuyez sur n\'importe quelle touche pour continuer.");
        getchar();
    }
    printf("\n%s", KWHT);
    return 0;
}