#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASK 256
#define FILE_NAME "todo.txt"

void addTask() {
    FILE *fp = fopen(FILE_NAME, "a");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    char task[MAX_TASK];
    printf("Enter your task: ");
    getchar();  
    fgets(task, MAX_TASK, stdin);

    fprintf(fp, "%s", task);
    fclose(fp);

    printf("Task added 👌\n");
}

void viewTasks() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) {
        printf("No tasks yet. File will be created when you add one.\n");
        return;
    }

    char line[MAX_TASK];
    int index = 1;

    printf("\n--- Your To-Do List ---\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%d. %s", index++, line);
    }
    printf("------------------------\n");

    fclose(fp);
}

void clearTasks() {
    FILE *fp = fopen(FILE_NAME, "w");
    if (!fp) {
        printf("Error clearing tasks.\n");
        return;
    }
    fclose(fp);
    printf("All tasks cleared 🧹\n");
}

int main() {
    int choice;

    while (1) {
        printf("\n----------------------\n");
        printf("   TO-DO MANAGER\n");
        printf("----------------------\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Clear All Tasks\n");
        printf("4. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: clearTasks(); break;
            case 4: printf("Bye ✌️\n"); exit(0);
            default: printf("Invalid option.\n");
        }
    }

    return 0;
}
