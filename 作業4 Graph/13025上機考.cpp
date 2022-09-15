#include <iostream>
#include <string>
#include <queue>
#include <memory.h>
#include <climits>
#include<iomanip>
using namespace std;

class node {//id儲存城市編號; cur_probability儲存的是，目前這條以目前id做為最新蒐集到的點(最新挖到的礦)的路線，累積的機率; pre_air_id儲存的是上個航空公司的id
public:
	node(int id = 0, float probability = 0) :id(id), cur_probability(probability) {
		memset(visited, false, sizeof(visited));
	}
	int id;
	float cur_probability;
	bool visited[101];
};

int main() {
	float probability[101][101]; memset(probability, 0, sizeof(probability));
	
	int n, m, r; cin >> n >> m >> r;

	for (auto i = 0; i < m; i++) {
		int a, b; float p; cin >> a >> b >> p;

		probability[a][b] = probability[b][a] = p;
	}

	for (auto i = 0; i < r; i++) {
		int start, end; cin >> start >> end;

		queue <node> Q;
		node first(start, 1);
		Q.push(first);

		float ans = 0;
		bool flag_exists_path = false;
		while (!Q.empty()) {
			flag_exists_path = true;
			node cur = Q.front(); Q.pop();

			if (cur.id == end) {
				if (cur.cur_probability > ans) ans = cur.cur_probability;
			}
			else {
				for (int i = 0; i < n; i++) {
					if (probability[cur.id][i]!=0)
						if (cur.cur_probability * probability[cur.id][i] > ans && !cur.visited[i]) {
							node temp;
							temp.cur_probability = cur.cur_probability * probability[cur.id][i];
							memcpy(temp.visited, cur.visited, sizeof(cur.visited)); temp.visited[i] = true;
							temp.id = i;
							Q.push(temp);
						}
				}
			}
		}
		cout << fixed << setprecision(5) << ((flag_exists_path) ? ans : 0.00000) << endl;
	}






}