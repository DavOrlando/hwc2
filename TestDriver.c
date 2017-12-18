/*
 * Autore: Davide Orlando
 *
 * TestDriver.c
 */

#include <stdlib.h>
#include <stdio.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "test/test_provider.c"
#include "test/test_reader.c"
#include "test/test_list_sync.c"
#include "test/test_accepter.c"
#include "test/test_dispatcher.c"


int main(int argc, char** argv){

	 //PER PRIMA COSA inizializzare cunit
	    CU_ErrorCode ret = CU_initialize_registry();
	    if(ret==CUE_SUCCESS)
	        printf("inizio dei test\n");
	    if(ret==CUE_NOMEMORY){
	        printf("ERRORE NEI TEST\n");
	        exit(-1);
	    }

	    CU_TestInfo suiteProvider[]= {
	            { "Creazione di un provider con zero messaggi", test_provider_init_zero_messaggi},
							{ "Creazione di un provider con un messaggio", test_provider_init_un_messaggio},
							{ "Creazione di un provider con due messaggi", test_provider_init_due_messaggi},
							{ "Creazione di un provider che produce un messaggio", test_provider_produci_primo_messaggio_nel_buffer},
							{ "Creazione di un provider che produce due messaggi", test_provider_produci_primo_e_secondo_messaggio_nel_buffer},
							{ "Creazione di un provider che produce solo POISON PILL", test_provider_con_zero_messaggi_produci_messaggi_nel_buffer},
							{ "Creazione di un provider con due messaggi che produce tutti i messaggi", test_provider_con_due_messaggi_produci_messaggi_nel_buffer},
	            CU_TEST_INFO_NULL
	        };
			CU_TestInfo suiteReader[]= {
							{ "Creazione di un reader lento", test_reader_init_reader_lento},
							{ "Creazione di un reader veloce", test_reader_init_reader_veloce},
							{ "Creazione di un reader verifica buffer vuoto", test_reader_init_verifica_reader_buffer_vuoto},
							{ "Creazione di un reader verifica size del buffer", test_reader_init_verifica_reader_buffer_size},
							{ "Lettura della poison pill e terminazione", test_reader_start_buffer_con_poison_pill_terminazione},
							{ "Lettura veloce di un messaggio, della poison pill e terminazione", test_reader_start_buffer_con_messaggio_reader_veloce_terminazione},
							{ "Lettura lenta di un messaggio, della poison pill e terminazione", test_reader_start_buffer_con_messaggio_reader_lento_terminazione},
							CU_TEST_INFO_NULL
					};
			CU_TestInfo suiteListSync[]= {
							{ "Creazione di una lista sincronizzata", test_list_sync_init_vuota},
							{ "Aggiunta di un elemento alla lista sincronizzata", test_list_sync_add_list_vuota},

							CU_TEST_INFO_NULL
					};

			CU_TestInfo suiteAccepter[]= {
							{ "Creazione di un accepter e verifica buffer vuoto",test_accepter_init},
							{ "Creazione di un accepter e verifica buffer vuoto",test_accepter_init_accepter_buffer_vuoto},
							{ "Estrazione di una richiesta di creazione",test_accepter_accepter_extract_buffer_con_un_msg},
							{ "Creazione di un reader dato un messaggio di richiesta di creazione",test_accepter_accepter_create_reader},
							{ "Verifica start dell'accepter con poison pill nell'accepter buffer",test_accepter_accepter_start_poison_pill},
							{ "Verifica start dell'accepter con messaggio nell'accepter buffer",test_accepter_accepter_start_una_richiesta_reader},
							{ "Verifica size dopo start dell'accepter con due messaggi nell'accepter buffer",test_accepter_accepter_start_due_richieste_reader_verifica_size},
							{ "Verifica politica fifo dopo start dell'accepter con due messaggi nell'accepter buffer",test_accepter_accepter_start_due_richieste_reader_verifica_fifo},
							{ "Verifica size dopo start dell'accepter con messaggio-poison_pill-messaggi nell'accepter buffer",test_accepter_accepter_start_richiesta_poison_richiesta_verifica_size},
							{ "Verifica creazione reader lento veloce con start",test_accepter_accepter_start_due_richieste_reader_verifica_creazione_veloce_lento},
							CU_TEST_INFO_NULL
					};
			CU_TestInfo suiteDispatcher[]= {
							{ "Verifica invio messaggio a tutti i reader con lista di reader vuota e messaggio NULL",test_dispatcher_multi_send_message_msg_null_list_empty},
							{ "Verifica invio poison pill a tutti i reader con lista di reader vuota",test_dispatcher_multi_send_message_poison_pill_list_empty},
							{ "Verifica invio poison pill a tutti i reader con lista di reader unitaria",test_dispatcher_multi_send_message_poison_pill_un_reader},
							{ "Verifica invio poison pill a tutti i reader con lista di reader doppia",test_dispatcher_multi_send_message_poison_pill_due_reader},
							{ "Verifica invio messaggio a tutti i reader con lista di reader doppia",test_dispatcher_multi_send_message_singolo_messaggio_due_reader},
							{ "Verifica dispatcher start provider buffer con poison pill",test_dispatcher_dispatcher_start_buffer_con_poison_pill_singolo_reader},
							{ "Verifica dispatcher start provider buffer con messaggio",test_dispatcher_dispatcher_start_buffer_con_messaggio_singolo_reader},
							{ "Verifica dispatcher start provider buffer con due messaggi",test_dispatcher_dispatcher_start_buffer_con_due_messaggi_singolo_reader},
							{ "Verifica dispatcher start provider buffer con due messaggi e due reader",test_dispatcher_dispatcher_start_buffer_con_due_messaggi_doppio_reader},
						 	{ "Verifica dispatcher start provider buffer con un messaggio e un reader pieno",test_dispatcher_dispatcher_start_buffer_con_un_messaggio_reader_pieno},
							{ "Verifica dispatcher start provider buffer con un messaggio e due reader, uno pieno e l'altro vuoto",test_dispatcher_dispatcher_start_buffer_con_un_messaggio_due_reader_uno_pieno},
							{ "Verifica dispatcher start provider buffer con un messaggio e due reader pieni",test_dispatcher_dispatcher_start_buffer_con_un_messaggio_due_reader_entrambi_pieni},
							{ "Verifica consegna al singolo reader accettato solo prima della presa in carico del messaggio",test_dispatcher_dispatcher_start_buffer_con_un_messaggio_consegna_solo_reader_accettati_prima},
							{ "Verifica consegna ai reader accettati solo prima della presa in carico del messaggio",test_dispatcher_dispatcher_start_buffer_con_un_messaggio_consegna_due_reader_accettati_prima},
							CU_TEST_INFO_NULL
					};
	    CU_SuiteInfo suites[] = {
	        { "Test sul provider", providerBefore, providerAfter, suiteProvider},
					{ "Test sul reader", readerBefore, readerAfter, suiteReader},
					{ "Test sulla lista sincronizzata", NULL, NULL, suiteListSync},
					{ "Test sull'accepter", NULL, NULL, suiteAccepter},
					{ "Test sul dispatcher", NULL, NULL, suiteDispatcher},

	        CU_SUITE_INFO_NULL
	    };

	    CU_ErrorCode error = CU_register_suites(suites);
	    if(error != CUE_SUCCESS){
	        printf("Errore con CUnit - codice %d\n", error);
	    }

	    //far girare i test - modo base, risultato su stdout
	    CU_basic_set_mode(CU_BRM_VERBOSE);
	        //CU_BRM_NORMAL 	Failures and run summary are printed.
	        //CU_BRM_SILENT 	No output is printed except error messages.
	        //CU_BRM_VERBOSE 	Maximum output of run details.
	    CU_ErrorCode tuttoOk = CU_basic_run_tests();
	    printf("CUnit error code-->%d\n", tuttoOk);

	    //lettura risultati
	    unsigned int failures = CU_get_number_of_failures();
	    if(failures == 0)
	        printf("Barra Verde!\n");
	    else
	        printf("%d fallimenti\n",CU_get_number_of_failures());

	    //alla fine, pulire. UNA SOLA volta
	    CU_cleanup_registry();

	    return 0;
}
