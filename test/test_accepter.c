/*
 * Test unitari per l'accepter
*/
#include "../accepter/accepter.h"

void test_accepter_init(){
  accepter_t* accepter = accepter_init();
  CU_ASSERT_PTR_NOT_NULL(accepter);
  accepter_destroy(accepter);
}

void test_accepter_init_accepter_buffer_vuoto(){
  accepter_t* accepter = accepter_init();
  CU_ASSERT_EQUAL(get_size(accepter->accepter_buffer),0);
  accepter_destroy(accepter);
}

void test_accepter_accepter_extract_buffer_con_un_msg(){
  accepter_t* accepter = accepter_init();
  msg_t* msg = msg_init_string("1");
  put_bloccante(accepter->accepter_buffer,msg);
  CU_ASSERT_EQUAL(accepter_extract(accepter),msg);
  accepter_destroy(accepter);
}

void test_accepter_accepter_create_reader(){
  accepter_t* accepter = accepter_init();
  msg_t* msg = msg_init_string("Reader");
  list_sync_t* list_sync = list_sync_init();
  CU_ASSERT_EQUAL(accepter_create_reader(msg,list_sync,1),0);
  sleep(1);
  CU_ASSERT_FALSE(isEmpty(list_sync->lista));
  reader_t* reader_veloce = (reader_t*) list_sync->lista->head->payload;
  CU_ASSERT_EQUAL(reader_veloce->tempo_di_consumo,1);
  list_sync_destroy(list_sync);
  accepter_destroy(accepter);
}


void test_accepter_accepter_start_poison_pill(){
  accepter_t* accepter = accepter_init();
  put_bloccante(accepter->accepter_buffer, POISON_PILL);
  list_sync_t* list_sync = list_sync_init();
  accepter_start(accepter,list_sync);
  CU_ASSERT_TRUE(isEmpty(list_sync->lista));
  list_sync_destroy(list_sync);
  accepter_destroy(accepter);
}

void test_accepter_accepter_start_una_richiesta_reader(){
  accepter_t* accepter = accepter_init();
  msg_t* msg = msg_init_string("Reader");
  put_bloccante(accepter->accepter_buffer, msg);
  put_bloccante(accepter->accepter_buffer, POISON_PILL);
  list_sync_t* list_sync = list_sync_init();
  accepter_start(accepter,list_sync);
  sleep(1);
  CU_ASSERT_FALSE(isEmpty(list_sync->lista));
  CU_ASSERT_EQUAL(size(list_sync->lista),1);
  list_sync_destroy(list_sync);
  accepter_destroy(accepter);
}

void test_accepter_accepter_start_due_richieste_reader_verifica_size(){
  accepter_t* accepter = accepter_init();
  msg_t* msg = msg_init_string("Reader");
  msg_t* msg2 = msg_init_string("Reader2");
  put_bloccante(accepter->accepter_buffer, msg);
  put_bloccante(accepter->accepter_buffer, msg2);
  put_bloccante(accepter->accepter_buffer, POISON_PILL);
  list_sync_t* list_sync = list_sync_init();
  accepter_start(accepter,list_sync);
  sleep(2);
  CU_ASSERT_EQUAL(size(list_sync->lista),2);
  list_sync_destroy(list_sync);
  accepter_destroy(accepter);
}

void test_accepter_accepter_start_due_richieste_reader_verifica_fifo(){
  accepter_t* accepter = accepter_init();
  msg_t* msg = msg_init_string("Reader");
  msg_t* msg2 = msg_init_string("Reader2");
  put_bloccante(accepter->accepter_buffer, msg);
  put_bloccante(accepter->accepter_buffer, msg2);
  put_bloccante(accepter->accepter_buffer, POISON_PILL);
  list_sync_t* list_sync = list_sync_init();
  accepter_start(accepter,list_sync);
  sleep(2);
  reader_t* reader =(reader_t*) list_sync->lista->head->payload;
  reader_t* reader2 =(reader_t*) list_sync->lista->tail->payload;
  CU_ASSERT_STRING_EQUAL(reader->nome,msg->content);
  CU_ASSERT_STRING_EQUAL(reader2->nome,msg2->content);
  list_sync_destroy(list_sync);
  accepter_destroy(accepter);
}

void test_accepter_accepter_start_due_richieste_reader_verifica_creazione_veloce_lento(){
  accepter_t* accepter = accepter_init();
  msg_t* msg = msg_init_string("Reader");
  msg_t* msg2 = msg_init_string("Reader2");
  put_bloccante(accepter->accepter_buffer, msg);
  put_bloccante(accepter->accepter_buffer, msg2);
  put_bloccante(accepter->accepter_buffer, POISON_PILL);
  list_sync_t* list_sync = list_sync_init();
  accepter_start(accepter,list_sync);
  sleep(2);
  reader_t* reader =(reader_t*) list_sync->lista->head->payload;
  reader_t* reader2 =(reader_t*) list_sync->lista->tail->payload;
  CU_ASSERT_EQUAL(reader->tempo_di_consumo,1);
  CU_ASSERT_EQUAL(reader2->tempo_di_consumo,3);
  list_sync_destroy(list_sync);
  accepter_destroy(accepter);
}

void test_accepter_accepter_start_richiesta_poison_richiesta_verifica_size(){
  accepter_t* accepter = accepter_init();
  msg_t* msg = msg_init_string("Reader");
  msg_t* msg2 = msg_init_string("Reader2");
  put_bloccante(accepter->accepter_buffer, msg);
  put_bloccante(accepter->accepter_buffer, POISON_PILL);
  put_bloccante(accepter->accepter_buffer, msg2);
  list_sync_t* list_sync = list_sync_init();
  accepter_start(accepter,list_sync);
  sleep(2);
  CU_ASSERT_EQUAL(size(list_sync->lista),1);
  list_sync_destroy(list_sync);
  accepter_destroy(accepter);
}
