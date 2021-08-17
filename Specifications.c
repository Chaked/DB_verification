
#include "DB_API.h"

// This test is a regular test case with determined commaned.
// It is used for sanity checks. Once I will make this work, I will execute the other tests.
void deterministic_test() {

	char* TABLE_NAME = "u";
	char* IDS_COLUMN = "i";
	char* NAMES_COLUMN = "n";

	database_t* DB = db_ctor();
	char* brk = "=========================\n";
	//parse_query(DB, "CREATE TABLE users ( INT id , STRING name )");
	dbapi_create_table(DB,TABLE_NAME,INT,IDS_COLUMN,STRING,NAMES_COLUMN,INT,NULL);
	//parse_query(DB, "INSERT users ( id = 100 , name = 'shakED' )");
	dbapi_insert(DB,TABLE_NAME,IDS_COLUMN,100,NULL,NAMES_COLUMN,0,"S",/*here foward are junk values*/NULL, 0, NULL);
	dbapi_insert(DB, TABLE_NAME, IDS_COLUMN, 20, NULL, NAMES_COLUMN, 0, "M",/*here foward are junk values*/NULL, 0, NULL);
	dbapi_insert(DB, TABLE_NAME, IDS_COLUMN, 301, NULL, NAMES_COLUMN, 0, "L",/*here foward are junk values*/NULL, 0, NULL);
	dbapi_select(DB, TABLE_NAME, NULL, 0, 0, NULL, NULL, 0, 0, NULL, NULL, 0, 0, NULL);
	dbapi_select(DB, TABLE_NAME, IDS_COLUMN, BIGGER_AND_EQUAL, 80, NULL, NULL, 0, 0, NULL, NULL, 0, 0, NULL);
	dbapi_select(DB, TABLE_NAME, IDS_COLUMN, BIGGER_AND_EQUAL, 80, NULL, NAMES_COLUMN, EQUAL, 0, "L", NULL, 0, 0, NULL);
	dbapi_drop_table(DB, TABLE_NAME);
	// When above succeed, change all the calls bellow to send a variable and not the string itself
	/*
	parse_query(DB, "INSERT users ( id = 100 , name = 'shakED' )");
	parse_query(DB, "SELECT * FROM users");
	parse_query(DB, "INSERT users ( id = 200 , name = 'Miel' )");
	parse_query(DB, "SELECT * FROM users");
	parse_query(DB, "INSERT users ( id = 300 , name = 'LiyaHallel' )");
	parse_query(DB, "SELECT * FROM users");
	//printf(brk);
	parse_query(DB, "SELECT * FROM users WHERE id = 300");
	//printf(brk);
	parse_query(DB, "SELECT * FROM users WHERE id <> 300");
	//printf(brk);
	parse_query(DB, "SELECT * FROM users WHERE id <= 300");
	//printf(brk);
	parse_query(DB, "SELECT * FROM users WHERE id <= 300 AND name = 'Miel'");
	//printf(brk);
	parse_query(DB, "DELETE users WHERE id < 300");
	//printf(brk);
	parse_query(DB, "CREATE TABLE users2 ( INT id , STRING name )");
	parse_query(DB, "SELECT * FROM users WHERE id = 300");
	//printf(brk);
	parse_query(DB, "SELECT * FROM users WHERE id <> 300");
	//printf(brk);
	parse_query(DB, "SELECT * FROM users WHERE id <= 300");
	//printf(brk);
	parse_query(DB, "INSERT users2 ( id = 1001 , name = 'shakED1' )");
	parse_query(DB, "INSERT users2 ( id = 2001 , name = 'Miel1' )");
	parse_query(DB, "INSERT users2 ( id = 3001 , name = 'LiyaHallel1' )");
	parse_query(DB, "SELECT * FROM users WHERE id <= 300 AND name = 'Miel'");
	//printf(brk);
	parse_query(DB, "SELECT * FROM users2");
	//printf(brk);
	parse_query(DB, "DROP TABLE users2");
	parse_query(DB, "SELECT * FROM users WHERE id <= 300 AND name = 'Miel'");
	//printf(brk);
	parse_query(DB, "SELECT * FROM users2");
	//printf(brk);
	*/
  	db_dtor(DB);
	done(0);
}

/*
// This a non deteministec program. It aims to prove that there are no memory leaks.
// There is a assert in done() which verifies that the ptr chosen (non deterministicaly) was freed.  
void no_mem_leaks() {
	database_t* DB = db_ctor();

	char buffer[1000];
	parse_query(DB, "CREATE TABLE users ( INT id , STRING name )");
	int id = nd();
	while (nd()) {
		int command = nd();
		assume(command >= 0 && command < 8);
		switch (command)
		{
		case 0: {
			char query[] = "CREATE TABLE users ( INT id , STRING name )";
			parse_query(DB, query); // Seahorn crashes if the query is sent directly to parse_query(). 
			break; }

		case 1: {
			char query[] = "DROP TABLE users";
			parse_query(DB, query); // Seahorn crashes if the query is sent directly to parse_query(). 
			break; }
		case 2: {
			char query[] = "INSERT users ( id = 100 , name = 'AAa' )";
			parse_query(DB, query); // Seahorn crashes if the query is sent directly to parse_query(). 
			break; }
		case 3: {
			char query[] = "INSERT users ( id = 200 , name = 'BBb' )";
			parse_query(DB, query); // Seahorn crashes if the query is sent directly to parse_query(). 
			break; }
		case 4: {
			char query[] = "INSERT users ( id = 300 , name = 'CCc' )";
			parse_query(DB, query); // Seahorn crashes if the query is sent directly to parse_query(). 
			break; }
		case 5: {
			char query[] = "DELETE users WHERE id < 290";
			parse_query(DB, query); // Seahorn crashes if the query is sent directly to parse_query(). 
			break; }
		case 6: {
			char query[] = "DELETE users WHERE id = 300";
			parse_query(DB, query); // Seahorn crashes if the query is sent directly to parse_query(). 
			break; }
		case 7: {
			char query[] = "SELECT * FROM users WHERE id <> 300";
			parse_query(DB, query); // Seahorn crashes if the query is sent directly to parse_query(). 
			break; }
		default:
			break;
		}
	}
	db_dtor(DB);
	done(1);
}

void insert_and_select() {
	database_t* DB = db_ctor();

	char create_query[] = "CREATE TABLE users ( INT id , STRING name )";
	parse_query(DB, create_query); // Seahorn crashes if the query is sent directly to parse_query(). 

	char* id = str_nd();
	assume(is_number(id));
	char* prefix = "INSERT users ( id = ";
	char* suffix = " , name = 'SlimShady' )";
	char* half_query = db_concat(prefix, id);
	char* query = db_concat(half_query, suffix);
	db_free(half_query);
	parse_query(DB,query);
	char* condition = db_concat("WHERE id = ",id);
	char* ptr = db_strtok(condition);
	list_t* conditions = parse_conditions();
	
	list_t* results = DB_select(DB, "users", conditions);
	sassert(results->next == NULL);
	sassert((((column_value_t*)(((row_t*)(results->value))->values->next->value))->value.i) == db_atoi(id));

	db_free(query);
	db_free(condition);
	free_list(conditions, CONDITION);
	db_dtor(DB);
}*/


int main() {
	deterministic_test();
	//no_mem_leak(DB);
	//insert_and_select();
}