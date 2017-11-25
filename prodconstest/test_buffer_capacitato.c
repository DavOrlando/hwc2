#include "../prodcons/buffer.h"
#include <pthread.h>
#include "CUnit/Basic.h"

#define CONTENUTO_MESSAGGIO_UNO "CONTENUTO_MESSAGGIO UNO" //Contenuto del messaggio 1
#define CONTENUTO_MESSAGGIO_DUE "CONTENUTO_MESSAGGIO DUE" //Contenuto del messaggio 1

#define DIMENSIONE_3 3 //maxsize del buffer
#define DIMENSIONE_2 2 //maxsize del buffer

static buffer_t* buffer = NULL; //buffer
static msg_t* messaggio_uno = NULL; //messaggio da inserire nel buffer
static msg_t* messaggio_due = NULL; //messaggio da inserire nel buffer

//  Funzione che inizializza la suite: inizializza i messaggi che serviranno
// nel buffer
int test_buffer_capacitato_setup(void){
    messaggio_uno = msg_init_string(CONTENUTO_MESSAGGIO_UNO);
    messaggio_due = msg_init_string(CONTENUTO_MESSAGGIO_DUE);
    return 0;
}

//  Funzione che distrugge la suite: distrugge i messaggi.
int test_buffer_unitario_clean(void){
    msg_destroy_string(messaggio_uno);
    msg_destroy_string(messaggio_due);
    return 0;
}

//  Funzione che ritorna un buffer inizializzato vuoto con tre posizioni.
buffer_t* get_buffer_dimensione_3_vuoto(){
    return buffer_init(DIMENSIONE_3);
}

//  Funzione che ritorna un buffer con dimensione 2 inizializzato e riempito.
buffer_t* get_buffer_dimensione_2_pieno(){
    buffer_t* buffer = buffer_init(DIMENSIONE_2);
    put_bloccante(buffer,messaggio_uno);
    put_bloccante(buffer,messaggio_due);
    return buffer;
}



