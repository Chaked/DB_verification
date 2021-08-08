#include "DB_API.h"

return_code_t parse_create(database_t* DB);
return_code_t parse_drop(database_t* DB);
return_code_t parse_insert(database_t* DB);
return_code_t parse_delete(database_t* DB);
return_code_t parse_select(database_t* DB);
return_code_t parse_query(database_t* DB, char* query);
char* parse_sql_format_string(char* str);
int db_atoi(char* str);
boolean_t is_number(char* str);
column_type_t string_to_type(char* str_type);
condition_type_t get_ctype(char* operator);
list_t* parse_conditions();
void print_results(list_t* results);
void parser_prompt();
