#include "reader.h"
#include "../msg/poison_pill.h"

// creazione di un provider con messaggi
reader_t* reader_init(char* nome,unsigned int tempo_di_consumo){
  reader_t* reader = malloc(sizeof(reader_t));
  reader->reader_buffer = buffer_init(SIZE);
  char* new_content = malloc(strlen(nome));
  strcpy(new_content, nome);
  reader->nome = new_content;
  reader->tempo_di_consumo = tempo_di_consumo;
  return reader;
}
// deallocazione di un provider
void reader_destroy(reader_t* reader){
  buffer_destroy(reader->reader_buffer);
  free(reader->nome);
  free(reader);
}


/*Esecuzione del reader che legge i messaggi dal reader_buffer e li consuma.
Se il messaggio è POISON_PILL termina.*/
int reader_start(reader_t* reader){
  while(1){
    msg_t* msg = get_bloccante(reader->reader_buffer);
    if(msg == POISON_PILL)
      break;
    sleep(reader->tempo_di_consumo);
  }
  return 0; //letture completate senza errori
}
