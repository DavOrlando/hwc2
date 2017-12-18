#ifndef ACCEPTER_H
#define ACCEPTER_H

#include "../buffer/buffer.h"
#include "../list/list_sync.h"
#include "../msg/poison_pill.h"

typedef struct {
    buffer_t* accepter_buffer;
} accepter_t;

typedef struct {
  accepter_t* accepter;
  list_sync_t* list_sync;
} accepter_creation_params;

// creazione di un accepter
accepter_t* accepter_init();
// deallocazione di un accepter
void accepter_destroy(accepter_t*);

msg_t* accepter_extract(accepter_t*);

int accepter_create_reader(msg_t*, list_sync_t*, unsigned int);
void accepter_start(accepter_t*,list_sync_t*);
void accepter_start_thread(void*);

#endif
