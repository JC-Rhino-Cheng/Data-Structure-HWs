#include <iostream>
#include <string>
class Stack_Operator;
class Queue;
using namespace std;
#define MAX 20//�U�ЫO��: "+, -, *, /"�p��20�ӡA�B"(, )"�p��20�ӡA�Boperand�Ƥp��20��
#define Max_Length (MAX * 3)



typedef enum _type {
	EMPTY, OPERAND,
	OPERATOR_L_BRACKET_INSERTED = 2, OPERATOR_R_BRACKET = 2, OPERATOR_ADDSUB, OPERATOR_MULDIV, OPERATOR_L_BRACKET_INSERTING
	//�W�@�C�ҦC���U�ؤ��Poperator���̷��u�����ǱƦC�C�V�u���A�Ʀr�N�V�j�C�䤤�Aleft bracket���Ϥ���inserted�Minserting�C
	//�b�u�O�i��queue���B�⪺�ɭԡA�ڥu�n���D���Oleft bracket�N�n�F�A�ҥH�ڥu�n�Τ@���inserted��inserting�Y�i�C
	//��������ڲΤ@���inserting�O? �]���o�˦b�갵stack���ɭԤ�K�ܦh�C�ӥu�nstack�b�B�zL_BRACKET_INSERTING�L��A�N�⥦�令INSERTED�A�o��code�|��²��C
	//OPERATOR_L_BRACKET_INSERTED�MOPERATOR_R_BRACKET���u���׬ۦP
}Type;



class Node {
	friend Queue;
	friend Stack_Operator;

public:
	Node(char c = 'E', Type t = EMPTY) : content(c), type(t) {} // Constructor
private:
	char content;//�x�s�@��char�A�����x�s���쩳�O�Ʀr�٬Oԣ�A�ϥ��N�O���n�B��ɦA�ഫ�N��F�C //�w�]��E (E for empty)
	Type type; //�w�]��EMPTY
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
	Node Operator[MAX + MAX];//�U�ЫO�ҩҦ��|�h�B��p��20�ӡA�Ҧ����k�A���p��20��//�Ĥ@�Ӧr���j�g�A�]��"o"perator�OC++��keyword
	int num_of_operators_in_stack;
};



class Stack_Computing {
	friend  Queue;

public:
	Stack_Computing(const string& str) :num_of_ints_in_stack(0) {
		for (const auto &c : str) {
			if (c == '+') {
				int op2 = stack_int[--num_of_ints_in_stack];//�]���Ostack���c�A�ҥHop2�|���Q���X�A�A�Ӥ~�Oop1�C
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
			else {//�Ʀr���b�o��
				stack_int[num_of_ints_in_stack++] = c - '0';
			}
		}
	}

private:
	int stack_int[MAX];//�U�ЫO�ҩҦ��Ʀr��stack�A���פp��20
	int num_of_ints_in_stack;
};



class Queue {
	//�C���R���@��Queue Node�A���n�O�o���@Node����content�Mtype!!!�]�n�O�o���@Queue�����Ҧ����ƥئr�A�~��T�O��ӵ{�����`�B��
public:
	Queue() : front(Max_Length - 1), end(0), now_length(0), num_of_l_bracket(0), num_of_r_bracket(0), num_of_operand(0), num_of_arith(0), first_printed(false) {}//Default Constructor

	void FirstAttempt(string str = "") {//�ϥΪ̩ҿ�J���Ĥ@�y�C�ҥH�ڭ̭n��str�ഫ���ڭ��x�sQueue���Ҧ��C
		for (const auto &c : str) {
			En(c);
		}
	}

	void Print(void) {
		//�D�k���L�k: ��B�⦡->"Invalid"
		//�X�k���L�k: ��B�⦡->Postfix->�B�⵲�G
		//�ҥH���ެO�_�X�k�A�ڥ��L�X�B�⦡�A���C

		//���L��B�⦡
		//�H�U��if-else�O�B�z"\n"���D
		if (!first_printed) //�p�G�S�L�L�A�N���u���L\n�A�L����v�A�����L����Y�i
			first_printed = true;
		else //�p�G�w�g���O�Ĥ@���L�F�A���N�n���L\n�A�A�L����
			cout << endl;
		//�A�Ӥ~�O���骺����
		for (int i = (front + 1) % Max_Length; item[i].type != EMPTY; i = (i + 1) % Max_Length)
			cout << item[i].content;

		//�A�ӭn�Ϥ��X�k�M�D�k�����P�C�L�Ҧ�
		if (is_valid()) {
			//�{�b�}�l�c��postfix
			Stack_Operator S_O;//�o��Stack�u�t�d�x�sOperator(�]�t�|�h�B��M���k�A��)���x�s�A�]���p�G�Ooperand�A�N�|������X��postfix�F�C
			string postfix = "";

			for (int i = (front + 1) % Max_Length; item[i].type != EMPTY; i = (i + 1) % Max_Length) {
				if (item[i].type == OPERAND) postfix += item[i].content;
				else {//�p�G�Oarithmetic�Ϊ̥��k�A��
					//�L�̷|�����ΥX�ҡA�u���ר����k��stack���N�|�u�̧ǡv���ưk���A���즳�@��operator���u���פ����C
					//����{�b�ڹJ�쪺operator�~�|�i��stack�̭�
					//�p�G�ڹJ�쪺operator�O���A���A�O�o�վ㥦��type�Ӻ���stack���u���ת����T�ʡC

					bool is_to_be_stacked_a_r_bracket = (item[i].type == OPERATOR_R_BRACKET);//�O���_�ӳƥ�
					for (int j = S_O.num_of_operators_in_stack - 1; j >= 0 && S_O.Operator[j].type >= item[i].type && !(S_O.Operator[j].type == OPERATOR_L_BRACKET_INSERTED && item[i].type == OPERATOR_L_BRACKET_INSERTING); j--) {
						//�W��for�����󦳤T��:
						//(1)j>=0�C
						//(2)S_O.Operator[j].type >= item[i].type�C
						//(3) !(S_O.Operator[j].type == OPERATOR_L_BRACKET_INSERTED && item[i].type == OPERATOR_L_BRACKET_INSERTING)�A�o�O�b����p�G�J������((2+3))�o�سs��h�ӥ��A�������p�A�b�n��ĤG��stack�i�h�e�N���p�ߧ�Ĥ@�ӵ�pop���A�ҳ]�p��
						bool met_l_bracket_while_popping = (S_O.Operator[j].type == OPERATOR_L_BRACKET_INSERTED);//�O���_�ӳƥΡA�o�u���b�ڪ�is_to_be_stacked_a_r_bracket�Otrue�����p�U�A�~"���i��"�|�Otrue
						if (!met_l_bracket_while_popping) postfix += S_O.Operator[j].content; // �p�G�J�쪺���O���A���A�~�[�J��r��̭�
						S_O.Pop();

						if (is_to_be_stacked_a_r_bracket && met_l_bracket_while_popping) break;//�ڦp�G���J��k�A���A�ڦb�Ĥ@�ӹJ�쪺���A���N�n����F�A�_�h�|��Ҧ����A�����R��......
					}
					if (!is_to_be_stacked_a_r_bracket)S_O.Push(item[i]);
					if (item[i].type == OPERATOR_L_BRACKET_INSERTING) S_O.Operator[S_O.num_of_operators_in_stack - 1].type = OPERATOR_L_BRACKET_INSERTED;

				}

			}
			//�w�g��Ҧ��F�賣���L�@�M�F�A�bS_O�ѤU��operator�n���ƿ�X
			for (int i = S_O.num_of_operators_in_stack - 1; i >= 0; i--) postfix += S_O.Operator[i].content;
			//post�ܦ��غc����
			cout << endl << postfix;

			Stack_Computing S_C(postfix);
			if (S_C.num_of_ints_in_stack != 1) cout << endl << "Something wrong is in Ur code!!!!!";
			else cout << endl << S_C.stack_int[0];

		}
		else cout << endl << "Invalid";

	}

	void En(char c = 'E') {//enqueue func
		//�|����end�s��(OK�A�b�̫�@��)�Bnow_length(OK)�Bnum_of_l_bracket(OK)�Bnum_of_r_bracket(OK)�Bnum_of_operand(OK)�Bnum_of_arith(OK)
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
		//�|����front(OK�A�bfor�̪��̫�@��)�Bnow_length(OK)�Bnum_of_l_bracket(OK)�Bnum_of_r_bracket(OK)�Bnum_of_operand(OK)�Bnum_of_arith(OK)

		while (n--) {
			int true_head = (front + 1) % Max_Length;//�����ڳo�����ؼЪ��u�u�����vindex
			char ori_c = item[true_head].content;//����o���ؼЪ��쥻�x�s�����e�Ȧs�U�ӡA�_�h�|�Q�걼�N�L�k����P�_

			item[true_head].content = 'E';//�⥦��^Empty���A
			item[true_head].type = EMPTY;//�⥦��^Empty���A
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
	Node item[Max_Length];//�U�ЫO�Ҥ��W�L20+20+20��
	int front; // �x�s�ثeQueue�����Ľd�򪺶}�Y(���]�t)
	int end; // �x�s�ثeQueue�����Ľd�򪺵���(�]�t)
	//�H�Wfront�Bend��檺�N��Aex: ���׬O60�A��array�s���N�O0~59�C����Afront�N�O59�A�]��59���U�@��(a.k.a 0)�~�O���Ī��d��Cend�N�O0�C
	int now_length;
	int num_of_l_bracket;//�������A�����h�֭�
	int num_of_r_bracket;//�����k�A�����h�֭�
	int num_of_operand;
	int num_of_arith;//�Y�n�Ovalid�A��U�@�w�nnum_of_operand == num_of_arith + 1�C
	bool is_valid(void) {
		// �Y�K�W���T��Ҽg����Ӧ��l���f�ֳq�L�A�]���N��N�Ovalid�A�]�����i��X�{invalid syntax(ex: 2++3)�C
		//�ҥH�ڳo�Ө禡���Ĥ@��if��²���ˬd�W���T��ҦC����Ӽf�ֱ���C�p�G�q�L�f�֡A�A��Syntax�ӧP�_�o��Queue���X�k�ʨ쩳�p��
		if ((num_of_l_bracket != num_of_r_bracket) || (num_of_operand != num_of_arith + 1)) return false;

		//�]��Infix�@�w�Ooperator�Moperand�u����v�A�ҥH�ڥu�n�O��W�@�ӱ��쪺�Ooperator(�o�Ө禡�̩һ���operator���O��arithmetic)�٬Ooperand�A�u�n��ӳs�򱽨쪺�����ۦP�A�ߧP�D�k�C
		//�]��Infix�@�w�Ooperand�}�Y�A�ҥH�ڴN�w���]�w�u�W�@�ӱ��쪺�v�Ooperator�A�o�˴N�i�H²���W��������ѵ��갵�X�ӡC
		bool operatored = true;//�w�]�٨S���L�ɡA�W�@�ӱ��L���Ooperator
		for (int i = (front + 1) % Max_Length; item[i].type != EMPTY; i = (i + 1) % Max_Length) {
			if (operatored) {//�p�G�W�@�ӬOarith�A�o�ӴN���ӭn�Ooperand�~�|�u�Ȯ�(�]���٨S�����ˬd��)�vOK
				if (item[i].type == OPERAND) {
					//�Ȯ�OK
					operatored = false;
				}
				else if (item[i].type == OPERATOR_ADDSUB || item[i].type == OPERATOR_MULDIV) return false;//�ߧP���D�k
				else;//�ѤU�����k��جA���Mvalid���P�_�L��
			}
			else {//�p�G�W�@�ӬOoperand�A�o�ӴN���ӭn�Oarith�~�|�u�Ȯ�(�]���٨S�����ˬd��)�vOK
				if (item[i].type == OPERATOR_ADDSUB || item[i].type == OPERATOR_MULDIV) {
					//�Ȯ�OK
					operatored = true;
				}
				else if (item[i].type == OPERAND)return false;//�ߧP���D�k
				else;//�ѤU�����k��جA���Mvalid���P�_�L��
			}
		}

		return true;//�w�g�����ˬd���F�A����OK
	}
	bool first_printed;// �]���榡�����Y(�̫�@�C���|�A���C)�A�ҥH�n�令�������r�ꪺ�e���n��X�e�~���C�C
};



int main() {
	Queue Q;

	string cmd;
	while (cin >> cmd) {//cin>>string�|�b�Ĥ@�ӪŮ�N���U��
		if (cmd == "Print") Q.Print();
		else if (cmd == "Enqueue") {
			string c; cin >> c;
			Q.En(c[0]);//c�Ostring�Cstr[0]�~�ണ���Xchar�C
		}
		else if (cmd == "Dequeue") {
			int n; cin >> n;
			Q.De(n);
		}
		else {//�u���ϥΪ̿�J�Ĥ@�y���ɭԡA�~�Q����o��
			Q.FirstAttempt(cmd);
		}
	}

	//system("pause");
}