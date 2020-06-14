#include "DB.h"

return_code_t create(char* table_name, list_t columns_declaration)
{
	return FAILURE;
}

return_code_t drop(char* table_name)
{
	return FAILURE;
}

return_code_t insert(char* table_name, list_t column_values)
{
	return FAILURE;
}

return_code_t delete(char* table_name, list_t conditions)
{
	return FAILURE;
}

list_t select(list_t columns_names, char* from, list_t conditions)
{
	return list_t();
}

response_t exec_query(char* query)
{
	return response_t();
}
