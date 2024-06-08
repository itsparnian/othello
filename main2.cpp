#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include <graphics.h>
#include <conio.h>
#define CELL_SIZE 100 
#define BOARD_SIZE 8
using namespace std;

//classes
class User{
protected:
	int ID;
	string UserName;
	string Password;
	int BestScore;
public:
	User() : ID(0), UserName("0"), Password("0"), BestScore(0){}
	void getInfo(){
		int id,t;
		string temp, username, password,user,bestscore;
		//get ID
		fstream users;
		users.open("users.txt", ios::in);
		if(!users){
			cerr << "users file not responding!";
		}
		while(!users.eof()){
		users >> id;
		getline(users,temp);
		}
		ID=id+1;
		users.close();
		
		//get username
		cout << "please enter your username: ";
		cin >> username;
		
		while(!isUserValid(username)){
			cout << "username already exits!" << endl << "enter username: ";
			cin >> username;
		}	
		UserName=username;
		// get password
		cout << "please enter your password: ";
		cin >> password;
		Password=password;
	}
	void putInFile(){
		fstream users;
		users.open("users.txt", ios::app);
		if(!users){
			cerr << "users file not responding!\n";
		}
		users << endl << ID << " " << UserName << " " << Password << " " << BestScore;
		users.close();
	}
	void addUser(){
		getInfo();
		putInFile();
	}
	void displayInfo(){
		cout << "ID: " << ID << endl << "Username: " << UserName << endl << "Password: " << Password << endl << "Best score: " <<BestScore << endl;
	}
	int isUserValid(string user){
		int id;
		string temp,exist;
		fstream users;
		users.open("users.txt", ios::in);
		while(!users.eof()){
			users >> id >> exist;
			if(user==exist){
				users.close();
				return 0;
			}
			getline(users,temp);
		}
		users.close();
		return 1;
	}
	int isAdmin(){
		if(ID==1){
			return 1;
		}
		return 0;
	}
	int getID(){
		return ID;
	}
	string getUsername(){
		return UserName;
	}
	string getPass(){
		return Password;
	}
	int getScore(){
		return BestScore;
	}
	
	void setID(int id){
		ID=id;
	}
	void setUserName(string username){
		UserName=username;
	}
	void setPassword(string password){
		Password=password;
	}
	void setScore(int score){
		BestScore=score;
	}
};	
class Admin : public User{
private:
	void setData(){
		this->ID=1;
		this->UserName="Admin";
		this->Password="admin";	
	}
public:
	Admin(){
		setData();
	}
};
class Game{
public:
	Game(){
	initwindow(BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
	drawBoard();
	setBoard();
	}
	
	void placePiece(int x, int y, int color) {
	    int centerX = x * CELL_SIZE + CELL_SIZE / 2;
	    int centerY = y * CELL_SIZE + CELL_SIZE / 2;
	    setfillstyle(SOLID_FILL, color);
	    fillellipse(centerX, centerY, CELL_SIZE / 2 - 10, CELL_SIZE / 2 - 10);
	}
	void drawBoard() {
		int i, j;
		for (i = 0; i < BOARD_SIZE; i++) {
		    for (j = 0; j < BOARD_SIZE; j++) {
		        rectangle(i * CELL_SIZE, j * CELL_SIZE, (i + 1) * CELL_SIZE, (j + 1) * CELL_SIZE);
	        }
	    }
	}
	void setBoard(){
		int i,j;
		placePiece(3,3,WHITE);
		placePiece(4,4,WHITE);
		placePiece(4,3,BLACK);
		placePiece(3,4,BLACK);
	}
	int flipLine(int x,int y,int color, int q[9][9]){
		int i,countB=0,countA=0,index=-1;
		for(i=x-1;i>=0;i--){
			if(q[i][y]==0){
				break;
			}
			if(q[x][y]==q[i][y]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=index+1;i<x;i++){
				countB++;
				if(color==1){
					q[i][y]=1;
					placePiece(i,y,WHITE);
				}
				if(color==-1){
					q[i][y]=-1;
					placePiece(i,y,BLACK);
				}
			}		
		}
		index=-1;
		for(i=x+1;i<8;i++){
			if(q[i][y]==0){
				break;
			}
			if(q[x][y]==q[i][y]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=x+1;i<index;i++){
				countA++;
				if(color==1){
					q[i][y]=1;
					placePiece(i,y,WHITE);
				}
				if(color==-1){
					q[i][y]=-1;
					placePiece(i,y,BLACK);
				}
			}		
		}
		return countA+countB;
	}
	int flipColumn(int x,int y,int color, int q[9][9]){
		int i,j,countB=0,countA=0,index=-1;
		for(j=y-1;j>=0;j--){
			if(q[x][j]==0){
				break;
			}
			if(q[x][y]==q[x][j]){
				index=j;
				break;
			}
		}
		if(index!=-1){
			for(j=index+1;j<y;j++){
				countB++;
				if(color==1){
					q[x][j]=1;
					placePiece(x,j,WHITE);
				}
				if(color==-1){
					q[x][j]=-1;
					placePiece(x,j,BLACK);
				}
			}		
		}
		index=-1;
		for(j=y+1;j<8;j++){
			if(q[x][j]==0){
				break;
			}
			if(q[x][y]==q[x][j]){
				index=j;
				break;
			}
		}
		if(index!=-1){
			for(j=y+1;j<index;j++){
				countA++;
				if(color==1){
					q[x][j]=1;
					placePiece(x,j,WHITE);
				}
				if(color==-1){
					q[x][j]=-1;
					placePiece(x,j,BLACK);
				}
			}		
		}
		return countA+countB;
	}
	int flipDiameter(int x,int y,int color,int q[9][9]){
		int i,j,countB=0,countA=0,countBB=0,countAA=0,index=-1;
		for(i=1;i<8;i++){
			if(x+i>=8 || y-i<0 || q[x+i][y-i]==0){
				break;
			}
			if(q[x][y]==q[x+i][y-i]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=1;i<index;i++){
					countB++;
					if(color==1){
						q[x+i][y-i]=1;
						placePiece(x+i,y-i,WHITE);
					}
					if(color==-1){
						q[i+x][y-i]=-1;
						placePiece(x+i,y-i,BLACK);
					}	
			}
		}
		index=-1;

		for(i=1;i<8;i++){
			if(x-i<0 || y+i>8 || q[x-i][y+i]==0){
				break;
			}
			if(q[x][y]==q[x-i][y+i]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=1;i<index;i++){
					countA++;
					if(color==1){
						q[x-i][y+i]=1;
						placePiece(x-i,y+i,WHITE);
					}
					if(color==-1){
						q[x-i][y+i]=-1;
						placePiece(x-i,y+i,BLACK);
					}	
			}
		}
		
		index=-1;

		for(i=1;i<8;i++){
			if(x-i<0 || y-i<0 || q[x-i][y-i]==0){
				break;
			}
			if(q[x][y]==q[x-i][y-i]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=1;i<index;i++){
					countBB++;
					if(color==1){
						q[x-i][y-i]=1;
						placePiece(x-i,y-i,WHITE);
					}
					if(color==-1){
						q[x-i][y-i]=-1;
						placePiece(x-i,y-i,BLACK);
					}	
			}
		}
		
		index=-1;

		for(i=1;i<8;i++){
			if(x+i>=8 || y+i>=8 || q[x+i][y+i]==0){
				break;
			}
			if(q[x][y]==q[x+i][y+i]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=1;i<index;i++){
					countAA++;
					if(color==1){
						q[x+i][y+i]=1;
						placePiece(x+i,y+i,WHITE);
					}
					if(color==-1){
						q[x+i][y+i]=-1;
						placePiece(x+i,y+i,BLACK);
					}	
			}
		}
		return countA+countB;
	}
	

};
	void highlightCell(int x, int y, int color) {
	    setcolor(color);
	    rectangle(x * CELL_SIZE + 1, y * CELL_SIZE + 1, x * CELL_SIZE + CELL_SIZE - 1, y * CELL_SIZE + CELL_SIZE - 1);
	    setcolor(WHITE);
	}

string getUsername(){
	string username;
	cout << "enter username: ";
	cin >> username;
	return username;
}
string getPassword(){
	string password;
	cout << "enter password: ";
	cin >> password;
	return password;
}
vector<User> fillUsers(vector<User> a){
	string line,user,pass;
	int id,score;
	User temp;
	fstream users;
	users.open("users.txt", ios::in);
	getline(users,line);
	while(!users.eof()){
		users >> id >> user >> pass >> score;
		temp.setID(id);
		temp.setUserName(user);
		temp.setPassword(pass);
		temp.setScore(score);
		a.push_back(temp);
	}
	users.close();
	return a;
}
void fillFile(vector<User> a){
	int i;
	User temp;
	Admin A;
	fstream users;
	users.open("users.txt", ios::out);
	users << A.getID() << " " << A.getUsername() << " " << A.getPass();
	for(i=0;i<a.size();i++){
		temp=a[i];
		users << endl << temp.getID() << " " << temp.getUsername() << " " << temp.getPass() << " " << temp.getScore();
	}
	users.close();
}
void showVec(vector <User> b){
	int i;
	User temp;
	for(i=0;i<b.size();i++){
		temp=b[i];
		cout << temp.getID() << " " << temp.getUsername() << " " << temp.getPass() << " " << temp.getScore() << endl;
	}
}
void showGame(int q[9][9]){
	int i,j;
	for(j=0;j<8;j++){
		for(i=0;i<8;i++){
			cout << q[i][j];
		}
		cout << endl;
	}
	cout << "********" << endl;
}

int main(){
	User w;
	int num,i,j,numb,t;
	string user;
	vector <User> list;
	list= fillUsers(list);
	int Q[9][9];
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			Q[i][j]=0;
		}
	}
	Q[3][3]=1;
	Q[4][4]=1;
	Q[3][4]=-1;
	Q[4][3]=-1;
    int gd = DETECT, gm;
    int x = 0, y = 0;
    int prevX = -1, prevY = -1;
    
	while(true){
	cout << "welcome to the othello game!\n" << "Menu:\n" << "1- add player\n" << "2- enter game\n" << "3- review last game\n" << "4- best scores\n" << "5- exit\n";
	cout << "enter option: ";
	cin >> num;
		
		if(num==1){
			list.clear();
			list= fillUsers(list);
			showVec(list);
			User temp;
			temp.addUser();
			list.push_back(temp);
			continue;
		}
		if(num==2){
			Admin a;
			list.clear();
			list= fillUsers(list);
			string user;
			string user1=getUsername();
			if(user1==a.getUsername()){
				if(getPassword()==a.getPass()){
					//admin panel
					cout << "Welcome Admin!" << endl << "choose option: " << endl << "1- delete user" << endl;
					cin >> numb;
					if(numb==1){
						//delete user
						cout << "you are deleting a user!";
						user=getUsername();
						int isValid=0;
						for(i=0;i<list.size();i++){
							User temp;
							temp=list[i];
							if(temp.getUsername()==user){
								list.erase(list.begin()+ i);
								showVec(list);
								fillFile(list);
								cout << "user deleted successfully!";
								isValid=1;
							}
						}
						if(!isValid){
							cout << "username does not exist!";
						}
						continue;
					}
				}
			}
			
			else{
				for(i=0;i<list.size();i++){
					User temp1=list[i];
					if(user1==temp1.getUsername()){
						if(getPassword()==temp1.getPass()){
							// user panel
							cout << "welcome user (" << temp1.getUsername() << ")" << endl << "1- single player\n2- two players\nchoose option: ";
							cin >> numb;
							if(numb==1){
								//bot game
							}
							if(numb==2){
								//2 player game	
								string user2=getUsername();
								for(i=0;i<list.size();i++){
									User temp2=list[i];
									if(user2==temp2.getUsername()){
										if(temp2.getPass()==getPassword()){
											cout << "second player confirmed!\n" << "player 1: " << temp1.getUsername() << endl << "player 2: " << temp2.getUsername();
											cout << "\npress 1 to start the game...\n";
											int start;
											cin >> start;
											fflush(stdin);
											if(start==1){
											cout << "the game starts... ";	
											Game play;
											highlightCell(x, y, YELLOW);	
																	
										    while (true) {
										        if (kbhit()) {
										            char ch = getch();
										            if (ch == 0)
										                ch = getch();
										            switch (ch) {
										                case 77:
										                    if (x < BOARD_SIZE - 1) {
										                        prevX = x;
										                        prevY = y;
										                        x++;
										                    }
										                    break;
										                case 75:
										                    if (x > 0) {
										                        prevX = x;
										                        prevY = y;
										                        x--;
										                    }
										                    break;
										                case 72:
										                    if (y > 0) {
										                        prevX = x;
										                        prevY = y;
										                        y--;
										                    }
										                    break;
										                case 80:
										                    if (y < BOARD_SIZE - 1) {
										                        prevX = x;
										                        prevY = y;
										                        y++;
										                    }
										                    break;
										                case 'w':
										                case 'W':
										                    play.placePiece(x, y, WHITE);
										                    Q[x][y]=1;
										                    t+=play.flipLine(x,y,1,Q);
										                    t+=play.flipColumn(x,y,1,Q);
										                    t+=play.flipDiameter(x,y,1,Q);
										                    break;
										                    
										                case 'b':
										                case 'B':
										                    play.placePiece(x, y, BLACK);
										                    Q[x][y]=-1;
										                    t+=play.flipLine(x,y,-1,Q);
										                    t+=play.flipColumn(x,y,-1,Q);
										                    t+=play.flipDiameter(x,y,-1,Q);
										                    break;
										                    
										                case 27:
										                    closegraph();
										                    return 0;
										            }
										            if (prevX != -1) {
										                highlightCell(prevX, prevY, BLACK);
										                play.drawBoard();
										            }
										            highlightCell(x, y, YELLOW);  
										        }
										    }
										            getch();
   													 closegraph();
												
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	
	}
}
