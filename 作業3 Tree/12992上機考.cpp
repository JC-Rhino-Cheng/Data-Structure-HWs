#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <ctype.h>
using namespace std;




class Node {
public:
	Node(int data = -1) :data(data), l(NULL), r(NULL) {}


	int data;
	Node *l, *r;
};


class Tree {
public:
	Tree(int* in, int* post, int in_count, int post_count) :in(in), post(post), in_count(in_count), post_count(post_count), root(NULL) {
		fill(visited, visited + 100001, true);
	}
	void prefix(Node* root){
		if (!root)return;

		if (pre.length() != 0)pre += "," + to_string(root->data);
		else pre += to_string(root->data);

		prefix(root->l);
		prefix(root->r);
	}
	Node* construct(int in_l_bound, int in_r_bound) {
		if (in_l_bound > in_r_bound)return NULL;
		
		for (int i = post_count - 1; visited[i]; i--);
		int key = post[--post_count];
		Node *root = new Node(key);

		int key_idx_in_in = 0;
		for (int i = 0; i < in_count; i++) {
			if (in[i] == key) {
				key_idx_in_in = i; break;
			}
		}

		root->r = construct(key_idx_in_in + 1, in_r_bound);
		root->l = construct(in_l_bound, key_idx_in_in - 1);

		return root;
	}


	Node *root;
	string pre = "";
	int *in, *post;
	int in_count, post_count;

	bool visited[100001];
};



int main() {
	string _in, _post;
	while (cin >> _in >> _post) {
		int in[100001], post[100001];
		int cur_in_idx = 0, cur_post_idx = 0;

		Tree T(in, post, cur_in_idx, cur_post_idx);

		for (unsigned int i = 0; i < _in.length(); i++) {
			if (isdigit(_in[i])) {
				int num = _in[i] - '0';
				i++;
				for (; isdigit(_in[i]); i++) {
					num *= 10; num += _in[i] - '0';
				}
				i--;
				in[cur_in_idx++] = num;
				T.visited[num] = false;
			}
			else if (_in[i] == ',') {
				i++;
				int num = _in[i] - '0';
				i++;
				for (; isdigit(_in[i]); i++) {
					num *= 10; num += _in[i] - '0';
				}
				i--;
				in[cur_in_idx++] = num;
				T.visited[num] = false;
			}
		}
		for (unsigned int i = 0; i < _post.length(); i++) {
			if (isdigit(_post[i])) {
				int num = _post[i] - '0';
				i++;
				for (; isdigit(_post[i]); i++) {
					num *= 10; num += _post[i] - '0';
				}
				i--;
				post[cur_post_idx++] = num;
				T.visited[num] = false;
			}
			else if (_in[i] == ',') {
				i++;
				int num = _post[i] - '0';
				i++;
				for (; isdigit(_post[i]); i++) {
					num *= 10; num += _post[i] - '0';
				}
				i--;
				post[cur_post_idx++] = num;
				T.visited[num] = false;
			}
		}

		T.in_count = cur_in_idx;
		T.post_count = cur_post_idx;
		
		T.root = T.construct(0, cur_in_idx - 1);
		T.prefix(T.root);
		cout << T.pre << endl;
	}










	//system("pause");
}



/*
4,2,5,1,8,6,9,3,7,10
4,5,2,8,9,6,10,7,3,1


*/