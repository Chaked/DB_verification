
#include "Infrastructures.h"


return_code_t free_list_node(list_t* to_delete, type_t value_type);
return_code_t free_list(list_t* list, type_t value_type);
list_t* add_to_list(list_t* list, void* value);
list_t* add_to_the_end(list_t* list, void* value);
list_t* remove_from_list(list_t* list, list_t* to_remove, type_t value_type);
int get_list_size(list_t* list);