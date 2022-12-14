/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: transaction.proto */

#ifndef PROTOBUF_C_transaction_2eproto__INCLUDED
#define PROTOBUF_C_transaction_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1004001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

typedef struct IpcMessage IpcMessage;

/* --- enums --- */

typedef enum _IpcMessage__Status
{
    IPC_MESSAGE__STATUS__TEST = 0,
    IPC_MESSAGE__STATUS__OK = 1,
    IPC_MESSAGE__STATUS__ERROR = 2,
    IPC_MESSAGE__STATUS__FINISH = 3,
    IPC_MESSAGE__STATUS__WAIT = 4,
    IPC_MESSAGE__STATUS__ASK_NEED_MSG = 5,
    IPC_MESSAGE__STATUS__NEED_MORE = 6,
    IPC_MESSAGE__STATUS__ACKN_OK = 7,
    IPC_MESSAGE__STATUS__MESSAGE_SENDED = 8,
    IPC_MESSAGE__STATUS__ENOUGH = 9,
    IPC_MESSAGE__STATUS__ALL_BLOCK_MSG_SENDED = 10,
    IPC_MESSAGE__STATUS__ALL_BLOCK_RECEIVED = 11 PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(IPC_MESSAGE__STATUS)
} IpcMessage__Status;

/* --- messages --- */

struct IpcMessage
{
    ProtobufCMessage base;
    protobuf_c_boolean has_transaction_msg;
    ProtobufCBinaryData transaction_msg;
    protobuf_c_boolean has_pubkey;
    ProtobufCBinaryData pubkey;
    char *timestamp;
    uint64_t time_num;
    IpcMessage__Status status_code;
};
#define IPC_MESSAGE__INIT                                                                                              \
    {                                                                                                                  \
        PROTOBUF_C_MESSAGE_INIT(&ipc_message__descriptor)                                                              \
        , 0, {0, NULL}, 0, {0, NULL}, NULL, 0, IPC_MESSAGE__STATUS__TEST                                               \
    }

/* IpcMessage methods */
void ipc_message__init(IpcMessage *message);
size_t ipc_message__get_packed_size(const IpcMessage *message);
size_t ipc_message__pack(const IpcMessage *message, uint8_t *out);
size_t ipc_message__pack_to_buffer(const IpcMessage *message, ProtobufCBuffer *buffer);
IpcMessage *ipc_message__unpack(ProtobufCAllocator *allocator, size_t len, const uint8_t *data);
void ipc_message__free_unpacked(IpcMessage *message, ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*IpcMessage_Closure)(const IpcMessage *message, void *closure_data);

/* --- services --- */

/* --- descriptors --- */

extern const ProtobufCMessageDescriptor ipc_message__descriptor;
extern const ProtobufCEnumDescriptor ipc_message__status__descriptor;

PROTOBUF_C__END_DECLS

#endif /* PROTOBUF_C_transaction_2eproto__INCLUDED */
