#ifndef PROVIDER_H
#define PROVIDER_H

#include "../msg/poison_pill.h"
#include "../buffer/buffer.h"

typedef struct {
    msg_t** messaggi;   // numero finito di messaggi
    unsigned int numero_messaggi;
    buffer_t* provider_buffer; // buffer dove vengono inseriti i messaggi prodotti
} provider_t;

// creazione di un provider con messaggi
provider_t* provider_init(unsigned int);
// deallocazione di un provider
void provider_destroy(provider_t*);

//il provider produce il messaggio nel provider_buffer
void provider_produci_messaggio(int , provider_t*);
//il provider produce tutti i messaggi compreso POISON_PILL
void provider_produci_messaggi(provider_t*);

void provider_start_thread(void*);

#endif
