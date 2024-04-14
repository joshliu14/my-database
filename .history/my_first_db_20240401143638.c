#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TABLES 10
#define MAX_COLUMNS 10
#define MAX_ROWS 100

// Structure to represent a table
struct Table {
    char name[50];
    char columns[MAX_COLUMNS][50];
    int num_columns;
    int data[MAX_ROWS][MAX_COLUMNS];
    int num_rows;
};

// Structure to represent the database
struct Database {
    struct Table tables[MAX_TABLES];
    int num_tables;
};

// Function prototypes
void create_table(struct Database *db, char *table_name, char *columns);
void insert_into(struct Database *db, char *table_name, char *values);
void select_from(struct Database *db, char *table_name);
void delete_from(struct Database *db, char *table_name, char *condition);
void exit_program();
void help();

int main() {
    struct Database db;
    db.num_tables = 0;

    while (1) {
        char query[100];
        printf("> ");
        fgets(query, sizeof(query), stdin);
        query[strcspn(query, "\n")] = '\0';  // Remove newline character

        char *command = strtok(query, " ");

        if (strcmp(command, "create") == 0) {
            char *table_name = strtok(NULL, " ");
            char *columns = strtok(NULL, "");
            create_table(&db, table_name, columns);
        } else if (strcmp(command, "insert") == 0) {
            char *table_name = strtok(NULL, " ");
            char *values = strtok(NULL, "");
            insert_into(&db, table_name, values);
        } else if (strcmp(command, "select") == 0) {
            char *table_name = strtok(NULL, "");
            select_from(&db, table_name);
        } else if (strcmp(command, "delete") == 0) {
            char *table_name = strtok(NULL, " ");
            char *condition = strtok(NULL, "");
            delete_from(&db, table_name, condition);
        } else if (strcmp(command, ".exit") == 0) {
            exit_program();
        } else if (strcmp(command, ".help") == 0) {
            help();
        } else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}

void create_table(struct Database *db, char *table_name, char *columns) {
    if (db->num_tables >= MAX_TABLES) {
        printf("Cannot create more tables.\n");
        return;
    }

    struct Table *table = &db->tables[db->num_tables++];
    strcpy(table->name, table_name);

    char *column = strtok(columns, ",");
    table->num_columns = 0;
    while (column != NULL && table->num_columns < MAX_COLUMNS) {
        strcpy(table->columns[table->num_columns++], column);
        column = strtok(NULL, ",");
    }

    table->num_rows = 0;

    printf("Table %s created successfully.\n", table_name);
}

void insert_into(struct Database *db, char *table_name, char *values) {
    struct Table *table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }

    if (table == NULL) {
        printf("Table %s does not exist.\n", table_name);
        return;
    }

    char *value = strtok(values, ",");
    if (table->num_columns != 0) {
        for (int i = 0; i < table->num_columns; i++) {
            table->data[table->num_rows][i] = atoi(value);
            value = strtok(NULL, ",");
        }
        table->num_rows++;
        printf("Record inserted successfully.\n");
    } else {
        printf("No columns defined for table %s.\n", table_name);
    }
}

void select_from(struct Database *db, char *table_name) {
    struct Table *table = NULL;
    for (int i = 0; i < db->num_tables; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            table = &db->tables[i];
            break;
        }
    }

    if (table == NULL) {
        printf("Table %s does not exist.\n", table_name);
        return;
    }

    printf("Table: %s\n", table->name);
    for (int i = 0; i < table->num_columns; i++) {
        printf("%s\t", table->columns[i]);
    }
    printf("\n");

    for (int i = 0; i < table->num_rows; i++) {
        for (int j = 0; j < table->num_columns; j++) {
            printf("%d\t", table->data[i][j]);
        }
        printf("\n");
    }
}

void delete_from(struct Database *db, char *table_name, char *condition) {
    printf("Delete operation not implemented yet.\n");
}

void exit_program() {
    printf("Exiting...\n");
    exit(0);
}

void help() {
    printf(".exit - Exit the program\n");
    printf(".help - Show help message\n");
    printf("create table <table_name> (<column1> <datatype>, <column2> <datatype>, ...) - Create a new table\n");
    printf("insert into <table_name> values (<value1>, <value2>, ...) - Insert a new record into a table\n");
    printf("select * from <table_name> - Select all records from a table\n");
    printf("delete from <table_name> where <condition> - Delete records from a table based on a condition\n");
}

