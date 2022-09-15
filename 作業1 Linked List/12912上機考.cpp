#include <iostream>
#include <string>
using namespace std;

class LinkedList;

class ListNode {
public:
	ListNode() :data(' '), next(NULL), pre(NULL) {}
	ListNode(char c) :data(c), next(NULL), pre(NULL) {}

	friend class LinkedList;

	char data;
	ListNode *next;
	ListNode *pre;
};

class LinkedList {
public:
	LinkedList() :num_of_nodes(0) {
		cursor = dummy_head = new ListNode('!');
		dummy_tail = new ListNode('@');

		dummy_head->next = dummy_tail;
		dummy_tail->pre = dummy_head;
	}
	void add_node(char c);
	void move_right(int p);
	void move_left(int p);
	void backspace();
	void print();


	ListNode *dummy_head, *dummy_tail;
	ListNode *cursor;
	int num_of_nodes;

};

void LinkedList::backspace() {
	if (cursor == dummy_head)return;

	auto* To_Be_Deleted = cursor;
	To_Be_Deleted->next->pre = cursor->pre;
	To_Be_Deleted->pre->next = cursor->next;
	cursor = cursor->pre;

	delete To_Be_Deleted;

	num_of_nodes--;
}

void LinkedList::print() {
	auto* cur = dummy_head->next;
	int count = 0;
	while (cur != dummy_tail) {
		cout << cur->data;
		cur = cur->next;
		
		count++;
	}
	if (count != num_of_nodes) cout << "There exists sth wrong w/ your code!";
	cout << endl;

}

void LinkedList::add_node(char c) {
	auto* new_node = new ListNode(c);
	new_node->next = cursor->next;
	new_node->next->pre = new_node;
	cursor->next = new_node;
	new_node->pre = cursor;

	cursor = new_node;
	num_of_nodes++;


}

void LinkedList::move_right(int p) {
	if (p > num_of_nodes) cursor = dummy_tail->pre;
	else {
		for (int i = 0; i < p && cursor != dummy_tail->pre; i++) cursor = cursor->next;
	}
}
void LinkedList::move_left(int p) {
	if (p > num_of_nodes)cursor = dummy_head;
	else {
		for (int i = 0; i < p&&cursor != dummy_head; i++) cursor = cursor->pre;
	}

}


int main() {

	LinkedList list;
	string command;
	string input;
	int p;
	while (cin >> command) {
		if (command == "Type") {
			cin >> input;
			if (input.empty()) {
				continue;
			}
			else {
				for (int i = 0; i < input.length(); i++) {
					list.add_node(input[i]);
				}
			}
		}
		else if (command == "Move") {
			cin >> p;
			if (p > 0) {
				list.move_right(p);
			}
			else if (p < 0) {
				list.move_left( (int)abs(p) );
			}
			else {
				continue;
			}
		}
		else if (command == "Backspace") {
			list.backspace();
		}
		else if (command == "Print") {
			list.print();
		}
		else {
			continue;
		}
	}



}