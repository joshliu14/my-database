#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TABLES 10
#define MAX_FIELDS 10
#define MAX_FIELD_NAME_LENGTH 50
#define MAX_STRING_LENGTH 100

// Structure for a string field in a table
typedef struct {
    char name[MAX_FIELD_NAME_LENGTH];
    char stringValue[MAX_STRING_LENGTH];
    int intValue;
    char type[MAX_FIELD_NAME_LENGTH];
} Field;

// Structure for a table
typedef struct {
    char name[MAX_FIELD_NAME_LENGTH];
    Field fields[MAX_FIELDS];
    int num_fields;
} Table;

// Structure for the schema
typedef struct {
    Table tables[MAX_TABLES];
    int num_tables;
} Schema;

// Function to create a new table
int create_table(Schema *schema, char *table_name, Field *fields, int num_fields) {
    if (schema->num_tables >= MAX_TABLES) {
        printf("Cannot create more tables. Limit reached.\n");
        return 0;
    }

    Table new_table;
    strcpy(new_table.name, table_name);
    memcpy(new_table.fields, fields, num_fields * sizeof(Field));
    new_table.num_fields = num_fields;
    schema->tables[schema->num_tables++] = new_table;
    return 1;
}

// Function to execute query language commands
void execute_query(Schema *schema, char *query) {
    // For simplicity, assume the query is in the format:
    // CREATE TABLE <table_name> (<field1_name> <field1_type>, <field2_name> <field2_type>, ...)
    char *token = strtok(query, " ");
    if (strcmp(token, "create") == 0) {
        token = strtok(NULL, " ");
        if (strcmp(token, "table") == 0) {
            token = strtok(NULL, " ");
            char table_name[MAX_FIELD_NAME_LENGTH];
            strcpy(table_name, token);
            token = strtok(NULL, " "); // Skip "("
            int num_fields = 0;
            Field fields[MAX_FIELDS];
            while ((token = strtok(NULL, " ,)"))) {
                if (strcmp(token, "string") == 0) {
                    strcopy(fields[num_fields++].type, "string");
                } else if (strcmp(token, "int") == 0) {
                    strcopy(fields[num_fields++].type, "int");
                } else {
                    strcpy(fields[num_fields].name, token);
                }
            }
            if (create_table(schema, table_name, fields, num_fields)) {
                printf("Table %s created successfully.\n", table_name);
            } else {
                printf("Failed to create table %s.\n", table_name);
            }
        }
    } else {
        printf("Invalid query.\n");
    }
}

// Function to handle meta commands
void handle_meta_command(char *command) {
    if (strcmp(command, ".exit") == 0) {
        exit(0);
    } else if (strcmp(command, ".help") == 0) {
        printf("Meta commands:\n.exit - Exit the program.\n.help - Display this help information.\nCreate table: CREATE TABLE <table_name> (<field1_name> <field1_type>, <field2_name> <field2_type>, ...)\n");
    } else {
        printf("Unrecognized command: %s\n", command);
    }
}

int main() {
    Schema schema = {0};
    char input[100];

    printf("Welcome to Basic Tabular Database Management System.\n");
    printf("Type '.help' for assistance.\n");

    // Main loop for CLI interface
    while (1) {
        printf("db> ");
        fgets(input, sizeof(input), stdin);
        // convert the string to lowercase
        input[strlen(input) - 1] = '\0'; // Remove newline character
        for(int i = 0; input[i]; i++){
            input[i] = tolower(input[i]);
        }

        // Check if it's a meta command
        if (input[0] == '.') {
            handle_meta_command(input);
            continue;
        }

        // Execute query language command
        execute_query(&schema, input);
    }

    return 0;
}
