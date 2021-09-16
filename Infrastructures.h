#include "seahorn/seahorn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum _column_type { INT = 0, STRING = 1 } column_type_t;

typedef enum _condition_type { EQUAL = 0, NOT_EQUAL = 1, BIGGER = 2, BIGGER_AND_EQUAL = 3, SMALLER = 4, SMALLER_AND_EQUAL = 5 } condition_type_t;

typedef enum _boolean { FALSE = 0, TRUE = 1 } boolean_t;

typedef enum _return_code { SUCCESS = 0, FAILURE = 1 } return_code_t;

typedef enum _type { COLUMN_DECLARATION = 0, COLUMN_VALUE = 1, CONDITION = 2, TABLE = 3, ROW = 4 } type_t;

//An abstract list
typedef struct _list {
	struct _list* next;
	void* value;
} list_t;

typedef union _value {
	int i;
	char str;
} value_t;


//An object defining a clomun
typedef struct _column_declaration {
	char name;
	column_type_t type;
} column_declaration_t;

//An object holding values that are assigned to columns
typedef struct _column_value {
	char name;
	column_type_t type;
	value_t value;
} column_value_t;

typedef struct _condition {
	condition_type_t ctype;
	boolean_t is_value_int;
	char column_name;
	value_t value;//This is char or int
} condition_t;

typedef struct _response {
	boolean_t is_list;
	//List of row_t*
	list_t* results;
	return_code_t code;
} response_t;

typedef struct _table {
	char name;
	//List of column_declaration_t*
	list_t* columns;
	//List of row_t*
	list_t* rows;
} table_t;

typedef struct _row {
	//List of column_value_t*
	list_t* values;
} row_t;

typedef struct _database {
	//List of table_t*
	list_t* tables;
} database_t;


void* db_malloc(size_t size);
void db_free(void* pointer);
void done(int test_id);

//SEAHORN SECTION
extern int nd(void);
extern char nd_str(void);
extern condition_type_t nd_cond(void);
extern column_type_t nd_column_t(void);
extern void* nd_ptr(void);

