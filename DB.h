
#include "List.h"


/*
Our DB will support the following queries:

"CREATE TABLE <Table Name> (
	 <Column type> <Column Name> ,
	.
	.
	.
	<Column type> <Column Name>
	)
"

"DROP TABLE <Table Name>"

"INSERT <Table Name> (
	<Column Name> = <Value> ,
	.
	.
	.
	<Column Name> = <Value>
	)"

"DELETE <Table Name> WHERE <Condition>"

"SELECT * FROM <Table Name> WHERE
  <COLUMN NAME> < [=, <>, >, >=, <, <=] > <VALUE> 
  AND <Condition> 
  .
  .
  .
  AND <Condition> "

  NOTE: String columns support only = and <> conditions.
*/

database_t* db_ctor();

void db_dtor(database_t* DB);

return_code_t DB_create(database_t* DB,  char* table_name, list_t* columns_declaration);

return_code_t DB_drop(database_t* DB,  char* table_name);

return_code_t DB_insert(database_t* DB,  char* table_name, list_t* column_values);

return_code_t DB_delete(database_t* DB,  char* table_name, list_t* conditions);

list_t* DB_select(database_t* DB,  char* from, list_t* conditions);



//Internal functions that are here due to C99 problem with implicit declarations
list_t* get_table_object_from_DB(database_t* DB, char* table_name);
row_t* column_list_to_row(list_t* column_values, table_t* table);
column_value_t* copy_column_value(column_value_t* column_value);

boolean_t check_row_validity(table_t* table, list_t* column_values);
list_t* get_row_to_delete(table_t* table, list_t* conditions);
row_t* row_satisfy_condition(row_t* row, list_t* conditions, table_t* table);
boolean_t string_condition(char* l_operand, char* r_operand, condition_t* condition);
boolean_t int_condition(int l_operand, int r_operand, condition_t* condition);
boolean_t condition_is_valid(condition_t* condition, column_type_t type);
row_t* copy_row(row_t* current);
boolean_t is_sql_format_string(char* value);
