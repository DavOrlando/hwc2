#include "../prodcons/buffer.h"
#include <pthread.h>
#include "CUnit/Basic.h"

#define UNO "Messaggio UNO"
#define DIMENSIONE_UNITARIA 1

static buffer_t* buffer = NULL;
static msg_t* messaggio = NULL;

int test_buffer_unitario_create();

/* The suite initialization function.*/
int test_buffer_unitario_setup(void);

/* The suite destroy function.*/
int test_buffer_unitario_clean(void);

/* Test che verifica la creazione del buffer vuoto con
massimo un posto per i messaggi. */
void test_buffer_unitario_creazione(void);
void test_buffer_unitario_creazione_messaggio(void);
void test_buffer_unitario_messaggio_prodotto_bloccante(void);
void test_buffer_unitario_messaggio_prodotto_non_bloccante(void);
void test_buffer_unitario_aumento_numero_messaggi_bloccante(void);
void test_buffer_unitario_aumento_numero_messaggi_non_bloccante(void);
void test_buffer_unitario_aumento_indice_bloccante(void);
void test_buffer_unitario_aumento_indice_non_bloccante(void);
void test_buffer_unitario_pieno_produci_non_bloccante(void);
