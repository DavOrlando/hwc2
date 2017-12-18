#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "../msg/poison_pill.h"
#include "../buffer/buffer.h"
#include "../list/list_sync.h"
#include "../reader/reader.h"

typedef struct {
  buffer_t* provider_buffer;
  list_sync_t* list_sync;
} dispatcher_creation_params;


void dispatcher_start(buffer_t*,list_sync_t*);
void dispatcher_multi_send_message(list_sync_t*,msg_t*);
void dispatcher_try_to_send_message(reader_t*, msg_t*);
void dispatcher_kill_reader(reader_t*);
void dispatcher_start_thread(void*);

#endif
