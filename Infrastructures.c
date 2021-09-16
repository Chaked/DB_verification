#include "Infrastructures.h"

unsigned int malloc_count = 0;

// Variables for memory leak verification
void* stored_ptr = NULL;
boolean_t have_we_stored_a_ptr = FALSE;
boolean_t have_we_freed_the_stored_ptr = FALSE;

// Variables for dereferencing non allocated pointer verification.
void* non_allocated = NULL;

void* db_malloc(size_t size) {
	void* result = malloc(size);
	if (result == NULL) {
		//printf("Problem allocating memory");
		return NULL;
	} else
		malloc_count++;

	assume(result != NULL);
	// For memory leak verification.
	if (!have_we_stored_a_ptr && nd()) {
		have_we_stored_a_ptr = TRUE;
		stored_ptr = result;
	}
	// For dereferencing non allocated pointer verification
	if(malloc_count == 1)
		non_allocated = nd_ptr(); //We initilize non_allocated on the first allocation.
	assume(non_allocated != result);

	return result;
}

void db_free(void* ptr) {
	// Assert we don't try to free when nothing is allocated
	sassert(malloc_count > 0); 
	//sassert(ptr); //Assert we are not freeing any null pointer. This does not work
	// Memory leak verification
	if (ptr == stored_ptr) 
		have_we_freed_the_stored_ptr = TRUE;
	malloc_count--;
	// Non allocated dereferencing verification. This results in verifier.error.split
	// sassert(ptr != non_allocated);
	free(ptr);
}

void done(int test_id) {
	switch (test_id)
	{
	case 0: 
		sassert(malloc_count == 0);
		//if (malloc_count == 0) printf("Test 0 is successful");
		break;
	case 1: 
		sassert(have_we_freed_the_stored_ptr);
		break;
	default:
		break;
	}

}
