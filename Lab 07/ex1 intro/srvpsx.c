#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>
/* ------------------------------------------------------------- */

#include "psx.h"
/* ------------------------------------------------------------- */

int main(void) {
  char* srvpsxqbasename = "psxsrvqueue";
  char srvpsxqname[PSX_QUEUE_NAME];
  char cntpsxqname[PSX_QUEUE_NAME];
  mqd_t qdsrv, qdcnt;
  ssize_t msgsize, bread, bwrite;
  struct message msg;

  printf("Server started...\n");
  setbuf(stdout, NULL);
  msgsize = sizeof(msg);

  /* Creating server psx queue */
  make_srv_psx_name(srvpsxqname, srvpsxqbasename, PSX_QUEUE_NAME);

  printf("Opening server queue \'%s\' for reading...", srvpsxqname);
  int rc = mq_unlink(srvpsxqname);

  struct mq_attr mqAttr;
  mqAttr.mq_msgsize = 1024;
  qdsrv = mq_open(srvpsxqname, O_RDWR | O_CREAT, PERM_FILE, &mqAttr);
  if (qdsrv < 0) {
    printf("   Error %d (%s) on server mq_open.\n", errno, strerror(errno));
    exit(1);
  }
  printf("OK\n");

  while (1) {
    /* Reading from queue */
    printf("Waiting for data...\n");

    char buffer[2048];
    // bread = mq_receive(qdsrv, buffer, sizeof(buffer), NULL);
    bread = mq_receive(qdsrv, (char*)&msg, mqAttr.mq_msgsize * msgsize, NULL);
    if (bread == -1) {
      printf("FAIL!\nError: %s\n", strerror(errno));
      break;
    }
    printf("Message from [%ld]: %s\n", (long)msg.pid, msg.data);

    /* Getting responce */
    printf("Your responce: ");
    if (stdin_readall(msg.data, MESSAGE_BUF_SIZE) == 0) break;

    /* Creating client queue name */
    make_cnt_psx_name(cntpsxqname, msg.pid, PSX_QUEUE_NAME);

    /* Opening client fifo for writing  */
    printf("Opening client queue \'%s\' for writing...", cntpsxqname);
    qdcnt = mq_open(cntpsxqname, O_WRONLY);
    if (qdcnt == -1) {
      printf("FAIL!\nError: %s\n", strerror(errno));
      break;
    }

    /* Sending responce */
    printf("OK\nWritting responce to client %ld...", (long)msg.pid);
    bwrite = mq_send(qdcnt, (const char*)&msg, sizeof(msg), 0);
    if (bwrite == -1) {
      printf("FAIL!\nError: %s\n", strerror(errno));
      break;
    }
    printf("OK\n");
    mq_close(qdcnt);
  }

  /* Cleaning up */
  mq_close(qdsrv);
  mq_unlink(srvpsxqname);

  return 0;
}
