/*
 * Test unitari per il provider
*/
#include "../provider/provider.h"

int providerAfter(){
  return 0;
}

int providerBefore(){
  return 0;
}

//creazione di un provider con zero messaggi
void test_provider_init_zero_messaggi(){
  provider_t* provider = provider_init(0);
  CU_ASSERT_EQUAL(provider->numero_messaggi,0);
  CU_ASSERT_EQUAL(provider->messaggi[0],POISON_PILL);
  provider_destroy(provider);
}

//creazione di un provider con un messaggio
void test_provider_init_un_messaggio(){
  provider_t* provider = provider_init(1);
  CU_ASSERT_EQUAL(provider->numero_messaggi,1);
  CU_ASSERT_EQUAL(provider->messaggi[0]->content,1);
  CU_ASSERT_EQUAL(provider->messaggi[1],POISON_PILL);
  provider_destroy(provider);
}

//creazione di un provider con due messaggi
void test_provider_init_due_messaggi(){
  provider_t* provider = provider_init(2);
  CU_ASSERT_EQUAL(provider->messaggi[0]->content,1);
  CU_ASSERT_EQUAL(provider->messaggi[1]->content,2);
  CU_ASSERT_EQUAL(provider->messaggi[2],POISON_PILL);
  provider_destroy(provider);
}

//provider che produce il messaggio che sta in una posizione
void test_provider_produci_primo_messaggio_nel_buffer(){
  provider_t* provider = provider_init(2);
  buffer_t* buffer = buffer_init(3);
  provider->provider_buffer = buffer;
  provider_produci_messaggio(0,provider);
  CU_ASSERT_EQUAL(provider->provider_buffer->messaggi[0]->content,1)
  buffer_destroy(buffer);
  provider_destroy(provider);
}

//provider che produce primo e secondo messaggio nel buffer + POISON PILL
void test_provider_produci_primo_e_secondo_messaggio_nel_buffer(){
  provider_t* provider = provider_init(2);
  buffer_t* buffer = buffer_init(3);
  provider->provider_buffer = buffer;
  provider_produci_messaggio(0,provider);
  provider_produci_messaggio(1,provider);
  CU_ASSERT_EQUAL(provider->provider_buffer->messaggi[1]->content,2)
  buffer_destroy(buffer);
  provider_destroy(provider);
}

//provider che produce solo POISON PILL in quanto non ha messaggi da produrre
void test_provider_con_zero_messaggi_produci_messaggi_nel_buffer(){
  provider_t* provider = provider_init(0);
  buffer_t* buffer = buffer_init(3);
  provider->provider_buffer = buffer;
  provider_produci_messaggi(provider);
  CU_ASSERT_EQUAL(provider->provider_buffer->messaggi[0],POISON_PILL);
  buffer_destroy(buffer);
  provider_destroy(provider);
}


//provider che produce tutti i suoi messaggi e poi POISON PILL
void test_provider_con_due_messaggi_produci_messaggi_nel_buffer(){
  provider_t* provider = provider_init(2);
  buffer_t* buffer = buffer_init(3);
  provider->provider_buffer = buffer;
  provider_produci_messaggi(provider);
  CU_ASSERT_EQUAL(provider->provider_buffer->messaggi[0]->content,1);
  CU_ASSERT_EQUAL(provider->provider_buffer->messaggi[1]->content,2);
  CU_ASSERT_EQUAL(provider->provider_buffer->messaggi[2],POISON_PILL);
  buffer_destroy(buffer);
  provider_destroy(provider);
}
