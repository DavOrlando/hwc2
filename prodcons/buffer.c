#include "buffer.h"

/* allocazione / deallocazione  buffer */
// creazione di un buffer vuoto di dim. max nota
buffer_t* buffer_init(unsigned int maxsize){
  buffer_t* buffer = (buffer_t*) malloc(sizeof(buffer_t));
  buffer->maxsize = maxsize;
  buffer->messaggi = (msg_t**) malloc(sizeof(msg_t*)*(buffer->maxsize));
  buffer->size = 0;
  buffer->indice_estrazione = 0;
  buffer->indice_inserimento = 0;
  buffer->uso_indice_inserimento = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
  for(int i=0;i<maxsize;i++)
    buffer->messaggi[i] = NULL;
  sem_init(&(buffer->vuoto), 0, buffer->maxsize);
  sem_init(&(buffer->pieno), 0, 0);
  return buffer;
}

// deallocazione di un buffer
void buffer_destroy(buffer_t* buffer){
  pthread_mutex_destroy(&(buffer->uso_indice_inserimento));
  sem_destroy(&(buffer->vuoto));
  sem_destroy(&(buffer->pieno));
  free(buffer->messaggi);
  free(buffer);
}



/* operazioni sul buffer */
// inserimento bloccante: sospende se pieno, quindi
// effettua l’inserimento non appena si libera dello spazio
// restituisce il messaggio inserito; N.B.: msg!=null
msg_t* put_bloccante(buffer_t* buffer, msg_t* msg){
  sem_wait(&(buffer->vuoto));
  pthread_mutex_lock(&(buffer->uso_indice_inserimento));
  buffer->messaggi[buffer->indice_inserimento] = msg;
  buffer->indice_inserimento = (buffer->indice_inserimento +1) % buffer->maxsize;
  buffer->size++;
  pthread_mutex_unlock(&(buffer->uso_indice_inserimento));
  sem_post(&(buffer->pieno));
  return msg;
}
// inserimento non bloccante: restituisce BUFFER_ERROR se pieno,
// altrimenti effettua l’inserimento e restituisce il messaggio
// inserito; N.B.: msg!=null
msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg){
  sem_trywait(&(buffer->vuoto));
  if (errno == EAGAIN)
    return BUFFER_ERROR;
  pthread_mutex_lock(&(buffer->uso_indice_inserimento));
  buffer->messaggi[buffer->indice_inserimento] = msg;
  buffer->indice_inserimento = (buffer->indice_inserimento +1) % buffer->maxsize;
  buffer->size++;
  pthread_mutex_unlock(&(buffer->uso_indice_inserimento));
  sem_post(&(buffer->pieno));
  return msg;
}
// estrazione bloccante: sospende se vuoto, quindi
// restituisce il valore estratto non appena disponibile
msg_t* get_bloccante(buffer_t* buffer){};
// estrazione non bloccante: restituisce BUFFER_ERROR se vuoto
// ed il valore estratto in caso contrario
msg_t* get_non_bloccante(buffer_t* buffer){};
