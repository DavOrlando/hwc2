#include "../prodcons/buffer.h"
#include <pthread.h>
#include "CUnit/Basic.h"

#define CONTENUTO_MESSAGGIO_UNO "CONTENUTO_MESSAGGIO UNO" //Contenuto del messaggio 1
#define CONTENUTO_MESSAGGIO_DUE "CONTENUTO_MESSAGGIO DUE" //Contenuto del messaggio 1

#define DIMENSIONE_3 3 //maxsize del buffer
#define DIMENSIONE_2 2 //maxsize del buffer

buffer_t* buffer_dimensione_due = NULL;
buffer_t* buffer_dimensione_tre = NULL;
msg_t* messaggio_uno = NULL; //messaggio da inserire nel buffer
msg_t* messaggio_due = NULL; //messaggio da inserire nel buffer


//  Funzione che inizializza la suite: inizializza i messaggi che serviranno
// nel buffer
int test_buffer_capacitato_setup(void){
    messaggio_uno = msg_init_string(CONTENUTO_MESSAGGIO_UNO);
    messaggio_due = msg_init_string(CONTENUTO_MESSAGGIO_DUE);
    return 0;
}

//  Funzione che distrugge la suite: distrugge i messaggi.
int test_buffer_capacitato_clean(void){
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

//  Funzione usa-getta per la produzione bloccante di un messaggio
void* produci_bloccante_messaggio_uno(buffer_t* buffer){
    put_bloccante(buffer,messaggio_uno);
    return NULL;
}

//  Funzione usa-getta per la produzione bloccante di un messaggio
void* produci_bloccante_messaggio_due(buffer_t* buffer){
    put_bloccante(buffer,messaggio_due);
    return NULL;
}


//  Funzione usa-getta per la produzione non bloccante di un messaggio
void* produci_nonbloccante_messaggio_uno(buffer_t* buffer){
    pthread_exit(put_non_bloccante(buffer,messaggio_uno));
    return NULL;
}

//  Funzione usa-getta per la produzione non bloccante di un messaggio
void* produci_nonbloccante_messaggio_due(buffer_t* buffer){
    pthread_exit(put_non_bloccante(buffer,messaggio_due));
    return NULL;
}

//  Funzione usa-getta per la consumazione bloccante di un messaggio
//  nel buffer.
void* consuma_bloccante(buffer_t* buffer){
    pthread_exit(get_bloccante(buffer));
    return NULL;
}


//  Funzione usa-getta per la consumazione non bloccante di un messaggio
//  nel buffer.
void* consuma_nonbloccante(buffer_t* buffer){
    pthread_exit(get_non_bloccante(buffer));
    return NULL;
}

//Verifico creazione del buffer pieno con dimensione 2.
void test_buffer_dimensione_due_pieno_creazione(void){
    buffer_dimensione_due = get_buffer_dimensione_2_pieno();
    CU_ASSERT_PTR_NOT_NULL(buffer_dimensione_due);
    CU_ASSERT_PTR_NOT_NULL(buffer_dimensione_due->messaggi[0]);
    CU_ASSERT_PTR_NOT_NULL(buffer_dimensione_due->messaggi[1]);
    CU_ASSERT_EQUAL(buffer_dimensione_due->maxsize,DIMENSIONE_2);
    buffer_destroy(buffer_dimensione_due);
}

//Verifico creazione del buffer vuoto con dimensione 3.
void test_buffer_dimensione_tre_vuoto_creazione(void){
    buffer_dimensione_tre = get_buffer_dimensione_3_vuoto();
    CU_ASSERT_PTR_NOT_NULL(buffer_dimensione_tre);
    CU_ASSERT_PTR_NULL(buffer_dimensione_tre->messaggi[0]);
    CU_ASSERT_EQUAL(buffer_dimensione_tre->maxsize,DIMENSIONE_3);
    buffer_destroy(buffer_dimensione_due);
}

//Produzione concorrente bloccante di molteplici messaggi in un buffer vuoto,
// il buffer non si riempie.
void test_buffer_dimensione_tre_vuoto_produci_produci_bloccante(void){
    buffer_dimensione_tre = get_buffer_dimensione_3_vuoto();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_create(&produttore, NULL,&produci_bloccante_messaggio_uno,buffer_dimensione_tre);
    sleep(1);
    pthread_create(&produttore2, NULL,&produci_bloccante_messaggio_due,buffer_dimensione_tre);
    pthread_join(produttore,NULL);
    pthread_join(produttore2,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_tre->messaggi[0],messaggio_uno);
    CU_ASSERT_EQUAL(buffer_dimensione_tre->messaggi[1],messaggio_due);
    CU_ASSERT_PTR_NULL(buffer_dimensione_tre->messaggi[2]);
    buffer_destroy(buffer_dimensione_tre);
}


//Produzione concorrente bloccante di molteplici messaggi in un buffer vuoto,
// il buffer non si riempie. Verifica indice
void test_buffer_dimensione_tre_vuoto_produci_produci_bloccante_verifica_indice(void){
    buffer_dimensione_tre = get_buffer_dimensione_3_vuoto();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_create(&produttore, NULL,&produci_bloccante_messaggio_uno,buffer_dimensione_tre);
    pthread_create(&produttore2, NULL,&produci_bloccante_messaggio_due,buffer_dimensione_tre);
    pthread_join(produttore,NULL);
    pthread_join(produttore2,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_tre->indice_inserimento,2);
    CU_ASSERT_PTR_NULL(buffer_dimensione_tre->messaggi[2]);
    buffer_destroy(buffer_dimensione_tre);
}

//Produzione concorrente bloccante di molteplici messaggi in un buffer vuoto,
// il buffer non si riempie. Verifica indice
void test_buffer_dimensione_tre_vuoto_produci_produci_nonbloccante_verifica_indice(void){
    buffer_dimensione_tre = get_buffer_dimensione_3_vuoto();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_create(&produttore, NULL,&produci_nonbloccante_messaggio_uno,buffer_dimensione_tre);
    pthread_create(&produttore2, NULL,&produci_nonbloccante_messaggio_due,buffer_dimensione_tre);
    pthread_join(produttore,NULL);
    pthread_join(produttore2,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_tre->indice_inserimento,2);
    CU_ASSERT_PTR_NULL(buffer_dimensione_tre->messaggi[2]);
    buffer_destroy(buffer_dimensione_tre);
}

//Produzione concorrente non bloccante di molteplici messaggi in un buffer vuoto,
// il buffer non si riempie.
void test_buffer_dimensione_tre_vuoto_produci_produci_nonbloccante(void){
    buffer_dimensione_tre = get_buffer_dimensione_3_vuoto();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_create(&produttore, NULL,&produci_nonbloccante_messaggio_uno,buffer_dimensione_tre);
    sleep(1);
    pthread_create(&produttore2, NULL,&produci_nonbloccante_messaggio_due,buffer_dimensione_tre);
    pthread_join(produttore,NULL);
    pthread_join(produttore2,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_tre->messaggi[0],messaggio_uno);
    CU_ASSERT_EQUAL(buffer_dimensione_tre->messaggi[1],messaggio_due);
    CU_ASSERT_PTR_NULL(buffer_dimensione_tre->messaggi[2]);
    buffer_destroy(buffer_dimensione_tre);
}

//Produzione concorrente bloccante di molteplici messaggi in un buffer pieno,
// il buffer è gia saturo
void test_buffer_dimensione_due_pieno_produci_produci_bloccante_attesa(void){
    buffer_dimensione_due = get_buffer_dimensione_2_pieno();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_create(&produttore, NULL,&produci_bloccante_messaggio_uno,buffer_dimensione_due);
    pthread_create(&produttore2, NULL,&produci_bloccante_messaggio_due,buffer_dimensione_due);
    sleep(1);
    printf("I produttori sono in attesa...");
    pthread_cancel(produttore);
    pthread_cancel(produttore2);
    buffer_destroy(buffer_dimensione_due);
}

//Produzione concorrente non bloccante di molteplici messaggi in un buffer pieno,
// il buffer è gia saturo. Due BUFFER_ERROR.
void test_buffer_dimensione_due_pieno_produci_produci_nonbloccante(void){
    buffer_dimensione_due = get_buffer_dimensione_2_pieno();
    msg_t* mes_inserito_uno;
    msg_t* mes_inserito_due;
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_create(&produttore, NULL,&produci_nonbloccante_messaggio_uno,buffer_dimensione_due);
    pthread_create(&produttore2, NULL,&produci_nonbloccante_messaggio_due,buffer_dimensione_due);
    pthread_join(produttore,&mes_inserito_uno);
    pthread_join(produttore2,&mes_inserito_due);
    CU_ASSERT_EQUAL(mes_inserito_uno,BUFFER_ERROR);
    CU_ASSERT_EQUAL(mes_inserito_due,BUFFER_ERROR);
    buffer_destroy(buffer_dimensione_due);
}

//Consumazione concorrente bloccante di molteplici messaggi in un buffer pieno.
void test_buffer_dimensione_due_pieno_consuma_consuma_bloccante_buffer_svuotato(void){
    buffer_dimensione_due = get_buffer_dimensione_2_pieno();
    pthread_t consumatore = NULL;
    pthread_t consumatore2 = NULL;
    pthread_create(&consumatore, NULL,&consuma_bloccante,buffer_dimensione_due);
    pthread_create(&consumatore2, NULL,&consuma_bloccante,buffer_dimensione_due);
    pthread_join(consumatore2,NULL);
    pthread_join(consumatore,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_due->size,0);
    buffer_destroy(buffer_dimensione_due);
}


//Consumazione concorrente bloccante di molteplici messaggi in un buffer pieno.
//Verifica indice
void test_buffer_dimensione_due_pieno_consuma_consuma_bloccante_verifica_indice(void){
    buffer_dimensione_due = get_buffer_dimensione_2_pieno();
    pthread_t consumatore = NULL;
    pthread_t consumatore2 = NULL;
    pthread_create(&consumatore, NULL,&consuma_bloccante,buffer_dimensione_due);
    pthread_create(&consumatore2, NULL,&consuma_bloccante,buffer_dimensione_due);
    pthread_join(consumatore2,NULL);
    pthread_join(consumatore,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_due->indice_estrazione,0);
    buffer_destroy(buffer_dimensione_due);
}

//Consumazione bloccante di un messaggio in un buffer pieno.
//Verifica indice
void test_buffer_dimensione_due_pieno_consuma_bloccante_verifica_indice(void){
    buffer_dimensione_due = get_buffer_dimensione_2_pieno();
    pthread_t consumatore = NULL;
    pthread_create(&consumatore, NULL,&consuma_bloccante,buffer_dimensione_due);
    pthread_join(consumatore,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_due->indice_estrazione,1);
    buffer_destroy(buffer_dimensione_due);
}

//Consumazione non bloccante di un messaggio in un buffer pieno.
//Verifica indice
void test_buffer_dimensione_due_pieno_consuma_nonbloccante_verifica_indice(void){
    buffer_dimensione_due = get_buffer_dimensione_2_pieno();
    pthread_t consumatore = NULL;
    pthread_create(&consumatore, NULL,&consuma_nonbloccante,buffer_dimensione_due);
    pthread_join(consumatore,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_due->indice_estrazione,1);
    buffer_destroy(buffer_dimensione_due);
}


//Consumazione concorrente non bloccante di molteplici messaggi in un buffer pieno.
//Verifica indice
void test_buffer_dimensione_due_pieno_consuma_consuma_nonbloccante_verifica_indice(void){
    buffer_dimensione_due = get_buffer_dimensione_2_pieno();
    pthread_t consumatore = NULL;
    pthread_t consumatore2 = NULL;
    pthread_create(&consumatore, NULL,&consuma_nonbloccante,buffer_dimensione_due);
    pthread_create(&consumatore2, NULL,&consuma_nonbloccante,buffer_dimensione_due);
    pthread_join(consumatore2,NULL);
    pthread_join(consumatore,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_due->indice_estrazione,0);
    buffer_destroy(buffer_dimensione_due);
}


//Consumazione concorrente bloccante di molteplici messaggi in un buffer pieno.
void test_buffer_dimensione_due_pieno_consuma_consuma_nonbloccante_buffer_svuotato(void){
    buffer_dimensione_due = get_buffer_dimensione_2_pieno();
    pthread_t consumatore = NULL;
    pthread_t consumatore2 = NULL;
    pthread_create(&consumatore, NULL,&consuma_nonbloccante,buffer_dimensione_due);
    pthread_create(&consumatore2, NULL,&consuma_nonbloccante,buffer_dimensione_due);
    pthread_join(consumatore2,NULL);
    pthread_join(consumatore,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_due->size,0);
    buffer_destroy(buffer_dimensione_due);
}

//Consumazioni e produzioni concorrenti di molteplici messaggi in buffer.
//Versione bloccante: prod-prod-cons verifica del messaggio
void test_buffer_dimensione_tre_vuoto_produci_produci_consuma_bloccante_buffer_messaggio_rimasto(void){
    buffer_dimensione_tre = get_buffer_dimensione_3_vuoto();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&produttore, NULL,&produci_bloccante_messaggio_uno,buffer_dimensione_tre);
    sleep(1);//permetto al produttore di inserire il primo messaggio
    pthread_create(&produttore2, NULL,&produci_bloccante_messaggio_due,buffer_dimensione_tre);
    pthread_create(&consumatore, NULL,&consuma_bloccante,buffer_dimensione_tre);
    pthread_join(produttore2,NULL);
    pthread_join(produttore,NULL);
    pthread_join(consumatore,NULL);
    CU_ASSERT_STRING_EQUAL(buffer_dimensione_tre->messaggi[1]->content,CONTENUTO_MESSAGGIO_DUE);
    CU_ASSERT_PTR_NULL(buffer_dimensione_tre->messaggi[0]);
    buffer_destroy(buffer_dimensione_tre);
}

//Consumazioni e produzioni concorrenti di molteplici messaggi in buffer.
//Versione bloccante: prod-prod-cons verifica un messaggio rimasto
void test_buffer_dimensione_tre_vuoto_produci_produci_consuma_bloccante_buffer_con_un_messaggio(void){
    buffer_dimensione_tre = get_buffer_dimensione_3_vuoto();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&produttore, NULL,&produci_bloccante_messaggio_uno,buffer_dimensione_tre);
    pthread_create(&produttore2, NULL,&produci_bloccante_messaggio_due,buffer_dimensione_tre);
    pthread_create(&consumatore, NULL,&consuma_bloccante,buffer_dimensione_tre);
    pthread_join(produttore2,NULL);
    pthread_join(produttore,NULL);
    pthread_join(consumatore,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_tre->size,1);
    buffer_destroy(buffer_dimensione_tre);
}


//Consumazioni e produzioni concorrenti di molteplici messaggi in buffer.
//Versione non bloccante: prod-prod-cons verifica un messaggio rimasto
void test_buffer_dimensione_tre_vuoto_produci_produci_consuma_nonbloccante_buffer_con_un_messaggio(void){
    buffer_dimensione_tre = get_buffer_dimensione_3_vuoto();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&produttore, NULL,&produci_nonbloccante_messaggio_uno,buffer_dimensione_tre);
    pthread_create(&produttore2, NULL,&produci_nonbloccante_messaggio_due,buffer_dimensione_tre);
    pthread_create(&consumatore, NULL,&consuma_nonbloccante,buffer_dimensione_tre);
    pthread_join(produttore2,NULL);
    pthread_join(produttore,NULL);
    pthread_join(consumatore,NULL);
    CU_ASSERT_EQUAL(buffer_dimensione_tre->size,1);
    buffer_destroy(buffer_dimensione_tre);
}


//Consumazioni e produzioni concorrenti di molteplici messaggi in buffer.
//Versione non bloccante: prod-prod-cons verifica del messaggio
void test_buffer_dimensione_tre_vuoto_produci_produci_consuma_nonbloccante_buffer_messaggio_rimasto(void){
    buffer_dimensione_tre = get_buffer_dimensione_3_vuoto();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&produttore, NULL,&produci_nonbloccante_messaggio_uno,buffer_dimensione_tre);
    sleep(1);//permetto al produttore di inserire il primo messaggio
    pthread_create(&produttore2, NULL,&produci_nonbloccante_messaggio_due,buffer_dimensione_tre);
    pthread_create(&consumatore, NULL,&consuma_nonbloccante,buffer_dimensione_tre);
    pthread_join(produttore2,NULL);
    pthread_join(produttore,NULL);
    pthread_join(consumatore,NULL);
    CU_ASSERT_STRING_EQUAL(buffer_dimensione_tre->messaggi[1]->content,CONTENUTO_MESSAGGIO_DUE);
    CU_ASSERT_PTR_NULL(buffer_dimensione_tre->messaggi[0]);
    buffer_destroy(buffer_dimensione_tre);
}

//  Crea la suite e inserisce tutti i test unitari alla suite
int test_buffer_capacitato_create(){
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite(
                          "Produzione e consumazione in un buffer di dimensione > 1",
                          test_buffer_capacitato_setup, test_buffer_capacitato_clean);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite,
                    "test_buffer_dimensione_due_pieno_creazione",
                    test_buffer_dimensione_due_pieno_creazione) == NULL ||
        CU_add_test(pSuite,
                     "test_buffer_dimensione_tre_vuoto_creazione",
                     test_buffer_dimensione_tre_vuoto_creazione) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_tre_vuoto_produci_produci_bloccante",
                    test_buffer_dimensione_tre_vuoto_produci_produci_bloccante) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_tre_vuoto_produci_produci_bloccante_verifica_indice",
                    test_buffer_dimensione_tre_vuoto_produci_produci_bloccante_verifica_indice) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_tre_vuoto_produci_produci_nonbloccante_verifica_indice",
                    test_buffer_dimensione_tre_vuoto_produci_produci_nonbloccante_verifica_indice) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_tre_vuoto_produci_produci_nonbloccante",
                    test_buffer_dimensione_tre_vuoto_produci_produci_nonbloccante) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_due_pieno_produci_produci_bloccante_attesa",
                    test_buffer_dimensione_due_pieno_produci_produci_bloccante_attesa) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_due_pieno_produci_produci_nonbloccante",
                    test_buffer_dimensione_due_pieno_produci_produci_nonbloccante) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_due_pieno_consuma_consuma_bloccante_buffer_svuotato",
                    test_buffer_dimensione_due_pieno_consuma_consuma_bloccante_buffer_svuotato) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_due_pieno_consuma_bloccante_verifica_indice",
                    test_buffer_dimensione_due_pieno_consuma_bloccante_verifica_indice) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_due_pieno_consuma_nonbloccante_verifica_indice",
                    test_buffer_dimensione_due_pieno_consuma_nonbloccante_verifica_indice) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_due_pieno_consuma_consuma_bloccante_verifica_indice",
                    test_buffer_dimensione_due_pieno_consuma_consuma_bloccante_verifica_indice) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_due_pieno_consuma_consuma_nonbloccante_verifica_indice",
                    test_buffer_dimensione_due_pieno_consuma_consuma_nonbloccante_verifica_indice) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_due_pieno_consuma_consuma_nonbloccante_buffer_svuotato",
                    test_buffer_dimensione_due_pieno_consuma_consuma_nonbloccante_buffer_svuotato) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_tre_vuoto_produci_produci_consuma_bloccante_buffer_messaggio_rimasto",
                    test_buffer_dimensione_tre_vuoto_produci_produci_consuma_bloccante_buffer_messaggio_rimasto) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_tre_vuoto_produci_produci_consuma_bloccante_buffer_con_un_messaggio",
                    test_buffer_dimensione_tre_vuoto_produci_produci_consuma_bloccante_buffer_con_un_messaggio) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_tre_vuoto_produci_produci_consuma_nonbloccante_buffer_con_un_messaggio",
                    test_buffer_dimensione_tre_vuoto_produci_produci_consuma_nonbloccante_buffer_con_un_messaggio) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_dimensione_tre_vuoto_produci_produci_consuma_nonbloccante_buffer_messaggio_rimasto",
                    test_buffer_dimensione_tre_vuoto_produci_produci_consuma_nonbloccante_buffer_messaggio_rimasto) == NULL
        )


    {
        CU_cleanup_registry();
        return CU_get_error();
    }
}
