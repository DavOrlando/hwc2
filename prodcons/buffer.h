#ifndef BUFFER_H
#define BUFFER_H

#include <semaphore.h>
#include <pthread.h>
#include "msg_string.h"
#include <errno.h>

#define BUFFER_ERROR  (msg_t *) NULL

typedef struct {
    msg_t** messaggi;   // contenitore di N messaggi
    unsigned int maxsize; // quanti messaggi può opsitare il buffer
    unsigned int size;
    unsigned int indice_estrazione;
    unsigned int indice_inserimento;
    sem_t pieno;
    sem_t vuoto;
    pthread_mutex_t uso_indice_estrazione;
    pthread_mutex_t uso_indice_inserimento;
} buffer_t;

/* allocazione / deallocazione  buffer */
// creazione di un buffer vuoto di dim. max nota
buffer_t* buffer_init(unsigned int);
// deallocazione di un buffer
void buffer_destroy(buffer_t*);
/* operazioni sul buffer */
// inserimento bloccante: sospende se pieno, quindi
// effettua l’inserimento non appena si libera dello spazio
// restituisce il messaggio inserito; N.B.: msg!=null
msg_t* put_bloccante(buffer_t*, msg_t* );
// inserimento non bloccante: restituisce BUFFER_ERROR se pieno,
// altrimenti effettua l’inserimento e restituisce il messaggio
// inserito; N.B.: msg!=null
msg_t* put_non_bloccante(buffer_t* , msg_t* );
// estrazione bloccante: sospende se vuoto, quindi
// restituisce il valore estratto non appena disponibile
msg_t* get_bloccante(buffer_t* );
// estrazione non bloccante: restituisce BUFFER_ERROR se vuoto
// ed il valore estratto in caso contrario
msg_t* get_non_bloccante(buffer_t* );

#endif
