
#include "DB_API.h"


char TABLE_NAME = 'U';
char IDS_COLUMN = 'I';
char NAMES_COLUMN = 'N';

// This test is a regular test case with determined commaned.
// It is used for sanity checks. Once I will make this work, I will execute the other tests.
void deterministic_test() {

	database_t* DB = db_ctor();
	char* brk = "=========================\n";
	//parse_query(DB, "CREATE TABLE users ( INT id , STRING name )");
	dbapi_create_table(DB,TABLE_NAME,INT,IDS_COLUMN,STRING,NAMES_COLUMN,INT, '\0');
	//parse_query(DB, "INSERT users ( id = 100 , name = 'shakED' )");
	dbapi_insert(DB, TABLE_NAME,IDS_COLUMN,100, '\0',NAMES_COLUMN,0,'s',/*here foward are junk values*/'\0', 0, '\0');
	dbapi_insert(DB, TABLE_NAME, IDS_COLUMN, 20, '\0', NAMES_COLUMN, 0, 'L',/*here foward are junk values*/'\0', 0, '\0');
	dbapi_insert(DB, TABLE_NAME, IDS_COLUMN, 301, '\0', NAMES_COLUMN, INT,'m' ,/*here foward are junk values*/'\0', 0, '\0');
	dbapi_select(DB, TABLE_NAME, '\0', 0, 0, '\0', '\0', 0, 0, '\0', '\0', 0, 0, '\0');
	dbapi_select(DB, TABLE_NAME, IDS_COLUMN, BIGGER_AND_EQUAL, 80, '\0', '\0', 0, 0, '\0', '\0', 0, 0, '\0');
	dbapi_delete(DB, TABLE_NAME, IDS_COLUMN, BIGGER_AND_EQUAL, 80, '\0', '\0', 0, 0, '\0', '\0', 0, 0, '\0');
	dbapi_delete(DB, TABLE_NAME, IDS_COLUMN, BIGGER_AND_EQUAL, 80, '\0', '\0', 0, 0, '\0', '\0', 0, 0, '\0');
	dbapi_select(DB, TABLE_NAME, IDS_COLUMN, BIGGER_AND_EQUAL, 80, '\0', NAMES_COLUMN, EQUAL,0, 'm', '\0', 0, 0, '\0');
	dbapi_drop_table(DB, TABLE_NAME);
	
  	db_dtor(DB);
	done(0);
}

// This a non deteministec program. It aims to prove that there are no memory leaks.
// There is a assert in done() which verifies that the ptr chosen (non deterministicaly) was freed.  
void memory_safety() {	
	database_t* DB = db_ctor();
    dbapi_create_table(DB,TABLE_NAME,INT,IDS_COLUMN,STRING,NAMES_COLUMN,INT, '\0');
	while (nd()) {
		int command = nd();
		assume(command >= 0 && command < 5);
		switch (command)
		{
		case 0: {
			//char query[] = "CREATE TABLE users ( INT id , STRING name )";
			dbapi_create_table(DB,TABLE_NAME,INT,IDS_COLUMN,STRING,NAMES_COLUMN,0, '\0');
			break; }
		case 1: {
			//char query[] = "DROP TABLE users";
			dbapi_drop_table(DB, TABLE_NAME);		 
			break; }
		case 2: {
			//char query[] = "INSERT users ( id = nd , name = nd )";
			int id = nd();
			char name = nd_str();
			dbapi_insert(DB, TABLE_NAME,IDS_COLUMN,id, '\0',NAMES_COLUMN,0,name, '\0', 0, '\0');
			break; }	
		case 3: {
			//char query[] = "DELETE users WHERE id < 290";
			int id = nd();
			char name = nd_str();
			condition_type_t cond1 = nd_cond();
			condition_type_t cond2 = nd_cond();
			dbapi_delete(DB, TABLE_NAME, IDS_COLUMN, cond1, id, '\0', NAMES_COLUMN, cond2, 0, name, '\0', 0, 0, '\0');
			break; }
		case 4: {
			//char query[] = "SELECT * FROM users WHERE id <> 300";
			int id = nd();
			char name = nd_str();
			condition_type_t cond1 = nd_cond();
			condition_type_t cond2 = nd_cond();
			dbapi_select(DB, TABLE_NAME, IDS_COLUMN, cond1, id, '\0', NAMES_COLUMN, cond2, 0, name, '\0', 0, 0, '\0');
			break; }
		default:
			break;
		}
	}
	db_dtor(DB);
	done(1);
}

int get_matching_value_i(char column_name, char c_names[] ,int c_values_i[]) {
	for (int i = 0; i < 3; i++) 
		if (column_name == c_names[i]) 
			return c_values_i[i];
	sassert(FALSE);
	return -1;
}

char get_matching_value_str(char column_name, char c_names[], char c_values_str[]) {
	for (int i = 0; i < 3; i++)
		if (column_name == c_names[i])
			return c_values_str[i];
	sassert(FALSE);
	return NULL;
}


void insert_and_select() {

	database_t* DB = db_ctor();

	// Declare the table properties non deterministicaly 
	char table_name = nd_str();
	column_type_t c_types[3];
	char c_names[3];
	for (int i = 0; i < 3; i++)
		c_types[i] = nd_column_t();
	for (int i = 0; i < 3; i++)
		c_names[i] = nd_str();

	dbapi_create_table(DB, table_name, c_types[0], c_names[0], c_types[1], c_names[1], c_types[2], c_names[2]);

	// Asserts the table is empty
	list_t* results = DB_select(DB, table_name, NULL);
	sassert(results == NULL);
	
	// Add a row with non determinstic values
	int c_values_i[3];
	char c_values_str[3];
	for (int i = 0; i < 3; i++) {
		c_values_i[i] = nd();
		c_values_str[i] = nd_str();
		if (c_types[i] == INT)
			assume(c_values_i[i] == 0);
		else
			assume(c_values_str[i] == '\0');
	}
	dbapi_insert(DB, table_name, c_names[0], c_values_i[0],	c_values_str[0], c_names[1], c_values_i[1], c_values_str[1], c_names[2], c_values_i[2], c_values_str[2]);

	// Check we can retrive the non determinstic row
	results = DB_select(DB, table_name, NULL);
	sassert(results && results->next == NULL);
	
	//sassert(current_row->value != NULL);
	list_t* current_column = ((row_t*)results->value)->values;
	while (current_column != NULL)
	{
		column_value_t* column = (column_value_t*)current_column->value;
		switch (column->type)
		{
		case INT:
			sassert(column->value.i == get_matching_value_i(column->name, c_names, c_values_i));
			break;
		case STRING:
			sassert(column->value.str == get_matching_value_str(column->name, c_names, c_values_str));
			break;
		default:
			sassert(FALSE);
			break;
		}
	}
	free_list(results, ROW);
	db_dtor(DB);
	done(1);
}

int main() {
	deterministic_test();
	insert_and_select();
	memory_safety();
}
