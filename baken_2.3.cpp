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
  double tan[n+1], batan[n+1][n+1], santan[n+1][n+1][n+1];		// sntn[i][j][k] := i>j>k のオッズ
  double baren[n+1][n+1], sanren[n+1][n+1][n+1];   // brn[i][j] := i-j のオッズ
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


  // 入力確認 
/*std::cout << tan[n] << std::endl;
  std::cout << batan[n][n-1] << std::endl;
  std::cout << baren[n][n-1] << std::endl;
  std::cout << santan[n][n-1][n-2] << std::endl;
  std::cout << sanren[n][n-1][n-2] << std::endl;
*/


  // batan_optの計算
  std::vector<std::vector<bool>> f_batan(n+1, std::vector<bool>(n+1, false));   // := 馬単を選択したか
  double batan_opt[n+1][n+1];    // := 馬単と三連単の良いほう
  for(i=1;i<n+1;i++){
    for(j=1;j<n+1;j++){
      if(i == j)  continue;

      double sansum = 0.0;   // := 三連単の逆数の和
      for(k=1;k<n+1;k++){
        if(i == k || j == k)    continue;
        sansum += 1.0/santan[i][j][k];
      }

      if(1.0/sansum <= batan[i][j]){   // 馬単のほうが高井
        batan_opt[i][j] = batan[i][j];
        f_batan[i][j] = true;
      }
      else{   // 三連単の合計のほうが高い
        batan_opt[i][j] = 1.0/sansum;
      }
    }
  }



  // tan_optの計算
  std::vector<bool> f_tan(n+1, false);     // := 短小，btn_optで短小が高井か
  double tan_opt[n+1];    // := batan_optと短小の良いほう
  for(i=1;i<n+1;i++){

    double batansum = 0.0;  // := batan_optの和
    for(j=1;j<n+1;j++){
      if(i == j)  continue;    
      batansum += 1.0/batan_opt[i][j];
    }

    if(1.0/batansum <= tan[i]){   // 短小選択
      tan_opt[i] = tan[i];
      f_tan[i] = true;
    }
    else{    // !tan
      tan_opt[i] = 1.0/batansum;
    }
  }



  // baren, btn_opt比較
  std::vector<std::vector<bool>> f_baren(n+1, std::vector<bool>(n+1, false));   // := btn_optとbrnでbrnのほうが高井か
  for(i=1;i<n+1;i++){
    for(j=i+1;j<n+1;j++){
      if(1.0/batan_opt[i][j] + 1.0/batan_opt[j][i] >= 1.0/baren[i][j]){   // barenのほうが高い
        f_baren[i][j] = true;
        f_baren[j][i] = true;
        //std::cout << "(" << i << ", " << j << ")" << " ";
      }
    }
  }//std::cout << std::endl;



  // BitFullSearch
  double odds_opt = 0.0;                     // := 合成オッズの最大値
  std::vector<bool> f_buy_tnpt(n+1, false);                                       // := オッズが最大の時のOOを選んだか
  std::vector<std::vector<bool>> f_buy_btnpt(n+1, std::vector<bool>(n+1, false));
  std::vector<std::vector<bool>> f_buy_brn(n+1, std::vector<bool>(n+1, false));

  for(int bit=0;bit<(1<<n);bit++){
    double odds = 0.0;
    std::vector<int> use, nouse;    // := baren を使う使わないグループ
    std::vector<bool> f_tnpt_unchi(n+1, false);                                       // := OOを選んだか
    std::vector<std::vector<bool>> f_btnpt_unchi(n+1, std::vector<bool>(n+1, false));
    std::vector<std::vector<bool>> f_brn_unchi(n+1, std::vector<bool>(n+1, false));


    // グループ作成
    for(i=0;i<n;i++){
      if(bit & (1<<i)){   // i番目のbitが1 <=> i番目の馬連使てがんばる
        use.push_back(i+1);
      }
      else{
        nouse.push_back(i+1);
      }
    }
    
    // 馬連を使う
    for(i=0;i<use.size();i++){

      // 2番目がuseのとき
      for(j=0;j<use.size();j++){
        if(use[i] == use[j])  continue;

        if(f_baren[use[i]][use[j]]){      // barenのほうが高い
          if(use[i] > use[j])  continue;  // 二重加算の防止
          odds += 1.0/baren[use[i]][use[j]];
          f_brn_unchi[use[i]][use[j]] = true;
          f_brn_unchi[use[j]][use[i]] = true;
        }
        else{                   // batan_optのほうが高い
          odds += 1.0/batan_opt[use[i]][use[j]];
          f_btnpt_unchi[use[i]][use[j]] = true;
        } 
      }

      // 2番目がnouseのとき   //baren使わない
      for(j=0;j<nouse.size();j++){
        if(use[i] == nouse[j])  continue;   //  多分いらない
        odds += 1.0/batan_opt[use[i]][nouse[j]];
        f_btnpt_unchi[use[i]][nouse[j]] = true;      
      }
    }


    // 馬連を使わない
    for(auto k : nouse){
      odds += 1.0/tan_opt[k];   // tan_opt一択
      f_tnpt_unchi[k] = true;
    }

    odds = 1.0/odds;
    //std::cout << odds << std::endl;

    // 最大値の更新とか
    if(odds > odds_opt){
      odds_opt = odds;
      // フラグのコピー
      for(i=1;i<n+1;i++){
        f_buy_tnpt[i] = f_tnpt_unchi[i];
        for(j=1;j<n+1;j++){
          f_buy_brn[i][j] = f_brn_unchi[i][j];
          f_buy_btnpt[i][j] = f_btnpt_unchi[i][j];
        }
      }
    }
  }



  // output
  // 馬連買ってなかったらそれ実質tan_opt
  for(i=1;i<n+1;i++){
    if(f_buy_tnpt[i]) continue;
    int cnt = 0;
    for(j=1;j<n+1;j++){
      if(f_buy_brn[i][j]){
        cnt++;
      }
    }
    if(cnt == 0){
      f_buy_tnpt[i] = true;
    }
  }

  // 買うやつ
  for(i=1;i<n+1;i++){

    if(f_buy_tnpt[i]){      // tan_optを選択
      if(f_tan[i]){       // tanがbatan_opt よりも合成オッズが高い
        std::cout << i << " " << tan[i] << std::endl;    // 短小を購入
      }

      else{   // batan_optがtanよりも高い
        for(j=1;j<n+1;j++){
          if(i == j)   continue;

          if(f_batan[i][j]){    // batanがsantanよりも高井
            std::cout << i << ">" << j << " " << batan[i][j] << std::endl;    // 馬単を購入
          }
          else{
            for(k=1;k<n+1;k++){
              if(i == k || j == k)  continue;
              std::cout << i << ">" << j << ">" << k << " " << santan[i][j][k] << std::endl;;   // santanを購入
            }
          }
        }
      }
    }

    else{   // baren混合を購入
      for(j=1;j<n+1;j++){
        if(i == j)  continue;

        if(f_buy_brn[i][j]){        // barenを買う
          if(i > j) continue;   // 重複対策
          std::cout << i << "=" << j << " " << baren[i][j] << std::endl;
        }
        else if(f_batan[i][j]){    // batanがsantanよりも高井
          std::cout << i << ">" << j << " " << batan[i][j] << std::endl;    // 馬単を購入
        }
        else{
          for(k=1;k<n+1;k++){
            if(i == k || j == k)  continue;
            std::cout << i << ">" << j << ">" << k << " " << santan[i][j][k] << std::endl;;   // santanを購入
          }
        }
      }
    }
  }

	// 合成オッズ
  std::cout << "odds = " << odds_opt << std::endl;

  return 0;
}
