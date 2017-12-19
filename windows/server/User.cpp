
#include "User.h"
#include "App.h"

const int User::MAXSTRLEN=256;

bool operator >(const Data& d1, const Data& d2) {
	//d1�� nick�� d2�� nick�� ���ϴ� �Լ�. d1�� ũ�� 1 �Ȱ����� 0 d1�� ������ -1
	int check = strcmp(d1.nick, d2.nick);
	if (check == 1)return true;
	else return false;
}

bool operator <(const Data& d1, const Data& d2) {
	//d1�� nick�� d2�� nick�� ���ϴ� �Լ�. d1�� ũ�� 1 �Ȱ����� 0 d1�� ������ -1
	int check = strcmp(d1.nick, d2.nick);
	if (check == -1)return true;
	else return false;
}

bool operator ==(const Data& d1, const Data& d2) {
	//d1�� nick�� d2�� nick�� ���ϴ� �Լ�. d1�� ũ�� 1 �Ȱ����� 0 d1�� ������ -1
	int check = strcmp(d1.nick, d2.nick);
	if (check == 0)return true;
	else return false;
}
bool operator !=(const Data& d1, const Data& d2) {
	//d1�� nick�� d2�� nick�� ���ϴ� �Լ�. d1�� ũ�� 1 �Ȱ����� 0 d1�� ������ -1
	int check = strcmp(d1.nick, d2.nick);
	if (check == 0)return false;
	else return true;
}

ostream& operator <<(ostream& os, const Data& d) {
	os << "\t\tȸ�� �̸� : " << d.nick << endl;
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
			oss << "LOG(" << getIP() << ":" << getPort() <<") : " << "������ �������ϴ�. ";
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
	//ģ�� ����Ʈ �ִ� �ӽ� ����!
	vector<string> friend_List;

	//��õ ģ�� ����Ʈ �ִ� �ӽ� ����
	vector<string> rec_List;

	//��õ ģ�� ���� �ӽ� ��
	map<string, int> rec_Map;

	//�ӽ� �� �ݺ� ����
	map<string, int>::iterator rec_Map_iter;

	//�ӽ� ä�ù� �ݺ� ��
	map<string, vector<string>>::iterator chat_iter;

	//�ӽ� ä�ù� �ݺ� ��2
	map<string, vector<string>>::iterator chat_iter2;

	//�ӽ� ä�ù� �ݺ� ��3
	map<string, vector<string>>::iterator chat_iter_room;

	//ģ�� string ��� ����
	string str_friend;

	//ģ�� string ��� ����2
	string rec_friend;

	memset(&data, 0, sizeof(Data));

	if (recv(this->client_socket, (char*)&data, sizeof(Data), 0) <= 0){
		throw ChatException(1100);
	}
	switch (data.type) {
	case SIGN_IN:
		cout << "Log(" << getIP() << "," << getPort() << ") �α��� ���� : " << data.nick << endl;

		//ģ�� �̸� �˻�!
		App::nickList.RetrieveItem(data.nick,found);
		if (found == true) {
			data.type = LOGIN_SUCCESS;
			data.status = LOGIN_ON;
			current_data = data;
			cout << "Log(" << getIP() << "," << getPort() << ") �α��� ���� : " << data.nick << endl;
			sendData(this->client_socket, data);
			break;
		}
		else {
			//������ ȸ�� ���� ���°�!
			data.type = LOGIN_FAIL;
			strcpy(data.data,"Nick�� �����ϴ� ���Ժ��� �ϼ���");
			sendData(this->client_socket,data);
			break;
		}
	case SIGN_UP:

		//ģ�� �̸� �˻�!
		App::nickList.RetrieveItem(data.nick, found);
		if (found == true) {
			data.type = SIGN_UP_FAIL;
			strcpy(data.data, "������ �̸��� ���� �մϴ�.");
			cout << "Log(" << getIP() << "," << getPort() << ")" << "ȸ�� ���� ����" << endl;
			sendData(this->client_socket, data);
			break;
		}
			App::nickList.Add(data.nick);
			App::friendList.AddVertex(data.nick);

			//�����ϴ� ���� �ڱ� �̸����� �� ä�ù浵 ���������!
			chat_iter = App::chatList.find(data.nick);
			chat_iter_room = App::chat_room.find(data.nick);

			if (chat_iter == App::chatList.end()) {
				App::chatList[data.nick] = vector<string>();
				chat_iter = App::chatList.find(data.nick);
				App::chat_room[data.nick] = vector<string>();
				chat_iter_room = App::chat_room.find(data.nick);
			}

			//ȸ���� �� �ڽ�!
			chat_iter->second.push_back(data.nick);

			data.type = SIGN_UP_SUCCESS;
			//���� �޼��� �ٽ� ��ȯ
			strcpy(data.data, "���� �ϼ̽��ϴ�.");

			sendData(this->client_socket,data);
			
			cout << "Log(" << getIP() << "," << getPort() << ")" << "ȸ�� ���� ����" << endl;
			App::nickList.PrintTree(cout); //ȸ����� ���� �ֱ�
			break;

	case LOGOUT ://�α׾ƿ� �� ��!
		current_data.type = LOGIN_OFF;
		break;

	case LIST_FRIEND://ģ�� ��� ������!

		 friend_List= App::friendList.PrintOneEdges(data.nick);
		 cout << "Log(" << getIP() << "," << getPort() << ") : ģ�� ��� : ";
		 
		for (int i = 0; i < friend_List.size(); i++) {
			cout << friend_List[i] << "\t";
			str_friend.append(friend_List[i]+" ");
		}

		cout << endl;

		strcpy(data.data,str_friend.c_str());
		str_friend = "";//�ٽ� �ʱ�ȭ!
		sendData(this->client_socket, data);
		
		break;
	case ADD_FRIEND://ģ�� �߰�!
		App::nickList.RetrieveItem(data.data, found);
		if (found == false) {
			data.type = ADD_FRIEND_FAIL;
			strcpy(data.data, "�������� �ʴ� ģ���Դϴ�.");
			sendData(this->client_socket,data);
			break;
		}
		App::friendList.AddEdge(data.nick, data.data, 1);
		App::friendList.AddEdge(data.data, data.nick, 1);
		cout << "Log(" << getIP() <<","<<getPort()<<") : ģ�� �߰� "<<data.data<<endl;
		
		break;
	case RECOMMANDATION_FRIEND://��õ ģ��! ģ�� List�� ���ش�!

		friend_List = App::friendList.PrintOneEdges(data.nick);
		rec_List=App::friendList.RecommandPath(data.nick);
		
		for (vector<string>::iterator iter = friend_List.begin(); iter != friend_List.end(); ++iter) {
			for (vector<string>::iterator iter2 = rec_List.begin(); iter2 != rec_List.end();) {
				if ((*iter2) == (*iter)||(*iter2)==data.nick) {
					iter2 = rec_List.erase(iter2);//�ش� �ε����� ������� �մ������ �ٽ� iter�� �־������
				}
				else {
					++iter2;//�װ� �ƴϸ� ��ĭ ����!
				}
			}
		}
		for (vector<string>::iterator iter = rec_List.begin(); iter != rec_List.end(); ++iter) {
			//map�� �̿��ؼ� counting���ش�!
			rec_Map_iter = rec_Map.find(*iter);

			if (rec_Map_iter != rec_Map.end()) {
				rec_Map_iter->second++;
			}
			else {
				rec_Map.insert(map<string, int>::value_type(*iter, 1));
			}
		}
		cout << "Log(" << getIP() << "," << getPort() << ") : ��õ ģ�� ";
		for (rec_Map_iter = rec_Map.begin(); rec_Map_iter != rec_Map.end(); ++rec_Map_iter)
		{
			cout <<"\t"<<rec_Map_iter->first << "�� Ƚ�� : " << rec_Map_iter->second;
			rec_friend.append("("+rec_Map_iter->first+") :");
			if (rec_Map_iter->second >=3 ) {
				rec_friend.append("�ʰ��� ��õ ģ��, ���� Ƚ�� : ");
			}
			else if (rec_Map_iter->second  >=2) {
				rec_friend.append("���� ��õ ģ��, ���� Ƚ�� : ");
			}
			else {
				rec_friend.append("�Ϲ� ��õ ģ��, ���� Ƚ�� : ");
			}
			rec_friend.append(to_string(rec_Map_iter->second)+"\n");
		}
		cout << endl;
		strcpy(data.data, rec_friend.c_str());
		rec_friend = "";
		sendData(this->client_socket, data);
		break;
	case LIST_CHAT://ä�� ����Ʈ ������ ��!
		chat_iter = App::chatList.find(data.nick);
		if (chat_iter == App::chatList.end())
		{
			break;
		}
		cout << "Log(" << getIP() << "," << getPort() << ") : ä�ù� LIST -";
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
		cout << "Log(" << getIP() << "," << getPort() << ") : ä�ù� �߰� -";
		cout << data.data << endl;

		//ä�ù��� ����µ� �ű�ٰ� �ڱ� �ڽ� �ֱ�!
		chat_iter = App::chatList.find(data.data);

		//�ڱ��ڽ��� ä�ù濡 �ڱⰡ ���� �ִ� ä�� �� �ֱ�!
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
			//�����ϴ� ä�ù��̱� ������ ����!
			break;
		}
		rec_friend = data.nick;
		chat_iter->second.push_back(data.nick);
		chat_iter2->second.push_back(data.data);
		chat_iter_room->second.push_back("("+rec_friend+") : ä�ù� ��������ϴ�");
		

		rec_friend = "";
		break;
	case INVITE_CHAT:
		rec_friend = data.data;
		cout << "Log(" << getIP() << "," << getPort() << ") : ä�ù濡 ģ�� �߰� -";
		cout << rec_friend << endl;
		//ä�ù� ã�� " "�� �ɰ���!
		chat_iter = App::chatList.find(rec_friend.substr(0, rec_friend.find(" ")));
		
		if (chat_iter == App::chatList.end()) {
			data.type = INVITE_CHAT_FAIL;
			strcpy(data.data, "���� ���� �ʴ� ä�ù��Դϴ�, ä�ù��� ���� ����� �ּ���!");
			sendData(this->client_socket,data);
			break;//�������� �ʴ� ä�ù��̱� ������ break!
		}
		else {
			//ä�ù� �ȿ� ��� �ֱ�!
			chat_iter->second.push_back(rec_friend.substr(rec_friend.find(" ") + 1));
			break;
		}
		rec_friend = "";//�ʱ�ȭ
		break;
	case CHECK_CHAT:
		cout << "Log(" << getIP() << "," << getPort() << ") : ä�ù� �ο� Ȯ��-";
		rec_friend = data.data;
		cout << rec_friend << endl;
		//ä�ù� ã�� 
		chat_iter = App::chatList.find(rec_friend);

		if (chat_iter == App::chatList.end()) {
			strcpy(data.data, "����");
			
		}
		else {
			//�ο� �־��ֱ�!
			for (vector<string>::iterator iter = chat_iter->second.begin(); iter != chat_iter->second.end(); ++iter) {
				rec_friend.append(" "+*iter);
			}
			
			strcpy(data.data, rec_friend.c_str());
		}
		rec_friend = "";
		sendData(this->client_socket, data);
		
		break;
	case IN_CHAT://ä�ù� ���� �Ϸ��� ����!
		cout << "Log(" << getIP() << "," << getPort() << ") : ä�ù� ���� - ";
		cout << data.data << endl;
		chat_iter_room = App::chat_room.find(data.data);

		if (chat_iter_room == App::chat_room.end()) {
			break;//���� �濡 �����Ϸ��� ��!
		}
		else {
			data.type = FIRST_ON_CHAT;
			room = data.data;
			cout << "�� �̸� : " << room << endl;
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
	case ON_CHAT://ä�� ���� ��!
		rec_friend = data.data;
		cout << "Log(" << getIP() << "," << getPort() << ") : ä�� �� -";
		cout << rec_friend << endl;
		//ä�ù� ��������!
		chat_iter_room = App::chat_room.find(room);
		
		if (chat_iter_room == App::chat_room.end()) {
			break;//���� �濡 �����Ϸ��� ��!
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
		cout << "Log(" << getIP() << "," << getPort() << ") : ä�� �� -";
		cout << rec_friend << endl;
		break;
	default:
		cout << "�� �� �Ѿ�Խ��ϴ�" << endl;
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
		//cout << i + 1 << "�� User��! : " << user->getIP()<< " : " <<user->getPort()<<" : "<<user->getSocket()<<endl;
		try{
			//cout << i + 1 << "�� User���� Data������ : " << endl;
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
		cout << "user" << i << "��° �� �̸� : " << user->getRoom() << endl;
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
	cout << "������ �������ϴ�. (" << user.getIP() << " : " << user.getPort() << ")" << endl;
}

