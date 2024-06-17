#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include <graphics.h>
#include <conio.h>
#define CELL_SIZE 100 
#define BOARD_SIZE 8
using namespace std;

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

//classes
class User{
private:
	int ID;
	string UserName;
	string Password;
	int BestScore;
	int LateScore;
	int Turn;
	int Color;
public:
	User() : ID(0), UserName("0"), Password("0"), BestScore(0), Color(0),LateScore(2) {}
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
		
		while(username=="Bot"){
			cout << "invalid username!" << endl << "enter username: ";
			cin >> username;		
		}
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
	int isTurn(){
		return this->Turn;
	}
	//getter
	int getID(){
		return ID;
	}
	string getUsername(){
		return UserName;
	}
	string getPass(){
		return Password;
	}
	int getBestScore(){
		return BestScore;
	}
	int getLateScore(){
		return LateScore;
	}
	int getColor(){
		return Color;
	}
	//setter
	virtual void setData(){
	}
	void updateScore(int q[9][9],int color){
		int i,j,count=0;
		for(i=0;i<8;i++){
			for(j=0;j<8;j++){
				if(q[i][j]==color){
					count+=1;
				}
			}
		}
		LateScore=count;
	}	
	void updateBestScore(int score){
		if(score>BestScore){
			BestScore=score;
		}
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
	void setBestScore(int score){
		BestScore=score;
	}
	void setColor(int colorNum){
		Color=colorNum;
	}
	void setTurn(){
		Turn=1;
	}
	void resetTurn(){
		Turn=0;
	}
};	

class Admin : public User{
private:
	void setData(){
		setID(1);
		setUserName("Admin");
		setPassword("admin");
	}
public:
	Admin(){
		setData();
	}
};
class Game{
public:
	Game(){
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
	void setBoard(int q[9][9]){
		int i,j;
		initwindow(BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
		drawBoard();
		for(i=0;i<8;i++){
			for(j=0;j<8;j++){
				if(q[i][j]==1){
					placePiece(i,j,WHITE);
				}
				if(q[i][j]==-1){
					placePiece(i,j,BLACK);
				}
			}
		}
		
	}
	
	void flipLine(int x,int y,int color, int q[9][9]){
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
	}
	int checkLine(int x,int y,int color, int q[9][9]){
		int i,countB=0,countA=0,index=-1;
		for(i=x-1;i>=0;i--){
			if(q[i][y]==0){
				break;
			}
			if(color==q[i][y]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=index+1;i<x;i++){
				countB++;
			}		
		}
		index=-1;
		for(i=x+1;i<8;i++){
			if(q[i][y]==0){
				break;
			}
			if(color==q[i][y]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=x+1;i<index;i++){
				countA++;
			}		
		}
		return countA+countB;
	}
	void flipColumn(int x,int y,int color, int q[9][9]){
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
	}
	int checkColumn(int x,int y,int color, int q[9][9]){
		int i,j,countB=0,countA=0,index=-1;
		for(j=y-1;j>=0;j--){
			if(q[x][j]==0){
				break;
			}
			if(color==q[x][j]){
				index=j;
				break;
			}
		}
		if(index!=-1){
			for(j=index+1;j<y;j++){
				countB++;
			}		
		}
		index=-1;
		for(j=y+1;j<8;j++){
			if(q[x][j]==0){
				break;
			}
			if(color==q[x][j]){
				index=j;
				break;
			}
		}
		if(index!=-1){
			for(j=y+1;j<index;j++){
				countA++;
			}		
		}
		return countA+countB;
	}
	void flipDiameter(int x,int y,int color,int q[9][9]){
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
	}
	int checkDiameter(int x,int y,int color,int q[9][9]){
		int i,j,countB=0,countA=0,countBB=0,countAA=0,index=-1;
		for(i=1;i<8;i++){
			if(x+i>=8 || y-i<0 || q[x+i][y-i]==0){
				break;
			}
			if(color==q[x+i][y-i]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=1;i<index;i++){
					countB++;	
			}
		}
		index=-1;

		for(i=1;i<8;i++){
			if(x-i<0 || y+i>8 || q[x-i][y+i]==0){
				break;
			}
			if(color==q[x-i][y+i]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=1;i<index;i++){
					countA++;
			}
		}
		
		index=-1;

		for(i=1;i<8;i++){
			if(x-i<0 || y-i<0 || q[x-i][y-i]==0){
				break;
			}
			if(color==q[x-i][y-i]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=1;i<index;i++){
					countBB++;
			}
		}
		
		index=-1;

		for(i=1;i<8;i++){
			if(x+i>=8 || y+i>=8 || q[x+i][y+i]==0){
				break;
			}
			if(color==q[x+i][y+i]){
				index=i;
				break;
			}
		}
		if(index!=-1){
			for(i=1;i<index;i++){
					countAA++;
	
			}
		}
		return countA+countB+countAA+countBB;
	}
	
//	User checkTurn(User a,User b){
//		if(a.isTurn()){
//			return a;
//		}
//		else if(b.isTurn()){
//			return b;
//		}
//	}
	int isMoveValid(int x,int y,int color, int q[9][9]){
		int count=0;
		count+=checkLine(x,y,color,q);
		count+=checkColumn(x,y,color,q);
		count+=checkDiameter(x,y,color,q);
		return count;
	}
	int passTurn(int q[9][9],int color){
		
		int i,j,count=0,a,b,c;
		for(i=0;i<8;i++){
			for(j=0;j<8;j++){
				if(q[i][j]==0){
					a=checkLine(i,j,color,q);
					b=checkColumn(i,j,color,q);
					c=checkDiameter(i,j,color,q);
					count+=a;
					count+=b;
					count+=c;
					//cout << a << b << c;
				}
			}
		}
//		cout << count;
		if(count==0){
			return 1;
		}
		return 0;
	}
	User getWinner(User user1, User user2){
		User draw;
		if(user1.getLateScore()>user2.getLateScore()){
			return user1;
		}
		else if(user2.getLateScore()>user1.getLateScore()){
			return user2;
		}
		else{
			return draw;
		}
	}
	int isBoardFull(int q[9][9]){
		int r=0,i,j;
		for(i=0;i<8;i++){
			for(j=0;j<8;j++){
				if(q[i][j]==0){
					return 0;
				}
			}
		}
		return 1;
	}
	void updateHistory(int q[9][9],User user1,User user2,int gamestatus){
		int i,j;
		fstream hist;
		hist.open("history.txt", ios::app);
		hist << endl;
		hist << user1.getUsername() << " " << user1.getColor() << " "<< user1.getLateScore()<<endl;
		hist << user2.getUsername() << " " << user2.getColor() << " " << user2.getLateScore()<< endl;
		if(gamestatus==1){
		hist << "status: finished" << endl; 	
		}
		if(gamestatus==0){
			hist << "status: paused" << endl;
		}
		for(j=0;j<8;j++){
			for(i=0;i<8;i++){
				if(q[i][j]==1){
					hist<< "W";
				}
				if(q[i][j]==-1){
					hist<< "B";
				}
				if(q[i][j]==0){
					hist << "0";
				}
			}
			if(j==7){
				break;
			}
			hist << endl;

		}
		hist.close();
	}
	void updateHistory(int q[9][9],User user1,User user2,int gamestatus,string turn,string level){
		int i,j;
		fstream hist;
		hist.open("history.txt", ios::app);
		hist << endl;
		hist << user1.getUsername() << " " << user1.getColor() << " "<< user1.getLateScore()<<endl;
		hist << user2.getUsername() << " " << user2.getColor() << " " << user2.getLateScore()<< endl;
		if(gamestatus==1){
		hist << "status: finished" << endl; 	
		}
		if(gamestatus==0){
			hist << "status: paused " << turn << " " << level << endl;
		}
		for(j=0;j<8;j++){
			for(i=0;i<8;i++){
				if(q[i][j]==1){
					hist<< "W";
				}
				if(q[i][j]==-1){
					hist<< "B";
				}
				if(q[i][j]==0){
					hist << "0";
				}
			}
			if(j==7){
				break;
			}
			hist << endl;
		}
		hist.close();
	}
	int checkPaused(User user1,User user2){
		string first,sec,temp,stat;
		fstream hist;
		hist.open("history.txt", ios::in);
		while(!hist.eof()){
			hist >> first;
			getline(hist,temp);
			hist >> sec;
			getline(hist,temp);
			hist >> temp >> stat;
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);

			if((first==user1.getUsername() || sec==user1.getUsername()) && (first==user2.getUsername() || sec==user2.getUsername()) && stat=="paused"){
				return 1;
			}
		}
		hist.close();
		return 0;	
	}
	void getLastArray(int q[9][9], User temp1,User temp2){
		string first,sec,temp,stat;
		int i=0,j=0;
		char a;
		fstream hist;
		hist.open("history.txt", ios::in);
		while(!hist.eof()){
			hist >> first;
			getline(hist,temp);
			hist >> sec;
			getline(hist,temp);
			hist >> temp >> stat;
			getline(hist,temp);
			if((first==temp1.getUsername() || sec==temp1.getUsername()) && (first==temp2.getUsername() || sec==temp2.getUsername()) && stat=="paused"){
				for(j=0;j<8;j++){
					for(i=0;i<8;i++){
						hist >> a;
						if(a=='B'){
						q[i][j]=-1;	
						}
						else if(a=='W'){
						q[i][j]=1;	
						}
						else{
						q[i][j]=0;	
						}
					}
					getline(hist,temp);
				}
			}
			else{
				getline(hist,temp);	
				getline(hist,temp);	
				getline(hist,temp);	
				getline(hist,temp);	
				getline(hist,temp);	
				getline(hist,temp);	
				getline(hist,temp);	
				getline(hist,temp);		
			}
		}
		hist.close();
	}	
	string whoseTurn(User user1,User user2){
		string first,sec,temp,stat,turn,t;
		fstream hist;
		hist.open("history.txt", ios::in);
		while(!hist.eof()){
			hist >> first;
			getline(hist,temp);
			hist >> sec;
			getline(hist,temp);
			hist >> temp >> stat;
			if(stat=="paused"){
				hist >> t;
			}
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			if((first==user1.getUsername() || sec==user1.getUsername()) && (first==user2.getUsername() || sec==user2.getUsername()) && stat=="paused"){
				turn=t;
			}
		}
		hist.close();
		return turn;
	}
	string checkLevel(User user1,User user2){
		string first,sec,temp,stat,turn,t;
		string l,level;
		fstream hist;
		hist.open("history.txt", ios::in);
		while(!hist.eof()){
			hist >> first;
			getline(hist,temp);
			hist >> sec;
			getline(hist,temp);
			hist >> temp >> stat;
			if(stat=="paused"){
				hist >> t >> l;
			}
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			if((first==user1.getUsername() || sec==user1.getUsername()) && (first==user2.getUsername() || sec==user2.getUsername()) && stat=="paused"){
				level=l;
			}
		}
		hist.close();
		return level;
	}
	int getColor(User user1,User user2,int num){
		string first,sec,temp,stat,turn,t;
		int c1,c2,color;
		fstream hist;
		hist.open("history.txt", ios::in);
		while(!hist.eof()){
			hist >> first >> c1;
			getline(hist,temp);
			hist >> sec >> c2;
			getline(hist,temp);
			hist >> temp >> stat;
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			getline(hist,temp);
			if((first==user1.getUsername() || sec==user1.getUsername()) && (first==user2.getUsername() || sec==user2.getUsername()) && stat=="paused"){
				if(num==1){
					color=c1;
				}
				if(num==2){
					color=c2;
				}
			}	
		}
		hist.close();
		return color;
	}
	void showLastGame(int q[9][9]){
		string first,sec,temp,stat;
		int i=0,j=0,s1,s2,score1,score2;
		char a;
		fstream hist;
		hist.open("history.txt", ios::in);
		while(!hist.eof()){
			hist >> first >> temp >> s1;
			getline(hist,temp);
			hist >> sec >> temp >> s2;
			getline(hist,temp);
			hist >> temp >> stat;
			getline(hist,temp);
			
			if(stat=="finished"){
				score1=s1;
				score2=s2;
				for(j=0;j<8;j++){
					for(i=0;i<8;i++){
						hist >> a;
						if(a=='B'){
						q[i][j]=-1;	
						}
						else if(a=='W'){
						q[i][j]=1;	
						}
						else{
						q[i][j]=0;	
						}
					}
					getline(hist,temp);
				}	
			}
			else{
			getline(hist,temp);	
			getline(hist,temp);	
			getline(hist,temp);	
			getline(hist,temp);	
			getline(hist,temp);	
			getline(hist,temp);	
			getline(hist,temp);	
			getline(hist,temp);			
			}
		}
		hist.close();
		cout << first << " vs " << sec << endl;
		cout << "scores:" << endl << first << ": " << score1 << endl << sec << ": " << score2 << endl;
		if(score1>score2){
			cout << first << " won!\n";
		}
		else if(score2>score1){
			cout << sec << " won!\n";
		}
		
		else if(score1==score2){
			cout << "draw!\n";
		}
		setBoard(q);
	}
};

class Bot : User{
private:
	void setData(){
		setUserName("Bot");		
	}
public:
	Bot(){
		setData();
	}
	int ChooseHardI(int q[9][9],int color){
		int max=0,i,j,a,indexI,indexJ,dif=0;
		Game botG;
		for(j=0;j<8;j++){
			for(i=0;i<8;i++){
				a=0;
				
				if(q[i][j]==0 && botG.isMoveValid(i,j,color,q)){
					a+=botG.checkColumn(i,j,color,q);
					a+=botG.checkDiameter(i,j,color,q);
					a+=botG.checkLine(i,j,color,q);
				}
				if(a>max && (i-j>=dif || j-i>=dif)){
					max=a;
					indexI=i;
					indexJ=j;
					if(i<j)
					dif=j-i;
					else
					dif=i-j;
				}
			}
		}
		return indexI;
	}
	int ChooseHardJ(int q[9][9],int color){
		int max=0,i,j,a,indexI,indexJ,dif=0;
		Game botG;
		for(j=0;j<8;j++){
			for(i=0;i<8;i++){
				a=0;
				if(q[i][j]==0 && botG.isMoveValid(i,j,color,q)){
					a+=botG.checkColumn(i,j,color,q);
					a+=botG.checkDiameter(i,j,color,q);
					a+=botG.checkLine(i,j,color,q);
				}
				if(a>max && (i-j>=dif || j-i>=dif)){
					max=a;
					indexI=i;
					indexJ=j;
					if(i<j)
					dif=j-i;
					else
					dif=i-j;
				}
			}
		}
		return indexJ;
	}
	int ChooseMediumI(int q[9][9],int color){
		int max=0,i,j,a,indexI,indexJ,dif=0;
		Game botG;
		for(j=0;j<8;j++){
			for(i=0;i<8;i++){
				a=0;
				
				if(q[i][j]==0 && botG.isMoveValid(i,j,color,q)){
					a+=botG.checkColumn(i,j,color,q);
					a+=botG.checkDiameter(i,j,color,q);
					a+=botG.checkLine(i,j,color,q);
				}
				if(a>max){
					max=a;
					indexI=i;
					indexJ=j;
				}
			}
		}
		return indexI;
	}
	int ChooseMediumJ(int q[9][9],int color){
		int max=0,i,j,a,indexI,indexJ,dif=0;
		Game botG;
		for(j=0;j<8;j++){
			for(i=0;i<8;i++){
				a=0;
				
				if(q[i][j]==0 && botG.isMoveValid(i,j,color,q)){
					a+=botG.checkColumn(i,j,color,q);
					a+=botG.checkDiameter(i,j,color,q);
					a+=botG.checkLine(i,j,color,q);
				}
				if(a>max){
					max=a;
					indexI=i;
					indexJ=j;
				}
			}
		}
		return indexJ;
	}
	int ChooseEasyI(int q[9][9],int color){
		int max=0,i,j,a,indexI;
		Game botG;
		for(j=0;j<8;j++){
			for(i=0;i<8;i++){
				a=0;
				if(q[i][j]==0 && botG.isMoveValid(i,j,color,q)){
					a+=botG.checkColumn(i,j,color,q);
					a+=botG.checkDiameter(i,j,color,q);
					a+=botG.checkLine(i,j,color,q);
					if(a>0){
					indexI=i;
					return indexI;
					}
				}
			}
		}
		return -1;
	}
	int ChooseEasyJ(int q[9][9],int color){
		int max=0,i,j,a,indexJ;
		Game botG;
		for(j=0;j<8;j++){
			for(i=0;i<8;i++){
				a=0;
				if(q[i][j]==0 && botG.isMoveValid(i,j,color,q)){
					a+=botG.checkColumn(i,j,color,q);
					a+=botG.checkDiameter(i,j,color,q);
					a+=botG.checkLine(i,j,color,q);
					if(a>0){
					indexJ=j;
					return indexJ;
					}
				}
			}
		}
		return -1;
	}
};

//functions
void highlightCell(int x, int y, int color) {
    setcolor(color);
    rectangle(x * CELL_SIZE + 1, y * CELL_SIZE + 1, x * CELL_SIZE + CELL_SIZE - 1, y * CELL_SIZE + CELL_SIZE - 1);
    setcolor(WHITE);
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
		temp.setBestScore(score);
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
		users << endl << temp.getID() << " " << temp.getUsername() << " " << temp.getPass() << " " << temp.getBestScore();
	}
	users.close();
}
void showVec(vector <User> b){
	int i;
	User temp;
	for(i=0;i<b.size();i++){
		temp=b[i];
		cout << temp.getID() << " " << temp.getUsername() << " " << temp.getPass() << " " << temp.getBestScore() << endl;
	}
}
void playGame(Game play,int Q[9][9],User temp1,User temp2,vector<User> list,int stat,string level){
	User w;
	int num,i,j,numb,t,s1=0,s2=0;
	string user;
    int gd = DETECT, gm,paused=0,end=0;
    int x = 0, y = 0;
    int prevX = -1, prevY = -1;
    
	cout << "the game starts... ";		
	if(temp1.isTurn()){
	cout << endl<< temp1.getUsername() << " turn:\n";
	}
	else{
	cout << endl<< temp2.getUsername() << " turn:\n";
	}
	play.setBoard(Q);
	highlightCell(x, y, YELLOW);
			
	User winner;
	while(true){
		if(play.isBoardFull(Q) || (play.passTurn(Q,1) && play.passTurn(Q,-1)) || end){
			cout << "\nGAME OVER!\n";
			temp1.updateScore(Q,-1);
		    temp2.updateScore(Q,1);
			temp1.updateBestScore(temp1.getLateScore());
			temp2.updateBestScore(temp2.getLateScore());
			winner=play.getWinner(temp1,temp2);
			for(i=0;i<list.size();i++){
				User temp=list[i];
				if(temp.getID()==temp1.getID()){
					temp.setBestScore(temp1.getBestScore());
					list[i]=temp;
				}
				if(temp.getID()==temp2.getID()){
					temp.setBestScore(temp2.getBestScore());
					list[i]=temp;
				}
			}
			fillFile(list);
			cout << "the winner is: " << play.getWinner(temp1,temp2).getUsername() << endl;
			cout << "scores:\n" << temp1.getUsername() << ": " << temp1.getLateScore()<< endl;
			cout << temp2.getUsername() << ": " << temp2.getLateScore() << endl;
			play.updateHistory(Q,temp1,temp2,1);	
			cout << "press any key to go back to menu...";
			getch();
			closegraph();
			break;
		}	
		Bot bot;
		Game play;
		while(temp2.isTurn() && temp2.getUsername() == "Bot" && level=="3"){
			int i,j,a;
			sleep(1);
			if(play.passTurn(Q,1)&& play.passTurn(Q,-1)){
				end=1;
				break;
			}
			i=bot.ChooseHardI(Q,1);
			j=bot.ChooseHardJ(Q,1);
			Q[i][j]=1;
			play.placePiece(i,j,WHITE);
			play.flipLine(i,j,1,Q);
			play.flipColumn(i,j,1,Q);
			play.flipDiameter(i,j,1,Q);
            if(play.passTurn(Q,-1)==0){
                temp2.resetTurn();
                temp1.setTurn();
                system("cls");
                cout << endl<< temp1.getUsername() << " turn:\n";
			}
			else{
                system("cls");
                cout << "BLACK has no moves!";
				cout << endl << temp2.getUsername() << " turn:\n";
				continue;
			}
		}
		while(temp2.isTurn() && temp2.getUsername() == "Bot" && level=="2"){
			int i,j,a;
			sleep(1);
			if(play.passTurn(Q,1)&& play.passTurn(Q,-1)){
				end=1;
				break;
			}
			i=bot.ChooseMediumI(Q,1);
			j=bot.ChooseMediumJ(Q,1);
			Q[i][j]=1;
			play.placePiece(i,j,WHITE);
			play.flipLine(i,j,1,Q);
			play.flipColumn(i,j,1,Q);
			play.flipDiameter(i,j,1,Q);
            if(play.passTurn(Q,-1)==0){
                temp2.resetTurn();
                temp1.setTurn();
                system("cls");
                cout << endl<< temp1.getUsername() << " turn:\n";
			}
			else{
                system("cls");
                cout << "BLACK has no moves!";
				cout << endl << temp2.getUsername() << " turn:\n";
				continue;
			}
		}			
		while(temp2.isTurn() && temp2.getUsername() == "Bot" && level=="1"){
			int i,j,a;
			sleep(1);
			if(play.passTurn(Q,1)&& play.passTurn(Q,-1)){
				end=1;
				break;
			}
			i=bot.ChooseEasyI(Q,1);
			j=bot.ChooseEasyJ(Q,1);
			
			Q[i][j]=1;
			play.placePiece(i,j,WHITE);
			play.flipLine(i,j,1,Q);
			play.flipColumn(i,j,1,Q);
			play.flipDiameter(i,j,1,Q);
            if(play.passTurn(Q,-1)==0){
                temp2.resetTurn();
                temp1.setTurn();
                system("cls");
                cout << endl<< temp1.getUsername() << " turn:\n";
			}
			else{
                system("cls");
                cout << "BLACK has no moves!";
				cout << endl << temp2.getUsername() << " turn:\n";
				continue;
			}
		}

		if(kbhit()) {
		        char ch = getch();
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
		                
		        	case 32:{
		        		int w;
		        		if(Q[x][y]==0){
							if(temp1.isTurn() && !play.isMoveValid(x,y,temp1.getColor(),Q)){
								cout<< "move not valid!\n";
							}
		            		if(temp1.isTurn() && play.isMoveValid(x,y,temp1.getColor(),Q)){
			                    play.placePiece(x, y, BLACK);
			                    Q[x][y]=-1;
			                    play.flipLine(x,y,-1,Q);
			                    play.flipColumn(x,y,-1,Q);
			                    play.flipDiameter(x,y,-1,Q);
			                    if(play.passTurn(Q,1)==0){
				                    temp1.resetTurn();
				                    temp2.setTurn();
				                    cout << endl<< temp2.getUsername() << " turn:\n";
				                    continue;
								}
								if(play.passTurn(Q,1)&& play.passTurn(Q,-1)){
									cout << "no moves left!\n";
									end=1;
								break;
								}
								else{
									cout << "WHITE has no moves!\n";
									cout << endl<< temp1.getUsername() << " turn:\n";
								}
							}
							if(temp2.isTurn() && !play.isMoveValid(x,y,temp2.getColor(),Q)){
								cout<< "move not valid!\n";
							}
							if(temp2.isTurn() && play.isMoveValid(x,y,temp2.getColor(),Q) && temp2.getUsername()!="Bot"){
								play.placePiece(x, y, WHITE);
			                    Q[x][y]=1;
			                    play.flipLine(x,y,1,Q);
			                    play.flipColumn(x,y,1,Q);
			                    play.flipDiameter(x,y,1,Q);
			                    if(play.passTurn(Q,-1)==0){
			                   		temp2.resetTurn();
			                    	temp1.setTurn();
			                    	cout << endl<< temp1.getUsername() << " turn:\n";
			                    	continue;
								}
								else{
									cout << "BLACK has no moves!\n";
									cout << endl<< temp2.getUsername() << " turn:\n";
								}
								if(play.passTurn(Q,1)&& play.passTurn(Q,-1)){
									cout << "no moves left!\n";
									end=1;
									break;
								}
							}
						}
						break;
					} 
					
					case 'p':
						cout << "\nGAME PAUSED!\n";
						temp1.updateScore(Q,-1);
					    temp2.updateScore(Q,1);
					    if(temp1.isTurn()){
					    play.updateHistory(Q,temp1,temp2,0,temp1.getUsername(),level);
						}
						else{
					    play.updateHistory(Q,temp1,temp2,0,temp2.getUsername(),level);
						}
					    paused=1;
						break;     
		        }
		        if (prevX != -1) {
		            highlightCell(prevX, prevY, BLACK);
		            play.drawBoard();
		        }
		        highlightCell(x, y, YELLOW);  
		        
		        if(paused==1){
		        	closegraph();
		        	break;
				}
		 	}	
	} 
}

int main(){
	User w;
	int num,i,j,numb,t;
	string user;
	vector <User> list;
	list= fillUsers(list);
    int gd = DETECT, gm,paused=0;
    int x = 0, y = 0;
    int prevX = -1, prevY = -1;
    
    //game
	while(true){
		cout << endl << "Menu:\n" << "1- add player\n" << "2- enter game\n" << "3- review last game\n" << "4- best scores\n" << "5- exit\n";
		cout << "enter option: ";
		cin >> num;
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
		
		//keys
		if(num==1){
			system("cls");
			cout << "you are adding a user!\n";
			list.clear();
			list= fillUsers(list);
			User temp;
			temp.addUser();
			list.push_back(temp);
			continue;
		}
		if(num==2){
			system("cls");
			Admin a;
			list.clear();
			list= fillUsers(list);
			string user;
			string user1=getUsername();
			//admin panel
			if(user1==a.getUsername()){
				if(getPassword()==a.getPass()){
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
			
			// user panel
			else{
				string p;
				for(i=0;i<list.size();i++){
					User temp1;
					temp1=list[i];
					if(user1==temp1.getUsername()){
						p=getPassword();
						while(p!=temp1.getPass()){
						cout << "wrong password! try again...\n" << "enter password: ";
						cin >> p;
						}
						if(p==temp1.getPass()){
							system("cls");
							cout << "welcome user (" << temp1.getUsername() << ")" << endl << "1- single player\n2- two players\nchoose option: ";
							cin >> numb;
								//bot game
							if(numb==1){
								User temp2;
								Game play;
								temp2.setUserName("Bot");
								system("cls");
								string level="0";
								if(play.checkPaused(temp1,temp2)){
									int gametype;
									cout<< "1- continue previous game\n" << "2- start new game\n" << "enter option: ";
									cin >> gametype;
									if(gametype==1){
										int c1,c2;
										system("cls");
										cout << temp1.getUsername() << " vs " << temp2.getUsername() << endl;
										cout << "you are continuing your last game!\n";
										play.getLastArray(Q,temp1,temp2);
										level=play.checkLevel(temp1,temp2);
										if(play.whoseTurn(temp1,temp2)==temp1.getUsername()){
											temp1.setTurn();
											temp2.resetTurn();
										}
										else{
											temp2.setTurn();
											temp1.resetTurn();
										}
										c1=play.getColor(temp1,temp2,1);
										c2=play.getColor(temp1,temp2,2);
										temp1.setColor(c1);
										temp2.setColor(c2);
										cout << "\npress 1 to start the game...\n";
										int start;
										cin >> start;
										if(start==1){
											system("cls");
											playGame(play,Q,temp1, temp2,list,0,level);	
										}
									}
									
									if(gametype==2){
										system("cls");
										for(i=0;i<9;i++){
											for(j=0;j<9;j++){
												Q[i][j]=0;
											}
										}
										Q[3][3]=1;
										Q[4][4]=1;
										Q[3][4]=-1;
										Q[4][3]=-1;
										cout << temp1.getUsername() << " vs " << temp2.getUsername() << endl;
										cout << "1-Easy\n2-Medium\n3-Hard\n" << "choose game level: ";
										cin >> level;
										system("cls");
										cout << "\nYou play as BLACK\n";
										cout << "Bot plays as WHITE\n";
										temp1.setColor(-1);
										temp2.setColor(1);
										temp1.setTurn();
										temp2.resetTurn();
										cout << "\npress 1 to start the game...\n";
										int start;
										cin >> start;
										if(start==1){
											system("cls");
											playGame(play,Q,temp1, temp2,list,1,level);	
										}
									}
								}
								else{
									int start;
									cout << temp1.getUsername() << " vs " << temp2.getUsername() << endl;
									cout << "1-Easy\n2-Medium\n3-Hard\n" << "choose game level: ";
									cin >> level;
									system("cls");
									cout << "\nYou play as BLACK\n";
									cout << "Bot plays as WHITE\n";
									temp1.setColor(-1);
									temp2.setColor(1);
									temp1.setTurn();
									temp2.resetTurn();
									cout << "\npress 1 to start the game...\n";
									cin >> start;
									if(start==1){
										system("cls");
										playGame(play,Q,temp1, temp2,list,1,level);	
									}
								}
							}
							if(numb==2){
								//2 player game
								system("cls");
								cout << "2 player game\n" << "player 2 information:\n";	
								Game play;
								string user2=getUsername();
								for(i=0;i<list.size();i++){
									User temp2=list[i];
									if(user2==temp2.getUsername()){
										p=getPassword();
										while(p!=temp2.getPass()){
										cout << "wrong password! try again...\n" << "enter password: ";
										cin >> p;
										}
										if(temp2.getPass()==p){
											system("cls");
											cout << "second player confirmed!\n" << "player 1: " << temp1.getUsername() << endl << "player 2: " << temp2.getUsername()<< endl;
											cout << "-------------------\n";
											if(play.checkPaused(temp1,temp2)){
												int gametype;
												cout<< "1- continue previous game\n" << "2- start new game\n" << "enter option: ";
												cin >> gametype;
												if(gametype==1){
													int c1,c2;
													system("cls");
													cout << temp1.getUsername() << " vs " << temp2.getUsername() << endl;
													cout << "you are continuing your last game!\n";
													play.getLastArray(Q,temp1,temp2);
													if(play.whoseTurn(temp1,temp2)==temp1.getUsername()){
														temp1.setTurn();
														temp2.resetTurn();
													}
													else{
														temp2.setTurn();
														temp1.resetTurn();
													}
													c1=play.getColor(temp1,temp2,1);
													c2=play.getColor(temp1,temp2,2);
													temp1.setColor(c1);
													temp2.setColor(c2);
													cout << "\npress 1 to start the game...\n";
													int start;
													cin >> start;
													if(start==1){
														system("cls");
														playGame(play,Q,temp1,temp2,list,0,"0");	
													}		
												}
												
												if(gametype==2){
													system("cls");
													for(i=0;i<9;i++){
														for(j=0;j<9;j++){
															Q[i][j]=0;
														}
													}
													Q[3][3]=1;
													Q[4][4]=1;
													Q[3][4]=-1;
													Q[4][3]=-1;
													cout << temp1.getUsername() << " vs " << temp2.getUsername() << endl;
													cout << "New game started!\n";
													cout << "player 1 plays as BLACK\n";
													cout << "player 2 plays as WHITE\n";
													temp1.setColor(-1);
													temp2.setColor(1);
													temp1.setTurn();
													temp2.resetTurn();
													cout << "\npress 1 to start the game...\n";
													int start;
													cin >> start;
													if(start==1){
														system("cls");
														playGame(play,Q,temp1, temp2,list,1,"0");	
													}
												}
											}
											else{
												system("cls");
												cout << "\nplayer 1 plays as BLACK\n";
												cout << "player 2 plays as WHITE\n";
												temp1.setColor(-1);
												temp2.setColor(1);
												temp1.setTurn();
												temp2.resetTurn();
												cout << "\npress 1 to start the game...\n";
												int start;
												cin >> start;
												if(start==1){
													system("cls");
													playGame(play,Q,temp1, temp2,list,1,"0");	
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
		if(num==3){
			system("cls");
			cout << "review last game:\n";
			Game play;
			play.showLastGame(Q);
			cout << "press any key to go back to menu...";
			getch();
			closegraph();
		}
		if(num==4){
			system("cls");
			int i,j;
			User temp1,temp2,temp;
			vector <User> t;
			string name;
			list.clear();
			list= fillUsers(list);
			t=list;
			for(i=0;i<t.size()-1;i++){
				for(j=i+1;j<t.size();j++){
					temp1=t[i];
					temp2=t[j];
					if(temp2.getBestScore()>temp1.getBestScore()){
						temp=temp1;
						temp1=temp2;
						temp2=temp;
					}
					t[i]=temp1;
					t[j]=temp2;
				}
			}
			cout << "Best scores:\n"; 
			for(i=0;i<t.size();i++){
				temp=t[i];
				cout << temp.getUsername() << " " << temp.getBestScore() << endl;
			}
			cout << "---------------------";

		}
		if(num==5){
			break;
		}
	}
}