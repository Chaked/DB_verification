#include "DB.h"	

/*
    The original use of this DB was with its query API, and using the
    parser to translate the queries to DB.h's API which demands creating internal calsses. 
    After talking to yakir I discovered Seahorn have hard time with long arrays and he
    suggested dropping the use of queries. So, this interface was created to ease the use of DB.h's API.

    As this calss only exist for testing the For Simplicity, we allow here up to 3 columns and 3 conditions. Using 
    less than these require sending NULL in the unwanted pointers. DB.h's API can support any
    number of columns and conditions of course. 
    */



return_code_t dbapi_create_table(database_t* DB, char* table_name, column_type_t col_t1, char* col_n1, column_type_t col_t2, char* col_n2, column_type_t col_t3, char* col_n3);

return_code_t dbapi_drop_table(database_t* DB, char* table_name);

return_code_t dbapi_insert(database_t* DB, char* table_name,  char* col_n1, int col_v_i1, char* col_v_str1, char* col_n2, int col_v_i2, char* col_v_str2, char* col_n3, int col_v_i3, char* col_v_str3);

return_code_t dbapi_delete(database_t* DB, char* table_name, char* col_n1, condition_type_t cond_t1, int col_v_i1, char* col_v_str1, char* col_n2, condition_type_t cond_t2, int col_v_i2, char* col_v_str2, char* col_n3, condition_type_t cond_t3, int col_v_i3, char* col_v_str3);

return_code_t dbapi_select(database_t* DB, char* table_name, char* col_n1, condition_type_t cond_t1, int col_v_i1, char* col_v_str1, char* col_n2, condition_type_t cond_t2, int col_v_i2, char* col_v_str2, char* col_n3, condition_type_t cond_t3, int col_v_i3, char* col_v_str3);

void print_results(list_t* results);

