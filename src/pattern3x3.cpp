#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "pattern3x3.h"

using std::cerr;
using std::endl;
using std::string;

double prob_ptn3x3[65536][256][4];
std::unordered_map<int, std::array<double, 4>> prob_ptn12;
std::unordered_map<int, std::array<double, 2>> prob_ptn_rsp;
std::unordered_set<int> ladder_ptn[2];


/**
 *  ファイルからパターンパラメータを読み込む
 *  Import pattern parameters from prob_ptn.txt etc.
 */
void ImportProbPtn3x3() {

	// 1. パターン配列を初期化
	//    Initialize all patterns.
	for(int j=0;j<65536;++j){
		for(int k=0;k<256;++k){
			for(int l=0;l<4;++l){
				prob_ptn3x3[j][k][l] = 0.0;
			}
		}
	}
	for(auto& lp: ladder_ptn) lp.clear();
	prob_ptn_rsp.clear();
	prob_ptn12.clear();

	double inv_temp = 1.0;
	std::ifstream ifs;
	string str;
	std::stringstream ss;

	// 2. 3x3パターン
	//    3x3 patterns.
	ss << "prob_ptn3x3.txt";
	ifs.open(ss.str());
	if (ifs.fail()) cerr << "file could not be opened: prob_ptn3x3.txt" << endl;

	while (getline(ifs, str)) {
		string line_str;
		std::istringstream iss(str);

		getline(iss, line_str, ',');
		int bf = stoul(line_str);

		std::array <double, 4> bf_prob;
		for (int i=0;i<4;++i) {
			getline(iss, line_str, ',');
			bf_prob[i] = stod(line_str) * inv_temp;
		}

		int stone_bf = bf & 0x0000ffff;
		int atari_bf = bf >> 24;
		for(int j=0;j<4;++j){
			prob_ptn3x3[stone_bf][atari_bf][j] = bf_prob[j];
		}

		Pattern3x3 tmp_ptn(bf);
		if(tmp_ptn.IsLadder(0)) ladder_ptn[0].insert(bf);
		if(tmp_ptn.IsLadder(1)) ladder_ptn[1].insert(bf);
	}
	ifs.close();

	// 3. responseパターン
	//    Response pattern
	ss.str("");
	ss << "prob_ptn_rsp.txt";
	ifs.open(ss.str());
	if (ifs.fail()) cerr << "file could not be opened: prob_ptn_rsp.txt" << endl;

	while (getline(ifs, str)) {
		string line_str;
		std::istringstream iss(str);

		getline(iss, line_str, ',');
		int bf = stoul(line_str);

		std::array <double, 2> bf_prob;
		for (int i=0;i<2;++i) {
			getline(iss, line_str, ',');
			bf_prob[i] = stod(line_str) * inv_temp;
		}

		prob_ptn_rsp.insert(std::make_pair(bf, bf_prob));
	}
	ifs.close();

	// 4. 12点パターン
	//    Extended 12 point pattern.
	ss.str("");
	ss << "prob_ptn12.txt";
	ifs.open("prob_ptn12.txt");
	if (ifs.fail()) cerr << "file could not be opened: prob_ptn12.txt" << endl;

	while (getline(ifs, str)) {
		string line_str;
		std::istringstream iss(str);

		getline(iss, line_str, ',');
		int bf = stoul(line_str);

		std::array <double, 4> bf_prob;
		for (int i=0;i<4;++i) {
			getline(iss, line_str, ',');
			bf_prob[i] = stod(line_str) * inv_temp;
		}

		prob_ptn12.insert(std::make_pair(bf, bf_prob));
	}
	ifs.close();

}

