#include <iostream>
#include <climits>
#include <deque>
#include <algorithm>

void Merge(int *initList, int *mergedList, const int l, const int m, const int n) {
	auto i1 = l, iResult = l, i2 = m + 1;
	for (; i1 <= m && i2 <= n; iResult++) {
		if (initList[i1] <= initList[i2]) {
			mergedList[iResult] = initList[i1];
			i1++;
		}
		else {
			mergedList[iResult] = initList[i2];
			i2++;
		}
	}

	std::copy(initList + i1, initList + m + 1, mergedList + iResult);
	std::copy(initList + i2, initList + n + 1, mergedList + iResult);
}



void MergePass(int *initList, int *resultList, const int n, const int s) {
	auto i = 1;
	for (; i <= n - 2 * s + 1; i += 2 * s) Merge(initList, resultList, i, i + s - 1, i + 2 * s - 1);

	if (i + s - 1 < n) Merge(initList, resultList, i, i + s - 1, n);
	else std::copy(initList + i, initList + n + 1, resultList + i);
}



void MergeSort(int *a, const int n) {
	auto *tempList = new int[n + 1];

	for (auto l = 1; l <= n; l *= 2) {
		MergePass(a, tempList, n, l);
		l *= 2;
		MergePass(tempList, a, n, l);
	}

	delete[]tempList;
}



int FindIndex(int *array, const float goal_num, int left = 1, int right = INT_MAX) {//若陣列有效範圍在1~9，則left接收1，right接收9
	if (right == INT_MAX) return 0;
	if (array[right] < goal_num) return right;
	if (array[left] > goal_num) return 0;


	auto original_right = right;
	while (left <= right) {
		if (left == right) {
			if (left == 1) return 0;
			else return left;
		}


		auto left_prev = left, right_prev = right;
		auto middle = (left + right) / 2;
		if (array[middle] == goal_num) return middle - 1;
		else if (array[middle] < goal_num) left = middle;
		else if (array[middle] > goal_num) right = middle;

		if (left_prev == left && right_prev == right) return left;
	}
}



int MoM(int *array, const int array_length, const int k) {//記得array是1-based! array[0]無效，array[1~n]有效。
	array += 1;
	const auto divisible = array_length % 5 == 0; const auto mod = array_length % 5;
	const auto num_seg = (divisible) ? (array_length / 5) : ((array_length / 5) + 1);

	int **two_D_array = new int*[num_seg];
	int *median = new int[num_seg + 1]; int it_for_median = 0;
	median[it_for_median++] = 0;
	for (auto i = 0; i < num_seg; i++) {
		two_D_array[i] = new int[5 + 1]; two_D_array[i][0] = 0;

		if (i != num_seg - 1) {
			std::copy(array + i * 5, array + i * 5 + 5, two_D_array[i] + 1);
			MergeSort(two_D_array[i], 5);
			median[it_for_median++] = two_D_array[i][3];
		}
		else {
			if (divisible) {
				std::copy(array + i * 5, array + i * 5 + 5, two_D_array[i] + 1);
				MergeSort(two_D_array[i], 5);
				median[it_for_median++] = two_D_array[i][3];
			}
			else {
				std::copy(array + i * 5, array + i * 5 + mod, two_D_array[i] + 1);
				MergeSort(two_D_array[i], mod);
				median[it_for_median++] = two_D_array[i][(mod + 1) / 2];
			}
		}
	}

	MergeSort(median, it_for_median - 1);
	const auto True_Median = median[(it_for_median - 1) / 2];
	delete [] median;

	auto result_for_when_arrayLength_is_small_enough = 0;
	if (array_length <= 5) result_for_when_arrayLength_is_small_enough = two_D_array[0][k];


	std::deque<int>median_found; median_found.push_back(True_Median); auto median_idx_in_median_found = 0;
	for (auto i = 0; i < num_seg; i++) {
		if (i != num_seg - 1) {
			for (auto j = 1; j < 6; j++) {
				if (two_D_array[i][j] < True_Median) {
					median_found.push_front(two_D_array[i][j]);
					median_idx_in_median_found++;
				}
				else if (two_D_array[i][j] > True_Median)median_found.push_back(two_D_array[i][j]);
			}
		}
		else {
			if (divisible) {
				for (auto j = 1; j < 6; j++) {
					if (two_D_array[i][j] < True_Median) {
						median_found.push_front(two_D_array[i][j]);
						median_idx_in_median_found++;
					}
					else if (two_D_array[i][j] > True_Median) median_found.push_back(two_D_array[i][j]);
				}
			}
			else {
				for (auto j = 1; j < mod + 1; j++) {
					if (two_D_array[i][j] < True_Median) {
						median_found.push_front(two_D_array[i][j]);
						median_idx_in_median_found++;
					}
					else if (two_D_array[i][j] > True_Median) median_found.push_back(two_D_array[i][j]);
				}
			}
		}

		delete [] two_D_array[i];
	}
	delete [] two_D_array;
	median_found.push_front(0); median_idx_in_median_found++;
	array -= 1;

	for (auto i = 0; i <= array_length; i++) array[i] = median_found[i];


	if (array_length <= 5) return result_for_when_arrayLength_is_small_enough;
	else if (median_idx_in_median_found == k) return True_Median;//因為上面那行會return，所以這行不會用到。<-不對，還是會用到，就是在100 53 100 99 98 97 96 95 94 93......的測資的時候
	else if (median_idx_in_median_found < k) return MoM(array + median_idx_in_median_found, array_length - median_idx_in_median_found, k - median_idx_in_median_found);
	else if (median_idx_in_median_found > k) return MoM(array, median_idx_in_median_found - 1, k);







	////Debug用
	//for (auto i = 0; i < num_seg; i++) {
	//	if (i != num_seg - 1) {
	//		for (auto j =1 ; j < 6; j++) {
	//			cout << two_D_array[i][j] << " ";
	//		}
	//		cout << endl;
	//	}
	//	else {
	//		for (auto j = 1; j < ((divisible)? 6:mod+1); j++) {
	//			cout << two_D_array[i][j] << " ";
	//		}
	//		cout << endl;
	//	}
	//	
	//}





	//return 0;
}







int Zhangs_MoM(int *arr, const int arr_length, const int k) {
	if (arr_length == 1 && k == 0) return arr[0];


	const auto num_seg = (arr_length + 4) / 5;
	int* medium_array = new int[num_seg];
	for (auto i = 0; i < num_seg; i++) {
		if (i * 5 + 4 < arr_length) {
			for (auto j = 0; j < 5; j++) {
				for (auto k = 0; k < 4; k++) {
					if (arr[i * 5 + j] < arr[i * 5 + k]) std::swap(arr[i * 5 + j], arr[i * 5 + k]);
				}
			}
			medium_array[i] = arr[i * 5 + 2];
		}
		else medium_array[i] = arr[i * 5];
	}


	const auto Medium_of_Medium = Zhangs_MoM(medium_array, num_seg, num_seg / 2);
	delete [] medium_array;


	
	for (auto i = 0; i < arr_length; i++) {
		if (arr[i] == Medium_of_Medium) {
			std::swap(arr[i], arr[arr_length - 1]);
			break;
		}
	}
	auto Num_of_nums_greater_than_MediumOfMedium = 0;
	for (auto i = 0; i < arr_length - 1; i++) if (arr[i] > Medium_of_Medium) std::swap(arr[i], arr[Num_of_nums_greater_than_MediumOfMedium++]);
	std::swap(arr[Num_of_nums_greater_than_MediumOfMedium], arr[arr_length - 1]);


	
	if (Num_of_nums_greater_than_MediumOfMedium + 1 == k) return Medium_of_Medium;
	else if (k < Num_of_nums_greater_than_MediumOfMedium + 1) return Zhangs_MoM(arr, Num_of_nums_greater_than_MediumOfMedium, k);
	else return Zhangs_MoM(arr + Num_of_nums_greater_than_MediumOfMedium + 1, arr_length - Num_of_nums_greater_than_MediumOfMedium - 1, k - Num_of_nums_greater_than_MediumOfMedium - 1);
}

















//10 2
//17 7 1 5 18 14 12 15 11 10
//
//6 5
//15 8 13 7 4 18
//
//9 4
//8 12 17 18 5 14 10 9 11
//
//4 2
//1 2 3 4
//
//5 3
//5 4 3 2 1
//
//4 1
//9 4 5 3
//
//100 53
//1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100
//
//100 53
//100 99 98 97 96 95 94 93 92 91 90 89 88 87 86 85 84 83 82 81 80 79 78 77 76 75 74 73 72 71 70 69 68 67 66 65 64 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1
//
//10 2
//25 21 98 100 76 22 43 60 89 87
//
//正解應該是:
//17
//7
//12
//3
//3
//9
//48
//48
//98


int Zhangs_QQpair(int *arr, const int l, const int r) {
	if (l >= r) return 0;


	const auto m = (l + r) / 2;
	const auto left_QQ = Zhangs_QQpair(arr, l, m);
	const auto right_QQ = Zhangs_QQpair(arr, m + 1, r);

	auto *Result_after_Merging = new int[r - l + 1];
	auto Combined_QQ_ToBe_Added = 0;
	auto i = l, j = m + 1;
	auto idx_for_cur_merging_array = 0, idx_of_first_element_to_meet_the_condition = i;
	while (i <= m && j <= r) {
		if (arr[i] <= arr[j]) Result_after_Merging[idx_for_cur_merging_array++] = arr[i++];
		else {
			Result_after_Merging[idx_for_cur_merging_array++] = arr[j++];
			for (; idx_of_first_element_to_meet_the_condition <= m; idx_of_first_element_to_meet_the_condition++) if (arr[idx_of_first_element_to_meet_the_condition] > 2 * arr[j - 1]) break;
			Combined_QQ_ToBe_Added += m + 1 - idx_of_first_element_to_meet_the_condition;
		}
	}
	while (i <= m) Result_after_Merging[idx_for_cur_merging_array++] = arr[i++];
	while (j <= r) Result_after_Merging[idx_for_cur_merging_array++] = arr[j++];


	
	for (auto i = l; i <= r; i++) arr[i] = Result_after_Merging[i - l];
	delete[]Result_after_Merging;

	return left_QQ + right_QQ + Combined_QQ_ToBe_Added;
}




/*
10 0
17 7 1 5 18 14 12 15 11 10

6 3
15 8 13 7 4 18

9 1
8 12 17 18 5 14 10 9 11

4 0
1 2 3 4

5 0
5 4 3 2 1

4 0
9 4 5 3

100 0
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100

100 0
100 99 98 97 96 95 94 93 92 91 90 89 88 87 86 85 84 83 82 81 80 79 78 77 76 75 74 73 72 71 70 69 68 67 66 65 64 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1


正解應該是:
4
13
18
0
4
2
0
2450
 */









int main() {
	int n, op;
	while (std::cin >> n >> op) {
		auto *array = new int[n + 1];
		for (auto i = 1; i <= n; i++) std::cin >> array[i];

		if (op == 0) {//MergeSort進行QQ
			/*auto count = 0;

			for (auto i = 1; i < n; i++) {
				auto *temp = new int[n - i + 1];
				copy(array + 1 + i, array + 1 + n, temp + 1);
				MergeSort(temp, n - i);

				const auto idx = FindIndex(temp, (static_cast<float>(array[i]) / 2), 1, n - i);
				count += idx;
			}

			cout << count << endl;*/
			std::cout << Zhangs_QQpair(array + 1, 0, n - 1) << std::endl;
		}
		else {//Median- of-Median進行K's
			auto k = n + 1 - op;

			//cout << MoM(array, n, k) << endl;
			std::cout << Zhangs_MoM(array + 1, n, op) << std::endl;
		}



		delete [] array;
	}
}