#include "test_buffer_unitario.h"



int test_buffer_unitario_create(){
  CU_pSuite pSuite = NULL;
  pSuite = CU_add_suite(
    "Produzione e consumazione in un buffer unitario",
     test_buffer_unitario_setup, test_buffer_unitario_clean);
  if (NULL == pSuite) {
     CU_cleanup_registry();
     return CU_get_error();
  }
  /* add the tests to the suite */
  if (CU_add_test(pSuite,
        "Verifica creazione del buffer",
        test_buffer_unitario_creazione) == NULL ||
      CU_add_test(pSuite,
        "Verifica creazione di un messaggio",
        test_buffer_unitario_creazione_messaggio) == NULL ||
      CU_add_test(pSuite,
        "Produzione non bloccante di un solo messaggio in un buffer vuoto",
        test_buffer_unitario_messaggio_prodotto_non_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Produzione bloccante di un solo messaggio in un buffer vuoto",
        test_buffer_unitario_messaggio_prodotto_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Verifica aumento dell'indice inserimento dopo produzione bloccante messaggio",
        test_buffer_unitario_aumento_indice_bloccante) == NULL  ||
      CU_add_test(pSuite,
        "Verifica aumento dell'indice inserimento dopo produzione non bloccante messaggio",
        test_buffer_unitario_aumento_indice_non_bloccante) == NULL  ||
      CU_add_test(pSuite,
        "Verifica aumento del numero dei messaggi dopo produzione bloccante messaggio",
        test_buffer_unitario_aumento_numero_messaggi_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Verifica aumento del numero dei messaggi dopo produzione non bloccante messaggio",
        test_buffer_unitario_aumento_numero_messaggi_non_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Produzione non bloccante in un buffer pieno",
        test_buffer_unitario_pieno_produci_non_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Consumazione bloccante di un solo messaggio da un buffer pieno",
        test_buffer_unitario_pieno_consuma_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Consumazione non bloccante di un solo messaggio da un buffer pieno",
        test_buffer_unitario_pieno_consuma_non_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Verifica decremento size dopo consumazione di un messaggio",
        test_buffer_unitario_pieno_consuma_bloccante_decremento_size) == NULL ||
      CU_add_test(pSuite,
        "Verifica aumento circolare indice dopo consumazione di un messaggio",
        test_buffer_unitario_pieno_consuma_bloccante_decremento_indice) == NULL ||
      CU_add_test(pSuite,
        "Verifica aumento circolare indice dopo consumazione di un messaggio",
        test_buffer_unitario_vuoto_consuma_non_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Consumazione bloccante e produzione bloccante concorrente di un messaggio in un buffer vuoto, prima il produttore",
        test_buffer_unitario_vuoto_produci_consuma_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Consumazione non bloccante e produzione non bloccante concorrente di un messaggio in un buffer vuoto, prima il produttore",
        test_buffer_unitario_vuoto_produci_consuma_non_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Consumazione non bloccante e produzione non bloccante concorrente di un messaggio in un buffer pieno, prima il produttore",
        test_buffer_unitario_pieno_produci_consuma_non_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Consumazione bloccante e produzione bloccante concorrente di un messaggio in un buffer vuoto, prima il consumatore",
        test_buffer_unitario_vuoto_consuma_produci_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Consumazione non bloccante e produzione non bloccante concorrente di un messaggio in un buffer vuoto, prima il consumatore",
        test_buffer_unitario_vuoto_consuma_produci_non_bloccante) == NULL ||
      CU_add_test(pSuite,
        "Consumazione non bloccante e produzione non bloccante concorrente di un messaggio in un buffer pieno, prima il consumatore",
        test_buffer_unitario_pieno_consuma_produci_non_bloccante) == NULL
      )

  {
     CU_cleanup_registry();
     return CU_get_error();
  }
}

/* The suite initialization function.*/
int test_buffer_unitario_setup(void){
  messaggio = msg_init_string(UNO);
  return 0;
}

/* The suite destroy function.*/
int test_buffer_unitario_clean(void){
  msg_destroy_string(messaggio);
  return 0;
}

buffer_t* get_buffer_unitario_vuoto(){
  return buffer_init(DIMENSIONE_UNITARIA);
}

buffer_t* get_buffer_unitario_pieno(){
  buffer_t* buffer = get_buffer_unitario_vuoto();
  put_bloccante(buffer,messaggio);
  return buffer;
}

void* produci_uno_bloccante(){
  put_bloccante(buffer,messaggio);
  pthread_exit(0);
  return NULL;
}

void* produci_uno_non_bloccante(){
  pthread_exit(put_non_bloccante(buffer,messaggio));
  return NULL;
}

void* consuma_uno_bloccante(){
  pthread_exit(get_bloccante(buffer));
  return NULL;
}

void* consuma_uno_non_bloccante(){
  pthread_exit(get_non_bloccante(buffer));
  return NULL;
}

void test_buffer_unitario_creazione_messaggio(void){
  CU_ASSERT_PTR_NOT_NULL(messaggio);
  CU_ASSERT_STRING_EQUAL(messaggio->content,UNO);
}

void test_buffer_unitario_creazione(void){
  buffer = get_buffer_unitario_vuoto();
  CU_ASSERT_PTR_NOT_NULL(buffer);
  CU_ASSERT_EQUAL(buffer->maxsize,DIMENSIONE_UNITARIA);
  buffer_destroy(buffer);
}

void test_buffer_unitario_messaggio_prodotto_bloccante(void){
  buffer = get_buffer_unitario_vuoto();
  CU_ASSERT_PTR_NULL(buffer->messaggi[0]);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_PTR_NOT_NULL(buffer->messaggi[0]);
  CU_ASSERT_STRING_EQUAL(buffer->messaggi[0]->content,UNO);
  buffer_destroy(buffer);
}

void test_buffer_unitario_messaggio_prodotto_non_bloccante(void){
  buffer = get_buffer_unitario_vuoto();
  CU_ASSERT_PTR_NULL(buffer->messaggi[0]);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_PTR_NOT_NULL(buffer->messaggi[0]);
  CU_ASSERT_STRING_EQUAL(buffer->messaggi[0]->content,UNO);
  buffer_destroy(buffer);
}


void test_buffer_unitario_aumento_indice_bloccante(void){
  buffer = get_buffer_unitario_vuoto();
  CU_ASSERT_EQUAL(buffer->indice_inserimento,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buffer->indice_inserimento,0);
  buffer_destroy(buffer);
}

void test_buffer_unitario_aumento_indice_non_bloccante(void){
  buffer = get_buffer_unitario_vuoto();
  CU_ASSERT_EQUAL(buffer->size,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buffer->size,1);
  buffer_destroy(buffer);
}

void test_buffer_unitario_aumento_numero_messaggi_bloccante(void){
  buffer = get_buffer_unitario_vuoto();
  CU_ASSERT_EQUAL(buffer->size,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buffer->size,1);
  buffer_destroy(buffer);
}

void test_buffer_unitario_aumento_numero_messaggi_non_bloccante(void){
  buffer = get_buffer_unitario_vuoto();
  CU_ASSERT_EQUAL(buffer->size,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buffer->size,1);
  buffer_destroy(buffer);
}



void test_buffer_unitario_pieno_produci_non_bloccante(void){
  buffer = get_buffer_unitario_pieno();
  msg_t *messaggioInserito = NULL;
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(thread,&messaggioInserito);
  CU_ASSERT_PTR_NULL(messaggioInserito);
  buffer_destroy(buffer);
}

void test_buffer_unitario_pieno_consuma_bloccante(void){
  buffer = get_buffer_unitario_pieno();
  msg_t *messaggioConsumato = NULL;
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&consuma_uno_bloccante,NULL);
  pthread_join(thread,&messaggioConsumato);
  CU_ASSERT_PTR_NOT_NULL(messaggioConsumato);
  CU_ASSERT_EQUAL(messaggioConsumato,messaggio);
}

void test_buffer_unitario_pieno_consuma_non_bloccante(void){
  buffer = get_buffer_unitario_pieno();
  msg_t *messaggioConsumato = NULL;
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&consuma_uno_non_bloccante,NULL);
  pthread_join(thread,&messaggioConsumato);
  CU_ASSERT_PTR_NOT_NULL(messaggioConsumato);
  CU_ASSERT_EQUAL(messaggioConsumato,messaggio);
}

void test_buffer_unitario_pieno_consuma_bloccante_decremento_size(void){
  buffer = get_buffer_unitario_pieno();
  int size = buffer->size;
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&consuma_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buffer->size,size-1);
}

void test_buffer_unitario_pieno_consuma_bloccante_decremento_indice(void){
  buffer = get_buffer_unitario_pieno();
  CU_ASSERT_EQUAL(0,buffer->indice_estrazione);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&consuma_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(0,buffer->indice_estrazione);
}


void test_buffer_unitario_vuoto_consuma_non_bloccante(void){
  buffer = get_buffer_unitario_vuoto();
  msg_t *messaggioConsumato = messaggio;
  CU_ASSERT_PTR_NOT_NULL(messaggioConsumato);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&consuma_uno_non_bloccante,NULL);
  pthread_join(thread,&messaggioConsumato);
  CU_ASSERT_PTR_NULL(messaggioConsumato);
  CU_ASSERT_EQUAL(messaggioConsumato,BUFFER_ERROR);
}

void test_buffer_unitario_vuoto_produci_consuma_bloccante(void){
  buffer = get_buffer_unitario_vuoto();
  msg_t *messaggioConsumato = NULL;
  pthread_t produttore = NULL;
  pthread_t consumatore = NULL;
  pthread_create(&produttore, NULL,&produci_uno_bloccante,NULL);
  pthread_create(&consumatore, NULL,&consuma_uno_bloccante,NULL);
  pthread_join(produttore,NULL);
  pthread_join(consumatore,&messaggioConsumato);
  CU_ASSERT_PTR_NOT_NULL(messaggioConsumato);
  CU_ASSERT_EQUAL(messaggioConsumato,messaggio);
}

//NOTE attenzione alla sequenza di interleaving produci-consuma non bloccanti
void test_buffer_unitario_vuoto_produci_consuma_non_bloccante(void){
  buffer = get_buffer_unitario_vuoto();
  msg_t *messaggioConsumato = NULL;
  pthread_t produttore = NULL;
  pthread_t consumatore = NULL;
  pthread_create(&produttore, NULL,&produci_uno_non_bloccante,NULL);
  pthread_create(&consumatore, NULL,&consuma_uno_non_bloccante,NULL);
  pthread_join(produttore,NULL);
  pthread_join(consumatore,&messaggioConsumato);
  CU_ASSERT_PTR_NULL(messaggioConsumato);
  CU_ASSERT_EQUAL(messaggioConsumato,BUFFER_ERROR);
}

void test_buffer_unitario_pieno_produci_consuma_non_bloccante(void){
  buffer = get_buffer_unitario_pieno();
  msg_t *messaggioConsumato = NULL;
  msg_t *messaggioInserito = NULL;
  pthread_t produttore = NULL;
  pthread_t consumatore = NULL;
  pthread_create(&produttore, NULL,&produci_uno_non_bloccante,NULL);
  pthread_create(&consumatore, NULL,&consuma_uno_non_bloccante,NULL);
  pthread_join(produttore,messaggioInserito);
  pthread_join(consumatore,&messaggioConsumato);
  CU_ASSERT_EQUAL(messaggioInserito,BUFFER_ERROR);
  CU_ASSERT_EQUAL(messaggioConsumato,messaggio);
}

void test_buffer_unitario_vuoto_consuma_produci_bloccante(void){
  buffer = get_buffer_unitario_vuoto();
  msg_t *messaggioConsumato = NULL;
  pthread_t produttore = NULL;
  pthread_t consumatore = NULL;
  pthread_create(&consumatore, NULL,&consuma_uno_bloccante,NULL);
  pthread_create(&produttore, NULL,&produci_uno_bloccante,NULL);
  pthread_join(consumatore,&messaggioConsumato);
  pthread_join(produttore,NULL);
  CU_ASSERT_PTR_NOT_NULL(messaggioConsumato);
  CU_ASSERT_EQUAL(messaggioConsumato,messaggio);
}

//NOTE attenzione alla sequenza di interleaving consuma-produci non bloccanti
void test_buffer_unitario_vuoto_consuma_produci_non_bloccante(void){
  buffer = get_buffer_unitario_vuoto();
  msg_t *messaggioConsumato = NULL;
  pthread_t produttore = NULL;
  pthread_t consumatore = NULL;
  pthread_create(&consumatore, NULL,&consuma_uno_non_bloccante,NULL);
  pthread_create(&produttore, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(consumatore,&messaggioConsumato);
  pthread_join(produttore,NULL);
  CU_ASSERT_PTR_NOT_NULL(messaggioConsumato);
  CU_ASSERT_EQUAL(messaggioConsumato,messaggio);
}

void test_buffer_unitario_pieno_consuma_produci_non_bloccante(void){
  buffer = get_buffer_unitario_pieno();
  msg_t *messaggioConsumato = NULL;
  msg_t *messaggioInserito = NULL;
  pthread_t produttore = NULL;
  pthread_t consumatore = NULL;
  pthread_create(&consumatore, NULL,&consuma_uno_non_bloccante,NULL);
  pthread_create(&produttore, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(consumatore,&messaggioConsumato);
  pthread_join(produttore,messaggioInserito);
  CU_ASSERT_EQUAL(messaggioInserito,BUFFER_ERROR);
  CU_ASSERT_EQUAL(messaggioConsumato,messaggio);
}

void test_buffer_unitario_vuoto_produci_due_messaggi(void){
  buffer = get_buffer_unitario_vuoto();
  msg_t *messaggioConsumato = NULL;
  msg_t *messaggioInserito = NULL;
  pthread_t produttore = NULL;
  pthread_t consumatore = NULL;
  pthread_create(&consumatore, NULL,&consuma_uno_non_bloccante,NULL);
  pthread_create(&produttore, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(consumatore,&messaggioConsumato);
  pthread_join(produttore,messaggioInserito);
  CU_ASSERT_EQUAL(messaggioInserito,BUFFER_ERROR);
  CU_ASSERT_EQUAL(messaggioConsumato,messaggio);
}
