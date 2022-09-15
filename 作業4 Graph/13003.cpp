#include <iostream>
#include <string>
#include <queue>
#include <climits>
#include <string.h>
constexpr auto MAX_CITIES = 100; //助教保證城市數量最多100個
constexpr auto MAX_AIRLINES = 10; //助教保證航空公司數量最多10個

class Node {
	friend int main();//有加這行的話，int main(){}這個function就可以看到所有class node裡面的東西
	//也可寫成friend auto main() -> int;
private:
	explicit Node(const int city_ID = 0, const int dist = 0, const int prev_air = 0) :city_ID(city_ID), cur_dist(dist), prev_airline(prev_air) {}
	const int city_ID;
	const int cur_dist;
	const int prev_airline;
};

int main() {
	int n; std::cin >> n;

	int dis[MAX_CITIES + 1][MAX_CITIES + 1][MAX_AIRLINES + 1]; memset(dis, -1, sizeof dis);

	std::string cmd;
	while (std::cin >> cmd) {
		if (cmd == "Add") {
			int from, to, dist_between_cur_from_and_cur_to, airline; std::cin >> from >> to >> dist_between_cur_from_and_cur_to >> airline;

			if (dis[from][to][airline] == -1 || dist_between_cur_from_and_cur_to < dis[from][to][airline]) dis[from][to][airline] = dist_between_cur_from_and_cur_to;
		}
		else if (cmd == "Delete") {
			int from, to, airline; std::cin >> from >> to >> airline;

			dis[from][to][airline] = -1;
		}
		else if (cmd == "Request") {
			int start_ID, end_ID, limit; std::cin >> start_ID >> end_ID >> limit;

			std::queue <Node> Q;
			Node start_node(start_ID, 0, -1);
			Q.push(start_node);

			auto least_cost = INT_MAX;

			while (!Q.empty()) {
				Node cur = Q.front();
				Q.pop();

				if (cur.city_ID == end_ID) {
					if (cur.cur_dist < least_cost) least_cost = cur.cur_dist;
				}
				else {
					for (auto i = 0; i < n; i++) {
						for (auto j = 0; j <= 10; j++) { //i、j兩層迴圈，是對這個目前所在的城市的(i: 所有可能出去的城市)、(j: 所有可能出去的航空公司)->i*j的排列組合考慮: 這個目前的城市到第i個城市透過第j個航空公司
							if (dis[cur.city_ID][i][j] != -1) {//如果想從目前城市到第i個城市(並且透過第j個航空公司)，是有路徑可以達到的，就要丟進queue。只不過，因為要考慮轉機的手續費，所以要再分兩個情況考慮
								if (cur.prev_airline == -1 || cur.prev_airline == j) {
									if (cur.cur_dist + dis[cur.city_ID][i][j] <= limit && cur.cur_dist + dis[cur.city_ID][i][j] < least_cost) Q.push(Node(i, cur.cur_dist + dis[cur.city_ID][i][j], j));
								}
								else {
									if (cur.cur_dist + dis[cur.city_ID][i][j] + 5 <= limit && cur.cur_dist + dis[cur.city_ID][i][j] < least_cost) Q.push(Node(i, cur.cur_dist + dis[cur.city_ID][i][j] + 5, j));
								}
							}
						}
					}
				}
			}

			std::cout << ((least_cost <= limit) ? least_cost : -1) << std::endl;
		}
	}
}