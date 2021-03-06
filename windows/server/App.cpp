
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
	//WriteDataFromFile();//데이터 써야할 때면 요걸로 하세요~
	ReadDataFromFile();
}

void App::printNewUser(const User *user) const {
	cout << "새 유저가 접속했습니다. (" << user->getIP() << ", " << user->getPort() << ")" << endl;
}

void App::printExceedUser(const User *user) const {
	cout << "새 유저가 접속에 실패했습니다. (" << user->getIP() << ", " << user->getPort() << ")" << endl;
}



void App::ReadDataFromFile()
{
	int index = 0;
	
	string file_nick;
	
	char filename[20];
	strcpy(filename, "sample.txt");
	m_InFile.open(filename);
	if (!m_InFile) {
		cout << "파일 오류 : 파일을 읽을 수 없습니다." << endl;
		return;
	}

	while (!m_InFile.eof()) {
		//최초에 파일 읽을 때 회원가입, 친구List, 자기자신 채팅방이 동시에 만들어진다!
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
		//회원은 나 자신!
		iter->second.push_back(file_nick);

	}
	m_InFile.close();

}
//데이터 파일로 넘기기~!
void App::WriteDataFromFile() {
	string out_nick;
	char filename[20];
	strcpy(filename, "sample.txt");
	m_Outfile.open(filename);
	if (!m_Outfile) {
		cout << "파일 생성 실패" << endl;
		return;
	}
	out_nick = "강석윤";
	for(int i=0;i<10;i++){
	m_Outfile << out_nick <<i<< endl;
	}
	m_Outfile.close();
}
void App::start() {
	chattingserver.binding();
	chattingserver.listening(10);


	cout << "============< 캌 캌 옼 톸 서버 프로그램 >============" << endl;

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

		oss << "(" << user->getIP() << ":" << user->getPort() <<") : " << "새 클라이언트 프로세스가 접속했습니다"; // 클라이언트에서 보이는 문장
		
		user->start();
	}
}