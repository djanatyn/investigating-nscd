#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void) {
  struct addrinfo *result, *rp;
  struct addrinfo hints = {
    .ai_family = AF_INET
  };

  char *host = "google.com";
  int success = getaddrinfo(host, NULL, &hints, &result);

  if (success != 0) {
    printf("failed");
  } else {
    printf("successful lookup: %s\n", host);
    for (rp = result; rp != NULL; rp = rp->ai_next) {
      char addr[INET_ADDRSTRLEN];
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;

      inet_ntop(AF_INET, &(ipv4->sin_addr), addr, sizeof addr);
      printf("returned address: %s\n", addr);
    }
  }
}
