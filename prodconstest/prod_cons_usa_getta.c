#include "../prodcons/buffer.h"

static buffer_t* buf_unitario = NULL; //buffer unitario
static msg_t* messaggio = NULL; //messaggio da inserire nel buffer

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

//  Funzione usa-getta per la produzione bloccante di un messaggio
//  nel buffer dopo una sleep.
void* produci_uno_bloccante_sleep(){
sleep(1);
pthread_exit(put_non_bloccante(buf_unitario,messaggio));
return NULL;
}

//  Funzione usa-getta per la produzione non bloccante di un messaggio
//  nel buffer dopo aver aspettato 1 secondo.
void* produci_uno_nonbloccante_sleep(){
sleep(1);
pthread_exit(put_non_bloccante(buf_unitario,messaggio));
return NULL;
}

//  Funzione usa-getta per la consumazione bloccante di un messaggio
//  nel buffer.
void* consuma_uno_bloccante(){
pthread_exit(get_bloccante(buf_unitario));
return NULL;
}

//  Funzione usa-getta per la consumazione bloccante di un messaggio
//  nel buffer ma con lo sleep iniziale.
void* consuma_uno_bloccante_sleep(){
sleep(1);
pthread_exit(get_bloccante(buf_unitario));
return NULL;
}

//  Funzione usa-getta per la consumazione bloccante di un messaggio
//  nel buffer ma con lo sleep iniziale.
void* consuma_uno_bloccante_inattesa(){
printf("Sto tendando di consumare ma non ci sono messaggi...");
pthread_exit(get_bloccante(buf_unitario));
return NULL;
}


//  Funzione usa-getta per la consumazione non bloccante di un messaggio
//  nel buffer.
void* consuma_uno_nonbloccante(){
pthread_exit(get_non_bloccante(buf_unitario));
return NULL;
}

//  Funzione usa-getta per la consumazione non bloccante di un messaggio
//  nel buffer ma con lo sleep iniziale.
//  Sto violando l'assunzione di progresso finito per cercare di abbassare la
//  probabilità di alcune seq. di interleaving. Utile solo per il test.
void* consuma_uno_nonbloccante_sleep(){
sleep(1);
pthread_exit(get_non_bloccante(buf_unitario));
return NULL;
}

