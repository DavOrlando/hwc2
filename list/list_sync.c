#include "list_sync.h"


list_sync_t* list_sync_init(void){
  list_sync_t* lista_sync = malloc(sizeof(list_sync_t));
  lista_sync->lista = list_init();
  lista_sync->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
}

void list_sync_destroy(list_sync_t* lista_sync){
  pthread_mutex_destroy(&(lista_sync->mutex));
  list_destroy(lista_sync->lista);
  free(lista_sync);
}
