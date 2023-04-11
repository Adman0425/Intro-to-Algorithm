#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

using namespace std;

ifstream inFile;
ofstream out;
void getData();
int getSum(vector <int>sum, int l, int r);
int FindFakeCoin(vector <int>data, int l, int r);


void getData(){
	inFile.open("input.txt");/*open input file*/
	out.open("output.txt");
	
	if(!inFile){/*if the file broken*/
		cout<<"Open file failed."<<endl;
		exit(1);
	}

	int num;
	while(inFile >> num){/*how many coins*/
		vector <int> weight;/*create a vector and empty it*/
		weight.clear();
		
		int times = num;
		while(times--){
			int coin;
			inFile >> coin;
			weight.push_back(coin);/*get coins into vector*/
		}
		
		//for(auto i: weight){/*print the vector*/
		//	cout << i << " ";
		//}
		//cout << endl;
		
		int fake_pos = FindFakeCoin(weight, 0, num-1);/*find fake coin*/
		cout << fake_pos << endl;
		out << fake_pos << endl;/*output the file*/
	}
	
	inFile.close();
	out.close();
}

int getSum(vector <int>sum, int l, int r){/*get sum*/
	int i, weightsum = 0;
	for (i = l;i <= r;i++){
		weightsum += sum[i];
	}
	return weightsum;
}

int FindFakeCoin(vector <int>data, int l, int r){
	int n = r-l+1;/*how many coins*/

	if(n==1){
		return l;
	}else if(n==4){
		if(data[l] == data[l+1] == data[l+2] && data[l] != data[l+3]){
			return l+3;
		}else if(data[l] == data[l+1] == data[l+3] && data[l] != data[l+2]){
			return l+2;
		}else if(data[l] == data[l+2] == data[l+3] && data[l] != data[l+1]){
			return l+1;
		}else{
			return l;
		}
	}
	
	int coin_num = ceil(n/3);/*divide into 3 piles and every pile has coin_num coins*/
	int lsum = getSum(data, l, l+coin_num-1);
	int msum = getSum(data, l+coin_num, l+2*coin_num-1);
	int rsum = getSum(data, l+2*coin_num, r);
	
	int ave_lsum = lsum/coin_num;/*get every pile's average to check which pile has fake coin*/
	int ave_msum = msum/coin_num;
	int ave_rsum = rsum/(n-2*coin_num);
	
	if(ave_lsum == ave_msum && ave_lsum != ave_rsum){/*find the "fake" pile*/
		if(n-2*coin_num==2){
			return FindFakeCoin(data, l+2*coin_num-1, r);
		}else{
			return FindFakeCoin(data, l+2*coin_num, r);
		}
	}else if(ave_lsum == ave_rsum && ave_lsum != ave_msum){
		if(coin_num==2){
			return FindFakeCoin(data, l+coin_num-1, l+2*coin_num-1);
		}else{
			return FindFakeCoin(data, l+coin_num, l+2*coin_num-1);
		}
	}else{
		if(coin_num==2){
			return FindFakeCoin(data, l, l+coin_num);
		}else{
			return FindFakeCoin(data, l, l+coin_num-1);
		}
	}
	
}

int main() {
	
	getData();
		
	return 0;
}

