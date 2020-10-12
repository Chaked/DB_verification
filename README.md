# DB_verification
This is an educational project to learn how to use Seahorn as a verifing tool.
In this project I have implemented an in-memory DB, supporting the following commands:

"CREATE TABLE \<Table Name\> (
	 \<Column type\> \<Column Name\> ,</br>
	.</br>
	.</br>
	.</br>
	\<Column type\> \<Column Name\></br>
	)</br>
"

"DROP TABLE \<Table Name\>"

"INSERT \<Table Name\> (</br>
	\<Column Name\> = \<Value\> ,</br>
	.</br>
	.</br>
	.</br>
	\<Column Name\> = \<Value\></br>
	)"</br>
"DELETE \<Table Name\> WHERE \<Condition\>"</br>

"SELECT * FROM \<Table Name\> WHERE</br>
  \<COLUMN NAME\> \< [=, <>, >, >=, <, <=] \> \<VALUE\> </br>
  AND \<Condition\> </br>
  .</br>
  .</br>
  .</br>
  AND \<Condition\> "</br>
    
 Several syntax notes:
1) String columns conditions support only those opperators: = and <> .
2) It is very important to mind the white spaces. Our parser is implemented by reading the next token between
the white spaces rather than building an AST with a grammer.
3) String values in the queries (inside conditions or in insert queries) must be tagged (e.g. name = 'Bruce').
Further more, a string inside the white spaces can't contain white spaces (because of note#2).

Our class hierarchy is as follow:</br>
database_t</br>
--list of table_t*</br>
----list of column_declaration_t* (contain name and type)</br>
----list of row_t*</br>
------list of column_value_t* (name, type and value)</br>
