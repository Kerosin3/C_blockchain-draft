#ifndef EVENT_LOOP
#define EVENT_LOOP

#include "acc_utils.h"
#include "connection_handlers.h"
#include "data_handlers.h"
#include "misc.h"
#include "setup.h"
#include "start_server.h"
#include "zlog.h"
#include <liburing.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>

extern circ_buf_t CBUF;
void event_loop(int serv_sock, struct io_uring *ring);

#endif
