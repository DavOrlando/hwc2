#include "../prodcons/buffer.h"
#include <pthread.h>
#include "CUnit/Basic.h"

#define CONTENUTO_MESSAGGIO "CONTENUTO_MESSAGGIO" //Contenuto del messaggio
#define DIMENSIONE_UNITARIA 1 //maxsize del buffer unitario

buffer_t* buf_unitario = NULL; //buffer unitario
msg_t* messaggio = NULL; //messaggio da inserire nel buffer

//  Funzione che inizializza la suite: inizializza il messaggio che verrà
//  inserito nel buffer.
int test_buffer_unitario_setup(void){
  messaggio = msg_init_string(CONTENUTO_MESSAGGIO);
  return 0;
}

//  Funzione che distrugge la suite: distrugge il messaggio.
int test_buffer_unitario_clean(void){
  msg_destroy_string(messaggio);
  return 0;
}

//  Funzione che ritorna un buffer unitario inizializzato vuoto.
buffer_t* get_buffer_unitario_vuoto(){
  return buffer_init(DIMENSIONE_UNITARIA);
}

//  Funzione che ritorna un buffer unitario inizializzato e riempito.
buffer_t* get_buffer_unitario_pieno(){
  buffer_t* buf_unitario = get_buffer_unitario_vuoto();
  put_bloccante(buf_unitario,messaggio);
  return buf_unitario;
}

//  Funzione usa-getta per la produzione bloccante di un messaggio
//  nel buffer.
void* produci_uno_bloccante(){
  put_bloccante(buf_unitario,messaggio);
  pthread_exit(0);
  return NULL;
}

void* produci_uno_bloccante_inattesa(){
    printf("Sto tendando di produrre ma non c'è spazio...");
    put_bloccante(buf_unitario,messaggio);
    pthread_exit(0);
    return NULL;
}

//  Funzione usa-getta per la produzione non bloccante di un messaggio
//  nel buffer.
void* produci_uno_nonbloccante(){
  pthread_exit(put_non_bloccante(buf_unitario,messaggio));
  return NULL;
}
//  Funzione usa-getta per la consumazione bloccante di un messaggio
//  nel buffer.
void* consuma_uno_bloccante(){
  pthread_exit(get_bloccante(buf_unitario));
  return NULL;
}



//  Funzione usa-getta per la consumazione non bloccante di un messaggio
//  nel buffer.
void* consuma_uno_nonbloccante(){
  pthread_exit(get_non_bloccante(buf_unitario));
  return NULL;
}


//Verifico creazione del messaggio durante l'inizializzazione della suite.
void test_buffer_unitario_creazione_messaggio(void){
  CU_ASSERT_PTR_NOT_NULL(messaggio);
  CU_ASSERT_STRING_EQUAL(messaggio->content,CONTENUTO_MESSAGGIO);
}

//Verifico creazione del buffer unitario vuoto.
void test_buffer_unitario_creazione(void){
  buf_unitario = get_buffer_unitario_vuoto();
  CU_ASSERT_PTR_NOT_NULL(buf_unitario);
  CU_ASSERT_PTR_NULL(buf_unitario->messaggi[0]);
  CU_ASSERT_EQUAL(buf_unitario->maxsize,DIMENSIONE_UNITARIA);
  buffer_destroy(buf_unitario);
}



//INIZIO TEST CONSUMATORE E PRODUTTORE ISOLATI


//Verifico produzione bloccante di un solo messaggio in un buffer vuoto.
void test_buffer_unitario_produci_bloccante_un_messaggio(void){
  buf_unitario = get_buffer_unitario_vuoto();
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_PTR_NOT_NULL(buf_unitario->messaggi[0]);
  CU_ASSERT_STRING_EQUAL(buf_unitario->messaggi[0]->content,CONTENUTO_MESSAGGIO);
  buffer_destroy(buf_unitario);
}

//Verifico produzione non bloccante di un solo messaggio in un buffer vuoto.
void test_buffer_unitario_produci_nonbloccante_un_messaggio(void){
  buf_unitario = get_buffer_unitario_vuoto();
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_nonbloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_PTR_NOT_NULL(buf_unitario->messaggi[0]);
  CU_ASSERT_STRING_EQUAL(buf_unitario->messaggi[0]->content,CONTENUTO_MESSAGGIO);
  buffer_destroy(buf_unitario);
}

//Verifica aumento dell'indice inserimento dopo produzione bloccante messaggio.
void test_buffer_unitario_produci_bloccante_un_messaggio_aumento_indice_uno(void){
  buf_unitario = get_buffer_unitario_vuoto();
  CU_ASSERT_EQUAL(buf_unitario->indice_inserimento,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buf_unitario->indice_inserimento,0);//buffer di dimensione 1
  buffer_destroy(buf_unitario);
}

//Verifica aumento dell'indice inserimento dopo produzione non bloccante messaggio
void test_buffer_unitario_produci_nonbloccante_un_messaggio_aumento_indice_uno(void){
  buf_unitario = get_buffer_unitario_vuoto();
  CU_ASSERT_EQUAL(buf_unitario->indice_inserimento,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_nonbloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buf_unitario->indice_inserimento,0);//buffer di dimensione 1
  buffer_destroy(buf_unitario);
}

//Verifica aumento del numero dei messaggi dopo produzione bloccante messaggio.
void test_buffer_unitario_produci_bloccante_un_messaggio_aumento_uno_size(void){
  buf_unitario = get_buffer_unitario_vuoto();
  CU_ASSERT_EQUAL(buf_unitario->size,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buf_unitario->size,1);
  buffer_destroy(buf_unitario);
}

//Verifica aumento del numero dei messaggi dopo produzione non bloccante messaggio
void test_buffer_unitario_produci_nonbloccante_un_messaggio_aumento_uno_size(void){
  buf_unitario = get_buffer_unitario_vuoto();
  CU_ASSERT_EQUAL(buf_unitario->size,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_nonbloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buf_unitario->size,1);
  buffer_destroy(buf_unitario);
}

//Produzione bloccante in un buffer pieno verifica l'attesa e poi esci.
void test_buffer_unitario_pieno_produci_bloccante_attesa(void){
    buf_unitario = get_buffer_unitario_pieno();
    msg_t *messaggio_inserito = NULL;
    pthread_t thread = NULL;
    pthread_create(&thread, NULL,&produci_uno_bloccante_inattesa,NULL);
    sleep(1);
    pthread_cancel(thread);
    buffer_destroy(buf_unitario);
}


//Produzione non bloccante in un buffer pieno verifica il ritorno di buffer error.
void test_buffer_unitario_pieno_produci_nonbloccante_buffer_error(void){
  buf_unitario = get_buffer_unitario_pieno();
  msg_t *messaggio_inserito = NULL;
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_nonbloccante,NULL);
  pthread_join(thread,&messaggio_inserito);
  CU_ASSERT_PTR_EQUAL(messaggio_inserito,BUFFER_ERROR);
  buffer_destroy(buf_unitario);
}

//Produzione non bloccante in un buffer pieno verifica size rimane uguale.
void test_buffer_unitario_pieno_produci_nonbloccante_size_uguale(void){
    buf_unitario = get_buffer_unitario_pieno();
    CU_ASSERT_PTR_EQUAL(buf_unitario->size,1);
    msg_t *messaggio_inserito = NULL;
    pthread_t thread = NULL;
    pthread_create(&thread, NULL,&produci_uno_nonbloccante,NULL);
    pthread_join(thread,&messaggio_inserito);
    CU_ASSERT_PTR_EQUAL(buf_unitario->size,1);
    buffer_destroy(buf_unitario);
}

//Consumazione bloccante di un solo messaggio da un buffer pieno.
void test_buffer_unitario_pieno_consuma_bloccante_un_messaggio(void){
  buf_unitario = get_buffer_unitario_pieno();
  msg_t *messaggio_consumato = NULL;
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&consuma_uno_bloccante,NULL);
  pthread_join(thread,&messaggio_consumato);
  CU_ASSERT_PTR_NOT_NULL(messaggio_consumato);
  CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,CONTENUTO_MESSAGGIO);//esattamente quel messaggio
  CU_ASSERT_PTR_NULL(buf_unitario->messaggi[0]);//messaggio eliminato dal buffer
  buffer_destroy(buf_unitario);
}

//Consumazione non bloccante di un solo messaggio da un buffer pieno
void test_buffer_unitario_pieno_consuma_nonbloccante_un_messaggio(void){
  buf_unitario = get_buffer_unitario_pieno();
  msg_t *messaggio_consumato = NULL;
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&consuma_uno_nonbloccante,NULL);
  pthread_join(thread,&messaggio_consumato);
  CU_ASSERT_PTR_NOT_NULL(messaggio_consumato);
  CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,CONTENUTO_MESSAGGIO);
  CU_ASSERT_PTR_NULL(buf_unitario->messaggi[0]);//messaggio eliminato dal buffer
  buffer_destroy(buf_unitario);
}

//Verifica decremento size dopo consumazione bloccante di un messaggio.
void test_buffer_unitario_pieno_consuma_bloccante_un_messaggio_decremento_uno_size(void){
  buf_unitario = get_buffer_unitario_pieno();
  int size = buf_unitario->size;//salvo size prima di consumare
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&consuma_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buf_unitario->size,size-1);//diminuita di uno
  buffer_destroy(buf_unitario);
}

//Verifica decremento size dopo consumazione non bloccante di un messaggio.
void test_buffer_unitario_pieno_consuma_nonbloccante_un_messaggio_decremento_uno_size(void){
    buf_unitario = get_buffer_unitario_pieno();
    int size = buf_unitario->size;//salvo size prima di consumare
    pthread_t thread = NULL;
    pthread_create(&thread, NULL,&consuma_uno_nonbloccante,NULL);
    pthread_join(thread,NULL);
    CU_ASSERT_EQUAL(buf_unitario->size,size-1);//diminuita di uno
    buffer_destroy(buf_unitario);
}

//Verifica aumento circolare dell'indice di estrazione dopo consumazione bloccante.
void test_buffer_unitario_pieno_consuma_bloccante_un_messaggio_aumento_indice_estrazione(void){
  buf_unitario = get_buffer_unitario_pieno();
  CU_ASSERT_EQUAL(buf_unitario->indice_estrazione,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&consuma_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buf_unitario->indice_estrazione,0);//dimensione del buffer grande uno
  buffer_destroy(buf_unitario);
}

//Verifica aumento circolare dell'indice di estrazione dopo consumazione non bloccante.
void test_buffer_unitario_pieno_consuma_nonbloccante_un_messaggio_aumento_indice_estrazione(void){
    buf_unitario = get_buffer_unitario_pieno();
    CU_ASSERT_EQUAL(buf_unitario->indice_estrazione,0);
    pthread_t thread = NULL;
    pthread_create(&thread, NULL,&consuma_uno_nonbloccante,NULL);
    pthread_join(thread,NULL);
    CU_ASSERT_EQUAL(buf_unitario->indice_estrazione,0);
    buffer_destroy(buf_unitario);
}

//Consumazione non bloccante in un buffer vuoto verifica il ritorno di buffer error.
void test_buffer_unitario_vuoto_consuma_nonbloccante_buffer_error(void){
  buf_unitario = get_buffer_unitario_vuoto();
  //metto un messaggio qualsiasi che verrà sostituito da buffererror
  msg_t *messaggio_consumato = msg_init_string("daSostituireConBufferErrorSeTuttoFunziona");
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&consuma_uno_nonbloccante,NULL);
  pthread_join(thread,&messaggio_consumato);
  CU_ASSERT_PTR_NULL(messaggio_consumato);
  CU_ASSERT_EQUAL(messaggio_consumato,BUFFER_ERROR);//sostituito messaggio con BUFFER_ERROR
  buffer_destroy(buf_unitario);
}

//Consumazione non bloccante in un buffer vuoto verifica size rimane 0.
void test_buffer_unitario_vuoto_consuma_nonbloccante_size_rimane_uguale(void){
    buf_unitario = get_buffer_unitario_vuoto();
    CU_ASSERT_EQUAL(buf_unitario->size,0);
    pthread_t thread = NULL;
    pthread_create(&thread, NULL,&consuma_uno_nonbloccante,NULL);
    pthread_join(thread,NULL);
    CU_ASSERT_EQUAL(buf_unitario->size,0);
    buffer_destroy(buf_unitario);
}


//FINE TEST CONSUMATORE E PRODUTTORE ISOLATI


//INIZIO TEST CONSUMATORE E PRODUTTORE INSIEME


//Consumazione bloccante e produzione bloccante concorrente di un messaggio
//in un buffer vuoto, prima il produttore, estrazione messaggio.
void test_buffer_unitario_vuoto_produci_consuma_bloccante_messaggio_estratto(void){
    buf_unitario = get_buffer_unitario_vuoto();
    msg_t *messaggio_consumato = NULL;
    pthread_t produttore = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&produttore, NULL,&produci_uno_bloccante,NULL);
    sleep(1);
    pthread_create(&consumatore, NULL,&consuma_uno_bloccante,NULL);
    pthread_join(produttore,NULL);
    pthread_join(consumatore,&messaggio_consumato);
    CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,CONTENUTO_MESSAGGIO);
    buffer_destroy(buf_unitario);
}

//Consumazione bloccante e produzione bloccante concorrente di un messaggio
//in un buffer vuoto, prima il produttore. Verifica del buffer vuoto.
void test_buffer_unitario_vuoto_produci_consuma_bloccante_buffer_svuotato(void){
    buf_unitario = get_buffer_unitario_vuoto();
    pthread_t produttore = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&produttore, NULL,&produci_uno_bloccante,NULL);
    sleep(1);
    pthread_create(&consumatore, NULL,&consuma_uno_bloccante,NULL);
    pthread_join(produttore,NULL);
    pthread_join(consumatore,NULL);
    CU_ASSERT_EQUAL(buf_unitario->messaggi[0],NULL);//messaggio consumato
    buffer_destroy(buf_unitario);
}



//Consumazione non bloccante e produzione non bloccante concorrente di un messaggio
//in un buffer vuoto, prima il produttore.
void test_buffer_unitario_vuoto_produci_consuma_nonbloccante_messaggio_estratto(void){
    buf_unitario = get_buffer_unitario_vuoto();
    msg_t *messaggio_consumato = NULL;
    pthread_t produttore = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&produttore, NULL,&produci_uno_nonbloccante,NULL);
    sleep(1);
    pthread_create(&consumatore, NULL,&consuma_uno_nonbloccante,NULL);
    pthread_join(produttore,NULL);
    pthread_join(consumatore,&messaggio_consumato);
    CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,CONTENUTO_MESSAGGIO);
    buffer_destroy(buf_unitario);
}

//Consumazione non bloccante e produzione non bloccante concorrente di un messaggio
//in un buffer vuoto, prima il produttore. Verifica del buffer vuoto.
void test_buffer_unitario_vuoto_produci_consuma_nonbloccante_buffer_svuotato(void){
    buf_unitario = get_buffer_unitario_vuoto();
    pthread_t produttore = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&produttore, NULL,&produci_uno_nonbloccante,NULL);
    sleep(1);
    pthread_create(&consumatore, NULL,&consuma_uno_nonbloccante,NULL);
    pthread_join(produttore,NULL);
    pthread_join(consumatore,NULL);
    CU_ASSERT_EQUAL(buf_unitario->messaggi[0],NULL);//messaggio consumato
    buffer_destroy(buf_unitario);
}


//Consumazione non bloccante e produzione non bloccante concorrente di un messaggio
//in un buffer pieno, prima il produttore. Verifica di errore sul messaggio inserito
//e di corretta consumazione
void test_buffer_unitario_pieno_produci_consuma_nonbloccante_fail_produzione_ok_consumazione(void){
    buf_unitario = get_buffer_unitario_pieno();
    msg_t *messaggio_consumato = NULL;
    msg_t *messaggio_inserito = NULL;
    pthread_t produttore = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&produttore, NULL,&produci_uno_nonbloccante,NULL);
    sleep(1);
    pthread_create(&consumatore, NULL,&consuma_uno_nonbloccante,NULL);
    pthread_join(produttore,&messaggio_inserito);
    pthread_join(consumatore,&messaggio_consumato);
    //uso il consumatore con la sleep così do il tempo al produttore
    //di produrre e quindi fallire ritornando BUFFER_ERROR
    CU_ASSERT_EQUAL(messaggio_inserito,BUFFER_ERROR);
    CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,CONTENUTO_MESSAGGIO);
    buffer_destroy(buf_unitario);
}


//Consumazione bloccante e produzione bloccante concorrente di un messaggio
//in un buffer vuoto, prima il consumatore. Verifica estrazione messaggio.
void test_buffer_unitario_vuoto_consuma_produci_bloccante_messaggio_estratto(void){
    buf_unitario = get_buffer_unitario_vuoto();
    msg_t *messaggio_consumato = NULL;
    pthread_t produttore = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&consumatore, NULL,&consuma_uno_bloccante,NULL);
    sleep(1);
    pthread_create(&produttore, NULL,&produci_uno_bloccante,NULL);
    pthread_join(consumatore,&messaggio_consumato);
    pthread_join(produttore,NULL);
    CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,CONTENUTO_MESSAGGIO);
    CU_ASSERT_EQUAL(buf_unitario->size, 0);
    buffer_destroy(buf_unitario);
}

//Consumazione bloccante e produzione bloccante concorrente di un messaggio
//in un buffer vuoto, prima il consumatore. Verifica buffer svuotato.
void test_buffer_unitario_vuoto_consuma_produci_bloccante_buffer_svuotato(void){
    buf_unitario = get_buffer_unitario_vuoto();
    pthread_t produttore = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&consumatore, NULL,&consuma_uno_bloccante,NULL);
    sleep(1);
    pthread_create(&produttore, NULL,&produci_uno_bloccante,NULL);
    pthread_join(consumatore,NULL);
    pthread_join(produttore,NULL);
    CU_ASSERT_PTR_NULL(buf_unitario->messaggi[0]);
    buffer_destroy(buf_unitario);
}

//Consumazione bloccante e produzione bloccante concorrente di un messaggio
//in un buffer vuoto, lanciati insieme. Verifica estrazione messaggio.
void test_buffer_unitario_vuoto_lanciati_insieme_consuma_produci_bloccante_messaggio_estratto(void){
    buf_unitario = get_buffer_unitario_vuoto();
    msg_t *messaggio_consumato = NULL;
    pthread_t produttore = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&consumatore, NULL,&consuma_uno_bloccante,NULL);
    pthread_create(&produttore, NULL,&produci_uno_bloccante,NULL);
    pthread_join(consumatore,&messaggio_consumato);
    pthread_join(produttore,NULL);
    CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,CONTENUTO_MESSAGGIO);
    buffer_destroy(buf_unitario);
}

//Consumazione non bloccante e produzione non bloccante concorrente di un messaggio
//in un buffer vuoto, prima la consumazione. Verifica BUFFER_ERROR e corretto inserimento.
void test_buffer_unitario_vuoto_consuma_produci_nonbloccante_fail_consumazione_ok_produzione(void){
    buf_unitario = get_buffer_unitario_vuoto();
    msg_t *messaggio_consumato = NULL;
    pthread_t produttore = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&consumatore, NULL,&consuma_uno_nonbloccante,NULL);
    sleep(1);
    pthread_create(&produttore, NULL,&produci_uno_nonbloccante,NULL);
    pthread_join(consumatore,&messaggio_consumato);
    pthread_join(produttore,NULL);
    CU_ASSERT_EQUAL(messaggio_consumato,BUFFER_ERROR);
    CU_ASSERT_STRING_EQUAL(buf_unitario->messaggi[0]->content,CONTENUTO_MESSAGGIO);//messaggio effettivamente inserito
    buffer_destroy(buf_unitario);
}

//Consumazione non bloccante e produzione non bloccante concorrente di un messaggio
//in un buffer pieno, prima il consumatore. Verifica estrazione messaggio e messaggio inserito.
void test_buffer_unitario_pieno_consuma_produci_nonbloccante_ok_consumazione_ok_produzione(void){
    buf_unitario = get_buffer_unitario_pieno();
    msg_t *messaggio_consumato = NULL;
    pthread_t produttore = NULL;
    pthread_t consumatore = NULL;
    pthread_create(&consumatore, NULL,&consuma_uno_nonbloccante,NULL);
    sleep(1);
    pthread_create(&produttore, NULL,&produci_uno_nonbloccante,NULL);
    pthread_join(consumatore,&messaggio_consumato);
    pthread_join(produttore,NULL);
    CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,CONTENUTO_MESSAGGIO);
    CU_ASSERT_STRING_EQUAL(buf_unitario->messaggi[0]->content,CONTENUTO_MESSAGGIO);
    buffer_destroy(buf_unitario);
}

//Produzione bloccante concorrente di due messaggi in un buffer unitario vuoto.
//La seconda produzione andrà in attesa passiva.
void test_buffer_unitario_vuoto_due_produci_bloccante_in_attesa_un_messaggio_nel_buffer(void){
    buf_unitario = get_buffer_unitario_vuoto();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_create(&produttore, NULL,&produci_uno_bloccante,NULL);
    pthread_create(&produttore2, NULL,&produci_uno_bloccante,NULL);
    sleep(2);
    pthread_cancel(produttore);
    pthread_cancel(produttore2);
    CU_ASSERT_EQUAL(buf_unitario->size,1);
    buffer_destroy(buf_unitario);
}

//Produzione bloccante concorrente di tre messaggi in un buffer unitario vuoto.
//La seconda produzione e la terza andranno in attesa passiva.
void test_buffer_unitario_vuoto_tre_produci_bloccante_in_attesa_un_messaggio_nel_buffer(void){
    buf_unitario = get_buffer_unitario_vuoto();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    pthread_t produttore3 = NULL;
    pthread_create(&produttore, NULL,&produci_uno_bloccante,NULL);
    pthread_create(&produttore2, NULL,&produci_uno_bloccante,NULL);
    pthread_create(&produttore3, NULL,&produci_uno_bloccante,NULL);
    sleep(2);
    pthread_cancel(produttore);
    pthread_cancel(produttore2);
    pthread_cancel(produttore3);
    CU_ASSERT_EQUAL(buf_unitario->size,1);
    buffer_destroy(buf_unitario);
}

//Produzione non bloccante concorrente di due messaggi in un buffer unitario vuoto.
//La seconda produzione ritornerà un errore BUFFER_ERROR
void test_buffer_unitario_vuoto_due_produci_nonbloccante(void){
    buf_unitario = get_buffer_unitario_vuoto();
    pthread_t produttore = NULL;
    pthread_t produttore2 = NULL;
    msg_t * messaggio_inserito = NULL;
    pthread_create(&produttore, NULL,&produci_uno_nonbloccante,NULL);
    sleep(1);
    pthread_create(&produttore2, NULL,&produci_uno_nonbloccante,NULL);
    pthread_join(produttore,NULL);
    pthread_join(produttore2,&messaggio_inserito);
    CU_ASSERT_EQUAL(buf_unitario->size,1);
    CU_ASSERT_EQUAL(messaggio_inserito,BUFFER_ERROR);
    buffer_destroy(buf_unitario);
}

//Consumazione bloccante concorrente di due messaggi in un buffer unitario pieno.
//La seconda consumazione andrà in attesa passiva.
void test_buffer_unitario_pieno_due_consuma_bloccante_in_attesa_buffer_svuotato(void){
    buf_unitario = get_buffer_unitario_pieno();
    pthread_t consumatore = NULL;
    pthread_t consumatore2 = NULL;
    pthread_create(&consumatore, NULL,&consuma_uno_bloccante,NULL);
    pthread_create(&consumatore2, NULL,&consuma_uno_bloccante,NULL);
    sleep(2);
    printf("I consumatori sono in attesa di messaggi...");
    pthread_cancel(consumatore);
    pthread_cancel(consumatore2);
    CU_ASSERT_EQUAL(buf_unitario->size,0);
    buffer_destroy(buf_unitario);
}

//Consumazione non bloccante concorrente di due messaggi in un buffer unitario pieno.
//La seconda consumazione riporterà BUFFER_ERROR
void test_buffer_unitario_pieno_due_consuma_nonbloccante_buffer_svuotato_buffer_error(void){
    buf_unitario = get_buffer_unitario_pieno();
    pthread_t consumatore = NULL;
    pthread_t consumatore2 = NULL;
    msg_t* messaggio_consumato = NULL;
    pthread_create(&consumatore, NULL,&consuma_uno_nonbloccante,NULL);
    sleep(1);
    pthread_create(&consumatore2, NULL,&consuma_uno_nonbloccante,NULL);
    pthread_join(consumatore,NULL);
    pthread_join(consumatore2,&messaggio_consumato);
    CU_ASSERT_EQUAL(buf_unitario->size,0);
    CU_ASSERT_EQUAL(messaggio_consumato,BUFFER_ERROR);
    buffer_destroy(buf_unitario);
}

//Consumazioni e produzioni concorrenti di molteplici messaggi in un buffer unitario vuoto.
//Versione blocante nella seq: prod-cons-prod-cons.
void test_buffer_unitario_vuoto_produci_consuma_produci_consuma_bloccante_buffer_svuotato(void){
    buf_unitario = get_buffer_unitario_vuoto();
    pthread_t c, c2, p, p2;
    pthread_create(&p, NULL,&produci_uno_bloccante,NULL);
    pthread_create(&c, NULL,&consuma_uno_bloccante,NULL);
    pthread_create(&p2, NULL,&produci_uno_bloccante,NULL);
    pthread_create(&c2, NULL,&consuma_uno_bloccante,NULL);
    pthread_join(p,NULL);
    pthread_join(p2,NULL);
    pthread_join(c,NULL);
    pthread_join(c2,NULL);
    CU_ASSERT_EQUAL(buf_unitario->size,0);
    buffer_destroy(buf_unitario);
}

//Consumazioni e produzioni concorrenti di molteplici messaggi in un buffer unitario vuoto.
//Versione blocante nella seq: prod-prod-cons-cons.
void test_buffer_unitario_vuoto_produci_produci_consuma_consuma_bloccante_buffer_svuotato(void){
    buf_unitario = get_buffer_unitario_vuoto();
    pthread_t c, c2, p, p2;
    pthread_create(&p, NULL,&produci_uno_bloccante,NULL);
    pthread_create(&p2, NULL,&produci_uno_bloccante,NULL);
    pthread_create(&c, NULL,&consuma_uno_bloccante,NULL);
    pthread_create(&c2, NULL,&consuma_uno_bloccante,NULL);
    pthread_join(p,NULL);
    pthread_join(p2,NULL);
    pthread_join(c,NULL);
    pthread_join(c2,NULL);
    CU_ASSERT_EQUAL(buf_unitario->size,0);
    buffer_destroy(buf_unitario);
}

//Consumazioni e produzioni concorrenti di molteplici messaggi in un buffer unitario vuoto.
//Versione blocante nella seq: cons-cons-prod-prod.
void test_buffer_unitario_vuoto_consuma_consuma_produci_produci_bloccante_buffer_svuotato(void){
    buf_unitario = get_buffer_unitario_vuoto();
    pthread_t c, c2, p, p2;
    pthread_create(&c, NULL,&consuma_uno_bloccante,NULL);
    pthread_create(&c2, NULL,&consuma_uno_bloccante,NULL);
    pthread_create(&p, NULL,&produci_uno_bloccante,NULL);
    pthread_create(&p2, NULL,&produci_uno_bloccante,NULL);
    pthread_join(p,NULL);
    pthread_join(p2,NULL);
    pthread_join(c,NULL);
    pthread_join(c2,NULL);
    CU_ASSERT_EQUAL(buf_unitario->size,0);
    buffer_destroy(buf_unitario);
}

//Consumazioni e produzioni concorrenti di molteplici messaggi in un buffer unitario vuoto.
//Versione blocante nella seq: cons-cons-prod-prod-prod.
void test_buffer_unitario_vuoto_consuma_consuma_produci_produci_produci_bloccante(void){
    buf_unitario = get_buffer_unitario_vuoto();
    pthread_t c, c2, p, p2,p3;
    pthread_create(&c, NULL,&consuma_uno_bloccante,NULL);
    pthread_create(&c2, NULL,&consuma_uno_bloccante,NULL);
    pthread_create(&p, NULL,&produci_uno_bloccante,NULL);
    pthread_create(&p2, NULL,&produci_uno_bloccante,NULL);
    pthread_create(&p3, NULL,&produci_uno_bloccante,NULL);
    pthread_join(p,NULL);
    pthread_join(p2,NULL);
    pthread_join(p3,NULL);
    pthread_join(c,NULL);
    pthread_join(c2,NULL);
    CU_ASSERT_STRING_EQUAL(buf_unitario->messaggi[0]->content,CONTENUTO_MESSAGGIO);
    buffer_destroy(buf_unitario);
}

//Consumazioni e produzioni concorrenti di molteplici messaggi in un buffer unitario vuoto.
//Versione non blocante nella seq: cons-cons-prod-prod-prod.
void test_buffer_unitario_vuoto_consuma_consuma_produci_produci_produci_nonbloccante(void){
    buf_unitario = get_buffer_unitario_vuoto();
    pthread_t c, c2, p, p2,p3;
    msg_t* m1;
    msg_t* m2;
    pthread_create(&c, NULL,&consuma_uno_nonbloccante,NULL);
    pthread_create(&c2, NULL,&consuma_uno_nonbloccante,NULL);
    sleep(1);
    pthread_create(&p, NULL,&produci_uno_nonbloccante,NULL);
    sleep(1);
    CU_ASSERT_EQUAL(buf_unitario->size,1);
    pthread_create(&p2, NULL,&produci_uno_nonbloccante,NULL);
    pthread_create(&p3, NULL,&produci_uno_nonbloccante,NULL);
    pthread_join(p,NULL);
    pthread_join(p2,NULL);
    pthread_join(p3,NULL);
    pthread_join(c,&m1);
    pthread_join(c2,&m2);
    CU_ASSERT_EQUAL(buf_unitario->size,1);
    CU_ASSERT_EQUAL(m2,BUFFER_ERROR);
    CU_ASSERT_EQUAL(m1,BUFFER_ERROR);
    buffer_destroy(buf_unitario);
}



//  Crea la suite e inserisce tutti i test unitari alla suite
int test_buffer_unitario_create(){
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite(
                          "Produzione e consumazione in un buffer unitario",
                          test_buffer_unitario_setup, test_buffer_unitario_clean);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite,
                    "test_buffer_unitario_creazione",
                    test_buffer_unitario_creazione) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_creazione_messaggio",
                    test_buffer_unitario_creazione_messaggio) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_produci_nonbloccante_un_messaggio",
                    test_buffer_unitario_produci_nonbloccante_un_messaggio) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_produci_bloccante_un_messaggio",
                    test_buffer_unitario_produci_bloccante_un_messaggio) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_produci_bloccante_un_messaggio_aumento_indice_uno",
                    test_buffer_unitario_produci_bloccante_un_messaggio_aumento_indice_uno) == NULL  ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_produci_nonbloccante_un_messaggio_aumento_indice_uno",
                    test_buffer_unitario_produci_nonbloccante_un_messaggio_aumento_indice_uno) == NULL  ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_produci_bloccante_un_messaggio_aumento_uno_size",
                    test_buffer_unitario_produci_bloccante_un_messaggio_aumento_uno_size) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_produci_nonbloccante_un_messaggio_aumento_uno_size",
                    test_buffer_unitario_produci_nonbloccante_un_messaggio_aumento_uno_size) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_produci_nonbloccante_buffer_error",
                    test_buffer_unitario_pieno_produci_nonbloccante_buffer_error) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_produci_nonbloccante_size_uguale",
                    test_buffer_unitario_pieno_produci_nonbloccante_size_uguale) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_consuma_bloccante_un_messaggio",
                    test_buffer_unitario_pieno_consuma_bloccante_un_messaggio) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_consuma_nonbloccante_un_messaggio",
                    test_buffer_unitario_pieno_consuma_nonbloccante_un_messaggio) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_consuma_bloccante_un_messaggio_decremento_uno_size",
                    test_buffer_unitario_pieno_consuma_bloccante_un_messaggio_decremento_uno_size) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_consuma_nonbloccante_un_messaggio_decremento_uno_size",
                    test_buffer_unitario_pieno_consuma_nonbloccante_un_messaggio_decremento_uno_size) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_produci_bloccante_attesa",
                    test_buffer_unitario_pieno_produci_bloccante_attesa) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_consuma_bloccante_un_messaggio_aumento_indice_estrazione",
                    test_buffer_unitario_pieno_consuma_bloccante_un_messaggio_aumento_indice_estrazione) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_consuma_nonbloccante_un_messaggio_aumento_indice_estrazione",
                    test_buffer_unitario_pieno_consuma_nonbloccante_un_messaggio_aumento_indice_estrazione) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_consuma_nonbloccante_buffer_error",
                    test_buffer_unitario_vuoto_consuma_nonbloccante_buffer_error) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_consuma_nonbloccante_size_rimane_uguale",
                    test_buffer_unitario_vuoto_consuma_nonbloccante_size_rimane_uguale) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_produci_consuma_bloccante_messaggio_estratto",
                    test_buffer_unitario_vuoto_produci_consuma_bloccante_messaggio_estratto) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_produci_consuma_bloccante_buffer_svuotato",
                    test_buffer_unitario_vuoto_produci_consuma_bloccante_buffer_svuotato) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_produci_consuma_nonbloccante_messaggio_estratto",
                    test_buffer_unitario_vuoto_produci_consuma_nonbloccante_messaggio_estratto) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_produci_consuma_nonbloccante_buffer_svuotato",
                    test_buffer_unitario_vuoto_produci_consuma_nonbloccante_buffer_svuotato) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_produci_consuma_nonbloccante_fail_produzione_ok_consumazione",
                    test_buffer_unitario_pieno_produci_consuma_nonbloccante_fail_produzione_ok_consumazione) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_consuma_produci_bloccante_messaggio_estratto",
                    test_buffer_unitario_vuoto_consuma_produci_bloccante_messaggio_estratto) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_consuma_produci_bloccante_buffer_svuotato",
                    test_buffer_unitario_vuoto_consuma_produci_bloccante_buffer_svuotato) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_lanciati_insieme_consuma_produci_bloccante_messaggio_estratto",
                    test_buffer_unitario_vuoto_lanciati_insieme_consuma_produci_bloccante_messaggio_estratto) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_consuma_produci_nonbloccante_fail_consumazione_ok_produzione",
                    test_buffer_unitario_vuoto_consuma_produci_nonbloccante_fail_consumazione_ok_produzione) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_consuma_produci_nonbloccante_ok_consumazione_ok_produzione",
                    test_buffer_unitario_pieno_consuma_produci_nonbloccante_ok_consumazione_ok_produzione) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_due_produci_bloccante_in_attesa_un_messaggio_nel_buffer",
                    test_buffer_unitario_vuoto_due_produci_bloccante_in_attesa_un_messaggio_nel_buffer) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_tre_produci_bloccante_in_attesa_un_messaggio_nel_buffer",
                    test_buffer_unitario_vuoto_tre_produci_bloccante_in_attesa_un_messaggio_nel_buffer) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_due_produci_nonbloccante",
                    test_buffer_unitario_vuoto_due_produci_nonbloccante) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_due_consuma_bloccante_in_attesa_buffer_svuotato",
                    test_buffer_unitario_pieno_due_consuma_bloccante_in_attesa_buffer_svuotato) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_pieno_due_consuma_nonbloccante_buffer_svuotato_buffer_error",
                    test_buffer_unitario_pieno_due_consuma_nonbloccante_buffer_svuotato_buffer_error) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_produci_consuma_produci_consuma_bloccante_buffer_svuotato",
                    test_buffer_unitario_vuoto_produci_consuma_produci_consuma_bloccante_buffer_svuotato) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_produci_produci_consuma_consuma_bloccante_buffer_svuotato",
                    test_buffer_unitario_vuoto_produci_produci_consuma_consuma_bloccante_buffer_svuotato) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_consuma_consuma_produci_produci_bloccante_buffer_svuotato",
                    test_buffer_unitario_vuoto_consuma_consuma_produci_produci_bloccante_buffer_svuotato) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_consuma_consuma_produci_produci_produci_bloccante",
                    test_buffer_unitario_vuoto_consuma_consuma_produci_produci_produci_bloccante) == NULL ||
        CU_add_test(pSuite,
                    "test_buffer_unitario_vuoto_consuma_consuma_produci_produci_produci_nonbloccante",
                    test_buffer_unitario_vuoto_consuma_consuma_produci_produci_produci_nonbloccante) == NULL

        )


    {
        CU_cleanup_registry();
        return CU_get_error();
    }
}
