#pragma once
/*
Our DB will support the following queries:

"CREATE TABLE <Table Name> (
	<Column Name>, <Column type>\n
	.
	.
	.
	<Column Name>, <Column type>)
"
"DROP TABLE <Table Name>"

"INSERT <Table Name> (
	<Column Name>=<Value>\n

	)"

"DELETE <Table Name> WHERE <Condition>"

"SELECT <Column1>,...,<ColumnN> FROM <Table Name> WHERE <Condition> "

*/

typedef enum _column_type {INT = 0, STRING = 1} column_type_t;

typedef enum _condition_type { EQUAL = 0, NOT_EQUAL = 1, BIGGER =2, BIGGER_AND_EQUAL =3 } condition_type_t ;

typedef enum _boolean { FALSE = 0, TRUE = 1} boolean;

typedef enum _return_code { SUCCESS = 0, FAILURE = 1 } return_code_t;


typedef struct _list {
	list_t * next;
	void * value;
} list_t;

typedef struct _column_declaration {
	char* name;
	column_type_t type;
} column_declaration_t;


typedef struct _column_value{
	char* name;
	column_type_t type;
	void* value;
} column_value_t;


typedef struct _condition {
	condition_type_t ctype;
	boolean l_operand_is_column;
	char* l_operand;
	boolean r_operand_is_column;
	char* r_operand;
} condition_t;

typedef struct _response {
	boolean is_list;
	list_t results;
	return_code_t code;
} response_t;

return_code_t create(char* table_name, list_t columns_declaration);

return_code_t drop(char* table_name);

return_code_t insert(char* table_name, list_t column_values);

return_code_t delete(char* table_name, list_t conditions);

list_t select(list_t columns_names, char* from, list_t conditions);

response_t exec_query(char* query);