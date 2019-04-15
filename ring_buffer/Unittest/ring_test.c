/*========================================================================
** ring_test.c
** Circular buffer testing
** ECEN5813
**========================================================================*/
/**
*	@file 			ring_test.c
*	@brief 		CUNIT test framework with the test designed for ring buffer
*	
*	@author 			Linfeng Li; leveraged the main part from http://cunit.sourceforge.net/example.html
*	@date 			Mar 18 2019 
*	@version  	1.0
*/
#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include <CUnit/CUnit.h>
#include "../inc/ring.h"


/* The suite initialization function.
 * initialize the ring buffer with 5 bytes capacity
 * Returns zero on success, non-zero otherwise.
 */
 static ring_t* ring;
int init_suite1(void)
{
   ring=init(5);
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
void test_full_empty(void)
{
	char data;
	
	//test without any data in the buffer
	//extract them all out after
	CU_ASSERT(entries(ring)==0);
	for(int i=0;i<5;i++){
		if(entries(ring)<ring->Length){
			CU_ASSERT((insert(ring,(char)i)!=-2));
			CU_ASSERT(entries(ring)==(i+1));
		}	
		else
			CU_ASSERT((insert(ring,(char)i)==(-2)));
	}
	CU_ASSERT(entries(ring)==ring->Length);
	for(int i=0;i<5;i++){
		if(entries(ring)!=0){
			CU_ASSERT((extract(ring,&data)!=-2));
			CU_ASSERT(data==i);
			CU_ASSERT(entries(ring)==(ring->Length-1-i));
		}
		else
			CU_ASSERT((extract(ring,&data)==-2));
	}
	
	
	//test with one byte data in the buffer 
	//extract them all out after
	CU_ASSERT(entries(ring)==0);
	CU_ASSERT((insert(ring,123)!=-2));
	for(int i=0;i<6;i++){
		if(entries(ring)<ring->Length){
			CU_ASSERT((insert(ring,(char)i)!=-2));
			CU_ASSERT(entries(ring)==(i+2));
		}	
		else
			CU_ASSERT((insert(ring,(char)i)==(-2)));
	}
	CU_ASSERT(entries(ring)==ring->Length);
	printf("\nIni:%d Outi:%d\n",ring->Ini,ring->Outi);
	for(int i=0;i<6;i++){
		if(entries(ring)!=0){
			CU_ASSERT((extract(ring,&data)!=-2));
			CU_ASSERT(data==((i==0)?123:(i-1)));
			CU_ASSERT(entries(ring)==(ring->Length-1-i));
		}
		else
			CU_ASSERT((extract(ring,&data)==-2));
	}
	//test whether there is no data inside
	CU_ASSERT(entries(ring)==0);
	
	
	//add some offse to the input and output pointer
	CU_ASSERT((insert(ring,0)!=-2));
	CU_ASSERT((extract(ring,&data)!=-2));
	CU_ASSERT((insert(ring,0)!=-2));
	CU_ASSERT((extract(ring,&data)!=-2));
	
	//insert data: 10,11,12,13,14
	for(int i=0;i<5;i++){
		CU_ASSERT((insert(ring,i+10)!=-2));
	}
	printf("\nIni:%d Outi:%d\n",ring->Ini,ring->Outi);
	//resize the buffer with 6 bytes of size
	CU_ASSERT(resize(ring,6)==0);
	printf("\nIni:%d Outi:%d\n",ring->Ini,ring->Outi);
	
	//test whether there is still 5 elements inside
	//extract them all out after
	CU_ASSERT(entries(ring)==5);
	CU_ASSERT(ring->Full == 0);
	for(int i=0;i<6;i++){
		if(entries(ring)!=0){
			CU_ASSERT((extract(ring,&data)!=-2));
			CU_ASSERT(data==(10+i));
			CU_ASSERT(entries(ring)==4-i);
		}
		else
			CU_ASSERT((extract(ring,&data)==-2));
	}
	
	
	//test whether there is no data inside
	CU_ASSERT(entries(ring)==0);
	//add some offse to the input and output pointer
	CU_ASSERT((insert(ring,0)!=-2));
	CU_ASSERT((extract(ring,&data)!=-2));
	
	
	//insert data: 51,52,53,54,55,56
	for(int i=0;i<6;i++){
		CU_ASSERT((insert(ring,i+51)!=-2));
	}
	CU_ASSERT(ring->Full == 1);
	CU_ASSERT(entries(ring)==6);
	printf("\nIni:%d Outi:%d\n",ring->Ini,ring->Outi);
	//resize the buffer with 4 bytes of size
	CU_ASSERT(resize(ring,4)==0);
	//test whether there is only 4 elements inside
	CU_ASSERT(entries(ring)==4);
	CU_ASSERT(ring->Full == 1);
	
	//test whether only lastest data is maintained
	for(int i=0;i<6;i++){
		if(entries(ring)!=0){
			CU_ASSERT((extract(ring,&data)!=-2));
			CU_ASSERT(data==(53+i));
			CU_ASSERT(entries(ring)==3-i);
		}
		else
			CU_ASSERT((extract(ring,&data)==-2));
	}
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
   if (NULL == CU_add_test(pSuite, "test of ring buffer", test_full_empty))
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
