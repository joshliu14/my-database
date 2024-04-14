#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FIELDS 10
#define MAX_RECORDS 100
#define FILENAME "database.bin"

typedef struct {
    char name[50];
    int type;  // 0 for string, 1 for int
} Field;

typedef struct {
    Field fields[MAX_FIELDS];
    int num_fields;
} Table;

typedef struct {
    char *values[MAX_FIELDS];
} Record;

Table tables[MAX_FIELDS];
int num_tables = 0;

void print_help() {
    printf("Query Language Commands:\n");
    printf("CREATE TABLE <table_name> (<field1_name> <field1_type>, <field2_name> <field2_type>, ...)\n");
    printf("INSERT INTO <table_name> VALUES (<value1>, <value2>, ...)\n");
    printf("SELECT * FROM <table_name>\n");
    printf("UPDATE <table_name> SET <field_name> = <new_value> WHERE <condition>\n");
    printf("DELETE FROM <table_name> WHERE <condition>\n");
    printf("\nMeta Commands:\n");
    printf(".help - Show this list of commands.\n");
    printf(".exit - Close this program.\n");
}

void save_tables_to_file() {
    FILE *file = fopen(FILENAME, "wb");
    if (!file) {
        printf("Error: Cannot open file for writing.\n");
        return;
    }

    fwrite(&num_tables, sizeof(int), 1, file);
    for (int i = 0; i < num_tables; i++) {
        fwrite(&tables[i], sizeof(Table), 1, file);
    }

    fclose(file);
}

void load_tables_from_file() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("No previous data found.\n");
        return;
    }

    fread(&num_tables, sizeof(int), 1, file);
    for (int i = 0; i < num_tables; i++) {
        fread(&tables[i], sizeof(Table), 1, file);
    }

    fclose(file);
}

int main() {
    printf("Welcome to the tabular database management system!\n");
    load_tables_from_file();

    char command[100];
    while (1) {
        printf("> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; // Remove newline character

        if (strcmp(command, ".exit") == 0) {
            save_tables_to_file();
            printf("Exiting...\n");
            break;
        } else if (strcmp(command, ".help") == 0) {
            print_help();
        } else {
            // Parse and execute query language commands
            // For simplicity, let's just print them for now
            printf("Query: %s\n", command);
        }
    }

    return 0;
}
