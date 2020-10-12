#include "List.h"

return_code_t free_list_node_rec(list_t* to_delete, type_t value_type) {
	void* value = to_delete->value;
	switch (value_type)
	{
	case COLUMN_DECLARATION:
		dbfree(((column_declaration_t*)value)->name);
		break;
	case COLUMN_VALUE:
		dbfree(((column_value_t*)value)->name);
		if (((column_value_t*)value)->type == STRING)
			dbfree(((column_value_t*)value)->value);
		break;
	case CONDITION:
		dbfree(((condition_t*)value)->column_name);
		if (!((condition_t*)value)->is_value_int)
			dbfree(((condition_t*)value)->value);
		break;
	case TABLE: {
		table_t* table = (table_t*)value;
		dbfree(table->name);
		free_list(table->columns, COLUMN_DECLARATION);
		free_list(table->rows, ROW);
		break;
	}
	case ROW:
		free_list(((row_t*)value)->values, COLUMN_VALUE);
		break;
	default:
		//printf_s("Received unknown list type to delete: %d", value_type);
		return FAILURE;
	}
	dbfree(to_delete->value);
	dbfree(to_delete);
	return SUCCESS;
}


typedef struct _state {
	list_t* to_delete;
	type_t value_type;
} state_t;

list_t* push_state(list_t * states, list_t* to_delete, type_t value_type) {
	state_t* state = dbmalloc(sizeof(state_t));
	state->to_delete = to_delete;
	state->value_type = value_type;
	return add_to_list(states, state);
}

list_t* push_to_state_list_of_nodes(list_t* given_states, list_t* list, type_t value_type) {
	list_t* states = given_states;
	while (list != NULL)
	{
		list_t* to_delete = list;
		list = list->next;
		states = push_state(states, to_delete, value_type);
	}
	return states;
}


return_code_t free_list_node(list_t* to_delete, type_t value_type) {
	list_t* states = NULL;
	states = push_state(states, to_delete, value_type);
	while (states) {
		list_t* current_state_list_node = states;
		states = states->next;
		state_t* current_state = (state_t*)current_state_list_node->value; // Peek at the first state
		void* value = current_state->to_delete->value;
		switch (current_state->value_type)
		{
		case COLUMN_DECLARATION:
			dbfree(((column_declaration_t*)value)->name);
			break;
		case COLUMN_VALUE:
			dbfree(((column_value_t*)value)->name);
			if (((column_value_t*)value)->type == STRING)
				dbfree(((column_value_t*)value)->value);
			break;
		case CONDITION:
			dbfree(((condition_t*)value)->column_name);
			if (!((condition_t*)value)->is_value_int)
				dbfree(((condition_t*)value)->value);
			break;
		case TABLE: {
			table_t* table = (table_t*)value;
			dbfree(table->name);
			states = push_to_state_list_of_nodes(states, table->rows, ROW);
			states = push_to_state_list_of_nodes(states, table->columns, COLUMN_DECLARATION);
			break;
		}
		case ROW:
			states = push_to_state_list_of_nodes(states, ((row_t*)value)->values, COLUMN_VALUE);
			break;
		default:
			//printf_s("Received unknown list type to delete: %d", current_state->value_type);
			return FAILURE;
		}
		dbfree(current_state->to_delete->value);
		dbfree(current_state->to_delete);
		dbfree(current_state);
		dbfree(current_state_list_node);
	}
	return SUCCESS;
}

return_code_t free_list(list_t* list, type_t value_type) {
	while (list != NULL)
	{
		list_t* to_delete = list;
		list = list->next;
		return_code_t code = free_list_node(to_delete, value_type);
		if (code == FAILURE)
			return FAILURE;
	}
	return SUCCESS;
}


list_t* add_to_list(list_t* list, void* value) {
	list_t* node = dbmalloc(sizeof(list_t));
	if (node == NULL)
		return NULL;

	node->value = value;
	node->next = list; // Equivalent to: (list == NULL ? NULL : list)
	return node;
}

list_t* add_to_the_end(list_t* list, void* value) {
	//Create the list node
	list_t* new_node = dbmalloc(sizeof(list_t));
	new_node->value = value;
	new_node->next = NULL;

	list_t* last_node = list;

	while (last_node != NULL && last_node->next != NULL)
		last_node = last_node->next;
	if (last_node == NULL)
		return new_node;

	last_node->next = new_node;
	return list;
}


// This function will return the new list without the element to remove.
list_t* remove_from_list(list_t* list, list_t* to_remove, type_t value_type) {
	if (list == NULL)
		return NULL;
	if (to_remove == NULL) {
		return list;
	}
	// If to_remove is the first node, we return the next node 
	// and we don't have to deal with swapping pointers
	if (list == to_remove) {
		list_t* next = list->next;
		free_list_node(to_remove, value_type);
		return next;
	}

	list_t* previous = list;
	list_t* current = list->next;
	int list_contains_to_remove = 0;
	while (current) {
		if (current == to_remove) {
			list_contains_to_remove = 1;
			break;
		}
		previous = current;
		current = current->next;
	}
	if (!list_contains_to_remove)
		// It can return list or NULL, I choose list.
		return list;
	//sassume(previous != NULL);
	previous->next = to_remove->next;
	free_list_node(to_remove, value_type);
	return list;
}


int get_list_size(list_t* list) {
	int size = 0;
	while (list != NULL)
	{
		size++;
		list = list->next;
	}
	return size;
}