﻿# DB_verification
This is an educational project to learn to use Seahorn as a verifing tool.
In this project I have implemented an in-memory DB, supporting the following commands:

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
    
 Several syntax notes:
1) String columns conditions support only those opperators: = and <> .
2) It is very important to mind the white spaces. Our parser is implemented by reading the next token between
the white spaces rather than building an AST with a grammer.
3) String values in the queries (inside conditions or in insert queries) must be tagged (e.g. name = 'Bruce').
Further more, a string inside the white spaces can't contain white spaces (because of note#2).

Our class hierarchy is as follow:
database_t
--list of table_t*
----list of column_declaration_t* (contain name and type)
----list of row_t*
------list of column_value_t* (name, type and value)
