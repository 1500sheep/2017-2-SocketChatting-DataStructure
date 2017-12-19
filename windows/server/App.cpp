
#include "App.h"


map<string, vector<User*> > App::room_map;
vector<User*> App::allUser;
BinarySearchTree<string> App::nickList;
Graph<string> App::friendList;
map<string, vector< string>> App::chatList;
map<string, vector<string>> App::chat_room;

const int App::MAXUSER = 10;
HANDLE App::hMutex = CreateMutex(NULL, FALSE, NULL);

App::App() : chattingserver(nullptr, 3490) {
	//WriteDataFromFile();//µ•¿Ã≈Õ Ω·æﬂ«“ ∂ß∏È ø‰∞…∑Œ «œººø‰~
	ReadDataFromFile();
}

void App::printNewUser(const User *user) const {
	cout << "ªı ¿Ø¿˙∞° ¡¢º”«ﬂΩ¿¥œ¥Ÿ. (" << user->getIP() << ", " << user->getPort() << ")" << endl;
}

void App::printExceedUser(const User *user) const {
	cout << "ªı ¿Ø¿˙∞° ¡¢º”ø° Ω«∆–«ﬂΩ¿¥œ¥Ÿ. (" << user->getIP() << ", " << user->getPort() << ")" << endl;
}



void App::ReadDataFromFile()
{
	int index = 0;
	
	string file_nick;
	
	char filename[20];
	strcpy(filename, "sample.txt");
	m_InFile.open(filename);
	if (!m_InFile) {
		cout << "∆ƒ¿œ ø¿∑˘ : ∆ƒ¿œ¿ª ¿–¿ª ºˆ æ¯Ω¿¥œ¥Ÿ." << endl;
		return;
	}

	while (!m_InFile.eof()) {
		//√÷√ ø° ∆ƒ¿œ ¿–¿ª ∂ß »∏ø¯∞°¿‘, ƒ£±∏List, ¿⁄±‚¿⁄Ω≈ √§∆√πÊ¿Ã µøΩ√ø° ∏∏µÈæÓ¡¯¥Ÿ!
		m_InFile >> file_nick;
		App::nickList.Add(file_nick);
		App::friendList.AddVertex(file_nick);
		auto iter = App::chatList.find(file_nick);
		auto iter2 = App::chat_room.find(file_nick);
		if (iter == App::chatList.end()) {
			App::chatList[file_nick] = vector<string>();
			App::chat_room[file_nick] = vector<string>();
			iter = App::chatList.find(file_nick);
			iter2 = App::chat_room.find(file_nick);
		}
		//»∏ø¯¿∫ ≥™ ¿⁄Ω≈!
		iter->second.push_back(file_nick);

	}
	m_InFile.close();

}
//µ•¿Ã≈Õ ∆ƒ¿œ∑Œ ≥—±‚±‚~!
void App::WriteDataFromFile() {
	string out_nick;
	char filename[20];
	strcpy(filename, "sample.txt");
	m_Outfile.open(filename);
	if (!m_Outfile) {
		cout << "∆ƒ¿œ ª˝º∫ Ω«∆–" << endl;
		return;
	}
	out_nick = "∞≠ºÆ¿±";
	for(int i=0;i<10;i++){
	m_Outfile << out_nick <<i<< endl;
	}
	m_Outfile.close();
}
void App::start() {
	chattingserver.binding();
	chattingserver.listening(10);


	cout << "============< Øó Øó û± ∑å º≠πˆ «¡∑Œ±◊∑• >============" << endl;

	while (true){		
		User *user = chattingserver.acceptUser();
		if (App::allUser.size() >= App::MAXUSER){
			printExceedUser(user);
			continue;
		}

		try {
	
			WaitForSingleObject(App::hMutex, INFINITE);
			App::allUser.push_back(user);
			ReleaseMutex(App::hMutex);
		}
		catch (ChatException e) {
			delete user;
			continue;
		}
		
		printNewUser(user);

		stringstream oss;

		oss << "(" << user->getIP() << ":" << user->getPort() <<") : " << "ªı ≈¨∂Û¿Ãæ∆Æ «¡∑ŒººΩ∫∞° ¡¢º”«ﬂΩ¿¥œ¥Ÿ"; // ≈¨∂Û¿Ãæ∆Æø°º≠ ∫∏¿Ã¥¬ πÆ¿Â
		
		user->start();
	}
}