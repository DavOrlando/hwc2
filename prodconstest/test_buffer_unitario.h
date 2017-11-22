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



buffer_t* get_buffer_unitario_vuoto(void);
buffer_t* get_buffer_unitario_pieno(void);


void* produci_uno_bloccante();

void* produci_uno_non_bloccante();

void* consuma_uno_bloccante();

void* consuma_uno_non_bloccante();

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
void test_buffer_unitario_pieno_consuma_bloccante(void);
void test_buffer_unitario_pieno_consuma_non_bloccante(void);
void test_buffer_unitario_pieno_consuma_bloccante_decremento_size(void);
void test_buffer_unitario_pieno_consuma_bloccante_decremento_indice(void);
void test_buffer_unitario_vuoto_consuma_non_bloccante(void);
void test_buffer_unitario_vuoto_produci_consuma_bloccante(void);
void test_buffer_unitario_vuoto_produci_consuma_non_bloccante(void);
void test_buffer_unitario_pieno_produci_consuma_non_bloccante(void);
void test_buffer_unitario_vuoto_consuma_produci_bloccante(void);
void test_buffer_unitario_vuoto_consuma_produci_non_bloccante(void);
void test_buffer_unitario_pieno_consuma_produci_non_bloccante(void);
