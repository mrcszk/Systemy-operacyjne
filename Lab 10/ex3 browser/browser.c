#include "defs.h"
#include "ssi.h"
 
/*[pgm]*/
int main(void)
{
    char url[100], s[500], *path = "", *p;
    SSI *ssip;
    int fd;
    ssize_t nread;

    while (true) {
        printf("URL: ");
        if (fgets(url, sizeof(url), stdin) == NULL)
        break;
        if ((p = strrchr(url, '\n')) != NULL)
            *p = '\0';
        if ((p = strchr(url, '/')) != NULL) {
            path = p + 1;
            *p = '\0';
        }
        snprintf(s, sizeof(s), "//%s:80", url);
        ec_null( ssip = ssi_open(s, false) )
        ec_neg1( fd = ssi_get_server_fd(ssip) )
        snprintf(s, sizeof(s), "GET /%s HTTP/1.0\r\n\r\n", path);
        ec_neg1( writeall(fd, s, strlen(s)) )
        while (true) {
            switch (nread = read(fd, s, sizeof(s))) {
            case 0:
                printf("EOF\n");
                break;
             case -1:
                EC_FAIL
             default:
                ec_neg1( writeall(STDOUT_FILENO, s, nread) )
                continue;
             }
             break;
         }
         ec_false( ssi_close(ssip) )
     }
     ec_false( !ferror(stdin) )
     exit(EXIT_SUCCESS);
 
exit(EXIT_FAILURE);
}
