#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define DATA_FILE "accounts.txt"

typedef struct {
    int accountNumber;
    int pin;
    double balance;
    char owner[50];
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

void loadAccounts();
void saveAccounts();
int findAccountIndex(int accountNumber);
void createAccount();
void login();
void accountMenu(int index);
void deposit(int index);
void withdraw(int index);
void transfer(int index);

int main() {
    loadAccounts();

    while (1) {
        int choice;
        printf("\n=== C ATM Banking System ===\n");
        printf("1. Create account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Choose: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

        if (choice == 1) {
            createAccount();
        } else if (choice == 2) {
            login();
        } else if (choice == 3) {
            saveAccounts();
            printf("Goodbye.\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}

void loadAccounts() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        return;
    }

    while (fscanf(file, "%d %d %lf %[^\n]",
                  &accounts[accountCount].accountNumber,
                  &accounts[accountCount].pin,
                  &accounts[accountCount].balance,
                  accounts[accountCount].owner) == 4) {
        accountCount++;
        if (accountCount >= MAX_ACCOUNTS) {
            break;
        }
    }

    fclose(file);
}

void saveAccounts() {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        printf("Could not save accounts.\n");
        return;
    }

    for (int i = 0; i < accountCount; i++) {
        fprintf(file, "%d %d %.2f %s\n",
                accounts[i].accountNumber,
                accounts[i].pin,
                accounts[i].balance,
                accounts[i].owner);
    }

    fclose(file);
}

int findAccountIndex(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Maximum accounts reached.\n");
        return;
    }

    Account account;

    printf("Account number: ");
    scanf("%d", &account.accountNumber);

    if (findAccountIndex(account.accountNumber) != -1) {
        printf("Account already exists.\n");
        return;
    }

    printf("PIN: ");
    scanf("%d", &account.pin);

    printf("Initial balance: ");
    scanf("%lf", &account.balance);

    printf("Owner name: ");
    getchar();
    fgets(account.owner, sizeof(account.owner), stdin);
    account.owner[strcspn(account.owner, "\n")] = '\0';

    if (account.balance < 0) {
        printf("Balance cannot be negative.\n");
        return;
    }

    accounts[accountCount++] = account;
    saveAccounts();
    printf("Account created.\n");
}

void login() {
    int accountNumber, pin;

    printf("Account number: ");
    scanf("%d", &accountNumber);

    printf("PIN: ");
    scanf("%d", &pin);

    int index = findAccountIndex(accountNumber);

    if (index == -1 || accounts[index].pin != pin) {
        printf("Invalid account number or PIN.\n");
        return;
    }

    accountMenu(index);
}

void accountMenu(int index) {
    while (1) {
        int choice;
        printf("\nWelcome, %s\n", accounts[index].owner);
        printf("1. Check balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Transfer\n");
        printf("5. Logout\n");
        printf("Choose: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Balance: $%.2f\n", accounts[index].balance);
        } else if (choice == 2) {
            deposit(index);
        } else if (choice == 3) {
            withdraw(index);
        } else if (choice == 4) {
            transfer(index);
        } else if (choice == 5) {
            saveAccounts();
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

void deposit(int index) {
    double amount;
    printf("Deposit amount: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Amount must be positive.\n");
        return;
    }

    accounts[index].balance += amount;
    saveAccounts();
    printf("Deposit successful.\n");
}

void withdraw(int index) {
    double amount;
    printf("Withdraw amount: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Amount must be positive.\n");
        return;
    }

    if (amount > accounts[index].balance) {
        printf("Insufficient balance.\n");
        return;
    }

    accounts[index].balance -= amount;
    saveAccounts();
    printf("Withdrawal successful.\n");
}

void transfer(int index) {
    int targetAccount;
    double amount;

    printf("Target account number: ");
    scanf("%d", &targetAccount);

    int targetIndex = findAccountIndex(targetAccount);

    if (targetIndex == -1) {
        printf("Target account not found.\n");
        return;
    }

    printf("Transfer amount: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Amount must be positive.\n");
        return;
    }

    if (amount > accounts[index].balance) {
        printf("Insufficient balance.\n");
        return;
    }

    accounts[index].balance -= amount;
    accounts[targetIndex].balance += amount;
    saveAccounts();
    printf("Transfer successful.\n");
}
