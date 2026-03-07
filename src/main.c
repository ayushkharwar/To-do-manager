#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TASK 256
#define FILE_NAME "todo.txt"
#define RATE_LIMIT_FILE "rate_limit.txt"
#define MAX_TASKS_PER_DAY 10

/* Returns today's date as an integer in YYYYMMDD format. */
static int todayDate() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    return (tm_info->tm_year + 1900) * 10000 +
           (tm_info->tm_mon + 1) * 100 +
           tm_info->tm_mday;
}

/*
 * Reads the stored date and count from the rate-limit file.
 * Returns the count for today, or 0 if the file is missing/stale.
 */
static int getRateLimitCount() {
    FILE *fp = fopen(RATE_LIMIT_FILE, "r");
    if (!fp) return 0;

    int stored_date = 0, stored_count = 0;
    if (fscanf(fp, "%d %d", &stored_date, &stored_count) != 2) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    return (stored_date == todayDate()) ? stored_count : 0;
}

/* Persists the updated count for today to the rate-limit file. */
static void setRateLimitCount(int count) {
    FILE *fp = fopen(RATE_LIMIT_FILE, "w");
    if (!fp) {
        printf("Warning: could not update rate limit counter.\n");
        return;
    }
    fprintf(fp, "%d %d\n", todayDate(), count);
    fclose(fp);
}

/* Prints the current rate-limit usage. */
static void checkRateLimit() {
    int used = getRateLimitCount();
    int remaining = MAX_TASKS_PER_DAY - used;
    printf("\n--- Rate Limit Info ---\n");
    printf("Daily limit  : %d tasks\n", MAX_TASKS_PER_DAY);
    printf("Used today   : %d tasks\n", used);
    printf("Remaining    : %d tasks\n", remaining > 0 ? remaining : 0);
    printf("-----------------------\n");
}

void addTask() {
    int used = getRateLimitCount();
    if (used >= MAX_TASKS_PER_DAY) {
        printf("Rate limit reached! You can add at most %d tasks per day.\n",
               MAX_TASKS_PER_DAY);
        printf("Used today: %d/%d. Try again tomorrow.\n",
               used, MAX_TASKS_PER_DAY);
        return;
    }

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

    setRateLimitCount(used + 1);

    printf("Task added 👌  (%d/%d tasks used today)\n",
           used + 1, MAX_TASKS_PER_DAY);
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
        printf("4. Check Rate Limit\n");
        printf("5. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: clearTasks(); break;
            case 4: checkRateLimit(); break;
            case 5: printf("Bye ✌️\n"); exit(0);
            default: printf("Invalid option.\n");
        }
    }

    return 0;
}
