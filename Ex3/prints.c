#define _CRT_NONSTDC_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include <string.h>
#include <tchar.h>

typedef struct ACCOUNTS {
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


void print_add_account(BOOL is_succuess, char* account_num_str, char* initial_balance_str, FILE* outfile) {
	char good_initial_str[] = "Successfully created bank account number ";
	char good_next_str[] = " with current balance of "; 
	char good_last_str[] = ".\n";
	char bad_initial_str[] = "!!! Account number ";
	char bad_next_str[] = " already exists. Can’t create account. Skipping command. !!!\n";
	char *temp_str = NULL;

	if (is_succuess) {
		temp_str = (char*)malloc(sizeof(char)* (strlen(good_initial_str) + strlen(account_num_str) + strlen(good_next_str) + strlen(initial_balance_str) + strlen(good_last_str) + 1));
		if (temp_str == NULL) {
			printf("Error in Memory Initialization.\n");
			exit(1);
		}
		strcpy(temp_str, good_initial_str);
		strcat(temp_str, account_num_str);
		strcat(temp_str, good_next_str);
		strcat(temp_str, initial_balance_str);
		strcat(temp_str, good_last_str);
	}
	else {
		temp_str = (char*)malloc(sizeof(char)*(strlen(bad_initial_str) + strlen(account_num_str) + strlen(bad_next_str) + 1));
		if (temp_str == NULL) {
			printf("Error in Memory Initialization.\n");
			exit(1);
		}
		strcpy(temp_str, bad_initial_str);
		strcat(temp_str, account_num_str);
		strcat(temp_str, bad_next_str);
	}

	fputs(temp_str, outfile);

	free(temp_str);
}

void print_remove_account(BOOL is_succuess, char* account_num_str, FILE* outfile) {
	char good_initial_str[] = "Successfully closed bank acount number ";
	char good_last_text[] = ".\n";
	char bad_initial_str[] = "!!! Account number ";
	char bad_last_str[] = " doesn’t exist. Can’t close account. Skipping command. !!!\n";
	char *temp_str = NULL;

	if (is_succuess) {
		temp_str = (char*)malloc(sizeof(char)* (strlen(good_initial_str) + strlen(account_num_str) + strlen(good_last_text) + 1));
		if (temp_str == NULL) {
			printf("Error in Memory Initialization.\n");
			exit(1);
		}
		strcpy(temp_str, good_initial_str);
		strcat(temp_str, account_num_str);
		strcat(temp_str, good_last_text);
	}
	else {
		temp_str = (char*)malloc(sizeof(char)* (strlen(bad_initial_str) + strlen(account_num_str) + strlen(bad_last_str) + 1));
		if (temp_str == NULL) {
			printf("Error in Memory Initialization.\n");
			exit(1);
		}
		strcpy(temp_str, bad_initial_str);
		strcat(temp_str, account_num_str);
		strcat(temp_str, bad_last_str);
	}

	fputs(temp_str, outfile);

	free(temp_str);
}

void print_change_amount(BOOL is_succuess, BOOL is_deposite, char* account_num_str, char* money_amount, FILE* outfile) {
	char good_deposite_initial[] = "Successfully deposited ";
	char good_deposite_next[] = " to account number ";
	char good_final[] = ".\n";
	char good_withdraw_initial[] = "Successfully withdrew ";
	char good_withdraw_next[] = " from account number ";
	char bad_deposite_initial[] = "!!! Unable to deposited ";
	char bad_deposite_next[] = " to account number ";
	char bad_final[] = ". Account doesn’t exist. Skipping command. !!!\n";
	char bad_withdraw_initial[] = "!!! Unable to withdrew ";
	char bad_withdraw_next[] = " from account number ";
	char *temp_str = NULL;

	if (is_succuess) {
		if (is_deposite) {
			temp_str = (char*)malloc(sizeof(char)*(strlen(good_deposite_initial) + strlen(money_amount) + strlen(good_deposite_next) + strlen(account_num_str) + strlen(good_final) + 1));
			if (temp_str == NULL) {
				printf("Error in Memory Initialization.\n");
				exit(1);
			}
			strcpy(temp_str, good_deposite_initial);
			strcat(temp_str, money_amount);
			strcat(temp_str, good_deposite_next);
			strcat(temp_str, account_num_str);
			strcat(temp_str, good_final);
		}
		else {
			temp_str = (char*)malloc(sizeof(char)*(strlen(good_withdraw_initial)+ strlen(money_amount) + strlen(good_withdraw_next) + strlen(account_num_str) + strlen(good_final) + 1));
			if (temp_str == NULL) {
				printf("Error in Memory Initialization.\n");
				exit(1);
			}
			strcpy(temp_str, good_withdraw_initial);
			strcat(temp_str, money_amount);
			strcat(temp_str, good_withdraw_next);
			strcat(temp_str, account_num_str);
			strcat(temp_str, good_final);
		}
	}
	else {
		if (is_deposite) {
			temp_str = (char*)malloc(sizeof(char)*(strlen(bad_deposite_initial) + strlen(money_amount) + strlen(bad_deposite_next) + strlen(account_num_str) + strlen(bad_final) + 1));
			if (temp_str == NULL) {
				printf("Error in Memory Initialization.\n");
				exit(1);
			}
			strcpy(temp_str, bad_deposite_initial);
			strcat(temp_str, money_amount);
			strcat(temp_str, bad_deposite_next);
			strcat(temp_str, account_num_str);
			strcat(temp_str, bad_final);
		}
		else {
			temp_str = (char*)malloc(sizeof(char)*(strlen(bad_withdraw_initial) + strlen(money_amount) + strlen(bad_withdraw_next) + strlen(account_num_str) + strlen(bad_final) + 1));
			if (temp_str == NULL) {
				printf("Error in Memory Initialization.\n");
				exit(1);
			}
			strcpy(temp_str, bad_withdraw_initial);
			strcat(temp_str, money_amount);
			strcat(temp_str, bad_withdraw_next);
			strcat(temp_str, account_num_str);
			strcat(temp_str, bad_final);
		}
	}

	fputs(temp_str, outfile);

	free(temp_str);
}

void print_accounts(accounts* head, FILE* outfile) {
	char initial_str[] = "Current balances in bank accounts are:\nBank Account #, Current Balance\n";
	char middle_str[] = ",";
	char end_str[] = "\n";
	char *temp_str = NULL;
	char temp_account_str[11] = { 0 }; // max value of 2147483647
	char temp_amount_str[17] = { 0 }; // max value of 1 trilion +'.' + 2 digits after the decimal dot + '\0'
	accounts *temp_account = NULL;
	BOOL first = TRUE;

	fputs(initial_str, outfile);

	while (temp_account != NULL) {
		sprintf(temp_account_str, "%d", temp_account->account_num);
		sprintf(temp_amount_str, "%.2f", temp_account->current_balance);
		temp_str = (char*)malloc(sizeof(char)*(strlen(temp_account_str) + strlen(middle_str) + strlen(temp_amount_str) + strlen(end_str) + 1));
		if (temp_str == NULL) {
			printf("Error in Memory Initialization.\n");
			exit(1);
		}
		strcpy(temp_str, temp_account_str);
		strcat(temp_str, middle_str);
		strcat(temp_str, temp_amount_str);
		strcat(temp_str, end_str);
		fputs(temp_str, outfile);
		free(temp_str);
		temp_account = temp_account->next_account;
	}


}
