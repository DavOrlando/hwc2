/*
 * Test unitari per la lista sincronizzata
*/
#include "../list/list_sync.h"


//creazione di un lista sincronizzata vuota
void test_list_sync_init_vuota(){
  list_sync_t* list_sync = list_sync_init();
  CU_ASSERT_EQUAL(size(list_sync->lista),0);
  list_sync_destroy(list_sync);
}

//aggiunta di un elemento alla lista sincronizzata vuota
void test_list_sync_add_list_vuota(){
  list_sync_t* list_sync = list_sync_init();
  CU_ASSERT_EQUAL(size(list_sync->lista),0);
  pthread_mutex_lock(&(list_sync->mutex));
  reader_t* reader = reader_init("test",1);
  addElement(list_sync->lista,reader);
  pthread_mutex_unlock(&(list_sync->mutex));
  CU_ASSERT_EQUAL(size(list_sync->lista),1);
  list_sync_destroy(list_sync);
}
