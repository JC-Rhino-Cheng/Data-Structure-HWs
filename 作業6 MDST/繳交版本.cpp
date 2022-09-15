#include <iostream>
#include <string>
#include <climits>
#include <algorithm>
#define MAX 100000
#define MAX_NUM_OF_VERTICES 100

//以下global variable的設計是不得已而為之，因為每個function都會用到

//計算一個「以i為起點、以j為終點的edge」的某一個動點p的ecc。此動點p滿足是個此edge上最小的最大的shortest path。
//如果這個動點p的ecc，比現有的(aka跟其他edge的比起來)current_radius_aka_ecc_min還小，就會把current_radius_aka_ecc_min更新，而且也會把動點p的位置記錄起來，可能是在edge內部，也可能是端點。
//如果是在edge內部，就把AC_1、AC_2分別設為i、j，而且還要更新此p點到i以及到j兩端點的距離分別是多少，分別給AC_dis_1和AC_dis_2，這樣dijkstra才能算距離。如果動點p是端點，則因為有可能是i也有可能是j，所以要判斷一下，找出正確的那一個之後設給AC_1，至於AC_2就會是-1。
//而事實上，實作的細節是先把AC_1和AC_2都唯一確定以後，判斷如果AC_2是-1代表動點p在端點。
void ecc_finding(const int i, const int j);
//簡單跑Floyd_Warshall
void Floyd_Warshall();
//Kariv_Hakimi負責跑r二維陣列的排序，以及把所有edge用for迴圈跑過一遍。回傳值是T_or_F，T表示是connected graph，F則否。
bool Kariv_Hakimi();
//在跑過Kariv_Hakimi之後，得到了AC(a.k.a. Absolute Centre)。如果需要更進一步，求從AC到各個點的SingleSource-N-AllDestination的距離的話，需要先跑Dijkstra才知道。有跑這個，才能對付題目的SOSPD。
void Dijkstra();


int shortest_path[MAX_NUM_OF_VERTICES + 1][MAX_NUM_OF_VERTICES + 1];//Floyd_Warshall跑完之後就會完成的陣列。shortest_path[m][n]紀錄的是從m到n的最短距離
double shortestpath_from_AC_to[MAX_NUM_OF_VERTICES + 1];//Dijkstra跑完之後才會完成的陣列。shortestpath_from_AC_to[q]紀錄的是，從AC點(可能在edge內部!)到q點的shortestpath。題目的SOSPD會用到。
int r[MAX_NUM_OF_VERTICES + 1][MAX_NUM_OF_VERTICES + 1];//r二維陣列，在MDST教授ppt的step2和step3會用到
double SHUNIU_current_radius_aka_ecc_min;//每個edge找到自己的ecc之後，都會檢查一遍。如果新找到的ecc比current_radius_aka_ecc_min還小，就會更新current_radius_aka_ecc_min成新找到的值
int SHUNIU_AC_1, SHUNIU_AC_2;//紀錄AC的位置，如果在端點就會是(SHUNIU_AC_1, SHUNIU_AC_2) = (???, -1)，但如果是在(i, j)這條edge的中間，就會記錄為(SHUNIU_AC_1, SHUNIU_AC_2) = (i, j)
double SHUNIU_AC_dis_1, SHUNIU_AC_dis_2;//分別紀錄AC到i端點以及到j端點的距離，這樣在跑dijkstra的時候才有辦法算SingleSource-N-AllDestination的距離。

int n; int cost[110][110];//儲存題目graph的資訊

int main() {
	const std::string Error_to_find_AC = "Not connected graph";

	
	std::cin >> n;
	for (auto i = 0; i < n; i++)for (auto j = 0; j < n; j++)cost[i][j] = ((i == j) ? 0 : INT_MIN);//(x, y)座標一樣代表的是同一個點做為edge的起迄，所以cost一定只能是0。其餘的edge的cost未知，交由使用者去用Add或Delete修改。

	std::string cmd;
	while (std::cin >> cmd) {
		if (cmd == "Add") {
			int v1, v2, w; std::cin >> v1 >> v2 >> w;

			if (v1 == v2)continue;//不能允許使用者亂改，把本來應該明明是同一個點做為起訖所以cost是0，卻亂改。
			cost[v1][v2] = cost[v2][v1] = w;
		}
		else if (cmd == "Delete") {
			int v1, v2; std::cin >> v1 >> v2;

			if (v1 == v2)continue;//不能允許使用者亂改，把本來應該明明是同一個點做為起訖所以cost是0，卻亂改。
			cost[v1][v2] = cost[v2][v1] = INT_MIN;
		}
		//以上Add和Delete兩個函式很簡單

		//以下三個功能(AC、Diameter、SOSPD，每呼叫一次，都要視為新的一題，所以都會先把current_radius_aka_ecc_min初始化為MAX，再算Floyd_Warshall以及Kariv_Hakimi。其實AC_1、AC_2也都要初始化為-1才對，只不過因為是在Kariv_Hakimi裡面改動所以你沒在這裡看到)
		else if (cmd == "AC") {
			SHUNIU_current_radius_aka_ecc_min = MAX;
			Floyd_Warshall();

			if (n == 1) std::cout << 0 << std::endl;//記得考慮只有一個vertex的情況，助教有說如果只有一個vertex則編號必為0，所以可以安心輸出0。
			else if (Kariv_Hakimi()) {//Kariv_Hakimi負責跑r二維陣列的排序，以及把所有edge用for迴圈跑過一遍。跑完之後，r就完工了，而且AC_1、SHUNIU_AC_2、SHUNIU_AC_dis_1、AC_dis_2也都一同完工。回傳值是T_or_F，T表示是connected graph，F則否。
				if (SHUNIU_AC_2 == -1) std::cout << SHUNIU_AC_1 << std::endl;
				else std::cout << SHUNIU_AC_1 << " " << SHUNIU_AC_2 << std::endl;
			}
			else std::cout << Error_to_find_AC << std::endl;
		}
		else if (cmd == "Diameter") {
			SHUNIU_current_radius_aka_ecc_min = MAX;
			Floyd_Warshall();

			if (n == 1) std::cout << 0 << std::endl;//記得考慮只有一個vertex的情況。如果只有一個vertex，則diameter必為0。
			else if (Kariv_Hakimi()) {//Kariv_Hakimi負責跑r二維陣列的排序，以及把所有edge用for迴圈跑過一遍。跑完之後，r就完工了，而且AC_1、SHUNIU_AC_2、SHUNIU_AC_dis_1、AC_dis_2也都一同完工。回傳值是T_or_F，T表示是connected graph，F則否。
				std::cout << 2 * SHUNIU_current_radius_aka_ecc_min << std::endl;
			}
			else std::cout << Error_to_find_AC << std::endl;
		}
		else if (cmd == "SOSPD") {
			SHUNIU_current_radius_aka_ecc_min = MAX;
			Floyd_Warshall();

			if (n == 1) std::cout << 0 << std::endl;//記得考慮只有一個vertex的情況。如果只有一個vertex，則diameter必為0，所以SingleSource-N-AllDestination全部加起來一定還是0。
			else if (Kariv_Hakimi()) {//Kariv_Hakimi負責跑r二維陣列的排序，以及把所有edge用for迴圈跑過一遍。跑完之後，r就完工了，而且AC_1、SHUNIU_AC_2、SHUNIU_AC_dis_1、AC_dis_2也都一同完工。回傳值是T_or_F，T表示是connected graph，F則否。
				Dijkstra();//在跑過Kariv_Hakimi之後，得到了AC。如果需要更進一步，求從AC到各個點的SingleSource-N-AllDestination的距離的話，需要先跑Dijkstra才知道。

				auto SOSPD = .0; //因為是"."0，所以auto的結果是double
				for (const auto &i : shortestpath_from_AC_to) SOSPD += i;//把所有從AC開始計算的shortestpath累加起來即可。
				std::cout << SOSPD << std::endl;
			}
			else std::cout << Error_to_find_AC << std::endl;
		}
	}


	//system("pause");
}

//就是按照課本的寫法把所有vertex做為source，到AllDestination的shortestpath都算出來
void Floyd_Warshall() {
	for (auto& i : shortest_path) for (auto& j : i) j = MAX;//預設值都先弄成最大，如果算出來比較小，再用小的值取代。這一句是「預設值都先弄成最大」的part

	for (auto i = 0; i < n; i++)  for (auto j = 0; j < n; j++) {
		if (i == j) shortest_path[i][j] = 0;
		else if (cost[i][j] != INT_MIN && cost[i][j] != 0/*白話文: 如果i和j兩個vertex之間有直接的edge，而且這條edge的起訖是不同vertex的話(如果起訖相同的話，在上面的if就處理過了)*/) shortest_path[i][j] = cost[i][j];
	}



	for (auto k = 0; k < n; k++) {
		for (auto i = 0; i < n; i++) for (auto j = 0; j < n; j++) {
			if (shortest_path[i][k] + shortest_path[k][j] < shortest_path[i][j]) shortest_path[i][j] = shortest_path[i][k] + shortest_path[k][j];//預設值都先弄成最大，如果算出來比較小，再用小的值取代。這一句是「如果算出來比較小，再用小的值取代」的part
		}
	}
}


//Kariv_Hakimi負責跑r二維陣列的排序，以及把所有edge用for迴圈跑過一遍。回傳值是T_or_F，T表示是connected graph，F則否。
bool Kariv_Hakimi() {
	//還記得line53說什麼嗎? ----->以下三個功能(AC、Diameter、SOSPD，每呼叫一次，都要視為新的一題，所以都會先把current_radius_aka_ecc_min初始化為MAX，再算Floyd_Warshall以及Kariv_Hakimi。其實AC_1、AC_2也都要初始化為-1才對，只不過因為是在Kariv_Hakimi裡面改動所以你沒在這裡看到)
	SHUNIU_AC_1 = -1, SHUNIU_AC_2 = -1;

	for (auto i = 0; i < n; i++)for (auto j = 0; j < n; j++) r[i][j] = j;//先把r二維陣列出處化為每個row都是0, 1, 2, 3, 4, 5, .......

	//這個大for迴圈就是在做bubble sort排序，依照shortest_path的大小順序排列好
	for (auto i = 0; i < n; i++) {
		for (auto k = n - 1 - 1; k >= 0; k--) {
			for (auto j = 0; j <= k; j++) {
				if (shortest_path[i][j] == MAX/*如果一個graph是connected，代表任意兩個不同vertex抓出來，一定要能夠攀得上關係!如果有任何一個攀不上關係(a.k.a. shortest_path[i][j]還是維持初始化過的的MAX值)，就代表不是connected*/)return false;

				if (shortest_path[i][r[i][j]] < shortest_path[i][r[i][j + 1]]) std::swap(r[i][j], r[i][j + 1]);
			}
			if (shortest_path[i][k + 1] == MAX)return false;/*如果一個graph是connected，代表任意兩個不同vertex抓出來，一定要能夠攀得上關係!如果有任何一個攀不上關係(a.k.a. shortest_path[i][j]還是維持初始化過的的MAX值)，就代表不是connected*///這句是在補檢查漏網之魚
		}
	}

	for (auto i = 0; i < n; i++) for (auto j = 0; j < n; j++) {
		if (cost[i][j] != INT_MIN/*只要存在有(i, j)這個edge的話*/) ecc_finding(i, j);//就計算這個edge裡面的所有動點p的ecc，如果有最小ecc的動點p是可以取代現有current_radius_aka_ecc_min的動點p的話，就取代並且記錄p的位置
	}

	return true;
}


//計算一個「以i為起點、以j為終點的edge」的某一個動點p的ecc。此動點p滿足是個此edge上最小的最大的shortest path。
//如果這個動點p的ecc，比現有的(aka跟其他edge的比起來)current_radius_aka_ecc_min還小，就會把current_radius_aka_ecc_min更新，而且也會把動點p的位置記錄起來，可能是在edge內部，也可能是端點。
//如果是在edge內部，就把AC_1、AC_2分別設為i、j，而且還要更新此p點到i以及到j兩端點的距離分別是多少，分別給AC_dis_1和AC_dis_2，這樣dijkstra才能算距離。如果動點p是端點，則因為有可能是i也有可能是j，所以要判斷一下，找出正確的那一個之後設給AC_1，至於AC_2就會是-1。
//而事實上，實作的細節是先把AC_1和AC_2都唯一確定以後，判斷如果AC_2是-1代表動點p在端點。
void ecc_finding(const int i, const int j) {
	
	//先把這條edge目前找到的ecc設為最大，如果這條edge的之後檢查的交點有較小的ecc，再更新為較小的那個。
	//等到這條edge的ecc確定已經是最小之後，再跟目前所有edge的樞紐ecc紀錄者比較到底誰比較小，如果樞紐紀錄者比這條還大，就把樞紐紀錄者更新。
	//樞紐紀錄者叫做current_radius_aka_ecc_min，還記得嗎?
	double cur_edge_cur_ecc = MAX;
	
	auto cur_edge_AC_1 = -1, cur_edge_AC_2 = -1;//和上面的cur_edge_cur_ecc相同的概念，而樞紐紀錄者叫做AC_1, SHUNIU_AC_2
	auto cur_edge_AC_dis_1 = .0, cur_edge_AC_dis_2 = .0; //因為是"."0，所以auto的結果是double //和上面的cur_edge_cur_ecc相同的概念，而樞紐紀錄者叫做AC_dis_1, SHUNIU_AC_dis_2

	//初始化好之後，接下來就是漫長的找尋有著最小的最大的shortestpath的動點p的旅程。
	if (shortest_path[i][r[i][0]] < shortest_path[j][r[j][0]]) {//距離i最遠的人和i之間的距離，比，距離j最遠的人和j之間的距離，還小。這暗示著，是ppt裡面P18的中間圖，或者左邊圖(圖形peak靠近j點)的情況。
		cur_edge_cur_ecc = shortest_path[i][r[i][0]];
		cur_edge_AC_1 = i;
	}
	else {//距離i最遠的人和i之間的距離，比，距離j最遠的人和j之間的距離，還大。這暗示著，是ppt裡面P18的右邊圖，或者左邊圖(圖形peak靠近i點)的情況。
		cur_edge_cur_ecc = shortest_path[j][r[j][0]];
		cur_edge_AC_1 = j;
	}
	//上面的if-else是先預設把動點p設在較可能的某一個尖端側。由較少的尖端往可能交點數量較多的圖中央來慢慢找真正的ecc，會比從中間往兩側找來得容易。

	//上面已經搞好較少的尖端，所以現在開始往可能交點數量較多的圖中央來慢慢找真正的ecc，跟課本的code是一樣的
	for (auto a = 0, b = 1; b < n; b++) {
		if (shortest_path[j][r[i][a]] < shortest_path[j][r[i][b]]) {//由code的設計，知道a一定比b小，所以r[i][a]、r[i][b]的功用是先用i來找到兩個不同的距離的點，r[i][a]這個點和i的距離，比r[i][b]這個點和i的距離還遠。i和"a"所代表的點比較遠，所以如果j和這個比較遠的點反而比較近，代表這兩條線之間有交點。看不懂的話，回去看ppt仔細體會。
			if (cur_edge_cur_ecc > (static_cast<double>(shortest_path[i][r[i][b]]) + static_cast<double>(shortest_path[j][r[i][a]]) + static_cast<double>(cost[i][j])) / 2) {
				//如果目前這條edge的最小ecc的紀錄者比目前找到的ecc還大，代表我需要更新紀錄了，連帶的也要把"這條edge"上紀錄的AC的位置以及這條edge上的AC點距離這條edge的兩端的距離都要更新。
				cur_edge_cur_ecc = (static_cast<double>(shortest_path[i][r[i][b]]) + static_cast<double>(shortest_path[j][r[i][a]]) + static_cast<double>(cost[i][j])) / 2;
				cur_edge_AC_1 = i;
				cur_edge_AC_2 = j;
				cur_edge_AC_dis_1 = cur_edge_cur_ecc - static_cast<double>(shortest_path[i][r[i][b]]);
				cur_edge_AC_dis_2 = cur_edge_cur_ecc - static_cast<double>(shortest_path[j][r[i][a]]);

			}
			a = b;
		}
	}


	//上面終於把這條edge的ecc找到了，接下來這個紀錄要跟樞紐紀錄者比較，如果樞紐紀錄者比這個紀錄還大，樞紐紀錄者就要被更新
	if (SHUNIU_current_radius_aka_ecc_min > cur_edge_cur_ecc) {
		SHUNIU_current_radius_aka_ecc_min = cur_edge_cur_ecc;
		SHUNIU_AC_1 = cur_edge_AC_1;
		SHUNIU_AC_2 = cur_edge_AC_2;

		if (SHUNIU_AC_2 != -1) {
			SHUNIU_AC_dis_1 = cur_edge_AC_dis_1;
			SHUNIU_AC_dis_2 = cur_edge_AC_dis_2;
		}
	}

}


void Dijkstra() {
	for (auto i = 0; i < n; i++) shortestpath_from_AC_to[i] = MAX;

	bool visited[MAX_NUM_OF_VERTICES + 1];	std::fill(visited, visited + MAX_NUM_OF_VERTICES + 1, false);

	//要分成AC到底在edge中間還是edge端點來考慮
	//先看在edge中間
	if (SHUNIU_AC_2 != -1) {
		//因為AC有可能在edge中間，所以shortest_path_from_AC_to得先把AC往edge兩端的距離先初始化，這樣才有辦法計算SingleSource-N-AllDestination的距離
		shortestpath_from_AC_to[SHUNIU_AC_1] = SHUNIU_AC_dis_1;
		shortestpath_from_AC_to[SHUNIU_AC_2] = SHUNIU_AC_dis_2;

		//接下來的for迴圈的每個iteration，就是在還沒走過的眾多edge中，挑一條累加起來距離最小的來走
		for (auto i = 0; i <= n; i++) {
			//這個for是在把所有還沒走過的edge，的累加的距離做更新，並且順便找到累加距離最小的是誰(紀錄他的ID為ID_of_next_vertex_to_visit)
			auto ID_of_next_vertex_to_visit = 0;//紀錄目前造成累加距離最小的是哪個vertex
			double shortest = MAX;//紀錄目前累加距離最小，的距離是多少
			for (auto j = 0; j < n; j++) {
				if (!visited[j] && shortestpath_from_AC_to[j] < shortest) {
					shortest = shortestpath_from_AC_to[j];
					ID_of_next_vertex_to_visit = j;
				}
			}
			//上面的j的for迴圈結束後，代表找到下一個要拜訪的目標了，所以劃記為已經拜訪。
			visited[ID_of_next_vertex_to_visit] = true;
			//拜訪完之後，到達了新的vertex，所以現在要以這個新vertex為中心，把shortestpath_from_AC_to一維陣列更新，這樣下次才能再用這個陣列的資訊挑選要走的再下一個目標是誰。
			for (auto j = 0; j < n; j++) {
				if (!visited[j] && shortestpath_from_AC_to[ID_of_next_vertex_to_visit] + static_cast<double>(cost[ID_of_next_vertex_to_visit][j]) < shortestpath_from_AC_to[j] && cost[ID_of_next_vertex_to_visit][j] > 0)
					shortestpath_from_AC_to[j] = shortestpath_from_AC_to[ID_of_next_vertex_to_visit] + static_cast<double>(cost[ID_of_next_vertex_to_visit][j]);
			}

		}

	}
	//再看在edge端點
	else {
		//在端點上的就變得很簡單，就跟課本code一模一樣，基本上也可以想成是上面的在edge中間的簡單化
		for (auto i = 0; i < n; i++) {
			if (cost[SHUNIU_AC_1][i] > 0) shortestpath_from_AC_to[i] = cost[SHUNIU_AC_1][i];
		}

		shortestpath_from_AC_to[SHUNIU_AC_1] = 0;
		visited[SHUNIU_AC_1] = true;


		for (auto i = 0; i < n - 1; i++) {
			auto ID_of_next_vertex_to_visit = 0;

			double shortest = MAX;
			for (auto j = 0; j < n; j++) {
				if (!visited[j] && shortestpath_from_AC_to[j] < shortest) {
					shortest = shortestpath_from_AC_to[j];
					ID_of_next_vertex_to_visit = j;
				}
			}

			visited[ID_of_next_vertex_to_visit] = true;

			for (auto j = 0; j < n; j++) {
				if (!visited[j] && shortestpath_from_AC_to[ID_of_next_vertex_to_visit] + static_cast<double>(cost[ID_of_next_vertex_to_visit][j]) < shortestpath_from_AC_to[j] && cost[ID_of_next_vertex_to_visit][j] > 0)
					shortestpath_from_AC_to[j] = shortestpath_from_AC_to[ID_of_next_vertex_to_visit] + static_cast<double>(cost[ID_of_next_vertex_to_visit][j]);
			}

		}
	}

}




/*
5
Add 0 1 3
Add 1 2 4
Add 1 4 9
Add 2 3 3
Add 3 4 7
AC
Diameter
SOSPD
Delete 0 1
AC




5 
Add 0 1 3
Add 1 2 4
Add 1 4 9
Add 4 3 7
Add 2 3 3
AC

*/