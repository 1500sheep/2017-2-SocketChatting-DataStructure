
#include "User.h"
#include "App.h"

const int User::MAXSTRLEN=256;

bool operator >(const Data& d1, const Data& d2) {
	//d1의 nick과 d2의 nick을 비교하는 함수. d1이 크면 1 똑같으면 0 d1이 작으면 -1
	int check = strcmp(d1.nick, d2.nick);
	if (check == 1)return true;
	else return false;
}

bool operator <(const Data& d1, const Data& d2) {
	//d1의 nick과 d2의 nick을 비교하는 함수. d1이 크면 1 똑같으면 0 d1이 작으면 -1
	int check = strcmp(d1.nick, d2.nick);
	if (check == -1)return true;
	else return false;
}

bool operator ==(const Data& d1, const Data& d2) {
	//d1의 nick과 d2의 nick을 비교하는 함수. d1이 크면 1 똑같으면 0 d1이 작으면 -1
	int check = strcmp(d1.nick, d2.nick);
	if (check == 0)return true;
	else return false;
}
bool operator !=(const Data& d1, const Data& d2) {
	//d1의 nick과 d2의 nick을 비교하는 함수. d1이 크면 1 똑같으면 0 d1이 작으면 -1
	int check = strcmp(d1.nick, d2.nick);
	if (check == 0)return false;
	else return true;
}

ostream& operator <<(ostream& os, const Data& d) {
	os << "\t\t회원 이름 : " << d.nick << endl;
	return os;
}
User::User(SOCKET cs, SOCKADDR_IN ca) : client_socket(cs), client_address(ca) {
	
}

User::~User(){
	closeSession();
	ThreadClose();
}

SOCKET User::getSocket() const {
	return this->client_socket;
}

char* User::getIP() const {
	static char *address = inet_ntoa(this->client_address.sin_addr);
	return address;
}

int User::getPort() const {
	return ntohs(this->client_address.sin_port);
}

void User::closeSession(){
	closesocket(this->client_socket);
}

DWORD User::run(void) {
	Data data;

	while (true) {
		try {
			recvData(data);
		
		} catch (ChatException e){
			for (auto uiter = App::allUser.begin(); uiter != App::allUser.end(); uiter++) {
				if (*uiter== this) {
					App::allUser.erase(uiter);
					break;
				}
			}
			stringstream oss;
			oss << "LOG(" << getIP() << ":" << getPort() <<") : " << "유저가 나갔습니다. ";
			printLeaveUser(*this);
			break;
		}
	}

	WaitForSingleObject(App::hMutex, INFINITE);

	ReleaseMutex(App::hMutex);
	delete this;
	return 0;
}


void User::sendData(SOCKET socket, const Data &data)
{
	WaitForSingleObject(App::hMutex, INFINITE);
	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}
	ReleaseMutex(App::hMutex);
}
void User::recvData(Data &data){
	bool found;
	//친구 리스트 넣는 임시 벡터!
	vector<string> friend_List;

	//추천 친구 리스트 넣는 임시 벡터
	vector<string> rec_List;

	//추천 친구 관련 임시 맵
	map<string, int> rec_Map;

	//임시 맵 반복 연산
	map<string, int>::iterator rec_Map_iter;

	//임시 채팅방 반복 맵
	map<string, vector<string>>::iterator chat_iter;

	//임시 채팅방 반복 맵2
	map<string, vector<string>>::iterator chat_iter2;

	//임시 채팅방 반복 맵3
	map<string, vector<string>>::iterator chat_iter_room;

	//친구 string 담는 변수
	string str_friend;

	//친구 string 담는 변수2
	string rec_friend;

	memset(&data, 0, sizeof(Data));

	if (recv(this->client_socket, (char*)&data, sizeof(Data), 0) <= 0){
		throw ChatException(1100);
	}
	switch (data.type) {
	case SIGN_IN:
		cout << "Log(" << getIP() << "," << getPort() << ") 로그인 정보 : " << data.nick << endl;

		//친구 이름 검사!
		App::nickList.RetrieveItem(data.nick,found);
		if (found == true) {
			data.type = LOGIN_SUCCESS;
			data.status = LOGIN_ON;
			current_data = data;
			cout << "Log(" << getIP() << "," << getPort() << ") 로그인 성공 : " << data.nick << endl;
			sendData(this->client_socket, data);
			break;
		}
		else {
			//없으면 회원 가입 없는것!
			data.type = LOGIN_FAIL;
			strcpy(data.data,"Nick이 없습니다 가입부터 하세요");
			sendData(this->client_socket,data);
			break;
		}
	case SIGN_UP:

		//친구 이름 검색!
		App::nickList.RetrieveItem(data.nick, found);
		if (found == true) {
			data.type = SIGN_UP_FAIL;
			strcpy(data.data, "동일한 이름이 존재 합니다.");
			cout << "Log(" << getIP() << "," << getPort() << ")" << "회원 가입 실패" << endl;
			sendData(this->client_socket, data);
			break;
		}
			App::nickList.Add(data.nick);
			App::friendList.AddVertex(data.nick);

			//가입하는 순간 자기 이름으로 된 채팅방도 만들어진다!
			chat_iter = App::chatList.find(data.nick);
			chat_iter_room = App::chat_room.find(data.nick);

			if (chat_iter == App::chatList.end()) {
				App::chatList[data.nick] = vector<string>();
				chat_iter = App::chatList.find(data.nick);
				App::chat_room[data.nick] = vector<string>();
				chat_iter_room = App::chat_room.find(data.nick);
			}

			//회원은 나 자신!
			chat_iter->second.push_back(data.nick);

			data.type = SIGN_UP_SUCCESS;
			//가입 메세지 다시 반환
			strcpy(data.data, "가입 하셨습니다.");

			sendData(this->client_socket,data);
			
			cout << "Log(" << getIP() << "," << getPort() << ")" << "회원 가입 성공" << endl;
			App::nickList.PrintTree(cout); //회원명수 보여 주기
			break;

	case LOGOUT ://로그아웃 할 때!
		current_data.type = LOGIN_OFF;
		break;

	case LIST_FRIEND://친구 목록 보내기!

		 friend_List= App::friendList.PrintOneEdges(data.nick);
		 cout << "Log(" << getIP() << "," << getPort() << ") : 친구 목록 : ";
		 
		for (int i = 0; i < friend_List.size(); i++) {
			cout << friend_List[i] << "\t";
			str_friend.append(friend_List[i]+" ");
		}

		cout << endl;

		strcpy(data.data,str_friend.c_str());
		str_friend = "";//다시 초기화!
		sendData(this->client_socket, data);
		
		break;
	case ADD_FRIEND://친구 추가!
		App::nickList.RetrieveItem(data.data, found);
		if (found == false) {
			data.type = ADD_FRIEND_FAIL;
			strcpy(data.data, "존재하지 않는 친구입니다.");
			sendData(this->client_socket,data);
			break;
		}
		App::friendList.AddEdge(data.nick, data.data, 1);
		App::friendList.AddEdge(data.data, data.nick, 1);
		cout << "Log(" << getIP() <<","<<getPort()<<") : 친구 추가 "<<data.data<<endl;
		
		break;
	case RECOMMANDATION_FRIEND://추천 친구! 친구 List를 빼준다!

		friend_List = App::friendList.PrintOneEdges(data.nick);
		rec_List=App::friendList.RecommandPath(data.nick);
		
		for (vector<string>::iterator iter = friend_List.begin(); iter != friend_List.end(); ++iter) {
			for (vector<string>::iterator iter2 = rec_List.begin(); iter2 != rec_List.end();) {
				if ((*iter2) == (*iter)||(*iter2)==data.nick) {
					iter2 = rec_List.erase(iter2);//해당 인덱스가 사라지고 앞당겨져서 다시 iter에 넣어줘야함
				}
				else {
					++iter2;//그게 아니면 한칸 전진!
				}
			}
		}
		for (vector<string>::iterator iter = rec_List.begin(); iter != rec_List.end(); ++iter) {
			//map을 이용해서 counting해준다!
			rec_Map_iter = rec_Map.find(*iter);

			if (rec_Map_iter != rec_Map.end()) {
				rec_Map_iter->second++;
			}
			else {
				rec_Map.insert(map<string, int>::value_type(*iter, 1));
			}
		}
		cout << "Log(" << getIP() << "," << getPort() << ") : 추천 친구 ";
		for (rec_Map_iter = rec_Map.begin(); rec_Map_iter != rec_Map.end(); ++rec_Map_iter)
		{
			cout <<"\t"<<rec_Map_iter->first << "의 횟수 : " << rec_Map_iter->second;
			rec_friend.append("("+rec_Map_iter->first+") :");
			if (rec_Map_iter->second >=3 ) {
				rec_friend.append("초강력 추천 친구, 관련 횟수 : ");
			}
			else if (rec_Map_iter->second  >=2) {
				rec_friend.append("강력 추천 친구, 관련 횟수 : ");
			}
			else {
				rec_friend.append("일반 추천 친구, 관련 횟수 : ");
			}
			rec_friend.append(to_string(rec_Map_iter->second)+"\n");
		}
		cout << endl;
		strcpy(data.data, rec_friend.c_str());
		rec_friend = "";
		sendData(this->client_socket, data);
		break;
	case LIST_CHAT://채팅 리스트 보여줄 때!
		chat_iter = App::chatList.find(data.nick);
		if (chat_iter == App::chatList.end())
		{
			break;
		}
		cout << "Log(" << getIP() << "," << getPort() << ") : 채팅방 LIST -";
		for (vector<string>::iterator iter = chat_iter->second.begin(); iter != chat_iter->second.end(); ++iter) {
			cout << "\t" << *iter;
			rec_friend.append(*iter+" ");
		}
		cout << endl;
		strcpy(data.data, rec_friend.c_str());
		rec_friend = "";
		sendData(this->client_socket, data);
		break;
	case ADD_CHAT:
		cout << "Log(" << getIP() << "," << getPort() << ") : 채팅방 추가 -";
		cout << data.data << endl;

		//채팅방을 만드는데 거기다가 자기 자신 넣기!
		chat_iter = App::chatList.find(data.data);

		//자기자신의 채팅방에 자기가 갖고 있는 채팅 룸 넣기!
		chat_iter2 = App::chatList.find(data.nick);

		chat_iter_room = App::chat_room.find(data.data);

		if (chat_iter ==App::chatList.end()) {
			App::chatList[data.data] = vector<string>();
			chat_iter = App::chatList.find(data.data);
			chat_iter2 = App::chatList.find(data.nick);

			App::chat_room[data.data] = vector<string>();
			chat_iter_room = App::chat_room.find(data.data);
		}
		else {
			//존재하는 채팅방이기 때문에 오류!
			break;
		}
		rec_friend = data.nick;
		chat_iter->second.push_back(data.nick);
		chat_iter2->second.push_back(data.data);
		chat_iter_room->second.push_back("("+rec_friend+") : 채팅방 만들었습니다");
		

		rec_friend = "";
		break;
	case INVITE_CHAT:
		rec_friend = data.data;
		cout << "Log(" << getIP() << "," << getPort() << ") : 채팅방에 친구 추가 -";
		cout << rec_friend << endl;
		//채팅방 찾기 " "로 쪼갠다!
		chat_iter = App::chatList.find(rec_friend.substr(0, rec_friend.find(" ")));
		
		if (chat_iter == App::chatList.end()) {
			data.type = INVITE_CHAT_FAIL;
			strcpy(data.data, "존재 하지 않는 채팅방입니다, 채팅방을 먼저 만들어 주세요!");
			sendData(this->client_socket,data);
			break;//존재하지 않는 채팅방이기 때문에 break!
		}
		else {
			//채팅방 안에 멤버 넣기!
			chat_iter->second.push_back(rec_friend.substr(rec_friend.find(" ") + 1));
			break;
		}
		rec_friend = "";//초기화
		break;
	case CHECK_CHAT:
		cout << "Log(" << getIP() << "," << getPort() << ") : 채팅방 인원 확인-";
		rec_friend = data.data;
		cout << rec_friend << endl;
		//채팅방 찾기 
		chat_iter = App::chatList.find(rec_friend);

		if (chat_iter == App::chatList.end()) {
			strcpy(data.data, "없음");
			
		}
		else {
			//인원 넣어주기!
			for (vector<string>::iterator iter = chat_iter->second.begin(); iter != chat_iter->second.end(); ++iter) {
				rec_friend.append(" "+*iter);
			}
			
			strcpy(data.data, rec_friend.c_str());
		}
		rec_friend = "";
		sendData(this->client_socket, data);
		
		break;
	case IN_CHAT://채팅방 입장 하려는 유저!
		cout << "Log(" << getIP() << "," << getPort() << ") : 채팅방 입장 - ";
		cout << data.data << endl;
		chat_iter_room = App::chat_room.find(data.data);

		if (chat_iter_room == App::chat_room.end()) {
			break;//없는 방에 입장하려는 것!
		}
		else {
			data.type = FIRST_ON_CHAT;
			room = data.data;
			cout << "방 이름 : " << room << endl;
			rec_friend.append(data.data);
			rec_friend.append("\n");
			for (vector<string>::iterator iter = chat_iter_room->second.begin(); iter != chat_iter_room->second.end(); ++iter) {
				rec_friend.append(*iter + "\n");
			}
			strcpy(data.data, rec_friend.c_str());
		}
			rec_friend = "";
			sendData(this->client_socket, data);
		break;
	case FIRST_ON_CHAT:
		break;
	case ON_CHAT://채팅 중일 때!
		rec_friend = data.data;
		cout << "Log(" << getIP() << "," << getPort() << ") : 채팅 중 -";
		cout << rec_friend << endl;
		//채팅방 가져오기!
		chat_iter_room = App::chat_room.find(room);
		
		if (chat_iter_room == App::chat_room.end()) {
			break;//없는 방에 입장하려는 것!
		}
		else {
			data.type = ON_CHAT;
			chat_iter_room->second.push_back(rec_friend);
			strcpy(data.data, rec_friend.c_str());
		}
		
		sendOneChat(data,rec_friend);
		rec_friend = "";
		break;
	case OUT_CHAT:
		room = "";
		rec_friend = data.data;
		cout << "Log(" << getIP() << "," << getPort() << ") : 채팅 중 -";
		cout << rec_friend << endl;
		break;
	default:
		cout << "잘 못 넘어왔습니다" << endl;
		break;
	}
	

}

void User::sendLoginFail() {
	Data data;
	memset(&data, 0, sizeof(Data));
	data.type = LOGIN_FAIL;
	sendData(this->client_socket, data);
}

void User::sendDataAll(const Data &data) {
	int len = App::allUser.size();
	for (int i=0; i<len; i++){
		User *user = App::allUser.at(i);
		//cout << i + 1 << "의 User다! : " << user->getIP()<< " : " <<user->getPort()<<" : "<<user->getSocket()<<endl;
		try{
			//cout << i + 1 << "의 User한테 Data보낸다 : " << endl;
			sendData(user->getSocket(), data);
		} catch(ChatException e) {}
	}
}

void User::sendChat(Data temp_data,const string str) {
	Data data = temp_data;
	data.type = ON_CHAT;
	
	strcpy(data.data, str.c_str());

	int len = App::allUser.size();
	for (int i = 0; i<len; i++) {
		User *user = App::allUser.at(i);
		cout << "user" << i << "번째 방 이름 : " << user->getRoom() << endl;
		try {
			if (user->getRoom() == this->getRoom()) {
				sendData(user->getSocket(), data);
			}
		}
		catch (ChatException e) {}
	}
}
void User::sendOneChat(Data temp_data, string str) {

	int len = App::allUser.size();
	for (int i = 0; i<len; i++) {
		User *user = App::allUser[i];
		try {
			if (user->getRoom() == this->getRoom()) {
				sendData(user->getSocket(), temp_data);
			}
		}
		catch (ChatException e) {}
	}
}

string User::recvNick() {
	Data data;
	recvData(data);
	
	if (data.type != SIGN_IN) throw ChatException(1200);
	return string(data.nick);
}

string User::recvRoom() {
	Data data;
	recvData(data);

	if (data.type != JOIN_ROOM) throw ChatException(1201);
	return string(data.data);
}



void User::printLeaveUser(const User &user) const {
	cout << "유저가 나갔습니다. (" << user.getIP() << " : " << user.getPort() << ")" << endl;
}

