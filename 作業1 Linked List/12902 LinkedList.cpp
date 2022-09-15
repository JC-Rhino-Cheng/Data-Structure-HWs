#include <iostream>
#include <string>
#include <climits>
class List;
using namespace std;

class Node {
	friend List;

public:
	Node(unsigned int val = 0) : data(val), next(NULL) {}
	~Node() = default; //=default means it's trivial 

protected:
	unsigned int data;
	Node* next;
};

class List {
public:
	Node *dummy_head, *dummy_tail;
	int num_of_nodes;

	/*--------------------------------------------------------------------*/
	/*������������������������������������������������������������*/
	//Constructor & Destructor
	List() : num_of_nodes(0) {
		dummy_head = new Node(INT_MIN);
		dummy_tail = dummy_head->next = new Node(INT_MAX);
	}
	~List() {
		auto* cur = dummy_head;
		while (cur) {
			auto* next_node = cur->next;
			delete cur;
			cur = next_node;
		}
	}
	/*������������������������������������������������������������*/
	/*--------------------------------------------------------------------*/




	/*--------------------------------------------------------------------*/
	/*������������������������������������������������������������*/
	//Functions
	void InsertFront(unsigned int val) {
		auto* new_node = new Node(val);
		new_node->next = dummy_head->next;
		dummy_head->next = new_node;

		num_of_nodes++;
	}
	void InsertBack(unsigned int val) {
		//�����u����tail
		auto *cur = dummy_head; auto & last = cur;//last�Ocur���O�W
		while (cur->next != dummy_tail) cur = cur->next;//last�w�O�u����tail

		last->next = new Node(val);
		last->next->next = dummy_tail;

		num_of_nodes++;
	}
	void InsertBefore(unsigned int K, unsigned int val) {
		auto* cur = dummy_head, *cur_next = dummy_head;
		while (cur_next && cur_next->data != K) {
			cur_next = cur_next->next;
			cur = (dummy_head->next == cur_next) ? cur : cur->next;
		}//���@�w�����K(�]��short-circuit evaluation)�A�ٻݧP�_�C

		if (cur != dummy_tail) { //K found
			auto* new_node = new Node(val);
			new_node->next = cur_next;
			cur->next = new_node;

			num_of_nodes++;
		}
		else;//K not found
	}
	void InsertAfter(unsigned int K, unsigned int val) {
		auto* cur = dummy_head;
		while (cur && cur->data != K) cur = cur->next;//���@�w�����K(�]��short-circuit evaluation)�A�ٻݧP�_�C

		if (cur) { //K found
			auto* new_node = new Node(val);
			new_node->next = cur->next;
			cur->next = new_node;

			num_of_nodes++;
		}
		else; //K not found

	}
	void Delete(unsigned int K) {
		auto* cur = dummy_head, *cur_next = dummy_head;
		while (cur_next && cur_next->data != K) {
			cur_next = cur_next->next;
			cur = (dummy_head->next == cur_next) ? cur : cur->next;
		}//���@�w�����K(�]��short-circuit evaluation)�A�ٻݧP�_�C

		if (cur != dummy_tail) { //K found
			cur->next = cur_next->next;

			delete cur_next;

			num_of_nodes--;
		}
		else; //K not found
	}
	void DeleteFront() {
		if (num_of_nodes == 0);//�ڥ��S�F��A�R�ӧ�
		else {
			auto* To_Be_Deleted = dummy_head->next;
			auto* To_Be_Connected = To_Be_Deleted->next;

			delete To_Be_Deleted;
			dummy_head->next = To_Be_Connected;

			num_of_nodes--;
		}
	}
	void DeleteBack() {
		if (num_of_nodes == 0);//�ڥ��S�F��A�R�ӧ�
		else {
			auto* cur_next = dummy_head->next;
			auto* cur = dummy_head;
			while (cur_next && cur_next->next != dummy_tail) {
				cur_next = cur_next->next;
				cur = cur->next;
			}//���̫�@�ӤH�H�Υ��e�������ӤH�F

			delete cur_next;
			cur->next = dummy_tail;

			num_of_nodes--;
		}

	}
	void Reverse(unsigned int L, unsigned int R) {
		if (L == R)return;//L==R�A�N�ڥ�����reverse�C

		bool L_found = false, R_found = false;

		auto* cur_L = dummy_head, *cur_next_L = dummy_head;
		auto* cur_R = dummy_head;
		while (cur_next_L && cur_next_L->data != L) {
			cur_next_L = cur_next_L->next;
			cur_L = (dummy_head->next == cur_next_L) ? cur_L : cur_L->next;
		}//���@�w�����L(�]��short-circuit evaluation)�A�ٻݧP�_�C
		if (cur_next_L)L_found = true;

		if (L_found) { //�u��L�T��s�b�A�~����R���N�q�C
			cur_R = cur_next_L->next;

			while (cur_R && cur_R->data != R) cur_R = cur_R->next;//���@�w�����R(�]��short-circuit evaluation)�A�ٻݧP�_�C
			if (cur_R) {
				R_found = true;

				auto* bu_dong_L = cur_L; //bu_dong:�����ʡC�N�Y�Q�nreverse��list�����䨺�Ӥ��ʪ�node
				auto* bu_dong_R = cur_R->next;
				auto* dong_L = bu_dong_L->next;

				SubReversing(bu_dong_L->next, cur_R, bu_dong_L, bu_dong_L->next);

				bu_dong_L->next = cur_R;
				dong_L->next = bu_dong_R;
			}
		}
	}
	void Clear() {
		if (num_of_nodes != 0) {
			auto* To_Be_Deleted = dummy_head->next;
			auto* next = To_Be_Deleted->next;
			while (next) {
				delete To_Be_Deleted;

				To_Be_Deleted = next;
				next = next->next;

				num_of_nodes--;
			}
			dummy_head->next = dummy_tail;
			if (num_of_nodes != 0)cout << "Clearance not done. There exists sth wrong." << endl;
		}
		else;
	}
	void Print() {
		auto* cur = dummy_head->next;
		while (cur != dummy_tail) {
			cout << cur->data << " ";
			cur = cur->next;
		}
		cout << endl;
	}

private:
	void SubReversing(Node* L_Bound, Node* R_Bound, Node* prev, Node* cur) {
		static bool flag_is_going_back = false;

		if (cur == R_Bound) {
			flag_is_going_back = true;

			cur->next = prev;

			return;
		}

		SubReversing(L_Bound, R_Bound, cur, cur->next);

		if (flag_is_going_back) {
			cur->next = prev;

			return;
		}
	}


	/*������������������������������������������������������������*/
	/*--------------------------------------------------------------------*/
};

int main() {
	List L;

	string cmd;
	while (cin >> cmd) {
		if (cmd == "InsertFront") {
			unsigned int Insert_Val; cin >> Insert_Val;
			L.InsertFront(Insert_Val);
		}
		else if (cmd == "InsertBack") {
			unsigned int Insert_Val; cin >> Insert_Val;
			L.InsertBack(Insert_Val);
		}
		else if (cmd == "InsertBefore") {
			unsigned int K, Insert_Val; cin >> K >> Insert_Val;
			L.InsertBefore(K, Insert_Val);
		}
		else if (cmd == "InsertAfter") {
			unsigned int K, Insert_Val; cin >> K >> Insert_Val;
			L.InsertAfter(K, Insert_Val);
		}
		else if (cmd == "Delete") {
			unsigned int Delete_Val; cin >> Delete_Val;
			L.Delete(Delete_Val);
		}
		else if (cmd == "DeleteFront") {
			L.DeleteFront();
		}
		else if (cmd == "DeleteBack") {
			L.DeleteBack();
		}
		else if (cmd == "Reverse") {
			unsigned int Rev_L_Bound, Rev_R_Bound; cin >> Rev_L_Bound >> Rev_R_Bound;
			L.Reverse(Rev_L_Bound, Rev_R_Bound);
		}
		else if (cmd == "Clear") {
			L.Clear();
		}
		else if (cmd == "Print") {
			L.Print();
		}
	}

	system("pause");
}