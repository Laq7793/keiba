// ver.1.0 : santan, batan, tan の合成オッズの最適化
// ver.2.0 : BitFullSearchによる baren を含めた最適化をしたと思ってた
// ver.2.1 : sanrenの入れ替えのあれ, BitFullSearchの修正，0対策
// ver.2.2 : 玲さんの入力に対応しようとした
// ver.2.3 : 霊さんの入力に対応した．

// rename baken.cpp to simu_gattai.cpp　❓
// ver.1.0 : 関数化したらしい 
// ver.1.1 : 入力をHにした，その他細かいあれ(詳細はgithubに記載予定)

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<sstream> 
#include "FileInput.h"
#include "cal_unpip.h"
#include "baken_format.h"


int main(void){
  int i, j, k;
  int n;
  std::cin >> n;        // n

  // 馬券の種類別のオッズのあれ的なアレ
  std::vector<double> tan(n+1);           // 単勝
  std::vector<double> waku(n+1);          // 枠連
  std::vector<std::vector<double>> umatan(n+1, std::vector<double>(n+1));       // 馬単
  std::vector<std::vector<std::vector<double>>> santan(n+1, std::vector<std::vector<double>>(n+1, std::vector<double>(n+1)));       // 三連単
  std::vector<std::vector<double>> umaren(n+1, std::vector<double>(n+1));       // 馬連
  std::vector<std::vector<std::vector<double>>> sanren(n+1, std::vector<std::vector<double>>(n+1, std::vector<double>(n+1)));       // 三連複
  std::vector<std::vector<double>> wide(n+1, std::vector<double>(n+1));         // ワイド

  // オッズのインプット
  InputOdds(n, tan, waku, umatan, santan, umaren, sanren, wide);

  // 購入リスト
  std::vector<sTansyou> Tan_list;
  std::vector<sUmatan> Umatan_list;
  std::vector<sSantan> Santan_list;
  std::vector<sUmaren> Umaren_list;
  
  // 計算してる
  cal(n, tan, umatan, santan, umaren, Tan_list, Umatan_list, Santan_list, Umaren_list);

  return 0;
}