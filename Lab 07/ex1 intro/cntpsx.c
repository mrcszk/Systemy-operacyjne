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


// gcc srvpsx.c -o srvpsx -lrt; gcc cntpsx.c -o cntpsx -lrt

int main(void) {
  char* srvpsxqbasename = "psxsrvqueue";
  char srvpsxqname[PSX_QUEUE_NAME];
  char cntpsxqname[PSX_QUEUE_NAME];
  mqd_t qdsrv, qdcnt;
  ssize_t msgsize, bread, bwrite;
  pid_t pid;
  struct message msg;

  printf("Server started...\n");
  setbuf(stdout, NULL);
  msgsize = sizeof(msg);
  pid = getpid();
  msg.pid = pid;

  /* Creating server psx queue */
  make_srv_psx_name(srvpsxqname, srvpsxqbasename, PSX_QUEUE_NAME);

  /* Opening client queue for reading  */
  printf("Opening server queue \'%s\' for writing...", srvpsxqname);
  qdsrv = mq_open(srvpsxqname, O_WRONLY);
  if (qdsrv == -1) {
    printf("FAIL!\nError: %s.\n", strerror(errno));
    return 0;
  }
  printf("OK\n");

  /* Creating client psx queue name */
  make_cnt_psx_name(cntpsxqname, pid, PSX_QUEUE_NAME);

  /* Opening client queue for reading  */
  printf("Opening client queue \'%s\' for reading...", cntpsxqname);
  mq_unlink(cntpsxqname);

  struct mq_attr mqAttr;
  mqAttr.mq_msgsize = 1024;
  qdcnt = mq_open(cntpsxqname, O_RDONLY | O_CREAT, PERM_FILE, &mqAttr);
  if (qdcnt == -1) {
    printf("FAIL!\nError: %s\n", strerror(errno));
    return 0;
  }
  printf("OK\n");

  while (1) {
    /* Getting message */
    printf("Your message: ");
    if (stdin_readall(msg.data, MESSAGE_BUF_SIZE) == 0) break;
    printf("MSG: %s; LEN: %ld\n", msg.data, sizeof(msg.data));

    /* Sending message */
    printf("Writting message...");
    bwrite = mq_send(qdsrv, (const char*)&msg, sizeof(msg), 0);
    if (bwrite == -1) {
      printf("FAIL!\nError: %s\n", strerror(errno));
      break;
    }
    printf("OK\n");

    /* Reading from queue */
    printf("Waiting for data...\n");
    bread =
        mq_receive(qdcnt, (char*)&msg, mqAttr.mq_msgsize * sizeof(msg), NULL);
    if (bread == -1) {
      printf("FAIL!\nError: %s\n", strerror(errno));
      break;
    }

    printf("Message from server: %s\n", msg.data);
  }

  /* Cleaning up */
  mq_close(qdsrv);
  mq_close(qdcnt);

  return 0;
}
