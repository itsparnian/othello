#include<iostream>
#include<vector>
#include<fstream>
#include<string>

using namespace std;

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
	string getUsername(){
		return this->UserName;
	}
	string getPass(){
		return this->Password;
	}
};


//functions
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
		cout << temp.getID() << temp.getUsername() << temp.getPass() << temp.getScore() << endl;
	}
}

int main(){
	User w;
	int num,i,j,numb;
	string user;
	vector <User> list;
	list= fillUsers(list);
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
			if(getUsername()==a.getUsername()){
				if(getPassword()==a.getPass()){
					//admin panel
					cout << "Welcome Admin!" << endl << "choose option: " << endl << "1- delete user" << endl;
					cin >> numb;
					if(numb==1){
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
						break;
					}
				}
			}
			else{
				
			}
		}
	
	}
}
