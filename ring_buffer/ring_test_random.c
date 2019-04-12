/*========================================================================
** ring_test.c
** Circular buffer testing
** ECEN5813
**========================================================================*/
/**
*	@file 			ring_test_randomizeed.c
*	@brief 		CUNIT test framework with the test designed for ring buffer.
*						randomly choose whether to extract or insert an element in to the buffer.
*						an extenal counter is used to keep track of the expected number of element in the buffer.
*						Also entries() is used to get the element number that actually resides in the buffer.
*						Two things should have agreement.
*	
*	@author 			Linfeng Li; leveraged the main part from http://cunit.sourceforge.net/example.html
*	@date 			Apr 11th 2019 
*	@version  	1.0
*/
#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include <CUnit/CUnit.h>
#include "ring.h"
#include <time.h>
#define BUFFER_SIZE (100)
#define TOTAL_LOOP (8000)
typedef enum{
	INSERT = 0,
	EXTRACT =1 ,
}operation;
/* The suite initialization function.
 * initialize the ring buffer with 5 bytes capacity
 * Returns zero on success, non-zero otherwise.
 */
static ring_t* ring;
static int external_cnt = 0; 
int init_suite1(void)
{
   ring=init(BUFFER_SIZE);
   if(ring == NULL)
	   return -1;
   else
	   return 0;
}

/* The suite cleanup function.
 * directly return
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
   return 0;
}

/* Simple test of fprintf().
 * Writes test data to the temporary file and checks
 * whether the expected number of bytes were written.
 */
void randomized_test(void)
{
	srand(time(NULL));
	operation op; 
	int status;
	char received_data;
	int max_entries=0;
	for(int i=0;i<TOTAL_LOOP;i++){
		op=rand()%2;
		switch(op){
			case INSERT:
				if(external_cnt<BUFFER_SIZE){
					external_cnt++;
					status=insert(ring,(char)rand());
					CU_ASSERT(status==0);
					CU_ASSERT(external_cnt==entries(ring));
				}
				else{
					status=insert(ring,(char)rand());
					CU_ASSERT(status==-2);
					CU_ASSERT(external_cnt==entries(ring));
				}	
				break;
			case EXTRACT:
				if(external_cnt!=0){
					external_cnt--;
					status=extract(ring,&received_data);
					CU_ASSERT(status==0);
					CU_ASSERT(external_cnt==entries(ring));
				}
				else{
					status=extract(ring,&received_data);
					CU_ASSERT(status==-2);
					CU_ASSERT(external_cnt==entries(ring));
				}	
				break;
		}
		if(entries(ring)>max_entries)
			max_entries = entries(ring);
	}
	printf("randomized test done with loop cnt: %d\n\n",TOTAL_LOOP);
	printf("In the random test: maximum used buffer size %d\n\n",max_entries);
}


/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if (NULL == CU_add_test(pSuite, "randomized test of ring buffer", randomized_test))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
