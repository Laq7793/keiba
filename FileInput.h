#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<sstream> 


// delimで分割して左から順にvectorにぶち込む関数でしゅ
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


// オッズのインプット，初期化はこの関数内でしております．
void InputOdds(int num_of_horses,
               std::vector<double>& tan,
               std::vector<double>& waku,
               std::vector<std::vector<double>>& umatan,
               std::vector<std::vector<std::vector<double>>>& santan,
               std::vector<std::vector<double>>& umaren,
               std::vector<std::vector<std::vector<double>>>& sanren,
               std::vector<std::vector<double>>& wide)
{

  int i, j, k;
  std::string unchi, unpip;       // := unchi, unpip
  double v_ini = 99999.9;   //   初期化の値

  // 初期化
  for(i=0;i<num_of_horses+1;i++){
    tan[i] = v_ini;
    waku[i] = v_ini;
    for(j=0;j<num_of_horses+1;j++){
      umatan[i][j] = v_ini;
      umaren[i][j] = v_ini;
      wide[i][j] = v_ini;
      for(k=0;k<num_of_horses+1;k++){
        santan[i][j][k] = v_ini;
        sanren[i][j][k] = v_ini;
      }
    }
  }


  // input
  // tan
  int num;
  std::cin >> unchi;
  for(i=1;i<num_of_horses+1;i++){   // tan, waku
    std::cin >> unchi >> tan[i] >> waku[i];

    // エラー処理(詳細はよくわからんぬ)
    if(std::cin.fail()){
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
      printf("INPUT ERROR : tan[%d] or fuku[%d]\n", i, i);
      tan[i] = v_ini;
      waku[i] = v_ini;
      continue;
    }
  }

  // umatan
  std::cin >> unchi >> num;
  for(i=1;i<num+1;i++){
    std::string str;    // := 「i>j」とか
    double TANAKA;      // := odds
    std::cin >> str >> TANAKA;
    if(std::cin.fail()){
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
      printf("INPUT ERROR : umatan, i = %d\n", i);
      continue;
    }
    std::vector<int> uma = split(str, '>');     // >で分割してvectorに入れる
    umatan[uma[0]][uma[1]] = TANAKA;
  }

  // santan
  std::cin >> unchi >> num;
  for(i=1;i<num+1;i++){
    std::string str;    // := 「i>j」とか
    double TANAKA;      // := odds
    std::cin >> str >> TANAKA;
    if(std::cin.fail()){
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
      printf("INPUT ERROR : santan, i = %d\n", i);
      continue;
    }
    std::vector<int> uma = split(str, '>');
    santan[uma[0]][uma[1]][uma[2]] = TANAKA;
  }

  // umaren
  std::cin >> unchi >> num;
  for(i=1;i<num+1;i++){
    std::string str;    // := 「i>j」とか
    double TANAKA;      // := odds
    std::cin >> str >> TANAKA;
    if(std::cin.fail()){
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
      printf("INPUT ERROR : umaren, i = %d\n", i);
      continue;
    }
    std::vector<int> uma = split(str, '-');
    umaren[uma[0]][uma[1]] = TANAKA;
    umaren[uma[1]][uma[0]] = TANAKA;
  }

  // sanren
  std::cin >> unchi >> num;
  for(i=1;i<num+1;i++){
    std::string str;    // := 「i>j」とか
    double TANAKA;      // := odds
    std::cin >> str >> TANAKA;
    if(std::cin.fail()){
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
      printf("INPUT ERROR : sanren, i = %d\n", i);
      continue;
    }
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
    if(std::cin.fail()){
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
      printf("INPUT ERROR : wide, i = %d\n", i);
      continue;
    }
    std::vector<int> uma = split(str, '-');
    wide[uma[0]][uma[1]] = TANAKA;
    wide[uma[1]][uma[0]] = TANAKA;
  }
  
  return;
}
