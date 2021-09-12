#include "Infrastructures.h"

unsigned int malloc_count = 0;

void* stored_ptr = NULL;
boolean_t have_we_stored_a_ptr = FALSE;
boolean_t have_we_freed_the_stored_ptr = FALSE;

void* db_malloc(size_t size) {
	void* result = malloc(size);
	if (result == NULL) {
		//printf("Problem allocating memory");
		return NULL;
	} else
		malloc_count++;

	//assume(result != NULL);
	if (!have_we_stored_a_ptr && nd()) {
		have_we_stored_a_ptr = TRUE;
		stored_ptr = result;
	}

	return result;
}

void db_free(void* ptr) {

	//sassert(malloc_count > 0); // Assert we don't try to free when nothing is allocated
	//sassert(ptr); //Assert we are not freeing any null pointer
	if (ptr == stored_ptr) 
		have_we_freed_the_stored_ptr = TRUE;
	malloc_count--;
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

char* copy_string(char* src) {
	int res_size = db_strlen(src) + 1;
	char* res = db_malloc(res_size);
	if (res == NULL)
		return NULL;
	db_strcpy(res, res_size ,src);
	return res;

}

char* db_strcpy(char* des, int dest_size, char* src) {
	if (des == NULL || src == NULL)
		return NULL;
	
	boolean_t last_char = FALSE;
	for (int i = 0; i < dest_size; i++) {
		des[i] = src[i];
		if (src[i] == '\0') 
			break;
	}
	return des;
}


char* db_strtok(char * str) {
	char delimeters[] = " \t\n\v\f\r";
	static char* internal_str = NULL;
	static int i = 0;
	static boolean_t first_time_reaching_the_end = TRUE;
	if (str == NULL && internal_str == NULL)
		return NULL;
	else if (str != NULL) {
		internal_str = str;
		i = 0;
		first_time_reaching_the_end = TRUE;
	}
	char* first_char = &internal_str[i];
	while (internal_str[i] != '\0') {
		if (internal_str[i] == ' ' ||
			internal_str[i] == '\t' ||
			internal_str[i] == '\n' ||
			internal_str[i] == '\v' ||
			internal_str[i] == '\f' ||
			internal_str[i] == '\r') {
			internal_str[i] = '\0';
			i++;
			return first_char;
		}
		i++;
	}
	if (first_time_reaching_the_end) {
		first_time_reaching_the_end = FALSE;
		return first_char;
	}
	return NULL;
}

int db_strlen(char* str) {
	int i = 0;
	while (str[i++] != '\0');
	return i-1;
}

int db_strcmp(char* str1, char* str2) {
	int i = 0;
	for (; str1[i] != '\0' || str2[i] != '\0'; i++) {
		if (str1[i] != str2[i])
			return 1;
	}
	if (str1[i] == str2[i])
		return 0;
	return 1;
}

char * db_concat(char* str1, char* str2) {
	int str1_size = db_strlen(str1);
	int str2_size = db_strlen(str2);
	char* result = db_malloc(str1_size + str2_size + 1);
	result = db_strcpy(result, str1_size+1, str1);
	db_strcpy(&result[str1_size], str2_size+1, str2);
	return result;
}



