#include <stdint.h>
#include "statement_processing.h"

#ifndef TABLE_OPERATIONS_H
#define TABLE_OPERATIONS_H

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

extern const uint32_t ID_SIZE;
extern const uint32_t USERNAME_SIZE;
extern const uint32_t EMAIL_SIZE;
extern const uint32_t ID_OFFSET;
extern const uint32_t USERNAME_OFFSET;
extern const uint32_t EMAIL_OFFSET;
extern const uint32_t ROW_SIZE;

extern const uint32_t PAGE_SIZE;
extern const uint32_t ROWS_PER_PAGE;
extern const uint32_t TABLE_MAX_ROWS;

typedef struct {
    int file_descriptor;
    uint32_t file_length;
    void* pages[TABLE_MAX_PAGES];
} Pager;

typedef struct {
    Pager* pager;
    uint32_t num_rows;
} Table;

typedef struct {
    Table* table;
    uint32_t row_num;
    bool end_of_table; // Indicates a position one past the last element
} Cursor;

MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table* table);
void serialize_row(Row* source, void* destination);
void deserialize_row(void* source, Row* destination);
void print_row(Row* row);

void* cursor_value(Cursor* cursor);
ExecuteResult execute_insert(Statement* statement, Table* table);
ExecuteResult execute_select(Statement* statement, Table* table);
ExecuteResult execute_statement(Statement* statement, Table* table);

Table* db_open(const char* filename);
Pager* pager_open(const char* filename);
void* get_page(Pager* pager, uint32_t page_num);
void pager_flush(Pager* pager, uint32_t page_num, uint32_t size);
void db_close(Table* table);

// Cursor functions
Cursor* table_start(Table* table);
Cursor* table_end(Table* table);
void cursor_advance(Cursor* cursor);
#endif
