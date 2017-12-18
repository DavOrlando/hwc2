#include "accepter.h"
#include "../reader/reader.h"

#define DIM_ACCEPTER_BUFFER 10

accepter_t* accepter_init(){
  accepter_t* accepter = malloc(sizeof(accepter));
  accepter->accepter_buffer = buffer_init(DIM_ACCEPTER_BUFFER);
  return accepter;
}

void accepter_destroy(accepter_t* accepter){
  buffer_destroy(accepter->accepter_buffer);
  free(accepter);
}

/*
  Estrae un messaggio dall'accepter_buffer
*/
msg_t* accepter_extract(accepter_t* accepter){
   return get_bloccante(accepter->accepter_buffer);
}


/*
  Crea il reader su un thread separato che ne gestisce il ciclo vita
*/
int accepter_create_reader_thread(void* parameters){
  reader_creation_params* params = (reader_creation_params*) parameters;
  reader_t* reader = reader_init(params->msg->content,params->tempo_di_consumo_reader);
  pthread_mutex_lock(&(params->list_sync->mutex));
  addElement(params->list_sync->lista,reader);
  pthread_mutex_unlock(&(params->list_sync->mutex));
  pthread_t thread_avvio_reader;
  pthread_create(&thread_avvio_reader,NULL,&reader_start,reader);
  pthread_join(thread_avvio_reader,NULL);
  pthread_mutex_lock(&(params->list_sync->mutex));
  removeElement(params->list_sync->lista,reader);
  pthread_mutex_unlock(&(params->list_sync->mutex));
  return 0;
}


/*
  Utilizza il messaggio per creare un reader con il nome che è dentro al
  messaggio.
*/
int accepter_create_reader(msg_t* msg, list_sync_t* list_sync, unsigned int tempo_di_consumo){
  reader_creation_params* params = malloc(sizeof(reader_creation_params));
  params->msg = msg;
  params->list_sync = list_sync;
  pthread_t thread_creazione_reader = NULL;
  const pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  params->tempo_di_consumo_reader = tempo_di_consumo;
  pthread_create(&thread_creazione_reader, &attr, &accepter_create_reader_thread,params);
  pthread_attr_destroy(&attr);
  return 0;
}



/*
funzione che identifica il ciclo di vita dell'accepter.
Ovvero estrazione di messaggi dalla coda di richieste
e creazione di reader fino alla ricezione della poison pill.
Si alterna la creazione di un reader veloce a quella di uno lento.
*/
void accepter_start(accepter_t* accepter,list_sync_t* list_sync){
  int i = 0; //per fare un reader lento e uno veloce
  while(1){ //ciclo di vita dell'accepter
    msg_t* msg = accepter_extract(accepter); //estrai messaggio dalla coda
    if(msg == POISON_PILL)
      break; //termina
    if(i%2==0)
      accepter_create_reader(msg, list_sync,1); //crea reader veloce
    else accepter_create_reader(msg, list_sync,3); //crea reader lento
    i++;
  }
}


/*Funzione per lancio accepter su un thread differente*/
void accepter_start_thread(void* parameters){
    accepter_creation_params* accepter_params = (accepter_creation_params*) parameters;
    accepter_start(accepter_params->accepter,accepter_params->list_sync);
}
