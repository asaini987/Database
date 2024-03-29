#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "statement_processing.h"
#include "table_operations.h"

int main(int argc, char const *argv[]) {
    InputBuffer* input_buffer = new_input_buffer();

    if (argc < 2) {
        printf("Must supply database filename\n");
        exit(EXIT_FAILURE);
    }

    const char* filename = argv[1];
    Table* table = db_open(filename);

    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer, table)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command: %s\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax error. Could not parse statement.\n");
                continue;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unknown keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
            case (PREPARE_STRING_TOO_LONG):
                printf("String is too long.\n");
                continue;
            case (PREPARE_NEGATIVE_ID):
                printf("Id must be positive.\n");
                continue;
        }
        
        switch (execute_statement(&statement, table)) {
            case (EXECUTE_SUCCESS):
                printf("Executed.\n");
                break;
            case (EXECUTE_TABLE_FULL):
                printf("Error: Table full.\n");
                break;
        }
    }

    return 0;
}

