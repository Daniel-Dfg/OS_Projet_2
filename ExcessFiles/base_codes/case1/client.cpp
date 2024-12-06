#include "../commons.hpp"
#include <string.h>

int main(){
    int sock = socket(AF_INET, SOCK_STREAM , 0);
    check_return_value(sock, "socket");
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET ;
    serv_addr.sin_port = htons (8080);
    //pas de 'bind' car on l'a déjà fait côté client

    // Conversion de string vers IPv4 ou IPv6 en binaire
    check_return_value(inet_pton(AF_INET, "127.0.0.1", & serv_addr.sin_addr ), "inet_pton");
    check_return_value(connect(sock,(struct sockaddr*)& serv_addr, sizeof (serv_addr)), "connect"); // au lieu de 'accept', on se connecte à une adresse déjà existante
    char message [] = "Message de test";
    check_return_value(write(sock, message, strlen(message)), "write");
    close(sock);
    return 0;
}
