#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <bits/stdc++.h>
#include<string>
#include<cstring>
#include<cctype>
#include<typeinfo>
#include<cstdlib>
#include<stdio.h>
#include <conio.h>
#include <Windows.h>

using namespace std;

const int BOARD_WIDTH=5; //The number of coloumns in the game board
const int BOARD_HEIGHT=5; //The number of rows in the game board
const int IN_LINE=4; //How much player's signs must be in line for win
const int EASY=1; //low level of AI
const int MEDIUM=2; //middle level of AI
const int HARD=3; //the highest level of AI
int	ai_level=HARD; //The level of AI default
	//Showing the main_menu function
void show_main_menu();
	//Showing the options_menu function
void show_options_menu();
	//Showing the rules function
void show_rules();
	//Whose turn will be the first function	
void first_turn(char *now_turn, char crosses, char nulles);
	//Changing player who turn next function
void next_turn(char *now_turn, char crosses, char nulles);
	//Showing gameplay screen function
void show_game_screen(char now_turn, char game_screen[][BOARD_WIDTH]);
	//Turn checking function
void turn(bool *exit_round, char now_turn, int game_board[][BOARD_WIDTH], char game_screen[][BOARD_WIDTH]);
	//For checking backend part function (only while codding/testing)
void show_how_it_runs(int game_board_cr[][BOARD_WIDTH], int game_board_nl[][BOARD_WIDTH]);
	//Check if there is win or draw rezult
bool check_for_end_rezult(int game_board[][BOARD_WIDTH], int win_or_draw);
	//Check if smb won the game function
bool check_for_win_combination(int game_board[][BOARD_WIDTH]);
	//Check for only draw result function
bool check_for_draw(int game_board_cr[][BOARD_WIDTH], int game_board_nl[][BOARD_WIDTH]);
	//AI turn processing function
void ai_turn(char n_t, int game_board[][BOARD_WIDTH], int game_board_op[][BOARD_WIDTH], char game_screen[][BOARD_WIDTH]);
	//searching for pottentional winner combination function (puts between two)
int check_between_two(int game_board[][BOARD_WIDTH], char game_screen[][BOARD_WIDTH]);
	//searching for pottentional winner combination function (puts next to two)	
int check_next_to_two(int game_board[][BOARD_WIDTH], char game_screen[][BOARD_WIDTH]);
/*			--------------------------------------
						Here the programm starts!!!
			--------------------------------------
*/			
int main() {
	bool end_the_game=0; //leave the programm if true
	string menu_mod;
	char now_turn; //Whose turn is it now
	char crosses='X', nulles='O'; //players(crosses - player1, nulles - player2) and symbols that are used (can be changed in options menu)
		//Main menu
	while(!end_the_game){
		system("cls");
		show_main_menu();
			//Virtual buttons initializating
		const string SINGLE_PLAYER="1";
		const string TWO_PLAYERS="2";						
		const string OPTIONS="3";
		const string RULES="4";			
		const string ABOUT="5";
		const string QUITE_THE_GAME="6";
			//Choosing option from main menu
		menu_mod="0";
		cout<<"\nPlease choose option from menu\n";
		while(menu_mod!=SINGLE_PLAYER && menu_mod!=TWO_PLAYERS && menu_mod!=OPTIONS && menu_mod!=RULES && menu_mod!=ABOUT && menu_mod!=QUITE_THE_GAME){
			cin>>menu_mod;
			cout<<"Wrong option, please try from the menu\n";
		}
		system("cls");
		if(menu_mod==QUITE_THE_GAME) // Quit the game
			end_the_game=1;
		if(menu_mod==OPTIONS){ //Options
			bool to_main_menu=0; //Return to the main menu if true
			while(!to_main_menu){
				system("cls");
				show_options_menu();
					//Virtual buttons initializating
				const string AI_LEVEL="1";
				const string GAME_SYMBOLS="2";
				const string QUITE_OPTIONS="3";
					//Choosing option from options menu
				string option_mod="0";
				cout<<"\nPlease choose option from menu\n";
				while(option_mod!=AI_LEVEL && option_mod!=GAME_SYMBOLS && option_mod!=QUITE_OPTIONS){
					cin>>option_mod;
					cout<<"Wrong option, please try from the menu\n";					
				}	
				if(option_mod==AI_LEVEL){ //Changing AI level
					string ai_level_string;
					do{
						system("cls");
						cout<<"\tChanging AI level\n";
						cout<<"\nChange to:\t low - Press 1\n\t\t middle - Press 2\n\t\t high - Press 3\n";
						cin>>ai_level_string; //Changing level of AI for single player mod
					}while(ai_level_string!="1" && ai_level_string!="2" && ai_level_string!="3");
					ai_level = (ai_level_string=="1"?EASY:(ai_level_string=="2"?MEDIUM:HARD));
				}
				if(option_mod==GAME_SYMBOLS){ //Changing game symbols
					system("cls");
					cout<<"\n\tEnter your player (or first player) game symbol\t ";
					cin>>crosses; //changing symbol for human or 1st player in two players game mod
					cout<<"\n\tEnter computer (or second player) game symbol\t ";
					cin>>nulles; //changing symbol for computer or 2nd player in two players game mod
				}
				system("cls");
				if(option_mod==QUITE_OPTIONS) //Return to the main menu
					to_main_menu=1;
			}
		}
		if(menu_mod==RULES) 	//Reading rules	
			show_rules();
		if(menu_mod==ABOUT){    //Information about the program	
			cout<<"\n\n\t\tTIC-TAC-TOE 5x5 ver 1.0\n\n";
			cout<<"\tMade by Serhii Osavelyuk 01.02.23.\n";
			cout<<"\tIt was a project after learning beginner course of C++ in Vinnitsya IT Academy.\n\n";
			system("pause");
		}
		if(menu_mod==SINGLE_PLAYER || menu_mod==TWO_PLAYERS){ //single player or two players mods
		/* * * * * * * * * * * * * * * * * * * *
				Here the game starts
		* * * * * * * * * * * * * * * * * * * */
		bool win=0, draw=0; //win or draw end resaults
			//Gameplay boards for calculation
		int game_board_cr[BOARD_HEIGHT][BOARD_WIDTH]={};//for 1st player (human)
		int game_board_nl[BOARD_HEIGHT][BOARD_WIDTH]={}; //for 2nd player (or computer)
			//Gameplay board for screen
		char game_screen[BOARD_HEIGHT][BOARD_WIDTH]={};
			for(int i=0;i<BOARD_HEIGHT;i++){
				for(int j=0;j<BOARD_WIDTH;j++){
					game_screen[i][j]=' ';
				}
			}
		first_turn(&now_turn, crosses, nulles);
			//game playing loop
		for(int i=0;i<BOARD_HEIGHT*BOARD_WIDTH;i++){
			bool exit_round=0; //leaving current game if true
			if(menu_mod==SINGLE_PLAYER)
				cout<<"\t\t\t\tSingle player game\n\n";
			if(menu_mod==TWO_PLAYERS)
				cout<<"\t\t\t\tTwo players game\n\n";
			show_game_screen(now_turn, game_screen);
			if(win==1)
				break;
			if(draw==1){
				cout<<"\n\n            Here will be no winner in the round:)\n\nIf you want return to the main menu press \"Esc\"\n\n";
				if (_getch()==27)
	    			break;
			}
			if(now_turn==crosses){  //human
				//Show_how_it_runs(game_board_cr, size, size, game_board_nl, size, size);
				turn(&exit_round, now_turn, game_board_cr, game_screen);
					if (exit_round==1)
						break;
				win=check_for_win_combination(game_board_cr);
			}
			else{  //computer
				//Show_how_it_runs(game_board_cr, size, size, game_board_nl, size, size);
				if(menu_mod==SINGLE_PLAYER)
				ai_turn(now_turn, game_board_nl, game_board_cr, game_screen);
				else if(menu_mod==TWO_PLAYERS)
				turn(&exit_round, now_turn, game_board_nl, game_screen);
					if (exit_round==1)
						break;
				win=check_for_win_combination(game_board_nl);	
			}
			if(win==0){ //After smb win the will be no turnes at last and we do not need changing turn
				next_turn(&now_turn, crosses, nulles);
			}
			draw=check_for_draw(game_board_cr, game_board_nl);
			system("cls");	
		}
		if(win==1)
			cout<<"\n\n             "<<now_turn<<"  won :)\n\n";		
		else
			cout<<"\n\n             Friendshtip won ;)\n\n";
		system("pause");
		}		
	}
}

	//Showing the main_menu function
void show_main_menu(){
	cout<<"\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\t\t\t\t\t";
	cout<<"\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
	cout<<"\xBA                          \xBA\t\t\t\t\t";
	cout<<"\xBA                          \xBA\n";
	cout<<"\xBA Single Player   Press 1  \xBA\t\t\t\t\t";
	cout<<"\xBA About           Press 5  \xBA\n";
	cout<<"\xBA                          \xBA\t\t\t\t\t";
	cout<<"\xBA                          \xBA\n";
	cout<<"\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\t\t\t\t\t";
	cout<<"\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n";
	cout<<"\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
	cout<<"\xBA                          \xBA\n";
	cout<<"\xBA Two Players     Press 2  \xBA\n";
	cout<<"\xBA                          \xBA\n";
	cout<<"\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n";
	cout<<"\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
	cout<<"\xBA                          \xBA\n";
	cout<<"\xBA Options         Press 3  \xBA\n";
	cout<<"\xBA                          \xBA\n";
	cout<<"\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n";
	cout<<"\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\t\t\t\t\t";
	cout<<"\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
	cout<<"\xBA                          \xBA\t\t\t\t\t";
	cout<<"\xBA                          \xBA\n";
	cout<<"\xBA Rules           Press 4  \xBA\t\t\t\t\t";
	cout<<"\xBA Quite the game  Press 6  \xBA\n";
	cout<<"\xBA                          \xBA\t\t\t\t\t";
	cout<<"\xBA                          \xBA\n";
	cout<<"\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\t\t\t\t\t";
	cout<<"\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n";
}

	//Showing the options_menu function
void show_options_menu(){
cout<<"\n\n\t\t\t\tOptions\n";
	cout<<"\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
	cout<<"\xBA                        \xBA\n";
	cout<<"\xBA AI level\tPress 1  \xBA\n";
	cout<<"\xBA                        \xBA\n";
	cout<<"\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n";
	cout<<"\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
	cout<<"\xBA                        \xBA\n";
	cout<<"\xBA Game symbols\tPress 2  \xBA\n";
	cout<<"\xBA                        \xBA\n";
	cout<<"\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n";
	cout<<"\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
	cout<<"\xBA                        \xBA\n";
	cout<<"\xBA Quit options\tPress 3  \xBA\n";
	cout<<"\xBA                        \xBA\n";
	cout<<"\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n";
}

	//Showing the rules function
void show_rules(){
	cout<<"\n\n\t\t\tRULES FOR TIC-TAC-TOE 5x5 \n\n";
	cout<<"\t1. The game is played on a grid that's 5 squares by 5 squares.\n";
	cout<<"\t2. Default you are X, your friend (or the computer in this case) is O.\n";
	cout<<"\tPlayers take turns putting their marks in empty squares.\n";
	cout<<"\t3. The first player to get 4 of his/her marks in a row\n";
	cout<<"\t(up, down, across, or diagonally) is the winner.\n";
	cout<<"\t4. When all 25 squares are full, the game is over.\n";
	cout<<"\tIf no player has 4 marks in a row, the game ends in a tie.\n\n";
	cout<<"\n\n\t\t\tHow to play\n\n";
	cout<<"\tAll visual buttons in all the menus and cells in the game board have their own digits.\n";			
	cout<<"\tTo make your choice in the game or option menus press one digit and press\"Enter\" button.\n";
	cout<<"\tTo make your turn in the game use two digits pressing buttons and press \"Enter\" button.\n";
	cout<<"\tYou can exit current game pressing exit button (\"e\") end \"Enter\".\n\n";						
	system("pause");
}

	//Whose turn will be the first function	
void first_turn(char *now_turn, char crosses, char nulles){
	int random;
	srand(time(NULL));
	random=rand()%10+1;
	if(random%2==0)
		*now_turn = crosses;		
	else
		*now_turn = nulles;
}

	//Changing player who turn next function
void next_turn(char *now_turn, char crosses, char nulles){
	if(*now_turn == crosses)
		*now_turn = nulles;		
	else
		*now_turn = crosses;			
}

	//Showing gameplay screen function
void show_game_screen(char now_turn, char game_screen[][BOARD_WIDTH]){
	cout<<"\t\tTurn "<<now_turn<<"\n\n";
	cout<<"     ";
	for(int i=1;i<=BOARD_WIDTH;i++){
		cout<<i<<"    ";
	}
	cout<<endl;
	for(int i=0;i<BOARD_HEIGHT;i++){
		cout<<"   ";
		for(int j=0;j<BOARD_WIDTH;j++){
			cout<<"\xCD\xCD\xCD\xCD\xCD";
		}
		cout<<"\n "<<i+1;		
		for(int k=0;k<BOARD_WIDTH;k++){
			cout<<" \xBA ";
			cout<<game_screen[i][k]<<" ";	
		}
		cout<<" \xBA\n";
	}
		cout<<"   ";	
	for(int i=1;i<=BOARD_WIDTH;i++){
		cout<<"\xCD\xCD\xCD\xCD\xCD";
	}	
}

	//Turn checking function
void turn(bool *exit_round, char now_turn, int game_board[][BOARD_WIDTH], char game_screen[][BOARD_WIDTH]){
	char turn_char[] = " "; //getting turn as string from keyboard
	int turn; //player's turn
	cout<<"\n\nEnter two digits for turn (row and column)\n";
	cout<<"To leave the round press \"e\"(EXIT)\n";
	for(;;){
		for(int i=0;i<2;i++){
			cin>>turn_char[i];
			if(turn_char[i]=='e'){
				*exit_round=1;				
				break;
			}
		}
		if(*exit_round==1)
			break;
		turn=0;	
		for(int i=0;i<2;i++){
			turn=turn*10+turn_char[i]-48;
		}
		if(turn/10<1 || turn/10>BOARD_HEIGHT || turn%10<1|| turn%10>BOARD_WIDTH) 		//Check if the number is correct
			cout<<"Wrong turn. Try again\n";
		else if(game_screen[turn/10-1][turn%10-1]!=' ')					//Check if the number is still available
			cout<<"The cell is already filled. Try an other one\n";
		else
			break;	
	}	
	game_board[turn/10-1][turn%10-1]=1;
	game_screen[turn/10-1][turn%10-1]=now_turn;								
}

	//For checking backend part (only while codding/testing)
void show_how_it_runs(int game_board_cr[][BOARD_WIDTH], int game_board_nl[][BOARD_WIDTH]){
	cout<<"\n\n";
	for(int i=0;i<BOARD_HEIGHT;i++){
		for(int j=0;j<BOARD_WIDTH;j++){
			cout<<game_board_cr[i][j]<<"   ";
		}	
		cout<<"\n\n";
	}
	cout<<"\n\n";
	for(int i=0;i<BOARD_HEIGHT;i++){
		for(int j=0;j<BOARD_WIDTH;j++){
			cout<<game_board_nl[i][j]<<"   ";
		}	
		cout<<"\n\n";
	}
	
}

	//Check if there is win or draw rezult
bool check_for_end_rezult(int game_board[][BOARD_WIDTH], int win_or_draw){
int row_counter=0, coloumn_counter=0, right_diagonale_counter=0, left_diagonale_counter=0;
	bool rezult=0;
		//Row checking
	for(int i=0;i<BOARD_HEIGHT;i++){
		for(int j=0;j<BOARD_WIDTH;j++){
			if(game_board[i][j]==win_or_draw){
				row_counter++;
				if(row_counter==IN_LINE)
					rezult=1;
			}else
				row_counter=0;
		}
		if(rezult==1)
			break;
		row_counter=0;
	}
		//Coloumn checking
	for(int i=0;i<BOARD_WIDTH;i++){
		for(int j=0;j<BOARD_HEIGHT;j++){
			if(game_board[j][i]==win_or_draw){
				coloumn_counter++;
				if(coloumn_counter==IN_LINE)
					rezult=1;
			}else
				coloumn_counter=0;
		}
		if(rezult==1)
			break;
		coloumn_counter=0;
	}
		//Diagonale checking
	for(int i=0;i<=BOARD_HEIGHT-IN_LINE;i++){
		for(int j=0;j<=BOARD_WIDTH-IN_LINE;j++){
				//Individual coefficients of movement to the next element for right-left (rkoef) and left-right (lkoef) diagonales
			for(int k=0, rkoef=0, lkoef=0;k<IN_LINE;k++){
				if(game_board[i+rkoef][j+rkoef]==win_or_draw){
					right_diagonale_counter++;
					if(right_diagonale_counter==IN_LINE)
						rezult=1;
					rkoef++;
				}else
					right_diagonale_counter=0;
				if(game_board[i+lkoef][j-1+IN_LINE-lkoef]==win_or_draw){
					left_diagonale_counter++;
					if(left_diagonale_counter==IN_LINE)
						rezult=1;
					lkoef++;
				}else
					left_diagonale_counter=0;	
			}
			if(rezult==1)
				break;
			right_diagonale_counter=0;
			left_diagonale_counter=0;
		}
		if(rezult==1)
			break;	
	}   
	
	return rezult;
}

	//Check if smb won the game function
bool check_for_win_combination(int game_board[][BOARD_WIDTH]){
	bool win=0;
	int for_win=1; //variable must be equal to "1" for win searching
	win=check_for_end_rezult(game_board, for_win);
	
	return win;
}

	//Check for only draw result function
bool check_for_draw(int game_board_cr[][BOARD_WIDTH], int game_board_nl[][BOARD_WIDTH]){
	bool cr_draw=1, nl_draw=1;
	int for_draw=0; //variable must be equal to "0" for draw searching
		//Checking null board for a draw for nulles
	cr_draw=!check_for_end_rezult(game_board_nl, for_draw);
		//Checking cross board for a draw for crosses
	nl_draw=!check_for_end_rezult(game_board_cr, for_draw);	
	return cr_draw*nl_draw;
}

	//AI turn processing
void ai_turn(char now_turn, int game_board[][BOARD_WIDTH], int game_board_op[][BOARD_WIDTH], char game_screen[][BOARD_WIDTH]){
	int turn=0; //player's turn
	cout<<"\n\nComputer is thinking\t";
	Sleep(500);
		//searching for winner combination (1st step)
	for(int i=0;i<BOARD_HEIGHT;i++){
		for(int j=0;j<BOARD_WIDTH;j++){
			if(game_screen[i][j]==' '){
				game_board[i][j]=1;
				if(check_for_win_combination(game_board)==1){
					turn=(i+1)*10+(j+1);
					break;
				}else
					game_board[i][j]=0;			
			}
			if(turn!=0)
				break;
		}
		if(turn!=0)
			break;
	}
	if(turn==0){
			//searching for opponent winner combination (2nd step)
		for(int i=0;i<BOARD_HEIGHT;i++){
			for(int j=0;j<BOARD_WIDTH;j++){
				if(game_screen[i][j]==' '){
				game_board_op[i][j]=1;
				if(check_for_win_combination(game_board_op)==1){
					turn=(i+1)*10+(j+1);
					game_board_op[i][j]=0;
					break;
				}else
					game_board_op[i][j]=0;			
				}
			if(turn!=0)
				break;
			}
		if(turn!=0)
			break;
		}	
	}
	if(turn==0 && ai_level>EASY){
			//searching for pottentional winner combinations (3-4th steps)	
		turn=check_between_two(game_board, game_screen);
	}
	if(turn==0 && ai_level>EASY){
		turn=check_next_to_two(game_board, game_screen);
	}			
	if(turn==0 && ai_level>EASY){
			//searching for opponent pottentional winner combination (5-6th steps)
		turn=check_between_two(game_board_op, game_screen);
	}
	if(turn==0 && ai_level>EASY){
		turn=check_next_to_two(game_board_op, game_screen);
	}
	if(turn==0 && ai_level==HARD){
			//Smart turn at the beginning (7th step)			
		if(game_screen[BOARD_HEIGHT/2][BOARD_WIDTH/2]==' ')
			turn=(BOARD_HEIGHT/2+1)*10+(BOARD_WIDTH/2+1);
		else if(game_screen[BOARD_HEIGHT/2+1][BOARD_WIDTH/2-1]==' ')
			turn=(BOARD_HEIGHT/2+2)*10+(BOARD_WIDTH/2);
		else if(game_screen[BOARD_HEIGHT/2+1][BOARD_WIDTH/2+1]==' ')
			turn=(BOARD_HEIGHT/2+2)*10+(BOARD_WIDTH/2+2);
		else if(game_screen[BOARD_HEIGHT/2-1][BOARD_WIDTH/2-1]==' ')
			turn=(BOARD_HEIGHT/2)*10+(BOARD_WIDTH/2);
		else if(game_screen[BOARD_HEIGHT/2-1][BOARD_WIDTH/2+1]==' ')
			turn=(BOARD_HEIGHT/2)*10+(BOARD_WIDTH/2+2);			
	}	
	if(turn==0){
			//Random turn - if no other logical turns (8th step)
		srand(time(NULL));
		for(int i=0;;i++){
			if(i==100000) //used for faster AI turning (giving AI time for loop)
				break;
			turn=rand()%45+11;
			if(turn/10<1 || turn/10>BOARD_HEIGHT || turn%10<1|| turn%10>BOARD_WIDTH);	//Check if the number is correct			
			else if(game_screen[turn/10-1][turn%10-1]!=' ');				//Check if the number is still available
			else
				break;
		}
	}
	if(turn==0){
			//Searching for turn from the begining (used for faster AI turning) (9th step)
		for(int i=0;i<BOARD_HEIGHT;i++){
			for(int j=0;j<BOARD_WIDTH;j++){
				if(game_screen[i][j]==' '){
					turn=(i+1)*10+(j+1);
					break;	
				}
			}
		}
	}
		//getting turn
	game_board[turn/10-1][turn%10-1]=1;
	game_screen[turn/10-1][turn%10-1]=now_turn;								
}

	//searching for pottentional winner combination (puts between two)
int check_between_two(int game_board[][BOARD_WIDTH], char game_screen[][BOARD_WIDTH]){
	int turn=0;
	for(int i=0;i<BOARD_HEIGHT;i++){
			for(int j=0;j<BOARD_WIDTH;j++){
				if(game_screen[i][j]==' '){
					if((game_board[i-1][j-1]==1 && game_board[i+1][j+1]==1 && game_screen[i-2][j-2]==' ' && game_screen[i+2][j+2]==' ') || 
					(game_board[i-1][j]==1 && game_board[i+1][j]==1 && game_screen[i-2][j]==' ' && game_screen[i+2][j]==' ') ||
					(game_board[i-1][j+1]==1 && game_board[i+1][j-1]==1 && game_screen[i-2][j+2]==' ' && game_screen[i+2][j-2]==' ') || 
					(game_board[i][j-1]==1 && game_board[i][j+1]==1 && game_screen[i][j-2]==' ' && game_screen[i][j+2]==' ')){
						turn=(i+1)*10+(j+1);
						break;								
					}
				}
				if(turn!=0)
				break;
			}
			if(turn!=0)
			break;
		}
	return turn;	
}

	//searching for pottentional winner combination (puts next to two)	
int check_next_to_two(int game_board[][BOARD_WIDTH], char game_screen[][BOARD_WIDTH]){
	int turn=0;
	for(int i=0;i<BOARD_HEIGHT;i++){
			for(int j=0;j<BOARD_WIDTH;j++){
				if(game_screen[i][j]==' '){
					if((game_board[i-1][j-1]==1 && game_board[i-2][j-2]==1 && game_screen[i-3][j-3]==' ' && game_screen[i+1][j+1]==' ') || 
					(game_board[i-1][j]==1 && game_board[i-2][j]==1 && game_screen[i-3][j]==' ' && game_screen[i+1][j]==' ') ||
					(game_board[i-1][j+1]==1 && game_board[i-2][j+2]==1 && game_screen[i-3][j+3]==' ' && game_screen[i+1][j-1]==' ') || 
					(game_board[i][j+1]==1 && game_board[i][j+2]==1 && game_screen[i][j+3]==' ' && game_screen[i][j-1]==' ') ||
					(game_board[i+1][j+1]==1 && game_board[i+2][j+2]==1 && game_screen[i+3][j+3]==' ' && game_screen[i-1][j-1]==' ') || 
					(game_board[i+1][j]==1 && game_board[i+2][j]==1 && game_screen[i+3][j]==' ' && game_screen[i-1][j]==' ') ||	
					(game_board[i+1][j-1]==1 && game_board[i+2][j-2]==1 && game_screen[i+3][j-3]==' ' && game_screen[i-1][j+1]==' ') || 
					(game_board[i][j-1]==1 && game_board[i][j-2]==1 && game_screen[i][j-3]==' ' && game_screen[i][j+1]==' ')){
						turn=(i+1)*10+(j+1);
						break;								
					}
				}
				if(turn!=0)
				break;
			}
			if(turn!=0)
			break;
		}
	return turn;	
}
//In this version of game you can change both width and height of the board. In old version the board was always a square.
