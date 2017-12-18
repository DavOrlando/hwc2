#ifndef LIST_SYNC_H
#define LIST_SYNC_H


#include "list.h"
#include <pthread.h>

/*Lista con possibilità di sincronizzazione, struttura dati con lista e mutex
garantisce la mutua esclusione.*/
typedef struct{
  pthread_mutex_t mutex;
	list_t* lista;
} list_sync_t;

list_sync_t* list_sync_init(void);

void list_sync_destroy(list_sync_t*);

#endif
