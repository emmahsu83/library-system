//==================================================
//Member4_BorrowReturn.cpp
//Module : Borrow and Return(Member 4)
//Project : Books Borrowing Systme (PRG2203 COursework 3)
//Compile : g++ -std=c++11 Member4_BorrowReturn.cpp -o member4
//Note : All cin/cout is done in main(). The other function only process data and return code.
//========================================================
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

//-------------constants (shared block)----------------
	const int MAX_ADMINS = 10;
	const int MAX_BOOKS = 100;
	const int MAX_BORROWERS = 100;
	const int MAX_LOANS = 5;
	
	const int RESULT_OK = 0;
	const int ERR_BORROWER_NOT_FOUND = 1;
	const int ERR_BOOK_NOT_FOUND = 2;
	const int ERR_NO_COPIES = 3;
	const int ERR_LOAN_LIMIT = 4;
	const int ERR_ALREADY_HAS_BOOK = 5;
	const int ERR_NOT_BORROWED = 6;
	
	struct Admin
	{
		string userId;
		string password;
	};
	
	struct Book
	{
		string title;
		string author;
		string isbn;
		int copiesAvailable;
		int copiesCheckedOut;
	};
	
	struct Borrower
	{
		string name;
		string address;
		string contact;
		string borrowedTitles[MAX_LOANS];
		
		int borrowedCount;
	};
	//------prototypes-------
	//My function (Member 4)
	int borrowBook(Borrower borrowers[], int borrowerCount, Book books[],
	               int bookCount, string borrowerName, string bookTitle);
	               
	int returnBook(Borrower borrowers[], int borrowerCount, Book books[],
	               int bookCount, string borrowerName, string bookTitle);
	//My private helper (only used inside this file)
	int findloanIndex(const Borrower &borroweer, string title);
	
	//Temporary stand-ins for thhe other members' functions.
	//Remove these 2 functions when merging (Member 2 and 3 own them).
	int findBookByTitle(const Book books[], int bookCount, string title);
	int findBorrowerByName(const Borrower borrowers[], int borrowerCount, string name);
	
	//DEMO ONLY: fills sample data. REMOVE when merging
	void loadSampleData(Borrower borrowers[], int &borrowerCount, Book books[], int &bookCount);
	
	//======================================================
	//findLoanIndex
	//Looks one borrower's borrowedTitle[]for a title.
	//Returns the index (0 .. borrowedCount-1) or -1 if not held.
	//===================================================
	int findLoanIndex(const Borrower &borrower, string title)
	{
		for(int i = 0; i <borrower.borrowedCount; i++)
		{
			if(borrower.borrowedTitles[i]==title)
			{
				return i;
			}
		}
		return -1;
	 }
	
	//borrowBook
	//Lets an existing borrower borrow an available book.
	//Parameters: borrowers/borrowerCount - borrower array and counter
	// books/bookCount - bok array adn counter
	// borrowerName, bookTitle - what the admin typed
	//Returns :RESULT_OK on success, otherwise one of the ERR_codes.
	//Nothing is changed unless every check passes.
	//=====================================================
	int borrowBook(Borrower borrowers[], int borrowerCount, Book books[], int bookCount, string borrowerName, string bookTitle)
	{
		//1.Borrower must exist
		int b = findBookByTitle(books, bookCount, bookTitle);
		if(b == -1)
		{
			return ERR_BOOK_NOT_FOUND;
		}
		//2.Book must exist
		int k = findBookByTitle(books, bookCount, bookTitle);
		if(k == -1)
		{
			return ERR_BOOK_NOT_FOUND;
		}
		//3.At least one copy must be available
		if (books[k].copiesAvailable <= 0)
		{
			return ERR_NO_COPIES;
		}
		//4.Borrower must be under the loan limit
		if (borrowers[b].borrowedCount>=MAX_LOANS)
		{
			return ERR_LOAN_LIMIT;
		}
		//5.Borrower must not already hold this title
		if (findLoanIndex(borrowers[b], books[k].title) != -1)
		{
			return ERR_ALREADY_HAS_BOOK;
		}
		//All checks passed: update inventory and borrower record
		books[k].copiesAvailable--;
		books[k].copiesCheckedOut++;
		borrowers[b].borrowedTitles[borrowers[b].borrowedCount] = books[k].title;
		borrowers[b].borrowedCount++;
		
		return RESULT_OK;
	}
	
	//===========================================================
	//returnBook
	//Lets a borrower return a book they currently hold.
	//Parameters and return valuse are the same as borrowBook
	//(it can also return ERR_NOT_BORROWED).
	//=====================================
	int returnBook(Borrower borrowers[], int borrowerCount, Book books[], int bookCount, string borrowerName, string bookTitle)
	{
		//1.Borrower must exist
		int b = findBorrowerByName(borrowers, borrowerCount, borrowerName);
		if (b == -1)
		{
			return ERR_BORROWER_NOT_FOUND;
		}
		//2.Book must exist
		int k = findBookByTitle(books, bookCount, bookTitle);
		if (k == -1)
		{
			return ERR_BOOK_NOT_FOUND;
		}
		//3.Borrower must actually hold this title
		int loanIndex = findLoanIndex(borrowers[b], books[k].title);
		if(loanIndex == -1)
		{
			return ERR_NOT_BORROWED;
		}
		
		//Remove the title: shift everythign after it one place to the left
		for (int i = loanIndex; i <borrowers[b].borrowedCount - 1; i++)
		{
			borrowers[b].borrowedTitles[i] = borrowers[b].borrowedTitles[i + 1];
		}
	//clear the last used slot and reduce the counter
	borrowers[b].borrowedTitles[borrowers[b].borrowedCount - 1] = "";
	borrowers[b].borrowedCount--;
	
	//Update the inventory
	books[k].copiesAvailable++;
	books[k].copiesCheckedOut--;
	
	return RESULT_OK;
		
	}
	
	 //============================================  
	 //TEMPORARY: findBookByTitle(Member 2 writes the real one)
	 //Returns index of the book, or -1 if not found.
	 //==============================================
	 int findBookByTitle(const Book books[], int bookCount, string title)
	 {
	 	for(int i = 0; i < bookCount; i++)
	 	{
	 		if(books[i].title == title)
	 		{
	 			return i;
			 }
		 }
		return -1;		
	 }     
	//=========================================
	//TEMPORARY: findBorrowerByName(Member 3 wirtes the real one)
	//Returns index of the borrower, or -1 if not found.
	//==========================================
	int findBorrowerByName(const Borrower borrowers[], int borrowerCount, string name)
	{
		for(int i = 0; i < borrowerCount; i++)
		{
			if(borrowers[i].name == name)
			{
				return i;
			}
		}
		return -1;
	}  
	//========================================
	//DEMO ONLY: sample data so every test case can be shown.
	//-Alice holds 5 books (at the loan limit)
	//-"Python Guide" has 0 copies available
	//============================================
	void loadSampleData(Borrower borrowers[], int &borrowerCount, Book books[], int & bookCount)
	{
		const string titles[7] = {"C++ Programming", "Java Basics", "Python Guide", "Data Structures", "Web Design", "Database Systems", "Networking Basics"};
		const string authors[7] = {"Tony Gaddis", "Herbert Schildt", "Mark Lutz", "Mark Wesis", "Jon Duckett", "Ramez Elmasri", "Andrew Tanenbaum"};
		const string isbns[7] = {"9781118008188", "9780133970777", "9780132126953"};
		const int available[7] = {3, 2, 0, 2, 1, 4, 3};
		const int checkedOut[7] = {1, 1, 1, 1, 1, 1, 0};
		
		bookCount = 7;
		for(int i = 0; i <bookCount; i++)
		{
			books[i].title = titles[i];
			books[i].author = authors[i];
			books[i].isbn   = isbns[i];
			books[i].copiesAvailable = available[i];
			books[i].copiesCheckedOut = checkedOut[i];
		}
		borrowerCount = 3;
		for(int i = 0; i <borrowerCount; i++)
		{
			borrowers[i].borrowedCount = 0;
		}
		borrowers[0].name = "John";
		borrowers[0].address = "12 Jalan Bunga, Seremban";
		borrowers[0].contact = "0123456789";
		borrowers[0].borrowedTitles[0] = "C++ Programming";
		borrowers[0].borrowedCount = 1;
		borrowers[1].name = "Alice";
		borrowers[1].address = "8 Taman Indah, Nilai";
		borrowers[1].contact = "0198765432";
		borrowers[1].borrowedTitles[0] = "Java Basics";
		borrowers[1].borrowedTitles[1] = "Python Guide";
		borrowers[1].borrowedTitles[2] = "Data Structures";
		borrowers[1].borrowedTitles[3] = "Web Design";
		borrowers[1].borrowedTitles[4] = "Database Systems";
		borrowers[1].borrowedCount = 5;
		
		borrowers[2].name = "Mei";
		borrowers[2].address = "5 Lorong Damai, Port Dickson";
		borrowers[2].contact = "0171112222";
		borrowers[2].borrowedCount = 0;
	}
	
	//main: ALl input and outpur happens here.
	
	int main()
	{
		Borrower borrowers[MAX_BORROWERS];
		Book books[MAX_BOOKS];
		int borrowerCount = 0;
		int bookCount = 0;
		string choice, name, title;
		bool running = true;
		
		loadSampleData(borrowers, borrowerCount, books, bookCount);
		
		while(running)
		{
			cout<<"\n=================================\n";
			cout<<" BOOKS BORROWINGS SYSTEM\n";
			cout<<" (Member 4 - Borrow/Return)\n";
			cout<<"===================================\n";
			cout<<"1. Borrow a book\n";
			cout<<"2. Return a book\n";
			cout<<"3. Display book inventory\n";
			cout<<"4. Display all borrowers\n";
			cout<<"5. Exit\n";
			cout<<"Enter your choice (1-5): ";
			getline(cin, choice);
			
			if (cin.fail()) //input stream closed 
			{
				break;
			}
			if (choice == "1" || choice == "2")
			{
				cout<<"Enter borrower name: ";
				getline(cin, name);
				cout<<"Enter book title: ";
				getline(cin, title);
				
			    //input validation: empty text and the | character 
			    if (name.empty() || title.empty())
			    {
			    	cout<<"Error: name and title must not be empty.\n";
				}
				else if(name.find('|') != string::npos || title.find('|') != string::npos)
				{
					cout<<"Error: the | character is not allowed.\n";
				}
				else
				{
					int result;
					if(choice == "1")
					{
						result =borrowBook(borrowers, borrowerCount, books, bookCount, name, title);
						
					}
					else
					{
						result = returnBook(borrowers, borrowerCount, books, bookCount, name, title);
						
					}
					//show a message for the result code
					if(result == RESULT_OK)
					{
						if(choice == "1")
						{
							cout<<"Borrowing successful! "<<name<<"borrrowed\""<<title<<"\".\n";
						}
						else
						{
							cout<<"Return successful! "<<name<<"returned\""<<title<<"\".\n";
						}
				
					}
					else if(result == ERR_BORROWER_NOT_FOUND)
					{
						cout<<"Error: borrower\""<<name<<"\" was not found.\n";
					}
					else if(result == ERR_BOOK_NOT_FOUND)
					{
						cout<<"Error: book\""<<title<<"\"are available.\n";
					}
					else if(result == ERR_NO_COPIES)
					{
						cout<<"Error: no copies of\""<<title<<"\"are available.\n";	
					}
					else if(result == ERR_LOAN_LIMIT)
					{
						cout<<"Error: "<<name<<"already holds"<<MAX_LOANS<<"books(the limit).\n";
					}
					else if(result == ERR_ALREADY_HAS_BOOK)
					{
						cout<<"Error: "<<name<<"already has\""<<title<<"\".\n";
					}
					else if(result == ERR_NOT_BORROWED)
					{
						cout<<"Error: "<<name<<"has not borrowed\""<<title<<"\".\n";
					}
				}
			}
			else if(choice == "3")
			{
				cout<<"\n---BOOK INVENTORY ---\n";
				for(int i = 0; i <bookCount; i++)
				{
					cout<<i + 1<<". "<<books[i].title<<"|"<<books[i].author<<"| ISBN: "<<books[i].copiesAvailable<<"| Checked out: "<<books[i].copiesCheckedOut<<"\n";
				}
			}
			else if(choice == "4")
			{
				cout<<"\n---BORROWERS---\n";
				for(int i = 0; i<borrowerCount; i++)
				{
					cout<<i + 1<<". "<<borrowers[i].name<<"|"<<borrowers[i].address<<"|"<<borrowers[i].contact<<"\n";
					cout<<" Books held("<<borrowers[i].borrowedCount<<"): ";
					if(borrowers[i].borrowedCount == 0)
					{
						cout<<"none";
					}
					for(int j = 0; j < borrowers[i].borrowedCount; j++)
					{
						cout<<borrowers[i].borrowedTitles[j];
						if(j < borrowers[i].borrowedCount - 1)
						{
							cout<<", ";
						}
					}
					cout<<"\n";
				}
			}
			else if(choice == "5")
			{
				cout<<"Goodbye!\n";
				running = false;
			}
			else
			{
		
	     		cout<<"Invalid choice. Please enter a number from 1 to 5.\n";
		}	}
			
		
		return 0;
	}
	
