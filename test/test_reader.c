/*
 * Test unitari per il reader
*/
#include "../reader/reader.h"

#define READER_VELOCE "Reader Veloce"
#define READER_LENTO "Reader Lento"

int readerAfter(){
  return 0;
}

int readerBefore(){
  return 0;
}

//creazione di un reader lento
void test_reader_init_reader_lento(){
  reader_t* reader = reader_init(READER_LENTO,3);
  CU_ASSERT_STRING_EQUAL(reader->nome,READER_LENTO);
  CU_ASSERT_EQUAL(reader->tempo_di_consumo, 3);
  reader_destroy(reader);
}

//creazione di un reader veloce
void test_reader_init_reader_veloce(){
  reader_t* reader = reader_init(READER_VELOCE,1);
  CU_ASSERT_STRING_EQUAL(reader->nome,READER_VELOCE);
  CU_ASSERT_EQUAL(reader->tempo_di_consumo, 1);
  reader_destroy(reader);
}

//creazione di un reader e verifica buffer vuoto
void test_reader_init_verifica_reader_buffer_vuoto(){
  reader_t* reader = reader_init(READER_VELOCE,1);
  CU_ASSERT_EQUAL(get_size(reader->reader_buffer), 0);
  reader_destroy(reader);
}

//creazione di un reader e verifica size
void test_reader_init_verifica_reader_buffer_size(){
  reader_t* reader = reader_init(READER_VELOCE,1);
  CU_ASSERT_EQUAL(reader->reader_buffer->maxsize, SIZE);
  reader_destroy(reader);
}

//verifica lettura della poison pill e terminazione
void test_reader_start_buffer_con_poison_pill_terminazione(){
  reader_t* reader = reader_init(READER_VELOCE,1);
  put_bloccante(reader->reader_buffer, POISON_PILL);
  CU_ASSERT_EQUAL(reader_start(reader),0);
  reader_destroy(reader);
}

//verifica lettura veloce di un messaggio,della poison pill e terminazione
void test_reader_start_buffer_con_messaggio_reader_veloce_terminazione(){
  reader_t* reader = reader_init(READER_VELOCE,1);
  put_bloccante(reader->reader_buffer, msg_init_int(1));
  put_bloccante(reader->reader_buffer, POISON_PILL);
  CU_ASSERT_EQUAL(reader_start(reader),0);
  reader_destroy(reader);
}

//verifica lettura lenta di un messaggio,della poison pill e terminazione
void test_reader_start_buffer_con_messaggio_reader_lento_terminazione(){
  reader_t* reader = reader_init(READER_LENTO,3);
  put_bloccante(reader->reader_buffer, msg_init_int(1));
  put_bloccante(reader->reader_buffer, POISON_PILL);
  CU_ASSERT_EQUAL(reader_start(reader),0);
  CU_ASSERT_EQUAL(get_size(reader->reader_buffer),0);
  reader_destroy(reader);
}
