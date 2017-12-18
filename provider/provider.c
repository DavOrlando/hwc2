#include "provider.h"

// creazione di un provider con messaggi
provider_t* provider_init(unsigned int numero_messaggi){
  provider_t* provider = (provider_t*) malloc( sizeof(provider_t) );
  provider->numero_messaggi = numero_messaggi;
  provider->messaggi = malloc( (numero_messaggi+1) * sizeof(msg_t*) );
  for(int i=0;i<numero_messaggi;i++)
    provider->messaggi[i] = msg_init_int(i+1);
  provider->messaggi[numero_messaggi]=POISON_PILL;
  return provider;
}

// deallocazione di un provider
void provider_destroy(provider_t* provider){
  free(provider->messaggi);
  free(provider);
}

//il provider produce il messaggio nel provider_buffer
void provider_produci_messaggio(int posizione, provider_t* provider){
  put_bloccante(provider->provider_buffer,provider->messaggi[posizione]);
}

//il provider produce tutti i messaggi compreso POISON_PILL
void provider_produci_messaggi(provider_t* provider){
  for(int i = 0; i<(provider->numero_messaggi)+1;i++)
    provider_produci_messaggio(i,provider);
}
//funzione per l'esecuzione del provider su un thread differente
void provider_start_thread(void* param){
  provider_t* provider = (provider_t*) param;
  provider_produci_messaggi(provider);
}
