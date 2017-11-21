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
        test_buffer_unitario_pieno_produci_non_bloccante) == NULL
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

void test_buffer_unitario_creazione(void){
  buffer = buffer_init(DIMENSIONE_UNITARIA);
  CU_ASSERT_PTR_NOT_NULL(buffer);
  CU_ASSERT_EQUAL(buffer->maxsize,DIMENSIONE_UNITARIA);
  buffer_destroy(buffer);
}

void test_buffer_unitario_creazione_messaggio(void){
  CU_ASSERT_PTR_NOT_NULL(messaggio);
  CU_ASSERT_STRING_EQUAL(messaggio->content,UNO);
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

void test_buffer_unitario_messaggio_prodotto_bloccante(void){
  buffer = buffer_init(DIMENSIONE_UNITARIA);
  CU_ASSERT_PTR_NULL(buffer->messaggi[0]);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_PTR_NOT_NULL(buffer->messaggi[0]);
  CU_ASSERT_STRING_EQUAL(buffer->messaggi[0]->content,UNO);
  buffer_destroy(buffer);
}

void test_buffer_unitario_messaggio_prodotto_non_bloccante(void){
  buffer = buffer_init(DIMENSIONE_UNITARIA);
  CU_ASSERT_PTR_NULL(buffer->messaggi[0]);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_PTR_NOT_NULL(buffer->messaggi[0]);
  CU_ASSERT_STRING_EQUAL(buffer->messaggi[0]->content,UNO);
  buffer_destroy(buffer);
}


void test_buffer_unitario_aumento_indice_bloccante(void){
  buffer = buffer_init(DIMENSIONE_UNITARIA);
  CU_ASSERT_EQUAL(buffer->indice_inserimento,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buffer->indice_inserimento,0);
  buffer_destroy(buffer);
}
void test_buffer_unitario_aumento_indice_non_bloccante(void){
  buffer = buffer_init(DIMENSIONE_UNITARIA);
  CU_ASSERT_EQUAL(buffer->size,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buffer->size,1);
  buffer_destroy(buffer);
}
void test_buffer_unitario_aumento_numero_messaggi_bloccante(void){
  buffer = buffer_init(DIMENSIONE_UNITARIA);
  CU_ASSERT_EQUAL(buffer->size,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buffer->size,1);
  buffer_destroy(buffer);
}
void test_buffer_unitario_aumento_numero_messaggi_non_bloccante(void){
  buffer = buffer_init(DIMENSIONE_UNITARIA);
  CU_ASSERT_EQUAL(buffer->size,0);
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(thread,NULL);
  CU_ASSERT_EQUAL(buffer->size,1);
  buffer_destroy(buffer);
}



void test_buffer_unitario_pieno_produci_non_bloccante(void){
  buffer = buffer_init(DIMENSIONE_UNITARIA);
  msg_t *messaggioInserito = NULL;
  pthread_t thread = NULL;
  pthread_create(&thread, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(thread,&messaggioInserito);
  CU_ASSERT_PTR_NOT_NULL(messaggioInserito);
  pthread_create(&thread, NULL,&produci_uno_non_bloccante,NULL);
  pthread_join(thread,&messaggioInserito);
  CU_ASSERT_PTR_NULL(messaggioInserito);
  buffer_destroy(buffer);
}
