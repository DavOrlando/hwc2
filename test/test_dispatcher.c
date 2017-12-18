#include "../dispatcher/dispatcher.h"



list_sync_t* get_mock_list_sync(int number_of_reader){
  list_sync_t* list_sync = list_sync_init();
  for(int i = 0;i<number_of_reader;i++){
    reader_t* reader = reader_init("reader test",1);
    addElement(list_sync->lista, reader);
  }
  return list_sync;
}


void test_dispatcher_multi_send_message_msg_null_list_empty(){
  list_sync_t* list_sync =get_mock_list_sync(0);
  dispatcher_multi_send_message(list_sync,NULL);
  CU_ASSERT_EQUAL(size(list_sync->lista),0);
}


void test_dispatcher_multi_send_message_poison_pill_list_empty(){
  list_sync_t* list_sync =get_mock_list_sync(0);
  dispatcher_multi_send_message(list_sync,POISON_PILL);
  CU_ASSERT_EQUAL(size(list_sync->lista),0);
}

void test_dispatcher_multi_send_message_poison_pill_un_reader(){
  list_sync_t* list_sync =get_mock_list_sync(1);
  dispatcher_multi_send_message(list_sync,POISON_PILL);
  reader_t* reader = (reader_t*)list_sync->lista->head->payload;
  CU_ASSERT_EQUAL(get_size(reader->reader_buffer),1);
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[0],POISON_PILL);
}


void test_dispatcher_multi_send_message_poison_pill_due_reader(){
  list_sync_t* list_sync =get_mock_list_sync(2);
  dispatcher_multi_send_message(list_sync,POISON_PILL);
  reader_t* reader = (reader_t*)list_sync->lista->head->payload;
  reader_t* reader2 = (reader_t*)list_sync->lista->tail->payload;
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[0],POISON_PILL);
  CU_ASSERT_EQUAL(reader2->reader_buffer->messaggi[0],POISON_PILL);
}

void test_dispatcher_multi_send_message_singolo_messaggio_due_reader(){
  list_sync_t* list_sync =get_mock_list_sync(2);
  msg_t* msg = msg_init_int(1);
  dispatcher_multi_send_message(list_sync,msg);
  reader_t* reader = (reader_t*)list_sync->lista->head->payload;
  reader_t* reader2 = (reader_t*)list_sync->lista->tail->payload;
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[0]->content,1);
  CU_ASSERT_EQUAL(reader2->reader_buffer->messaggi[0]->content,1);
}


void test_dispatcher_dispatcher_start_buffer_con_poison_pill_singolo_reader(){
  buffer_t* provider_buffer = buffer_init(1);
  put_bloccante(provider_buffer,POISON_PILL);
  list_sync_t* list_sync = get_mock_list_sync(1);
  dispatcher_start(provider_buffer,list_sync);
}


void test_dispatcher_dispatcher_start_buffer_con_messaggio_singolo_reader(){
  buffer_t* provider_buffer = buffer_init(2);
  msg_t* msg = msg_init_int(1);
  put_bloccante(provider_buffer,msg);
  put_bloccante(provider_buffer,POISON_PILL);
  list_sync_t* list_sync = get_mock_list_sync(1);
  dispatcher_start(provider_buffer,list_sync);
  reader_t* reader = (reader_t*) list_sync->lista->head->payload;
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[0]->content,1);
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[1],POISON_PILL);
}

void test_dispatcher_dispatcher_start_buffer_con_due_messaggi_singolo_reader(){
  buffer_t* provider_buffer = buffer_init(3);
  msg_t* msg = msg_init_int(1);
  msg_t* msg2 = msg_init_int(2);
  put_bloccante(provider_buffer,msg);
  put_bloccante(provider_buffer,msg2);
  put_bloccante(provider_buffer,POISON_PILL);
  list_sync_t* list_sync = get_mock_list_sync(1);
  dispatcher_start(provider_buffer,list_sync);
  reader_t* reader = (reader_t*) list_sync->lista->head->payload;
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[0]->content,1);
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[1]->content,2);
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[2],POISON_PILL);
}

void test_dispatcher_dispatcher_start_buffer_con_due_messaggi_doppio_reader(){
  buffer_t* provider_buffer = buffer_init(3);
  msg_t* msg = msg_init_int(1);
  msg_t* msg2 = msg_init_int(2);
  put_bloccante(provider_buffer,msg);
  put_bloccante(provider_buffer,msg2);
  put_bloccante(provider_buffer,POISON_PILL);
  list_sync_t* list_sync = get_mock_list_sync(2);
  dispatcher_start(provider_buffer,list_sync);
  reader_t* reader = (reader_t*) list_sync->lista->head->payload;
  reader_t* reader2 = (reader_t*) list_sync->lista->head->next->payload;
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[0]->content,1);
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[1]->content,2);
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[2],POISON_PILL);
  CU_ASSERT_EQUAL(reader2->reader_buffer->messaggi[0]->content,1);
  CU_ASSERT_EQUAL(reader2->reader_buffer->messaggi[1]->content,2);
  CU_ASSERT_EQUAL(reader2->reader_buffer->messaggi[2],POISON_PILL);
}

void test_dispatcher_dispatcher_start_buffer_con_un_messaggio_reader_pieno(){
  buffer_t* provider_buffer = buffer_init(2);
  msg_t* msg = msg_init_int(1);
  put_bloccante(provider_buffer,msg);
  put_bloccante(provider_buffer,POISON_PILL);
  list_sync_t* list_sync = get_mock_list_sync(1);
  reader_t* reader = (reader_t*) list_sync->lista->head->payload;
  put_bloccante(reader->reader_buffer,msg);
  put_bloccante(reader->reader_buffer,msg);
  put_bloccante(reader->reader_buffer,msg);
  dispatcher_start(provider_buffer,list_sync);
  CU_ASSERT_NOT_EQUAL(reader->reader_buffer->messaggi[0],POISON_PILL);
  CU_ASSERT_NOT_EQUAL(reader->reader_buffer->messaggi[1],POISON_PILL);
  CU_ASSERT_NOT_EQUAL(reader->reader_buffer->messaggi[2],POISON_PILL);
  get_bloccante(reader->reader_buffer);
  sleep(1);
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[0],POISON_PILL);
}


void test_dispatcher_dispatcher_start_buffer_con_un_messaggio_due_reader_uno_pieno(){
  buffer_t* provider_buffer = buffer_init(2);
  msg_t* msg = msg_init_int(1);
  put_bloccante(provider_buffer,msg);
  put_bloccante(provider_buffer,POISON_PILL);
  list_sync_t* list_sync = get_mock_list_sync(2);
  reader_t* reader = (reader_t*) list_sync->lista->head->payload;
  reader_t* reader2 = (reader_t*) list_sync->lista->tail->payload;
  put_bloccante(reader->reader_buffer,msg);
  put_bloccante(reader->reader_buffer,msg);
  put_bloccante(reader->reader_buffer,msg);
  dispatcher_start(provider_buffer,list_sync);
  get_bloccante(reader->reader_buffer);
  sleep(1);
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[0],POISON_PILL);
  CU_ASSERT_EQUAL(reader2->reader_buffer->messaggi[0]->content,1);
  CU_ASSERT_EQUAL(reader2->reader_buffer->messaggi[1],POISON_PILL);
}

void test_dispatcher_dispatcher_start_buffer_con_un_messaggio_due_reader_entrambi_pieni(){
  buffer_t* provider_buffer = buffer_init(2);
  msg_t* msg = msg_init_int(1);
  put_bloccante(provider_buffer,msg);
  put_bloccante(provider_buffer,POISON_PILL);
  list_sync_t* list_sync = get_mock_list_sync(2);
  reader_t* reader = (reader_t*) list_sync->lista->head->payload;
  reader_t* reader2 = (reader_t*) list_sync->lista->tail->payload;
  put_bloccante(reader->reader_buffer,msg);
  put_bloccante(reader->reader_buffer,msg);
  put_bloccante(reader->reader_buffer,msg);
  put_bloccante(reader2->reader_buffer,msg);
  put_bloccante(reader2->reader_buffer,msg);
  put_bloccante(reader2->reader_buffer,msg);
  dispatcher_start(provider_buffer,list_sync);
  get_bloccante(reader->reader_buffer);
  get_bloccante(reader2->reader_buffer);
  sleep(1);
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[0],POISON_PILL);
  CU_ASSERT_EQUAL(reader2->reader_buffer->messaggi[0],POISON_PILL);
}


void test_dispatcher_dispatcher_start_buffer_con_un_messaggio_consegna_solo_reader_accettati_prima(){
  buffer_t* provider_buffer = buffer_init(1);
  msg_t* msg = msg_init_int(1);
  put_bloccante(provider_buffer,POISON_PILL);
  list_sync_t* list_sync = get_mock_list_sync(1);
  reader_t* reader = (reader_t*) list_sync->lista->head->payload;
  dispatcher_start(provider_buffer,list_sync);
  reader_t* reader2 = reader_init("reader 2",1);
  addElement(list_sync->lista,reader);
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[0],POISON_PILL);
  CU_ASSERT_PTR_NULL(reader2->reader_buffer->messaggi[0]);
}


void test_dispatcher_dispatcher_start_buffer_con_un_messaggio_consegna_due_reader_accettati_prima(){
  buffer_t* provider_buffer = buffer_init(1);
  msg_t* msg = msg_init_int(1);
  put_bloccante(provider_buffer,POISON_PILL);
  list_sync_t* list_sync = get_mock_list_sync(2);
  reader_t* reader = (reader_t*) list_sync->lista->head->payload;
  reader_t* reader2 = (reader_t*) list_sync->lista->tail->payload;
  dispatcher_start(provider_buffer,list_sync);
  reader_t* reader3 = reader_init("reader 3",1);
  addElement(list_sync->lista,reader);
  CU_ASSERT_EQUAL(reader->reader_buffer->messaggi[0],POISON_PILL);
  CU_ASSERT_EQUAL(reader2->reader_buffer->messaggi[0],POISON_PILL);
  CU_ASSERT_PTR_NULL(reader3->reader_buffer->messaggi[0]);
}
