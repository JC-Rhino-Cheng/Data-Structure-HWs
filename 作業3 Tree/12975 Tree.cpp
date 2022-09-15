#include <iostream>
#include <string>
#include <string.h>//for memcpy
#include <climits>
#include <ctype.h>
#define  init_capacity 300000 //最大1200000，所以最多會changesize: 2次。30萬->60萬->120萬。總共兩次
//#define  init_capacity 1200000 //最大1200000，所以最多會changesize: 2次。30萬->60萬->120萬。總共兩次
//#define  init_capacity 2 //最大1200000，所以最多會changesize: 2次。30萬->60萬->120萬。總共兩次
using namespace std;

class Tree {
public:
	Tree() :capacity(init_capacity) {
		t = new int[capacity + 1]();//+1是因為t[0]是被棄置不用的
	}
	void rebirth() {
		fill(t + 1, t + capacity + 1, INT_MIN); //t+1: 因為t[0]被棄置不用。t + capacity + 1: 因為fill是跟py的slice一樣左包含右不包含，所以需要+capacity，"再+1"
		pre = in = post = ""; post_idx = 1;//一開始post_idx就從1開始，雖然0-based也可以。不過還是跟t[0]棄置不用的寫法統一一下比較好
		height = 0; weight_sum = 0; maximum_path_sum = INT_MIN; 
		binary_tower = 0;
		max_idx = -1;//紀錄t陣列中，最大的有效idx是誰，這樣我在這個範圍以右的地方就可以直接不看
		
		l_subtree_route = r_subtree_route = ""; foldable = false;
		flag_computed_traverse = false; flag_computed_h_ws_mps = false; flag_computed_foldable = false; flag_computed_binarytower = false;
	}
	void recalculate_traverse() {
		pre = in = post = ""; post_idx = 1;
		Traverse();

		flag_computed_traverse = true;
	}
	void recalculate_h_ws_mps() {
		height = 0; weight_sum = 0; maximum_path_sum = INT_MIN;
		Height_and_WeightSum_and_MaximumPathSum();

		flag_computed_h_ws_mps = true;
	}
	void recalculate_foldable() {
		l_subtree_route = r_subtree_route = ""; foldable = false;

		//需要針對t[1]沒效，以及t[1]有效但t[2]或t[3]無效特判。如果t[1~3]皆有效，再來看t[1]的左subtree和右subtree的路徑轉彎是否順序都一模一樣
		if ((t[1] == INT_MIN || t[1] == INT_MAX))foldable = true;//t[1]沒效
		else {//t[1]有效
			L_STree_Route(); R_STree_Route();
			if ((t[2] == INT_MIN || t[2] == INT_MAX) && (t[3] == INT_MIN || t[3] == INT_MAX))foldable = true;//t[2~3]皆無效
			else if ((t[2] == INT_MIN || t[2] == INT_MAX) && (t[3] != INT_MIN && t[3] != INT_MAX))foldable = false;//t[2]有效，t[3]無效
			else if ((t[2] != INT_MIN && t[2] != INT_MAX) && (t[3] == INT_MIN || t[3] == INT_MAX))foldable = false;//t[3]有效，t[2]無效
			else {//t[2~3]皆有效
				if (l_subtree_route != r_subtree_route)foldable = false;
				else foldable = true;
			}
		}

		flag_computed_foldable = true;
	}
	int recalculate_binarytower(int cur_idx=1) {//看註解leetcode
		flag_computed_traverse = true;
		if (t[cur_idx] == INT_MIN || t[cur_idx] == INT_MAX) return 1;

		int l = recalculate_binarytower(cur_idx * 2);
		int r = recalculate_binarytower(cur_idx * 2 + 1);
		int product = l * r;

		if (!product) {
			binary_tower++;
			return 2;
		}

		if (product == 1)return 0;
		else return 1;
	}
	void DeleteLeaf(int cur_idx = 1) {
		if (t[cur_idx] == INT_MIN || t[cur_idx] == INT_MAX)return;

		flag_computed_traverse = flag_computed_h_ws_mps = flag_computed_foldable = flag_computed_binarytower = false;
		binary_tower = 0;

		if ((t[cur_idx * 2] == INT_MIN || t[cur_idx * 2] == INT_MAX) && (t[cur_idx * 2 + 1] == INT_MIN || t[cur_idx * 2 + 1] == INT_MAX)) {
			t[cur_idx] = INT_MAX;
			if (cur_idx == max_idx)max_idx = cur_idx / 2;
			return;
		}
		else {
			DeleteLeaf(cur_idx * 2);
			DeleteLeaf(cur_idx * 2 + 1);
		}
	}
	void Construct_original(string& cmd) {
		string stack;
		auto cur_idx = 1;
		for (auto i = 0; i < cmd.length(); i++) {
			if (cmd[i] == '(') {
				stack += cmd[i];
				if (i != 0)cur_idx *= 2;//看到"("，就進到child裡面。先預設進leftchild，如果left已經被建構完成了(不管是否為NULL都有紀錄，如果是空的，值會是INTMAX。如果不是空的，值就是該有的值)，就cur_idx++，寫在line97
				if (cur_idx >= capacity)double_1Darray_size();//如果cur_idx超標，要馬上擴增array並搬遷
				
				if (t[cur_idx] != INT_MIN) cur_idx++;

				continue;
			}
			else if (cmd[i]=='-') {
				i++;
				auto num = cmd[i] - '0';
				i++;
				for (; isdigit(cmd[i]); i++) {
					num *= 10; num += cmd[i] - '0';
				}
				i--;
				
				if (cur_idx >= capacity)double_1Darray_size();//如果cur_idx超標，要馬上擴增array並搬遷
				t[cur_idx] = num * -1;//因為是已知是負數，而我目前只處理正的部分，所以要加負號
				if (max_idx < cur_idx)max_idx = cur_idx;//順便維護max_idx

				continue;
			}
			else if (isdigit(cmd[i])) {
				auto num = cmd[i] - '0';
				i++;
				for (; isdigit(cmd[i]); i++) {
					num *= 10; num += cmd[i] - '0';
				}
				i--;

				if (cur_idx >= capacity)double_1Darray_size();//如果cur_idx超標，要馬上擴增array並搬遷
				t[cur_idx] = num;
				if (max_idx < cur_idx)max_idx = cur_idx;//順便維護max_idx

				continue;
			}
			else if (cmd[i] == ')') {
				stack.pop_back();
				if (t[cur_idx] == INT_MIN) t[cur_idx] = INT_MAX;//如果是")"，代表這個node一定已經要有標記才能放回去。如果為非空的node，值已經早就被上面兩個elif修改了，我就不用動。可是如果是INT_MIN，代表他是個空的node，我還是要留下標記才能放他走，所以把INT_MIN改成INT_MAX
				cur_idx /= 2;//記得把cur_idx搞回這個child的root

				continue;
			}
		}
		if (stack.length() != 0)cout << "There's sth wrong when constructing the tree!" << endl;
	}
	void Traverse() {
		PreOrder();
		InOrder();
		PostOrder();
	}
	void PreOrder(int cur_idx = 1) {
		if (t[cur_idx] == INT_MIN || t[cur_idx] == INT_MAX) return;

		pre += to_string(t[cur_idx]) + ' ';

		PreOrder(cur_idx * 2);
		PreOrder(cur_idx * 2 + 1);
	}
	void InOrder(int cur_idx = 1) {
		if (t[cur_idx] == INT_MIN || t[cur_idx] == INT_MAX) return;

		InOrder(cur_idx * 2);

		in += to_string(t[cur_idx]) + ' ';

		InOrder(cur_idx * 2 + 1);
	}
	void PostOrder(int cur_idx = 1) {
		if (t[cur_idx] == INT_MIN || t[cur_idx] == INT_MAX) return;

		PostOrder(cur_idx * 2);
		PostOrder(cur_idx * 2 + 1);

		post += to_string(t[cur_idx]) + ' ';
	}
	void Height_and_WeightSum_and_MaximumPathSum(int cur_idx = 1, int height = 0, int sum = 0) {
		//height和weightsum和maximumpathsum共用這個函式
		//其中，this->height用height；weightsum和maximumpathsum共用sum
		if (t[cur_idx] == INT_MIN || t[cur_idx] == INT_MAX) return;

		//需要(1)檢查高度是否須更新、(2)binarytower、(3)需要檢查maximumpathsum是否須更新、(4)另需要更新weightsum
		if (height + 1/*這層的高度是height + 1，為何是這個數字，想想root的高度和height之間的關係即可*/ > this->height)this->height = height + 1;//更新高度
		if ((t[cur_idx * 2] == INT_MIN || t[cur_idx * 2] == INT_MAX) && (t[cur_idx * 2 + 1] == INT_MIN || t[cur_idx * 2 + 1] == INT_MAX))
			if (sum + t[cur_idx] > maximum_path_sum)maximum_path_sum = sum + t[cur_idx];//更新maximum_path_sum
		weight_sum += t[cur_idx];//更新weightsum

		Height_and_WeightSum_and_MaximumPathSum(cur_idx * 2, height + 1, sum + t[cur_idx]);
		Height_and_WeightSum_and_MaximumPathSum(cur_idx * 2 + 1, height + 1, sum + t[cur_idx]);
	}
	void L_STree_Route(int cur_idx = 2) {
		if (t[cur_idx] == INT_MIN || t[cur_idx] == INT_MAX) return;

		if (t[cur_idx * 2] != INT_MAX) {
			l_subtree_route += "l";
			L_STree_Route(cur_idx * 2);
		}
		if (t[cur_idx * 2 + 1] != INT_MAX) {
			l_subtree_route += "r";
			L_STree_Route(cur_idx * 2 + 1);
		}
	}
	void R_STree_Route(int cur_idx = 3) {
		if (t[cur_idx] == INT_MIN || t[cur_idx] == INT_MAX) return;

		if (t[cur_idx * 2 + 1] != INT_MAX) {
			r_subtree_route += "l";
			R_STree_Route(cur_idx * 2 + 1);
		}
		if (t[cur_idx * 2] != INT_MAX) {
			r_subtree_route += "r";
			R_STree_Route(cur_idx * 2);
		}
	}
	void double_1Darray_size() {
		int* new_array = new int[capacity * 2 + 1]();//+1是因為t[0]棄置不用
		memcpy(new_array + 1, t + 1, sizeof(int)*capacity);
		fill(new_array + capacity + 1, new_array + capacity * 2 + 1, INT_MIN);
		delete[]t;
		t = new_array;
		
		capacity *= 2;
	}


	string pre, in, post; int post_idx;
	int height;	int weight_sum;	int maximum_path_sum; int binary_tower;
	string l_subtree_route, r_subtree_route; bool foldable;
	int max_idx;

	bool flag_computed_traverse;
	bool flag_computed_h_ws_mps;//height, weightsum, maximumpathsum
	bool flag_computed_foldable;
	bool flag_computed_binarytower;

	int *t;
	int capacity;
	
};





int main() {
	//while (true) {
		Tree T;

		string cmd;
		while (cin >> cmd) {
			//if (cmd == "End") break;//他就算給我End，我就直接晃到else那邊去，反正程式也不會建造一棵新的樹。Why? 你去看Construct_original的寫法if-elif-elif之後完全沒有else就知道了。

			if (cmd == "Traverse") {
				if (!T.flag_computed_traverse) T.recalculate_traverse();
				cout << T.pre << endl << T.in << endl << T.post << endl;
			}
			else if (cmd == "Height") {
				if (!T.flag_computed_h_ws_mps) T.recalculate_h_ws_mps();
				cout << T.height << endl;
			}
			else if (cmd == "WeightSum") {
				if (!T.flag_computed_h_ws_mps) T.recalculate_h_ws_mps();
				cout << T.weight_sum << endl;
			}
			else if (cmd == "MaximumPathSum") {
				if (!T.flag_computed_h_ws_mps) T.recalculate_h_ws_mps();
				cout << ((T.maximum_path_sum == INT_MIN) ? 0 : T.maximum_path_sum) << endl;
			}
			else if (cmd == "BinaryTower" || cmd=="B") {
				if (!T.flag_computed_binarytower) {
					int root_status = T.recalculate_binarytower();
					if (!root_status)T.binary_tower++;
				}
				cout << T.binary_tower << endl;
			}
			else if (cmd == "Foldable") {
				if (!T.flag_computed_foldable) T.recalculate_foldable();
				cout << ((T.foldable) ? "Yes" : "No") << endl;
			}
			else if (cmd == "DeleteLeaf") {
				T.DeleteLeaf();
			}
			else {//建構樹的地方在這邊
				T.rebirth();
				T.Construct_original(cmd);
			}
		}
	//}





	//system("pause");
}



/*
 *Foldable仍是錯的
 *
 * 
 */
/*
 (-1(-2(-4()())(-5()()))(-3(-6()())(-7()())))
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
End
(1(2()(3()()))(4()()))
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
End
 (-1(-2(-4()())(-5()()))(-3(-6()())(-7()())))
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
End
(1(2()(-3()()))(4()()))
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
End
(1(2()(3(4()())()))(5(6()(7()()))()))
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
Traverse
Height
WeightSum
MaximumPathSum
BinaryTower
Foldable
DeleteLeaf
End
 */


//class Solution {
//private:
//	// camera count
//	int cc;
//	/*
//		this function recursively makes sure all
//		sub trees are surveilled. It returns the status of
//		the root:
//			0: root is not under surveillance
//			1: root is under surveillance but doesn't have camera
//			2: root has camera
//	*/
//	int dfs(TreeNode *root)
//	{
//		// if root doesn't exist, it is surveilled
//		if (!root) return 1;
//		// ints to store status of children
//		int l = 0, r = 0, prod;
//		// surveil children's subtrees
//		l = dfs(root->left);
//		r = dfs(root->right);
//		// utility varialble to make code shorter
//		prod = l * r;
//		// if none of the children are under surveillance
//		// root must have a camera
//		if (!prod)
//		{
//			++cc;
//			return 2;
//		}
//		// if both the children are under surveillance
//		// but don't have cameras, root is not under surveillance
//		if (prod == 1) return 0;
//		// at least one of the children has a camera
//		// so root is surveilled and doesn't have camera
//		else return 1;
//	}
//public:
//	Solution() : cc(0) {}
//	int minCameraCover(TreeNode* root)
//	{
//		// if root is not under surveillance 
//		// need to add camera to root
//		if (!dfs(root)) ++cc;
//		// return number of cameras
//		return cc;
//	}
//};
//想法來自leetcode: https://tg.pe/xkhV or https://ppt.cc/fUKiYx or http://t.cn/A6G0dh80 or https://is.gd/EbWGKp or https://pse.is/38un8s