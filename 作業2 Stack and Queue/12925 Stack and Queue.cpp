#include <iostream>
#include <string>
class Stack_Operator;
class Queue;
using namespace std;
#define MAX 20//助教保證: "+, -, *, /"小於20個，且"(, )"小於20個，且operand數小於20個
#define Max_Length (MAX * 3)



typedef enum _type {
	EMPTY, OPERAND,
	OPERATOR_L_BRACKET_INSERTED = 2, OPERATOR_R_BRACKET = 2, OPERATOR_ADDSUB, OPERATOR_MULDIV, OPERATOR_L_BRACKET_INSERTING
	//上一列所列的各種不同operator有依照優先順序排列。越優先，數字就越大。其中，left bracket有區分為inserted和inserting。
	//在只是進行queue的運算的時候，我只要知道它是left bracket就好了，所以我只要統一選擇inserted或inserting即可。
	//那為什麼我統一選擇inserting呢? 因為這樣在實做stack的時候方便很多。而只要stack在處理L_BRACKET_INSERTING過後，就把它改成INSERTED，這樣code會最簡單。
	//OPERATOR_L_BRACKET_INSERTED和OPERATOR_R_BRACKET的優先度相同
}Type;



class Node {
	friend Queue;
	friend Stack_Operator;

public:
	Node(char c = 'E', Type t = EMPTY) : content(c), type(t) {} // Constructor
private:
	char content;//儲存一個char，不管儲存的到底是數字還是啥，反正就是等要運算時再轉換就對了。 //預設為E (E for empty)
	Type type; //預設為EMPTY
};



class Stack_Operator {
	friend  Queue;

	Stack_Operator() :num_of_operators_in_stack(0) {}//Default Constructor

	void Pop() {
		num_of_operators_in_stack--;
		Operator[num_of_operators_in_stack].content = 'E';
		Operator[num_of_operators_in_stack].type = EMPTY;
	}

	void Push(Node& n) {
		Operator[num_of_operators_in_stack].content = n.content;
		Operator[num_of_operators_in_stack].type = n.type;
		num_of_operators_in_stack++;
	}

private:
	Node Operator[MAX + MAX];//助教保證所有四則運算小於20個，所有左右括弧小於20個//第一個字母大寫，因為"o"perator是C++的keyword
	int num_of_operators_in_stack;
};



class Stack_Computing {
	friend  Queue;

public:
	Stack_Computing(const string& str) :num_of_ints_in_stack(0) {
		for (const auto &c : str) {
			if (c == '+') {
				int op2 = stack_int[--num_of_ints_in_stack];//因為是stack結構，所以op2會先被取出，再來才是op1。
				int op1 = stack_int[--num_of_ints_in_stack];
				stack_int[num_of_ints_in_stack++] = op1 + op2;
			}
			else if (c == '-') {
				int op2 = stack_int[--num_of_ints_in_stack];
				int op1 = stack_int[--num_of_ints_in_stack];
				stack_int[num_of_ints_in_stack++] = op1 - op2;
			}
			else if (c == '*') {
				int op2 = stack_int[--num_of_ints_in_stack];
				int op1 = stack_int[--num_of_ints_in_stack];
				stack_int[num_of_ints_in_stack++] = op1 * op2;
			}
			else if (c == '/') {
				int op2 = stack_int[--num_of_ints_in_stack];
				int op1 = stack_int[--num_of_ints_in_stack];
				stack_int[num_of_ints_in_stack++] = op1 / op2;
			}
			else {//數字都在這裡
				stack_int[num_of_ints_in_stack++] = c - '0';
			}
		}
	}

private:
	int stack_int[MAX];//助教保證所有數字的stack，長度小於20
	int num_of_ints_in_stack;
};



class Queue {
	//每次刪掉一個Queue Node，都要記得維護Node中的content和type!!!也要記得維護Queue中的所有的數目字，才能確保整個程式正常運行
public:
	Queue() : front(Max_Length - 1), end(0), now_length(0), num_of_l_bracket(0), num_of_r_bracket(0), num_of_operand(0), num_of_arith(0), first_printed(false) {}//Default Constructor

	void FirstAttempt(string str = "") {//使用者所輸入的第一句。所以我們要把str轉換成我們儲存Queue的模式。
		for (const auto &c : str) {
			En(c);
		}
	}

	void Print(void) {
		//非法的印法: 原運算式->"Invalid"
		//合法的印法: 原運算式->Postfix->運算結果
		//所以不管是否合法，我先印出運算式再說。

		//先印原運算式
		//以下的if-else是處理"\n"問題
		if (!first_printed) //如果沒印過，就不「先印\n再印本體」，直接印本體即可
			first_printed = true;
		else //如果已經不是第一次印了，那就要先印\n，再印本體
			cout << endl;
		//再來才是本體的部分
		for (int i = (front + 1) % Max_Length; item[i].type != EMPTY; i = (i + 1) % Max_Length)
			cout << item[i].content;

		//再來要區分合法和非法的不同列印模式
		if (is_valid()) {
			//現在開始構建postfix
			Stack_Operator S_O;//這個Stack只負責儲存Operator(包含四則運算和左右括弧)的儲存，因為如果是operand，就會直接輸出到postfix了。
			string postfix = "";

			for (int i = (front + 1) % Max_Length; item[i].type != EMPTY; i = (i + 1) % Max_Length) {
				if (item[i].type == OPERAND) postfix += item[i].content;
				else {//如果是arithmetic或者左右括弧
					//他們會先蝗蟲出境，優先度足夠逃離stack的就會「依序」全數逃離，直到有一個operator的優先度不夠。
					//之後現在我遇到的operator才會進到stack裡面
					//如果我遇到的operator是左括號，記得調整它的type來維持stack的優先度的正確性。

					bool is_to_be_stacked_a_r_bracket = (item[i].type == OPERATOR_R_BRACKET);//記錄起來備用
					for (int j = S_O.num_of_operators_in_stack - 1; j >= 0 && S_O.Operator[j].type >= item[i].type && !(S_O.Operator[j].type == OPERATOR_L_BRACKET_INSERTED && item[i].type == OPERATOR_L_BRACKET_INSERTING); j--) {
						//上面for的條件有三個:
						//(1)j>=0。
						//(2)S_O.Operator[j].type >= item[i].type。
						//(3) !(S_O.Operator[j].type == OPERATOR_L_BRACKET_INSERTED && item[i].type == OPERATOR_L_BRACKET_INSERTING)，這是在防止如果遇到類似((2+3))這種連續多個左括號的情況，在要把第二個stack進去前就不小心把第一個給pop掉，所設計的
						bool met_l_bracket_while_popping = (S_O.Operator[j].type == OPERATOR_L_BRACKET_INSERTED);//記錄起來備用，這只有在我的is_to_be_stacked_a_r_bracket是true的情況下，才"有可能"會是true
						if (!met_l_bracket_while_popping) postfix += S_O.Operator[j].content; // 如果遇到的不是左括號，才加入到字串裡面
						S_O.Pop();

						if (is_to_be_stacked_a_r_bracket && met_l_bracket_while_popping) break;//我如果有遇到右括號，我在第一個遇到的左括號就要停止了，否則會把所有左括號都刪掉......
					}
					if (!is_to_be_stacked_a_r_bracket)S_O.Push(item[i]);
					if (item[i].type == OPERATOR_L_BRACKET_INSERTING) S_O.Operator[S_O.num_of_operators_in_stack - 1].type = OPERATOR_L_BRACKET_INSERTED;

				}

			}
			//已經把所有東西都掃過一遍了，在S_O剩下的operator要全數輸出
			for (int i = S_O.num_of_operators_in_stack - 1; i >= 0; i--) postfix += S_O.Operator[i].content;
			//post至此建構完成
			cout << endl << postfix;

			Stack_Computing S_C(postfix);
			if (S_C.num_of_ints_in_stack != 1) cout << endl << "Something wrong is in Ur code!!!!!";
			else cout << endl << S_C.stack_int[0];

		}
		else cout << endl << "Invalid";

	}

	void En(char c = 'E') {//enqueue func
		//會改變end編號(OK，在最後一行)、now_length(OK)、num_of_l_bracket(OK)、num_of_r_bracket(OK)、num_of_operand(OK)、num_of_arith(OK)
		item[end].content = c;
		now_length++;

		switch (c) {
			case '(':
				num_of_l_bracket++;
				item[end].type = OPERATOR_L_BRACKET_INSERTING;
				break;
			case ')':
				num_of_r_bracket++;
				item[end].type = OPERATOR_R_BRACKET;
				break;
			case '+':
			case '-':
				num_of_arith++;
				item[end].type = OPERATOR_ADDSUB;
				break;
			case '*':
			case '/':
				num_of_arith++;
				item[end].type = OPERATOR_MULDIV;
				break;
			default/*0~9*/:
				num_of_operand++;
				item[end].type = OPERAND;
				break;
		}

		end = (end + 1) % Max_Length;
	}

	void De(int n = 0) {//dequeue func
		//會改變front(OK，在for裡的最後一行)、now_length(OK)、num_of_l_bracket(OK)、num_of_r_bracket(OK)、num_of_operand(OK)、num_of_arith(OK)

		while (n--) {
			int true_head = (front + 1) % Max_Length;//先找到我這輪的目標的「真正的」index
			char ori_c = item[true_head].content;//先把這輪目標的原本儲存的內容暫存下來，否則會被刷掉就無法後續判斷

			item[true_head].content = 'E';//把它刷回Empty狀態
			item[true_head].type = EMPTY;//把它刷回Empty狀態
			now_length--;

			switch (ori_c) {
				case '(':
					num_of_l_bracket--;
					break;
				case ')':
					num_of_r_bracket--;
					break;
				case '+':
				case '-':
					num_of_arith--;
					break;
				case '*':
				case '/':
					num_of_arith--;
					break;
				default/*0~9*/:
					num_of_operand--;
					break;
			}

			front = true_head;
		}
	}

private:
	Node item[Max_Length];//助教保證不超過20+20+20個
	int front; // 儲存目前Queue的有效範圍的開頭(不包含)
	int end; // 儲存目前Queue的有效範圍的結尾(包含)
	//以上front、end兩行的意思，ex: 長度是60，那array編號就是0~59。那麼，front就是59，因為59的下一個(a.k.a 0)才是有效的範圍。end就是0。
	int now_length;
	int num_of_l_bracket;//紀錄左括號有多少個
	int num_of_r_bracket;//紀錄右括號有多少個
	int num_of_operand;
	int num_of_arith;//若要是valid，當下一定要num_of_operand == num_of_arith + 1。
	bool is_valid(void) {
		// 即便上面三行所寫的兩個式子都審核通過，也不代表就是valid，因為有可能出現invalid syntax(ex: 2++3)。
		//所以我這個函式的第一個if先簡單檢查上面三行所列的兩個審核條件。如果通過審核，再用Syntax來判斷這個Queue的合法性到底如何
		if ((num_of_l_bracket != num_of_r_bracket) || (num_of_operand != num_of_arith + 1)) return false;

		//因為Infix一定是operator和operand「交錯」，所以我只要記住上一個掃到的是operator(這個函式裡所說的operator都是指arithmetic)還是operand，只要兩個連續掃到的類型相同，立判非法。
		//因為Infix一定是operand開頭，所以我就預先設定「上一個掃到的」是operator，這樣就可以簡單把上面那行註解給實做出來。
		bool operatored = true;//預設還沒掃過時，上一個掃過的是operator
		for (int i = (front + 1) % Max_Length; item[i].type != EMPTY; i = (i + 1) % Max_Length) {
			if (operatored) {//如果上一個是arith，這個就應該要是operand才會「暫時(因為還沒全部檢查完)」OK
				if (item[i].type == OPERAND) {
					//暫時OK
					operatored = false;
				}
				else if (item[i].type == OPERATOR_ADDSUB || item[i].type == OPERATOR_MULDIV) return false;//立判為非法
				else;//剩下的左右兩種括號和valid的判斷無關
			}
			else {//如果上一個是operand，這個就應該要是arith才會「暫時(因為還沒全部檢查完)」OK
				if (item[i].type == OPERATOR_ADDSUB || item[i].type == OPERATOR_MULDIV) {
					//暫時OK
					operatored = true;
				}
				else if (item[i].type == OPERAND)return false;//立判為非法
				else;//剩下的左右兩種括號和valid的判斷無關
			}
		}

		return true;//已經全部檢查完了，完全OK
	}
	bool first_printed;// 因為格式的關係(最後一列不會再換列)，所以要改成全部的字串的前面要輸出前才換列。
};



int main() {
	Queue Q;

	string cmd;
	while (cin >> cmd) {//cin>>string會在第一個空格就停下來
		if (cmd == "Print") Q.Print();
		else if (cmd == "Enqueue") {
			string c; cin >> c;
			Q.En(c[0]);//c是string。str[0]才能提取出char。
		}
		else if (cmd == "Dequeue") {
			int n; cin >> n;
			Q.De(n);
		}
		else {//只有使用者輸入第一句的時候，才被分到這裡
			Q.FirstAttempt(cmd);
		}
	}

	//system("pause");
}