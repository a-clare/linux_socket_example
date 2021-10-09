#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[]) {

  int server_fd = 0;
  int client_fd = 0;
  struct sockaddr_in serv_addr;

  char sendBuff[1025];
  time_t ticks;

  /*
   * We are creating a socket for IPv4 (AF_INET), and we want it to
   * communicate using TCP/IP (SOCK_STREAM)
   * Protocol is always 0
   * This ours (the servers) file descriptor.
   */
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  /*
   * Setup our server address ifnormation.
   * This is setting the IP address and port number to a well known
   * address so clients know how to find us
   */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);

  /*
   * We need to 'bind' our server to the address information
   */
  if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("Bind on server_fd failed\n");
    return 1;
  }

  /*
   * Listen() marks our file descriptor (the server) as passive. We want to
   * wait until active sockets (the clients) try to connect to us.
   * 10 == backlog, which specifies the limit on how many pending connections we
   * will accept
   */
  listen(server_fd, 10);

  while (1) {
    /*
     * accept() will block until a connection attempt is made
     * We are listening for connections on the server_fd socket
     * the addr and addr_len arguments can be used to get the address information
     * of the connecting socket
     */
    client_fd = accept(server_fd, (struct sockaddr *) NULL, NULL);

    ticks = time(NULL);
    snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
    write(client_fd, sendBuff, strlen(sendBuff));

    close(client_fd);
    sleep(1);
  }
}