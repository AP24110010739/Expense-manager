#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Expense {
    float amount;
    char category[20];
    int day, month, year;
    struct Expense* next;
};

struct Expense* head = NULL;
float daily_limit = 0;

// ---------------- LOAD ----------------
void loadFromFile() {
    FILE* file = fopen("expenses.txt", "r");
    if (!file) return;

    while (1) {
        struct Expense* newNode = malloc(sizeof(struct Expense));

        if (fscanf(file, "%f %s %d %d %d",
                   &newNode->amount,
                   newNode->category,
                   &newNode->day,
                   &newNode->month,
                   &newNode->year) != 5) {
            free(newNode);
            break;
        }

        newNode->next = head;
        head = newNode;
    }
    fclose(file);
}

// ---------------- ADD ----------------
void addExpense() {
    float amount;
    char category[20];
    int d, m, y;

    printf("Enter amount: ");
    scanf("%f", &amount);

    if (daily_limit > 0 && amount > daily_limit) {
        char ch;
        printf("⚠ Exceeds limit! Continue? (y=Yes / n=No): ");
        scanf(" %c", &ch);

        if (ch == 'n' || ch == 'N') {
            printf("Cancelled\n");
            return;
        }
    }

    printf("Enter event/category: ");
    scanf("%s", category);

    printf("Enter date (DD-MM-YYYY): ");
    scanf("%d-%d-%d", &d, &m, &y);

    struct Expense* node = malloc(sizeof(struct Expense));
    node->amount = amount;
    strcpy(node->category, category);
    node->day = d;
    node->month = m;
    node->year = y;
    node->next = head;
    head = node;

    printf("Added\n");
}

// ---------------- DELETE ----------------
void deleteExpense() {
    float amount;
    int d, m, y;

    printf("Enter amount and date (DD-MM-YYYY): ");
    scanf("%f %d-%d-%d", &amount, &d, &m, &y);

    struct Expense *temp = head, *prev = NULL;

    while (temp) {
        if (temp->amount == amount &&
            temp->day == d &&
            temp->month == m &&
            temp->year == y) {

            if (prev == NULL)
                head = temp->next;
            else
                prev->next = temp->next;

            free(temp);
            printf("Deleted\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Not found\n");
}

// ---------------- VIEW ----------------
void viewExpenses() {
    struct Expense* temp = head;

    if (!temp) {
        printf("No data\n");
        return;
    }

    while (temp) {
        printf("%.2f | %s | %02d-%02d-%d\n",
               temp->amount, temp->category,
               temp->day, temp->month, temp->year);
        temp = temp->next;
    }
}

// ---------------- DAILY LIMIT ----------------
void setDailyLimit() {
    printf("Enter daily limit: ");
    scanf("%f", &daily_limit);
}

void viewDailyLimit() {
    printf("Daily Limit: %.2f\n", daily_limit);
}

// ---------------- MONTHLY ----------------
void monthlyStats() {
    int m, y;

    printf("Enter month: ");
    scanf("%d", &m);

    printf("Enter year: ");
    scanf("%d", &y);

    struct Expense* temp = head;
    float total = 0, max = 0, min = 999999;
    int count = 0;

    while (temp) {
        if (temp->month == m && temp->year == y) {
            total += temp->amount;

            if (temp->amount > max) max = temp->amount;
            if (temp->amount < min) min = temp->amount;

            count++;
        }
        temp = temp->next;
    }

    if (count == 0) {
        printf("No data\n");
        return;
    }

    printf("Total: %.2f\nMax: %.2f\nMin: %.2f\nAvg: %.2f\n",
           total, max, min, total / count);
}

// ---------------- CATEGORY ----------------
void categorySpending() {
    char cat[20];
    float total = 0;

    printf("Enter category: ");
    scanf("%s", cat);

    struct Expense* temp = head;

    while (temp) {
        if (strcmp(temp->category, cat) == 0)
            total += temp->amount;
        temp = temp->next;
    }

    printf("Total: %.2f\n", total);
}

// ---------------- SAVE ----------------
void saveToFile() {
    FILE* file = fopen("expenses.txt", "w");

    struct Expense* temp = head;

    while (temp) {
        fprintf(file, "%.2f %s %d %d %d\n",
                temp->amount, temp->category,
                temp->day, temp->month, temp->year);
        temp = temp->next;
    }

    fclose(file);
    printf("Saved\n");
}

// ---------------- SORT ----------------
struct Expense* getMiddle(struct Expense* head) {
    struct Expense *slow = head, *fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

struct Expense* merge(struct Expense* a, struct Expense* b) {
    if (!a) return b;
    if (!b) return a;

    if (a->amount > b->amount) {
        a->next = merge(a->next, b);
        return a;
    } else {
        b->next = merge(a, b->next);
        return b;
    }
}

struct Expense* mergeSort(struct Expense* head) {
    if (!head || !head->next) return head;

    struct Expense* mid = getMiddle(head);
    struct Expense* next = mid->next;
    mid->next = NULL;

    struct Expense* left = mergeSort(head);
    struct Expense* right = mergeSort(next);

    return merge(left, right);
}

void sortExpenses() {
    head = mergeSort(head);
    printf("Sorted\n");
}

// ---------------- MAIN ----------------
int main() {
    int choice;
    loadFromFile();

    while (1) {
        printf("\n===== Expense Manager =====\n");
        printf("1. Add Expense\n");
        printf("2. Delete Expense\n");
        printf("3. View Expenses\n");
        printf("4. Set Daily Limit\n");
        printf("5. View Daily Limit\n");
        printf("6. Monthly Statistics\n");
        printf("7. Category Spending\n");
        printf("8. Save to File\n");
        printf("9. Sort Expenses\n");
        printf("10. Exit\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addExpense(); break;
            case 2: deleteExpense(); break;
            case 3: viewExpenses(); break;
            case 4: setDailyLimit(); break;
            case 5: viewDailyLimit(); break;
            case 6: monthlyStats(); break;
            case 7: categorySpending(); break;
            case 8: saveToFile(); break;
            case 9: sortExpenses(); break;
            case 10: exit(0);
            default: printf("Invalid choice\n");
        }
    }
}