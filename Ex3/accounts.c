#define _CRT_SECURE_NO_WARNINGS /* to suppress compiler warnings (VS 2010 ) */
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

typedef struct ACCOUNTS{
	int account_num;
	double initial_balance;
	double current_balance;
	double total_deposited;
	double total_withdrawled;
	int arr_loc;
	int num_deposites;
	int num_withdrawls;
	HANDLE account_mutex;
	TCHAR *mutex_name;
	struct ACCOUNTS *next_account;
}	accounts;


accounts* create_account(int account_num,char* account_num_str, double initial_balance, int arr_loc) {
	int i = 0;
	accounts *new_account = (accounts*)malloc(sizeof(accounts));
	char temp_str[] = "mutex_account_";
	char *mutex_str = (char*)malloc(sizeof(char)*(strlen(temp_str) + strlen(account_num_str) + 1));

	if (mutex_str == NULL) {
		printf("Error in Memory Initialization");
		exit(1);
	}

	if (new_account == NULL) {
		printf("Error in Memory Initialization");
		exit(1);
	}

	new_account->account_num = account_num;
	new_account->initial_balance = initial_balance;
	new_account->current_balance = initial_balance;
	new_account->arr_loc = 0;
	new_account->num_deposites = 0;
	new_account->num_withdrawls = 0;
	new_account->total_deposited = 0;
	new_account->total_withdrawled = 0;
	new_account->next_account = NULL;
	
	strcpy(mutex_str, temp_str);
	strcat(mutex_str, account_num_str);

	new_account->mutex_name = (TCHAR*) malloc (sizeof(TCHAR)*(strlen(temp_str) + strlen(account_num_str) + 1));
	if (new_account->mutex_name == NULL) {
		printf("Error in Memory Initialization");
		exit(1);
	}
	
	for (i = 0; i <= strlen(mutex_str); i++) {
		new_account->mutex_name[i] = (TCHAR)mutex_str[i];
	}
	new_account->account_mutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		new_account->mutex_name);

	free(mutex_str);

	return new_account;
}

accounts* add_account(int account_num, char* account_num_str, double initial_balance, int arr_loc, accounts* head, BOOL* acount_exist) {
	accounts *new_node = create_account(account_num, account_num_str, initial_balance, arr_loc);
	accounts *iter = NULL;
	accounts *prev = NULL;

	if (head == NULL) {
		return new_node;
	}

	if (new_node->account_num < head->account_num) {
		new_node->next_account = head;
		return new_node;
	}

	iter = head;
	while ((iter != NULL) && (new_node->account_num > iter->account_num)) {
		prev = iter;
		iter = iter->next_account;
	}

	if (new_node->account_num == iter->account_num) {
		*acount_exist = TRUE;
		CloseHandle(new_node->account_mutex);
		free(new_node->mutex_name);
		free(new_node);
		return head;
	}

	prev->next_account = new_node;
	new_node->next_account = iter;

	return head;
}

void change_balance(int account_num, double change_amount, accounts* head, BOOL is_deposite, BOOL* account_exist) {
	accounts* temp_account = head;

	while ((temp_account != NULL) && (temp_account->account_num != account_num)) {
		temp_account = temp_account->next_account;
	}

	if (temp_account == NULL) {
		*account_exist = FALSE;
		return;
	}

	if (is_deposite) {
		temp_account->current_balance = temp_account->current_balance + change_amount;
		temp_account->num_deposites++;
		temp_account->total_deposited = temp_account->total_deposited + change_amount;
	}
	else {
		temp_account->current_balance = temp_account->current_balance - change_amount;
		temp_account->num_withdrawls++;
		temp_account->total_withdrawled = temp_account->total_withdrawled + change_amount;
	}
}

accounts* remove_account(int account_num, accounts* head, BOOL* account_exist) {
	accounts* temp_account = head;
	accounts *prev = NULL;

	if (head == NULL) {
		*account_exist = FALSE;
		return NULL;
	}

	if (head->account_num == account_num) {
		temp_account = head->next_account;
		CloseHandle(head->account_mutex);
		free(head->mutex_name);
		free(head);
		return temp_account;
	}

	while (temp_account != NULL)  {
		if (temp_account->account_num == account_num) {
			prev->next_account = temp_account->next_account;
			CloseHandle(temp_account->account_mutex);
			free(temp_account->mutex_name);
			free(temp_account);
			return head;
		}
		prev = temp_account;
		temp_account = temp_account->next_account;
	}

	if (temp_account == NULL) {
		*account_exist = FALSE;
		return head;
	}

	return head;
}