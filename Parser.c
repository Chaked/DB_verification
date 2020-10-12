#include "Parser.h"

char DELIMETERS[] = " \t\n\v\f\r";

return_code_t parse_create(database_t* DB) {

	char* ptr = db_strtok(NULL);
	if (db_strcmp(ptr, "TABLE"))
		return FAILURE;
	char* table_name = db_strtok(NULL);
	ptr = db_strtok(NULL);
	if (db_strcmp(ptr, "("))
		return FAILURE;

	list_t* columns_declaration = NULL;
	ptr = db_strtok(NULL);
	while (ptr != NULL && db_strcmp(ptr, ")"))
	{
		column_declaration_t* column = dbmalloc(sizeof(column_declaration_t));
		column->type = string_to_type(ptr);
		column->name = copy_string(db_strtok(NULL));
		columns_declaration = add_to_the_end(columns_declaration, column);

		ptr = db_strtok(NULL);
		if (db_strcmp(ptr, ",")) // We have reached the end or there is a problem
			break;
		ptr = db_strtok(NULL);
	}
	if (!ptr || db_strcmp(ptr, ")"))
	{
		printf_s("Syntax problem in: %s", ptr);
		free_list(columns_declaration, COLUMN_DECLARATION);
		return FAILURE;
	}
	return DB_create(DB, table_name, columns_declaration);
}


return_code_t parse_drop(database_t* DB) {
	char* ptr = db_strtok(NULL);
	if (db_strcmp(ptr, "TABLE"))
		return FAILURE;
	char* table_name = db_strtok(NULL);
	return DB_drop(DB, table_name);
}


return_code_t parse_insert(database_t* DB) {
	char* table_name = db_strtok(NULL);

	list_t* column_values = NULL;
	char* ptr = db_strtok(NULL);
	if (db_strcmp(ptr, "("))
		return FAILURE;

	ptr = db_strtok(NULL);
	while (ptr != NULL && db_strcmp(ptr, ")"))
	{
		column_value_t* column = dbmalloc(sizeof(column_value_t));
		column->name = copy_string(ptr);
		ptr = db_strtok(NULL);
		if (db_strcmp(ptr, "=")) {
			dbfree(column);
			free_list(column_values, COLUMN_VALUE);
			printf_s("Syntax problem in: %s", ptr);
			return FAILURE;
		}
		ptr = db_strtok(NULL);
		if (is_number(ptr)) {
			column->value = (void*)atoi(ptr);
			column->type = INT;
		}
		else if (is_sql_format_string(ptr)) {
			column->value = parse_sql_format_string(ptr);
			column->type = STRING;
		}
		else {
			printf_s("Syntax problem in: %s", ptr);
			dbfree(column->name);
			dbfree(column);
			free_list(column_values, COLUMN_VALUE);
			return FAILURE;
		}

		column_values = add_to_the_end(column_values, column);
		ptr = db_strtok(NULL);
		if (db_strcmp(ptr, ",")) // We have reached the end or there is a problem
			break;
		ptr = db_strtok(NULL);
	}
	if (!ptr || db_strcmp(ptr, ")"))
	{
		printf_s("Syntax problem in: %s", ptr);
		free_list(column_values, COLUMN_VALUE);
		return FAILURE;
	}
	return_code_t code = DB_insert(DB, table_name, column_values);
	free_list(column_values, COLUMN_VALUE);
	return code;
}


return_code_t parse_delete(database_t* DB) {
	char* table_name = copy_string(db_strtok(NULL));
	char* ptr = db_strtok(NULL);
	list_t* conditions = NULL;
	if (!db_strcmp(ptr, "WHERE"))
		// This will return NULL on empty condition list and on any error inside parse_conditions()
		conditions = parse_conditions();
	else {
		printf_s("Syntax problem in: %s", ptr);
		return FAILURE;
	}
	return_code_t code = DB_delete(DB, table_name, conditions);
	dbfree(table_name);
	free_list(conditions, CONDITION);
	return code;
}

return_code_t parse_select(database_t* DB) {
	char* ptr = db_strtok(NULL);
	if (db_strcmp(ptr, "*"))
		return FAILURE;
	ptr = db_strtok(NULL);
	if (db_strcmp(ptr, "FROM"))
		return FAILURE;
	char* table_name = db_strtok(NULL);
	ptr = db_strtok(NULL);
	list_t* conditions = NULL;
	if (ptr && !db_strcmp(ptr, "WHERE"))
		conditions = parse_conditions();
	if (ptr && db_strcmp(ptr, "WHERE"))
		return FAILURE;

	list_t* results = DB_select(DB, table_name, conditions);

	print_results(results);
	free_list(results, ROW);
	free_list(conditions, CONDITION);
	return SUCCESS;
}

list_t* parse_conditions()
{
	list_t* conditions = NULL;
	char* ptr = db_strtok(NULL);
	while (ptr != NULL)
	{
		char* column_name = copy_string(ptr);
		char* operator = copy_string(db_strtok(NULL));
		char* value = copy_string(db_strtok(NULL));

		condition_t* condition = dbmalloc(sizeof(condition_t));
		condition->column_name = column_name;
		condition->ctype = get_ctype(operator);
		if (is_number(value)) {
			condition->value = (void*)atoi(value);
			condition->is_value_int = TRUE;
		}
		else if (is_sql_format_string(value)) {
			condition->value = parse_sql_format_string(value);
			condition->is_value_int = FALSE;
		}
		else {
			printf_s("Syntax problem in: %s", value);
			dbfree(column_name);
			dbfree(operator);
			dbfree(value);
			dbfree(condition);
			free_list(conditions, CONDITION);
			return NULL;
		}
		ptr = db_strtok(NULL);
		if (ptr != NULL && db_strcmp(ptr, "AND")) {
			printf_s("Syntax problem in: %s", ptr);
			dbfree(column_name);
			dbfree(operator);
			dbfree(value);
			dbfree(condition);
			free_list(conditions, CONDITION);
			return NULL;
		}
		dbfree(operator);
		dbfree(value);
		conditions = add_to_list(conditions, condition);
		ptr = db_strtok(NULL);
	}
	return conditions;
}

boolean_t is_sql_format_string(char* str) {
	return str[0] == '\'' && str[db_strlen(str) - 1] == '\'';
}

char* parse_sql_format_string(char* str) {
	str[db_strlen(str) - 1] = '\0';
	char* formatted = copy_string(&str[1]);
	return formatted;
}

column_type_t string_to_type(char* str_type) {
	if (!db_strcmp(str_type, "INT"))
		return INT;
	if (!db_strcmp(str_type, "STRING"))
		return STRING;
	printf_s("Rceived a bad type: %s", str_type);
	return 0;
}

boolean_t is_number(char* str) {
	if (str == NULL || !db_strcmp(str, ""))
		return FALSE;
	boolean_t result = TRUE;
	while (str[0] != '\0')
	{
		result = result && (str[0] >= '0' && str[0] <= '9');
		str++;
	}
	return result;

}

condition_type_t get_ctype(char* operator) {
	if (!db_strcmp(operator,"="))
		return EQUAL;
	if (!db_strcmp(operator,"<>"))
		return NOT_EQUAL;
	if (!db_strcmp(operator,">"))
		return BIGGER;
	if (!db_strcmp(operator,">="))
		return BIGGER_AND_EQUAL;
	if (!db_strcmp(operator,"<"))
		return SMALLER;
	if (!db_strcmp(operator,"<="))
		return SMALLER_AND_EQUAL;
	printf_s("Unknown condition operator: %s", operator);
	//sassert(FALSE);
	return EQUAL;
}

void print_results(list_t* results) {
	if (!results) {
		printf_s("No results\n");
	}
	list_t* current_row = results;
	while (current_row != NULL)
	{
		//sassert(current_row->value != NULL);
		list_t* current_column = ((row_t*)current_row->value)->values;
		while (current_column != NULL)
		{
			column_value_t* column = (column_value_t*)current_column->value;
			switch (column->type)
			{
			case INT:
				printf_s("%d", (int)column->value);
				break;
			case STRING:
				printf_s("%s", (char*)column->value);
				break;
			default:
				printf_s("Can't print this column");
				break;
			}
			printf_s("%s", current_column->next ? "," : "\n");
			current_column = current_column->next;
		}
		current_row = current_row->next;
	}
}

return_code_t parse_query(database_t* DB, char* query) {
	printf_s(">\n%s\n", query);

	char* ptr = db_strtok(query);
	if (!ptr) {
		printf_s("Please enter a command\n");
		return SUCCESS;
	}
	if (!db_strcmp(ptr, "CREATE"))
		return parse_create(DB);
	if (!db_strcmp(ptr, "DROP"))
		return parse_drop(DB);
	if (!db_strcmp(ptr, "INSERT"))
		return parse_insert(DB);
	if (!db_strcmp(ptr, "DELETE"))
		return parse_delete(DB);
	if (!db_strcmp(ptr, "SELECT"))
		return parse_select(DB);
	if (!db_strcmp(ptr, "EXIT")) {
		return FAILURE;
	}

	printf_s("Unknown command: %s", ptr);
	return FAILURE;
}

void parser_prompt() {
	printf_s("Initiliazing DB - ");
	char buffer[1000];
	database_t* DB = db_ctor();
	printf_s("OK TO GO!\n");
	return_code_t result = SUCCESS;
	while (result == SUCCESS)
	{
		putchar('>');
		fgets(buffer,1000,stdout);
		result = parse_query(DB, buffer);
		putchar('\n');
	}
	db_dtor(DB);
}