// Basic Banking System

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#define el std::endl
#define filename "Accounts.txt"
#define cwd std::filesystem::current_path() // current working directory


class BankAccount
{
private:
	std::string account_number;
	std::string first_name;
	std::string middle_initial;
	std::string last_name;
	float balance;
	bool closed;

public:
	BankAccount() {};
	BankAccount(std::string bal) // constructor for new BankAccount object with specified starting balance
	{
		try
		{
			set_acct_num("");
			set_first_name("");
			set_middle_initial("");
			set_last_name("");
			set_balance(bal);
			set_closed(false);
		}
		catch (int e)
		{
			switch (e)
			{
			case 5:
				std::cout << "Accounts file may not exist or you may not have read and write access." << el;
				std::cout << el;
			}
		}
	};
	// BankAccount :BankAccount("0.00") {}; //constructor for new BankAccount object with balance of zero
	~BankAccount() {};

	// getter functions
	std::string get_acct_num();
	std::string get_first_name();
	std::string get_middle_initial();
	std::string get_last_name();
	float get_balance();
	bool is_closed();

	// setter functions
	void set_acct_num(std::string account_number);
	void set_first_name(std::string first_name);
	void set_middle_initial(std::string middle_initial);
	void set_last_name(std::string last_name);
	void set_balance(std::string balance);
	void set_closed(bool closed);

	// other member functions
	void withdraw() {};
	void deposit() {};
	void check_acct_balance() {};

	// overload operators
	friend std::ifstream& operator>>(std::ifstream& ifs, BankAccount& ba);
	friend std::ofstream& operator<<(std::ofstream& ofs, BankAccount& ba);
};


std::string get_accounts_file_path_str(std::string file_name)
{
	decltype(cwd) file_path = cwd.generic_string() + "\\" + file_name;
	file_path.make_preferred();
	std::string file_path_str = file_path.generic_string();
	return file_path_str;
}

std::vector<BankAccount> get_all_accounts()
{
	std::ifstream ifs;
	std::vector<BankAccount> accounts;
	BankAccount acct;

	std::string accounts_file = get_accounts_file_path_str(filename);

	ifs.open(accounts_file);
	if (!ifs.is_open())
	{
		std::cout << "Cannot open accounts file." << el;
		throw 5;
	}
	else
	{
		while (!ifs.eof())
		{
			ifs >> acct;
			BankAccount& acct_ref = acct;
			accounts.insert(accounts.begin(), acct_ref);
		}
	}
	return accounts;
}

int check_account_number_unique(std::string rand_16_digits)
{
	std::string acct_num, accounts_file;
	std::vector<BankAccount>accounts_vec;
	std::ifstream ifs;

	accounts_vec = get_all_accounts();

	for (BankAccount acct : accounts_vec)
	{
		if (acct.get_acct_num() == rand_16_digits)
		{
			return 1;
		}
		else {}
	}
	// if all current account numbers do not match new rand_16_digits
	return 0;
}

std::string rand_16_digits()
{
	char rand_16_num_str[17]; // 16 chars plus null terminator
	rand_16_num_str[16] = '\0'; // 0-15 indices = 16 chars, put null terminator in last index of char array

	for (int i = 0; i < 16; i++)
	{
		// use time to generate random number
		std::chrono::utc_clock::time_point tp = std::chrono::utc_clock().now();
		std::string s = std::format("{:%Y%m%d%H%M%S}", tp);
		rand_16_num_str[i] = s[21]; // last element of time value
	}

	int is_unqiue = check_account_number_unique(rand_16_num_str);

	if (is_unqiue == 0)
	{
		return rand_16_num_str;
	}
	else
	{
		rand_16_digits(); // keep trying until unique number created
	}
}

// allows for hyphens
bool check_string_only_letters(std::string str)
{
	for (int i = 0; i < str.size(); i++)
	{
		int ascii = (int)str[i]; // change char to int in ASCII codes

		if (
			! // NOT
			(
				((ascii >= 65) && (ascii <= 90)) // Upper case ASCII A-Z
				||
				((ascii >= 97) && (ascii <= 122)) // Lower case ASCII a-z
				||
				ascii == 45 // ASCII hyphen
				)
			) {
			return false;
		}
		else {}
	}
	// return true if all elements of str are ASCII letters or hyphen
	return true;
}

std::string str_to_upper_case(std::string str)
{
	// assumes check_str_only_letters function has alreay been performed
	std::string upper_str;
	for (int i = 0; i < str.size(); i++)
	{
		int ascii = (int)str[i]; // change char to int in ASCII codes
		std::cout << "Letter - " << str[i] << "  ASCII - " << ascii << el;

		if ((ascii >= 97) && (ascii <= 122)) // ASCII a-z letters have codes 97-122 (inclusive)
		{
			ascii -= 32; // each lower and upper case letter separated by 32 ASCII code values
			upper_str += (char)ascii; // int to char then char append to string
		}
		else { upper_str += str[i]; }
	}
	return upper_str;
}

std::string user_confirm_input() {
	std::string yes_no;
	std::cout << "Please enter 'y' for yes or 'n' for no: ";
	std::cin >> yes_no;
	yes_no = str_to_upper_case(yes_no); // convert user confirmation input into upper case
	std::cout << el;
	return yes_no;
}

std::string enter_first_name() {
	std::string first_name;

	std::cout << "Please enter your First Name: ";
	std::cin >> first_name;
	bool all_letters = check_string_only_letters(first_name);

	if (all_letters == true)
	{
		first_name = str_to_upper_case(first_name);
		std::cout << el;

		std::string yes_no;
		std::cout << "Is " << first_name << " correct? ";
		yes_no = user_confirm_input();
		std::cout << el;

		while (yes_no != "Y" && yes_no != "N" && yes_no != "YES" && yes_no != "NO") {
			yes_no = user_confirm_input();
			std::cout << el;
		}

		if (yes_no[0] == 'Y') { return first_name; }
		else { first_name = enter_first_name(); return first_name; }
	}
	else
	{
		std::cout << "Please enter only letters for a first name." << el;
		first_name = enter_first_name();
		return first_name;
	}
}

std::string enter_middle_initial() {
	std::string middle_initial;

	std::cout << "Please enter your Middle Initial: ";
	std::cin >> middle_initial;
	bool all_letters = check_string_only_letters(middle_initial);

	if (all_letters == true)
	{
		middle_initial = str_to_upper_case(middle_initial);
		std::cout << el;

		if (middle_initial.length() > 1) {
			std::cout << "Please enter a single letter." << el;
			middle_initial = enter_middle_initial();
		}

		return middle_initial;
	}
	else
	{
		std::cout << "Please enter only letters for a middle initial." << el;
		middle_initial = enter_middle_initial();
		return middle_initial;
	}
}

std::string enter_last_name() {
	std::string last_name;

	std::cout << "Please enter your Last Name: ";
	std::cin >> last_name;
	bool all_letters = check_string_only_letters(last_name);

	if (all_letters == true)
	{
		last_name = str_to_upper_case(last_name);
		std::cout << el;

		std::string yes_no;
		std::cout << "Is " << last_name << " correct? ";
		yes_no = user_confirm_input();
		std::cout << el;

		while (yes_no != "Y" && yes_no != "N" && yes_no != "YES" && yes_no != "NO") {
			yes_no = user_confirm_input();
			std::cout << el;
		}

		if (yes_no[0] == 'Y') { return last_name; }
		else { last_name = enter_last_name(); return last_name; }
	}
	else
	{
		std::cout << "Please enter only letters for a last name." << el;
		last_name = enter_last_name();
		return last_name;
	}
}

void write_account_to_file(BankAccount& account)
{
	std::string file_path_str = get_accounts_file_path_str(filename);

	std::ofstream ofs;
	ofs.open(file_path_str, std::ios::app);
	if (!ofs.is_open())
	{
		std::cout << "Cannot open accounts file." << el;
	}
	else
	{
		ofs << account;
		ofs.close();
	}
}

void write_accounts_to_file(std::vector<BankAccount> accounts)
{
	std::string acounts_file = get_accounts_file_path_str(filename);

	std::ofstream ofs;
	ofs.open(acounts_file, std::ios::trunc);
	if (!ofs.is_open())
	{
		std::cout << "Cannot open accounts file." << el;
	}
	else
	{
		for (auto& acct : accounts)
		{
			ofs << acct;
		}
		ofs.close();
	}
}

// getters
std::string BankAccount::get_acct_num()
{
	return account_number;
}
std::string BankAccount::get_first_name()
{
	return first_name;
}
std::string BankAccount::get_middle_initial()
{
	return middle_initial;
}
std::string BankAccount::get_last_name()
{
	return last_name;
}
float BankAccount::get_balance()
{
	return balance;
}
bool BankAccount::is_closed()
{
	return closed;
}

// setters
void BankAccount::set_acct_num(std::string new_account_num)
{
	if (new_account_num.length() == 0)
	{
		std::string rand_acct_num = rand_16_digits();
		account_number = rand_acct_num;
	}
	else
	{
		while (new_account_num.length() != 16)
		{
			std::cout << "Account number must be 16 digits long." << el;
			std::cout << "Enter new account number: ";
			new_account_num = "";
			std::cin >> new_account_num;
		}
		account_number = new_account_num;
	}
}

void BankAccount::set_first_name(std::string first_name = "")
{
	if (first_name.length() == 0)
	{
		std::string first_name_input = enter_first_name();
		this->first_name = first_name_input;
	}
	else
	{
		this->first_name = first_name;
	}
}

void BankAccount::set_middle_initial(std::string middle_initial = "")
{
	if (middle_initial.length() == 0)
	{
		std::string middle_initial_input = enter_middle_initial();
		this->middle_initial = middle_initial_input;
	}
	else
	{
		this->middle_initial = middle_initial;
	}
}

void BankAccount::set_last_name(std::string last_name = "")
{
	if (last_name.length() == 0)
	{
		std::string last_name_input = enter_last_name();
		this->last_name = last_name_input;
	}
	else
	{
		this->last_name = last_name;
	}
}

void BankAccount::set_balance(std::string new_balance = "0.00")
{
	float new_bal = std::stof(new_balance);
	balance = new_bal;
}

void BankAccount::set_closed(bool closed)
{
	this->closed = closed;
}


// member functions
std::ofstream& operator<<(std::ofstream& ofs, BankAccount& ba)
{
	ofs << el;
	ofs << ba.account_number << el;
	ofs << ba.first_name << el;
	ofs << ba.middle_initial << el;
	ofs << ba.last_name << el;
	ofs << std::to_string(ba.balance) << el;
	ofs << ba.closed;
	return ofs;
}

std::ifstream& operator>>(std::ifstream& ifs, BankAccount& ba)
{
	ifs >> ba.account_number;
	ifs >> ba.first_name;
	ifs >> ba.middle_initial;
	ifs >> ba.last_name;
	ifs >> ba.balance;
	ifs >> ba.closed;
	return ifs;
}

// Global Functions
void open_account(std::string initial_bal = "0.00")
{
	BankAccount bank_acct(initial_bal);
	write_account_to_file(bank_acct);
}

void close_account()
{
	std::string acct_num;
	std::cout << "Enter account number to close: ";
	std::cin >> acct_num;

	std::vector<BankAccount> accounts = get_all_accounts();

	for (auto& acct : accounts)
	{
		if (acct.get_acct_num() == acct_num)
		{
			acct.set_closed(true);
		}
	}

	// remove account with closed status = true
	accounts.erase(
		std::remove_if(
			accounts.begin(),
			accounts.end(),
			[](BankAccount acct) { return acct.is_closed(); }
		),
		accounts.end()
	);

	// write only open accounts back to file
	write_accounts_to_file(accounts);
}

void withdraw()
{
	std::vector<BankAccount> accounts = get_all_accounts();

	std::string acct_num;
	std::cout << "Enter account number to withdraw from: ";
	std::cin >> acct_num;

	float withdraw_amt;
	std::cout << "Enter amount to withdraw: ";
	std::cin >> withdraw_amt;

	for (auto& acct : accounts)
	{
		if (acct.get_acct_num() == acct_num)
		{
			float bal = acct.get_balance();
			if (bal < withdraw_amt)
			{
				std::cout << "Balance insufficient." << el;
			}
			else
			{
				std::string bal_str = std::to_string(bal - withdraw_amt);
				acct.set_balance(bal_str);
			}

		}
	}
	// write only open accounts back to file
	write_accounts_to_file(accounts);
}

void deposit()
{
	std::vector<BankAccount> accounts = get_all_accounts();

	std::string acct_num;
	std::cout << "Enter account number to deposit into: ";
	std::cin >> acct_num;

	float deposit_amt;
	std::cout << "Enter amount to deposit: ";
	std::cin >> deposit_amt;

	for (auto& acct : accounts)
	{
		if (acct.get_acct_num() == acct_num)
		{
			float bal = acct.get_balance();
			std::string bal_str = std::to_string(bal + deposit_amt);
			acct.set_balance(bal_str);
		}
	}

	// write only open accounts back to file
	write_accounts_to_file(accounts);
}

void check_acct_balance()
{
	std::vector<BankAccount> accounts = get_all_accounts();

	std::string acct_num;
	std::cout << "Enter account number: ";
	std::cin >> acct_num;

	for (auto& acct : accounts)
	{
		if (acct.get_acct_num() == acct_num)
		{
			std::cout << "Balance is: " << acct.get_balance() << el;
			break;
		}
		else {}
	}
}

void display_users_accounts()
{
	std::vector<BankAccount> accounts = get_all_accounts();

	std::string first_name;
	first_name = enter_first_name();

	std::string middle_initial;
	middle_initial = enter_middle_initial();

	std::string last_name;
	last_name = enter_last_name();

	for (auto acct : accounts)
	{
		if (acct.get_first_name() == first_name && acct.get_middle_initial() == middle_initial && acct.get_last_name() == last_name)
		{
			std::string account_holder = acct.get_first_name() + " " + acct.get_middle_initial() + " " + acct.get_last_name();
			std::cout << "Account Number: " << acct.get_acct_num() << el;
			std::cout << "Account Holder: " << account_holder << el;
			std::cout << "Balance: " << acct.get_balance() << el;
		}
	}
}



void menu()
{
	std::map<int, std::string> choice_map;
	choice_map.insert(std::pair<int, std::string>(1, "1. Open Account"));
	choice_map.insert(std::pair<int, std::string>(2, "2. Close Account"));
	choice_map.insert(std::pair<int, std::string>(3, "3. Withdraw"));
	choice_map.insert(std::pair<int, std::string>(4, "4. Deposit"));
	choice_map.insert(std::pair<int, std::string>(5, "5. Check Account Balance"));
	choice_map.insert(std::pair<int, std::string>(6, "6. See Users Accounts"));
	choice_map.insert(std::pair<int, std::string>(7, "7. Quit"));

	// display choices
	std::map<int, std::string>::iterator itr;
	for (itr = choice_map.begin(); itr != choice_map.end(); itr++)
	{
		std::cout << itr->second << el;
	}

	int menu_choice;
	std::cout << el;
	std::cout << "Please enter a number from the options above." << el;
	std::cin >> menu_choice;
	std::cout << el;

	switch (menu_choice)
	{
	case 1:
		open_account();
		menu();
		break;
	case 2:
		close_account();
		menu();
		break;
	case 3:
		withdraw();
		menu();
		break;
	case 4:
		deposit();
		menu();
		break;
	case 5:
		check_acct_balance();
		menu();
		break;
	case 6:
		display_users_accounts();
		menu();
		break;
	case 7:
		std::cout << "Quitting" << el;
	}
}

int main()
{
	menu();
	return 0;
}