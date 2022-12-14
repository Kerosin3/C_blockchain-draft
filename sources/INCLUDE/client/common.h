#ifndef COMMON
#define COMMON

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <threads.h>

#define DEFAULT_BUF_SIZE 4096
#define MAX_PEERS 100

extern atomic_uint_least64_t block_written_size;
extern atomic_bool block_transmitted;
extern mtx_t peer_connection_accepted;
extern mtx_t block_created_mtx;
extern int flag_block_created;
extern unsigned char *buffer_BLOCK_DATA;
extern atomic_bool kill_thread_p2p;

#endif
