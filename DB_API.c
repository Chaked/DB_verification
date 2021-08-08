#include "DB_API.h"

column_declaration_t* column_declaration_ctor(column_type_t col_t, char* col_n) {
	column_declaration_t* column = db_malloc(sizeof(column_declaration_t));
	column->type = col_t;
	column->name = copy_string(col_n);
	return column;
}

column_value_t* column_value_ctor( char * col_n, int col_v_i,char* col_v_str) {
	column_value_t* column = db_malloc(sizeof(column_value_t));
	column->type = col_v_str?STRING:INT;
	column->name = copy_string(col_n);
	if(column->type == INT)
		column->value.i = col_v_i;
	else
		column->value.str = copy_string(col_v_str);
	return column;
}

condition_t* condition_ctor(char* col_n, condition_type_t col_ct, int col_v_i, char* col_v_str) {
	condition_t* condition = db_malloc(sizeof(condition_t));
	condition->column_name = copy_string(col_n);
	condition->ctype = col_ct;
	if (col_v_str == NULL) {
		condition->value.i = col_v_i;
		condition->is_value_int = TRUE;
	}
	else {
		condition->value.str = copy_string(col_v_str);
		condition->is_value_int = FALSE;
	}
	return condition;
}


return_code_t dbapi_create_table(database_t* DB, char* table_name, column_type_t col_t1, char* col_n1, column_type_t col_t2, char* col_n2, column_type_t col_t3, char* col_n3) {

	if (col_n1 == NULL)
		return FAILURE;

	list_t* columns_declarations = NULL;
	if (col_n1) {
		columns_declarations = add_to_the_end(columns_declarations, column_declaration_ctor(col_t1, col_n1));
		if (col_n2) {
			columns_declarations = add_to_the_end(columns_declarations, column_declaration_ctor(col_t2, col_n2));
			if (col_n3) {
				columns_declarations = add_to_the_end(columns_declarations, column_declaration_ctor(col_t3, col_n3));
			}
		}
	}
	return DB_create(DB, table_name, columns_declarations);
}

return_code_t dbapi_drop_table(database_t* DB, char* table_name) {
	DB_drop(DB, table_name);
}

return_code_t dbapi_insert(database_t* DB, char* table_name, char* col_n1, int col_v_i1, char* col_v_str1,  char* col_n2, int col_v_i2, char* col_v_str2,  char* col_n3, int col_v_i3, char* col_v_str3) {
	if (col_n1 == NULL)
		return FAILURE;
	list_t* column_values = NULL;
	if (col_n1) {
		column_values = add_to_the_end(column_values, column_value_ctor(col_n1,col_v_i1, col_v_str1));
		if (col_n2) {
			column_values = add_to_the_end(column_values, column_value_ctor( col_n2, col_v_i2, col_v_str2));
			if (col_n3) {
				column_values = add_to_the_end(column_values, column_value_ctor( col_n3, col_v_i3, col_v_str3));
			}
		}
	}
	DB_insert(DB, table_name, column_values);
	free_list(column_values, COLUMN_VALUE);
}


return_code_t dbapi_delete(database_t* DB, char* table_name, char* col_n1, condition_type_t cond_t1, int col_v_i1, char* col_v_str1, char* col_n2, condition_type_t cond_t2, int col_v_i2, char* col_v_str2, char* col_n3, condition_type_t cond_t3, int col_v_i3, char* col_v_str3) {

	list_t* conditions = NULL;

	if (col_n1) {
		conditions = add_to_the_end(conditions, condition_ctor(col_n1, cond_t1, col_v_i1, col_v_str1));
		if (col_n2) {
			conditions = add_to_the_end(conditions, condition_ctor(col_n2, cond_t2, col_v_i2, col_v_str2));
			if (col_n3) {
				conditions = add_to_the_end(conditions, condition_ctor(col_n3, cond_t3, col_v_i3, col_v_str3));
			}
		}
	}

	return_code_t code = DB_delete(DB, table_name, conditions);
	free_list(conditions, CONDITION);
	return code;

}

return_code_t dbapi_select(database_t* DB, char* table_name, char* col_n1, condition_type_t cond_t1, int col_v_i1, char* col_v_str1, char* col_n2, condition_type_t cond_t2, int col_v_i2, char* col_v_str2, char* col_n3, condition_type_t cond_t3, int col_v_i3, char* col_v_str3) {

	list_t* conditions = NULL;
	
	if (col_n1) {
		conditions = add_to_the_end(conditions, condition_ctor(col_n1, cond_t1, col_v_i1, col_v_str1));
		if (col_n2) {
			conditions = add_to_the_end(conditions, condition_ctor(col_n2, cond_t2, col_v_i2, col_v_str2));
			if (col_n3) {
				conditions = add_to_the_end(conditions, condition_ctor(col_n3, cond_t3, col_v_i3, col_v_str3));
			}
		}
	}

	list_t* results = DB_select(DB, table_name, conditions);
	print_results(results);
	free_list(results, ROW);
	free_list(conditions, CONDITION);
	return SUCCESS;
}



