#ifndef READER_H
#define READER_H

#include <semaphore.h>
#include <pthread.h>
#include "../buffer/buffer.h"
#include "../list/list_sync.h"

#define SIZE 3

typedef struct {
    char* nome;
    buffer_t* reader_buffer; // buffer dove vengono inseriti i messaggi
    unsigned int tempo_di_consumo;
} reader_t;


typedef struct {
  list_sync_t* list_sync;
  msg_t* msg;
  int tempo_di_consumo_reader;
} reader_creation_params;


// creazione di un provider con messaggi
reader_t* reader_init(char*,unsigned int);
// deallocazione di un provider
void reader_destroy(reader_t*);
int reader_start(reader_t*);

#endif
