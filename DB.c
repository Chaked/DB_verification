#include "DB.h"

database_t db_ctor()
{
	return database_t;
}

return_code_t create(database_t * DB,char* table_name, list_t columns_declaration)
{
	return FAILURE;
}

return_code_t drop(database_t * DB,char* table_name)
{
	return FAILURE;
}

return_code_t insert(database_t * DB,char* table_name, list_t column_values)
{
	return FAILURE;
}

return_code_t delete(database_t * DB,char* table_name, list_t conditions)
{
	return FAILURE;
}

list_t select(database_t * DB,list_t columns_names, char* from, list_t conditions)
{
	return 0;
}

response_t exec_query(database_t * DB,char* query)
{
	return 0;
}
