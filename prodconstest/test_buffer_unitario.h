

//  Crea la suite e inserisce tutti i test unitari alla suite
int test_buffer_unitario_create();

//  Funzione che inizializza la suite: inizializza il messaggio con un
//  messaggio stringa con contenuto UNO ("Messaggio UNO").
int test_buffer_unitario_setup(void);

//  Funzione che distrugge la suite: distrugge il messaggio.
int test_buffer_unitario_clean(void);

//  Funzione che ritorna un buffer unitario inizializzato vuoto.
buffer_t* get_buffer_unitario_vuoto(void);

//  Funzione che ritorna un buffer unitario inizializzato e riempito.
buffer_t* get_buffer_unitario_pieno(void);

//  Funzione usa-getta per la produzione bloccante di un messaggio
//  nel buffer.
void* produci_uno_bloccante(void);
//  Funzione usa-getta per la produzione non bloccante di un messaggio
//  nel buffer.
void* produci_uno_non_bloccante(void);
//  Funzione usa-getta per la consumazione bloccante di un messaggio
//  nel buffer.
void* consuma_uno_bloccante(void);
//  Funzione usa-getta per la consumazione non bloccante di un messaggio
//  nel buffer.
void* consuma_uno_non_bloccante(void);

//  Funzione usa-getta per la consumazione non bloccante di un messaggio
//  nel buffer ma con lo sleep iniziale.
//  Sto violando l'assunzione di progresso finito per cercare di abbassare la
//  probabilità di alcune seq. di interleaving. Utile solo per il test.
void* consuma_uno_non_bloccante_sleep(void);


//Verifico creazione del buffer unitario vuoto
void test_buffer_unitario_creazione(void);
//Verifico creazione del messaggio durante l'inizializzazione della suite.
void test_buffer_unitario_creazione_messaggio(void);
//Verifico produzione bloccante di un solo messaggio in un buffer vuoto
void test_buffer_unitario_messaggio_prodotto_bloccante(void);
//Verifico produzione non bloccante di un solo messaggio in un buffer vuoto.
void test_buffer_unitario_messaggio_prodotto_non_bloccante(void);
//Verifica aumento del numero dei messaggi dopo produzione bloccante messaggio.
void test_buffer_unitario_aumento_numero_messaggi_bloccante(void);
//Verifica aumento del numero dei messaggi dopo produzione non bloccante messaggio
void test_buffer_unitario_aumento_numero_messaggi_non_bloccante(void);
//Verifica aumento dell'indice inserimento dopo produzione bloccante messaggio.
void test_buffer_unitario_aumento_indice_bloccante(void);
//Verifica aumento dell'indice inserimento dopo produzione non bloccante messaggio
void test_buffer_unitario_aumento_indice_non_bloccante(void);
//Produzione non bloccante in un buffer pieno.
void test_buffer_unitario_pieno_produci_non_bloccante(void);
//Consumazione bloccante di un solo messaggio da un buffer pieno.
void test_buffer_unitario_pieno_consuma_bloccante(void);
//Consumazione non bloccante di un solo messaggio da un buffer pieno
void test_buffer_unitario_pieno_consuma_non_bloccante(void);
//Verifica decremento size dopo consumazione bloccante di un messaggio
void test_buffer_unitario_pieno_consuma_bloccante_decremento_size(void);
//Verifica decremento size dopo consumazione non bloccante di un messaggio.
void test_buffer_unitario_pieno_consuma_non_bloccante_decremento_size(void);
//Verifica aumento circolare dell'indice di estrazione dopo consumazione bloccante.
void test_buffer_unitario_pieno_consuma_bloccante_aumento_indice(void);
//Verifica aumento circolare indice di estrazione dopo consumazione non bloccante.
void test_buffer_unitario_pieno_consuma_non_bloccante_aumento_indice(void);
//Verifica consumazione non bloccante da un buffer vuoto.
void test_buffer_unitario_vuoto_consuma_non_bloccante(void);
//Consumazione bloccante e produzione bloccante concorrente di un messaggio
//in un buffer vuoto, prima il produttore.
void test_buffer_unitario_vuoto_produci_consuma_bloccante(void);
//Consumazione non bloccante e produzione non bloccante concorrente di un messaggio
//in un buffer vuoto, prima il produttore.
void test_buffer_unitario_vuoto_produci_consuma_non_bloccante(void);
//Consumazione non bloccante e produzione non bloccante concorrente di un messaggio
//in un buffer pieno, prima il produttore.
void test_buffer_unitario_pieno_produci_consuma_non_bloccante(void);
//Consumazione bloccante e produzione bloccante concorrente di un messaggio
//in un buffer vuoto, prima il consumatore.
void test_buffer_unitario_vuoto_consuma_produci_bloccante(void);
//Consumazione non bloccante e produzione non bloccante concorrente di un messaggio
//in un buffer vuoto, prima il consumatore.
void test_buffer_unitario_vuoto_consuma_produci_non_bloccante(void);
//Consumazione non bloccante e produzione non bloccante concorrente di un messaggio
//in un buffer pieno, prima il consumatore.
void test_buffer_unitario_pieno_consuma_produci_non_bloccante(void);
