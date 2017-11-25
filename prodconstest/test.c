//
//  test.c
//  hwc
//  In questo file utilizzo CUnit per creare le suite
//  di test di cui ho bisogno e per eseguire i test
//  all'interno di esse definite.
//  Created by Davide Orlando on 22/11/17.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CUnit/Basic.h"

#include "prod_cons_usa_getta.c"
//  suite di test che utilizzano un buffer di
//  dimensione massima pari a 1.
#include "test_buffer_unitario.c"


//  Chiama delle funzioni che creano le suite e le
//  aggiungo al registro dei test. Successivamente
//  esegue i test in modalita basic.
int main()
{
   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   test_buffer_unitario_create();

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
