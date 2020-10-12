#include "DB.h"




list_t* get_table_object_from_DB(database_t* DB, char* table_name) {
	list_t* current = DB->tables;
	while (current != NULL)
	{
		table_t* current_table = current->value;
		//sassert(current_table != NULL);
		if (!db_strcmp(table_name, current_table->name))
			return current;
		current = current->next;
	}
	//assume(current == NULL);
	return NULL;
}


row_t* column_list_to_row(list_t* column_values, table_t* table) {
	if (table == NULL ||
		column_values == NULL ||
		get_list_size(column_values) != get_list_size(table->columns) ||
		!check_row_validity(table, column_values))
		return NULL;
	list_t* copy = NULL;
	while (column_values != NULL) {
		//sassume(column_values->value != NULL);
		copy = add_to_list(copy, (void*)copy_column_value(column_values->value));
		column_values = column_values->next;
	}
	row_t* result = dbmalloc(sizeof(row_t));
	result->values = copy;
	return result;
}

//Deep copy of a column value
column_value_t* copy_column_value(column_value_t* column_value) {
	column_value_t* copy = dbmalloc(sizeof(column_value_t));
	copy->name = dbmalloc(db_strlen(column_value->name) + 1);
	db_strcpy(copy->name, db_strlen(column_value->name) + 1,column_value->name);
	copy->type = column_value->type;
	switch (copy->type) {
	case INT:
		copy->value = column_value->value;
		break;
	case STRING:
		copy->value = dbmalloc(db_strlen(column_value->value) + 1);
		db_strcpy(copy->value, db_strlen(column_value->value) + 1,column_value->value);
		break;
	default:
		//sassert(FALSE);
		break;
	}
	return copy;
}


boolean_t check_row_validity(table_t* table, list_t* column_values) {
	list_t* column_index = table->columns;

	while (column_values != NULL)
	{

		if (column_index == NULL)
			return FALSE;

		column_value_t* current = column_values->value;
		column_value_t* current_index = column_index->value;

		if (db_strcmp(current->name, current_index->name))
			return FALSE;
		if (current->type != current_index->type)
			return FALSE;
		column_index = column_index->next;
		column_values = column_values->next;
	}

	return column_index == NULL ? TRUE : FALSE;
}


// Returns everytime the first row to satisfy the condition
list_t* get_row_to_delete(table_t* table, list_t* conditions) {
	list_t* current_row = table->rows;
	while (current_row)
	{
		if (row_satisfy_condition(current_row->value, conditions, table))
			return current_row;
		current_row = current_row->next;
	}
	//No row satisfy the condition.
	return NULL;
}


//This returns the pointer if this is true and NULL if false
row_t* row_satisfy_condition(row_t* row, list_t* conditions, table_t* table) {
	if (conditions == NULL)
		return row;
	boolean_t result = TRUE;
	list_t* current_column = row->values;
	while (current_column)
	{
		column_value_t* column = current_column->value;
		list_t* current_condition = conditions;
		while (current_condition)
		{
			condition_t* condition = current_condition->value;


			if (db_strcmp(condition->column_name, column->name)) {
				current_condition = current_condition->next;
				continue;
			}
			if (!condition_is_valid(condition, column->type))
				return NULL;
			//If we are here, the condition matches our column and is valid.
			switch (column->type)
			{
			case STRING:
				result = result && string_condition(column->value, condition->value, condition);
				break;
			case INT:
				result = result && int_condition((int)column->value, (int)condition->value, condition);
				break;
			default:
				break;
			}
			current_condition = current_condition->next;
		}
		current_column = current_column->next;
	}
	return result ? row : NULL;
}

boolean_t string_condition(char* l_operand, char* r_operand, condition_t* condition) {
	switch (condition->ctype)
	{
	case EQUAL:
		return !db_strcmp(l_operand, r_operand);
	case NOT_EQUAL:
		return db_strcmp(l_operand, r_operand);
	default:
		printf_s("Illegal string condition");
		break;
	}
	//sassert(FALSE);
	//Undefiend behaviuor
	return FALSE;
}

boolean_t int_condition(int l_operand, int r_operand, condition_t* condition) {
	switch (condition->ctype)
	{
	case EQUAL:
		return l_operand == r_operand;
	case NOT_EQUAL:
		return l_operand != r_operand;
	case BIGGER:
		return l_operand > r_operand;
	case BIGGER_AND_EQUAL:
		return l_operand >= r_operand;
	case SMALLER:
		return l_operand < r_operand;
	case SMALLER_AND_EQUAL:
		return l_operand <= r_operand;

	default:
		printf_s("Illegal string condition");
		break;
	}
	//sassert(FALSE);
	//Undefiend behaviuor
	return FALSE;
}
boolean_t condition_is_valid(condition_t* condition, column_type_t type) {
	return condition != NULL &&
		((type == STRING && !condition->is_value_int &&
		(condition->ctype == EQUAL ||
			condition->ctype == NOT_EQUAL)) ||
			(type == INT && condition->is_value_int));
}



row_t* copy_row(row_t* row_to_copy) {
	row_t* new_row = dbmalloc(sizeof(row_t));
	//memcpy(new_row, row_to_copy, sizeof(row_t));
	new_row->values = NULL;
	list_t* current_column = row_to_copy->values;
	while (current_column != NULL)
	{
		//Create the row
		column_value_t* new_column = dbmalloc(sizeof(column_value_t));
		memcpy(new_column, current_column->value, sizeof(column_value_t));
		new_column->name = copy_string(new_column->name);
		if (new_column->type == STRING) 
			new_column->value = copy_string(new_column->value);
		new_row->values = add_to_the_end(new_row->values, new_column);
		current_column = current_column->next;
	}
	return new_row;
}


database_t* db_ctor()
{
	database_t* DB = (database_t*)dbmalloc(sizeof(database_t));
	DB->tables = NULL;
	//sassert(malloc_count == 0);
	return DB;
}

void db_dtor(database_t* DB)
{
	free_list(DB->tables, TABLE);
	/*list_t* current_table = DB->tables;
	while (current_table != NULL) {
		table_t* table = current_table->value;
		free_list(table->columns);
		list_t* current_row = table->rows;
		while (current_row != NULL)
		{
			free_list(((row_t*)current_row->value)->values);
			dbfree(current_row->value);
			list_t* to_delete = current_row;
			current_row = current_row->next;
			dbfree(current_row);
		}
		list_t* to_delete = current_table;
		current_table = current_table->next;
		dbfree(to_delete);

	}*/
	dbfree(DB);
	//sassert(malloc_count == 0);
}

return_code_t DB_create(database_t* DB,  char* table_name, list_t* columns_declaration)
{
	table_t* table = dbmalloc(sizeof(table_t));
	if (table == NULL)
		return FAILURE;
	table->columns = columns_declaration;
	table->name = copy_string(table_name);
	table->rows = NULL;

	DB->tables = add_to_list(DB->tables, table);
	//sassert(DB->tables != NULL);
	return SUCCESS;
}

return_code_t DB_drop(database_t* DB,  char* table_name)
{
	list_t* table_node = get_table_object_from_DB(DB, table_name);
	if (table_node == NULL)
		//This list doesn't contain that table, nothing to remove.
		return SUCCESS;
	DB_delete(DB, table_name, NULL);
	list_t* result = remove_from_list(DB->tables, table_node,TABLE);
	if (result == NULL)
		return FAILURE;
	DB->tables = result;
	return SUCCESS;
}

return_code_t DB_insert(database_t* DB,  char* table_name, list_t* column_values)
{
	list_t* table_node = get_table_object_from_DB(DB, table_name);
	if (!table_node) {
		printf_s("No such table: %s\n", table_name);
		return FAILURE;
	}
	table_t* table = (table_t*)table_node->value;
	row_t* row = column_list_to_row(column_values, table);
	list_t* result = add_to_list(table->rows, row);
	if (result == NULL)
		return FAILURE;
	table->rows = result;
	return SUCCESS;
}

// If the condition is empty, all rows will be deleted.
return_code_t DB_delete(database_t* DB,  char* table_name, list_t* conditions)
{
	int deleted = 0;
	list_t* table_node = get_table_object_from_DB(DB, table_name);
	table_t* table = (table_t*)table_node->value;
	list_t* to_delete = get_row_to_delete(table, conditions);
	while (to_delete) {
		list_t* result = remove_from_list(table->rows, to_delete,ROW);
		table->rows = result;
		deleted += to_delete ? 1 : 0;
		to_delete = get_row_to_delete(table, conditions);
	}
	printf_s("%d rows were deleted", deleted);
	return SUCCESS;
}


list_t* DB_select(database_t* DB,  char* from, list_t* conditions)
{
	list_t* table_node = get_table_object_from_DB(DB, from);
	if (!table_node) {
		printf_s("No such table: %s\n", from);
		return NULL;
	}
	table_t* table = (table_t*)table_node->value;
	list_t* results = NULL;
	list_t* current = table->rows;
	int results_count = 0;
	while (current)
	{
		if (row_satisfy_condition(current->value, conditions, table)) {
			row_t* to_add = copy_row((row_t*)current->value);
			if (!to_add) {
				printf_s("Problem allocating memory");
				free_list(results, ROW);
				return NULL;
			}
			list_t* temp_results = add_to_list(results, to_add);
			if (!temp_results) {
				printf_s("Problem allocating memory");
				free_list(results, ROW);
				return NULL;
			}
			results_count++;
			results = temp_results;
		}
		current = current->next;
	}
	return results;
}

