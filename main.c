#include "provider/provider.h"
#include "dispatcher/dispatcher.h"
#include "accepter/accepter.h"
#include "reader/reader.h"
#include "list/list_sync.h"


void submitRequest(buffer_t* requests, char name[]){
  put_non_bloccante(requests, msg_init_string(name));
}

void termina_reader_rimasti(list_sync_t* list_sync){
  iterator_t* iterator = iterator_init(list_sync);
  while(hasNext(iterator)){
    reader_t* reader = (reader_t*) next(iterator);
    put_bloccante(reader->reader_buffer, POISON_PILL);
  }
}
void lancia_richieste_nuovi_reader(accepter_t* accepter){
  submitRequest(accepter->accepter_buffer,"Reader 1");
  submitRequest(accepter->accepter_buffer,"Reader 2");
}

int main(){
  pthread_t dispatcher_thread =NULL;
  pthread_t accepter_thread=NULL;
  pthread_t provider_thread=NULL;

  provider_t* provider = provider_init(3);
  buffer_t* provider_buffer = buffer_init(4);
  accepter_t* accepter = accepter_init();
  list_sync_t* list_sync = list_sync_init();
  provider->provider_buffer = provider_buffer;

  printf("%s\n", "Creazione degli attori conclusa!");
  printf("%s\n", "Lancio del provider in corso");
  pthread_create(&provider_thread,NULL,&provider_start_thread,provider);
  printf("%s\n", "Lancio del provider concluso");

  accepter_creation_params* accepter_params = malloc(sizeof(accepter_creation_params));
  accepter_params->list_sync = list_sync;
  accepter_params->accepter = accepter;
  dispatcher_creation_params* dispatcher_params = malloc(sizeof(dispatcher_creation_params));
  dispatcher_params->list_sync = list_sync;
  dispatcher_params->provider_buffer = provider->provider_buffer;

  printf("%s\n", "Lancio dell'accepter in corso");
  pthread_create(&accepter_thread,NULL,&accepter_start_thread,accepter_params);
  printf("%s\n", "Lancio dell'accepter concluso");

  printf("%s\n", "Lancio di richieste di nuovi reader in corso");
  lancia_richieste_nuovi_reader(accepter);
  printf("%s\n", "Lancio di richieste di nuovi reader concluse");

  printf("%s\n", "Lancio del dispatcher in corso");
  pthread_create(&dispatcher_thread,NULL,&dispatcher_start_thread,dispatcher_params);
  printf("%s\n", "Lancio del dispatcher concluso");
  pthread_join(provider_thread,NULL);
  printf("%s\n", "Provider spento");
  put_bloccante(accepter->accepter_buffer,POISON_PILL);
  pthread_join(dispatcher_thread,NULL);
  printf("%s\n", "Dispatcher spento");
  pthread_join(accepter_thread, NULL);
  printf("%s\n", "Accepter spento");
  termina_reader_rimasti(list_sync);
}
