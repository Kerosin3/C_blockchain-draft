#ifndef IPCMESSAGESCLIENT
#define IPCMESSAGESCLIENT

#include "../serdes/transaction.pb-c.h"
#include "../accounts/acc_utils.h"
#include <sodium/crypto_sign.h>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <liburing.h>
#include <linux/io_uring.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>

long long get_date_usec_rec();
IpcMessage__Status read_response_ONLY_STATUS(void* buf,size_t len);
size_t get_a_message(void* buf,size_t len,signed_message_t* a_msg);
size_t send_ONLY_status_code( IpcMessage* message,void* socket_buf, IpcMessage__Status STATUS);

#endif
