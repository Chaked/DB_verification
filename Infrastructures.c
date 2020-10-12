#include "Infrastructures.h"

unsigned int malloc_count = 0;

void* stored_ptr = NULL;
boolean_t have_we_stored_a_ptr = FALSE;
boolean_t have_we_freed_the_stored_ptr = FALSE;

void* dbmalloc(size_t size) {
	void* result = malloc(size);
	if (result == NULL) {
		printf_s("Problem allocating memory");
		return NULL;
	} else
		malloc_count++;

	//assume(result != NULL);
	if (!have_we_stored_a_ptr){// && nd()) {
		have_we_stored_a_ptr = TRUE;
		stored_ptr = result;
	}

	return result;
}

void dbfree(void* ptr) {

	//sassert(malloc_count > 0);
	//sassert(ptr);
	//sassert this is freed only when it isn't pointed
	if (ptr == stored_ptr) 
		have_we_freed_the_stored_ptr = TRUE;
	malloc_count--;
	free(ptr);
}

void done(int test_id) {
	//	sassert(malloc_count == 0);
	//sassert(have_we_freed_the_stored_ptr);
}

char* copy_string(char* src) {
	int res_size = db_strlen(src) + 1;
	char* res = dbmalloc(res_size);
	if (res == NULL)
		return NULL;
	db_strcpy(res, res_size ,src);
	return res;

}

char* db_strcpy(char* des, int dest_size, char* src) {
	if (des == NULL)
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


