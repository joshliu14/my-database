#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TABLES 10
#define MAX_FIELDS 10
#define MAX_FIELD_NAME_LENGTH 50
#define MAX_VALUES 100

// Structure for a string field in a table
typedef struct {
    char name[MAX_FIELD_NAME_LENGTH];
    char *stringValues[MAX_VALUES];
    int intValues[MAX_VALUES];
    char type[MAX_FIELD_NAME_LENGTH];
    int num_values;
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

void trim(char *s) {
    int i = strlen(s)-1;
    while (i > 0) {
        // Decrements if a whitespace character is found
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n') i--;
        else break;
    }
    // Puts the null-terminator before whitespace
    s[i+1] = '\0';
    int count = 0;
    // Increments if a whitespace character is found
    while (s[count] == ' ' || s[count] == '\t' || s[count] == '\n') count++;
    if (count != 0) {
        i = 0;
        // Shifts the string to the beginning
        while (s[i+count] != '\0') {
            s[i] = s[i+count];
            i++;
        }
        s[i] = '\0';
    }
}

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

void print_table_values(Schema *schema, char *table_name) {
    for (int i = 0; i < schema -> num_tables; i++) {
        if (strcmp(schema -> tables[i].name, table_name) == 0) {
            printf("Table name: %s\n", schema -> tables[i].name);
            for (int j = 0; j < schema -> tables[i].num_fields; j++) {
                printf("%s: ", schema -> tables[i].fields[j].name);
                for (int k = 0; k < schema -> tables[i].fields[j].num_values; k++) {
                    if (strcmp(schema -> tables[i].fields[j].type, "STRING") == 0) {
                        printf("%s\n", schema -> tables[i].fields[j].stringValues[k]);
                    } else {
                        printf("%d\n", schema -> tables[i].fields[j].intValues[k]);
                    }
                }
            }
            return;
        }
    }
    printf("Table not found.\n");
}
// Function to execute query language commands
void execute_query(Schema *schema, char *query) {
    // For simplicity, assume the query is in the format:
    // CREATE TABLE <table_name> (<field1_name> <field1_type>, <field2_name> <field2_type>, ...)
    char *token = strtok(query, " ");
    if (strcmp(token, "CREATE") == 0) {
        token = strtok(NULL, " ");
        if (strcmp(token, "TABLE") == 0) {
            token = strtok(NULL, " ");
            char table_name[MAX_FIELD_NAME_LENGTH];
            strcpy(table_name, token);
            int num_fields = 0;
            Field fields[MAX_FIELDS];
            while ((token = strtok(NULL, " ,()"))) {
                if (strcmp(token, "STRING") == 0) {
                    strcpy(fields[num_fields++].type, "STRING");
                } else if (strcmp(token, "INT") == 0) {
                    strcpy(fields[num_fields++].type, "INT");
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
    } else if (strcmp(token, "SHOW") == 0) {
        token = strtok(NULL, " ");
        if (strcmp(token, "TABLES") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                printf("Invalid query.\n");
                return;
            }
            for (int i = 0; i < schema -> num_tables; i++) {
                printf("Table name: %s\n", schema -> tables[i].name);
            }
        }      
    } else if (strcmp(token, "SELECT") == 0) {
        token = strtok(NULL, " ");
        if (strcmp(token, "*") == 0) {
            token = strtok(NULL, " ");
            if (strcmp(token, "FROM") != 0) {
                printf("Invalid query.\n");
                return;
            }
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("Invalid query.\n");
                return;
            }
            print_table_values(schema, token);   
        }
    } else if (strcmp(token, "INSERT") == 0) {
        token = strtok(NULL, " ");
        if (strcmp(token, "INTO") != 0) {
            printf("Invalid syntax");
            return;
        }
        token = strtok(NULL, " ");
        for (int i = 0; i < schema -> num_tables; i++) {
            if (strcmp(schema -> tables[i].name, token) == 0) {
                token = strtok(NULL, " ");
                if (strcmp(token, "VALUES") != 0) {
                    printf("Invalid query.\n");
                    return;
                }
                for (int j = 0; j < schema -> tables[i].num_fields; j++) {
                    token = strtok(NULL, ",()");
                    if (token == NULL) {
                        printf("Not enough values.\n");
                        return;
                    }
                    trim(token);
                    if (strcmp(schema -> tables[i].fields[j].type, "STRING") == 0) {
                        char *value = malloc(strlen(token) + 1);
                        strcpy(value, token);
                        schema -> tables[i].fields[j].stringValues[schema -> tables[i].fields[j].num_values++] = value;
                    } else {
                        int value = atoi(token);
                        schema -> tables[i].fields[j].intValues[schema -> tables[i].fields[j].num_values++] = value;
                    }
                }   
                token = strtok(NULL, " ,()");
                if (token != NULL) {
                    printf("Too many values.\n");
                    return;
                }
                printf("Values inserted successfully.\n");
                return;
            }
        }
        printf("Table not found.\n");
    } else {
        printf("Invalid query.\n");
    }
}

// Function to handle meta commands
void handle_meta_command(char *command) {
    if (strcmp(command, ".EXIT") == 0) {
        exit(0);
    } else if (strcmp(command, ".HELP") == 0) {
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
            input[i] = toupper(input[i]);
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
