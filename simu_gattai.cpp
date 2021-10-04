// ver.1.0 : santan, batan, tan の合成オッズの最適化
// ver.2.0 : BitFullSearchによる baren を含めた最適化をしたと思ってた
// ver.2.1 : sanrenの入れ替えのあれ, BitFullSearchの修正，0対策
// ver.2.2 : 玲さんの入力に対応しようとした
// ver.2.3 : 霊さんの入力に対応した．

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<sstream> 
#include "cal_unpip.h"
#include "baken_format.h"

// delimで分割して左から順にvectorにぶち込む
std::vector<int> split(const std::string &str, char delim) {
  std::vector<int> elems;   // :=ぶち込まれるやつ
  std::stringstream ss(str);
  std::string s;  // := delimで分割されたやつ
  int num;  // := のint
  while(std::getline(ss, s, delim)){    // delimで分割するじゃん？
    if(!s.empty()){   
      num = std::stoi(s);   // 数字に変換
      elems.push_back(num);
    }
  }
  return elems;
}

int main(void){
  int i, j, k;
  int n;
  std::cin >> n;        // n
  double fuku[n+1], wide[n+1][n+1];   // 使いません
  double tan[n+1];//, batan[n+1][n+1]; //santan[n+1][n+1][n+1];		// sntn[i][j][k] := i>j>k のオッズ
  double sanren[n+1][n+1][n+1];   
  std::vector<std::vector<std::vector<double>>> santan(n+1, std::vector<std::vector<double>>(n+1, std::vector<double>(n+1, 1.0)));
  std::vector<std::vector<double>> baren(n+1, std::vector<double>(n+1, 1.0));
  std::vector<std::vector<double>> batan(n+1, std::vector<double>(n+1, 1.0));
  // brn[i][j] := i-j のオッズ
  std::string unchi, unpip;       // := unchi, unpip
  double v_ini = 999999.9;   //   初期化の値


  // 初期化
  for(i=0;i<n+1;i++){
    tan[i] = v_ini;
    fuku[i] = v_ini;
    for(j=0;j<n+1;j++){
      batan[i][j] = v_ini;
      baren[i][j] = v_ini;
      wide[i][j] = v_ini;
      for(k=0;k<n+1;k++){
        santan[i][j][k] = v_ini;
        sanren[i][j][k] = v_ini;
      }
    }
  }


  // input
  // tan
  int num;
  std::cin >> unchi;
  for(i=1;i<n+1;i++){   // tan, fuku
    std::cin >> unchi >> tan[i] >> fuku[i];
  }

  // batan
  std::cin >> unchi >> num;
  for(i=1;i<num+1;i++){
    std::string str;    // := 「i>j」とか
    double TANAKA;      // := odds
    std::cin >> str >> TANAKA;
    std::vector<int> uma = split(str, '>');     // >で分割してvectorに入れる
    batan[uma[0]][uma[1]] = TANAKA;
  }

  // santan
  std::cin >> unchi >> num;
  for(i=1;i<num+1;i++){
    std::string str;    // := 「i>j」とか
    double TANAKA;      // := odds
    std::cin >> str >> TANAKA;
    std::vector<int> uma = split(str, '>');
    santan[uma[0]][uma[1]][uma[2]] = TANAKA;
  }

  // baren
  std::cin >> unchi >> num;
  for(i=1;i<num+1;i++){
    std::string str;    // := 「i>j」とか
    double TANAKA;      // := odds
    std::cin >> str >> TANAKA;
    std::vector<int> uma = split(str, '-');
    baren[uma[0]][uma[1]] = TANAKA;
    baren[uma[1]][uma[0]] = TANAKA;
  }

  // batan
  std::cin >> unchi >> num;
  for(i=1;i<num+1;i++){
    std::string str;    // := 「i>j」とか
    double TANAKA;      // := odds
    std::cin >> str >> TANAKA;
    std::vector<int> uma = split(str, '-');
    sort(uma.begin(), uma.end());   // なんか怖いじゃん？
    do{
      sanren[uma[0]][uma[1]][uma[2]] = TANAKA;
    }while(std::next_permutation(uma.begin(), uma.end()));
  }

  // wide
  std::cin >> unchi >> num;
  for(i=1;i<num+1;i++){
    std::string str;    // := 「i>j」とか
    double TANAKA;      // := odds
    std::cin >> str >> TANAKA;
    std::vector<int> uma = split(str, '-');
    wide[uma[0]][uma[1]] = TANAKA;
    wide[uma[1]][uma[0]] = TANAKA;
  }

      std::vector<sTansyou> Tansyou;
    std::vector<sUmaren> Umaren;
    std::vector<sUmatan> Umatan;
    std::vector<sSantan> Santan;
  

  cal(santan,batan,baren,tan,n,Umatan,Umaren,Santan,Tansyou);
  return 0;
}