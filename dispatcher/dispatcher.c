#include "dispatcher.h"


/*Invio del messaggio POISON_PILL al reader.*/
void dispatcher_kill_reader(reader_t* reader){
  put_bloccante(reader->reader_buffer,POISON_PILL);
}


/*Invio del messaggio effettivo con vari tentativi per cercare di terminare
i reader più lenti. Si usa la put_non_bloccante in quanto riporta BUFFER_ERROR
se il buffer del reader è pieno. Dopo tre volte che risulta pieno si
invia POISON_PILL per terminarlo.*/
void dispatcher_try_to_send_message(reader_t* reader, msg_t* msg){
  for(int tentativi=0;;tentativi++){
    if(put_non_bloccante(reader->reader_buffer,msg) != BUFFER_ERROR) return;
    if(tentativi == 2){
      pthread_t thread = NULL;
      const pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
      pthread_create(&thread, &attr, &dispatcher_kill_reader,reader);
      pthread_attr_destroy(&attr);
      return;
    }
    sleep(1);
  }
}

/*Invio del messaggio msg a tutti i reader presenti nella lista nel momento
del blocco della lista.*/
void dispatcher_multi_send_message(list_sync_t* list_sync, msg_t* msg){
  if(msg!=NULL){
    pthread_mutex_lock(&(list_sync->mutex));
    iterator_t* iter = iterator_init(list_sync->lista);
      while(hasNext(iter)){
        reader_t* reader = (reader_t*) next(iter);
        dispatcher_try_to_send_message(reader,msg);
      }
    pthread_mutex_unlock(&(list_sync->mutex));
  }
}

/*Inizio estrazione di messaggi dal provider buffer e ridistribuzione
verso i reader. */
void dispatcher_start(buffer_t* provider_buffer, list_sync_t* list_sync){
  msg_t* msg = NULL;
  while( (msg = get_bloccante(provider_buffer) ) != POISON_PILL){
    dispatcher_multi_send_message(list_sync,msg);
  }
  dispatcher_multi_send_message(list_sync,msg);//invia la POISON_PILL
}

/*funzione per eseguire il dispatcher su un thread differente*/
void dispatcher_start_thread(void* parameters){
    dispatcher_creation_params* dispatcher_params = (dispatcher_creation_params*) parameters;
    dispatcher_start(dispatcher_params->provider_buffer,dispatcher_params->list_sync);
}
