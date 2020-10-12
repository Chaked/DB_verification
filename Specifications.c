
#include "Parser.h"


void test(database_t* DB) {
	char* brk = "=========================\n";
	char query[] = "CREATE TABLE users ( INT id , STRING name )";
	parse_query(DB, query);
	/*parse_query(DB, "INSERT users ( id = 100 , name = 'shakED' )");
	parse_query(DB, "SELECT * FROM users");
	parse_query(DB, "INSERT users ( id = 200 , name = 'Miel' )");
	parse_query(DB, "SELECT * FROM users");
	parse_query(DB, "INSERT users ( id = 300 , name = 'LiyaHallel' )");
	parse_query(DB, "SELECT * FROM users");
	//printf_s(brk);
	parse_query(DB, "SELECT * FROM users WHERE id = 300");
	//printf_s(brk);
	parse_query(DB, "SELECT * FROM users WHERE id <> 300");
	//printf_s(brk);
	parse_query(DB, "SELECT * FROM users WHERE id <= 300");
	//printf_s(brk);
	parse_query(DB, "SELECT * FROM users WHERE id <= 300 AND name = 'Miel'");
	//printf_s(brk);
	parse_query(DB, "DELETE users WHERE id < 300");
	//printf_s(brk);
	parse_query(DB, "CREATE TABLE users2 ( INT id , STRING name )");
	parse_query(DB, "SELECT * FROM users WHERE id = 300");
	//printf_s(brk);
	parse_query(DB, "SELECT * FROM users WHERE id <> 300");
	//printf_s(brk);
	parse_query(DB, "SELECT * FROM users WHERE id <= 300");
	//printf_s(brk);
	parse_query(DB, "INSERT users2 ( id = 1001 , name = 'shakED1' )");
	parse_query(DB, "INSERT users2 ( id = 2001 , name = 'Miel1' )");
	parse_query(DB, "INSERT users2 ( id = 3001 , name = 'LiyaHallel1' )");
	parse_query(DB, "SELECT * FROM users WHERE id <= 300 AND name = 'Miel'");
	//printf_s(brk);
	parse_query(DB, "SELECT * FROM users2");
	//printf_s(brk);
	parse_query(DB, "DROP TABLE users2");
	parse_query(DB, "SELECT * FROM users WHERE id <= 300 AND name = 'Miel'");
	//printf_s(brk);
	parse_query(DB, "SELECT * FROM users2");
	//printf_s(brk);
	*/
	

	return;
}

void specs(database_t* DB) {
	char buffer[1000];
	parse_query(DB, "CREATE TABLE users ( INT id , STRING name )");
	int id = 6;// nd();
	//sprintf_s(buffer, "INSERT users ( id = %d , name = 'shakED' )", id);
	parse_query(DB, buffer);
}


int main() {
	//printf_s("Initiliazing DB - ");
	char buffer[1000];
	database_t* DB = db_ctor();
	//printf_s("OK TO GO!\n");
	test(DB);
	//specs(DB);
	db_dtor(DB);
	done();
}