#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

void display_hostent(struct hostent *h) {
  int i;

  printf("host name: %s; type: %d; len: %d\n", h->h_name, h->h_addrtype,
         h->h_length);
  for (i = 0; h->h_aliases[i] != NULL; i++)
    printf("\t%s\n", h->h_aliases[i]);
  if (h->h_addrtype == AF_INET) {
    for (i = 0; h->h_addr_list[i] != NULL; i++)
      printf("\t%s\n", inet_ntoa(*((struct in_addr *)h->h_addr_list[i])));
  }
}

void display_netent(struct netent *n) {
  int i;
  printf("network name: %s; type: %d\n", n->n_name, n->n_addrtype);
  for (i = 0; n->n_aliases[i] != NULL; i++)
    printf("\t%s\n", n->n_aliases[i]);
}

void display_servent(struct servent *s) {
  int i;
  printf("service name: %s; port: %d; protocol: %s\n", s->s_name, s->s_port,
         s->s_proto);
  for (i = 0; s->s_aliases[i] != NULL; i++)
    printf("\t%s\n", s->s_aliases[i]);
}

void display_protoent(struct protoent *p) {
  int i;
  printf("protocol name: %s; protocol number: %d\n", p->p_name, p->p_proto);
  for (i = 0; p->p_aliases[i] != NULL; i++)
    printf("\t%s\n", p->p_aliases[i]);
}

void hostdb() {
  struct hostent *h;

  printf("### HOSTS ###\n");
  sethostent(1);
  while ((h = gethostent()) != NULL)
    display_hostent(h);
  endhostent();

  struct netent *n;
  printf("\n### NETWORKS ###\n");
  setnetent(1);
  while ((n = getnetent()) != NULL)
    display_netent(n);
  endnetent();

  struct servent *s;
  printf("\n### SERVICES ###\n");
  setservent(1);
  while ((s = getservent()) != NULL)
    display_servent(s);
  endservent();

  struct protoent *p;
  printf("\n### PROTOCOLS ###\n");
  setprotoent(1);
  while ((p = getprotoent()) != NULL)
    display_protoent(p);
  endprotoent();
}

void ifdb() {
  printf("\n### INTERFACES ###\n");
  struct if_nameindex *ni;
  int i;

  ni = if_nameindex();
  for (i = 0; ni[i].if_index != 0 || ni[i].if_name != NULL; i++)
    printf("index: %d; name: %s\n", ni[i].if_index, ni[i].if_name);
  if_freenameindex(ni);
}

int main(int argc, char *argv[]) {
  hostdb();
  ifdb();
  return 0;
}
