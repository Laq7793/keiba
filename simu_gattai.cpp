// ver.1.0 : santan, batan, tan の合成オッズの最適化
// ver.2.0 : BitFullSearchによる baren を含めた最適化をしたと思ってた
// ver.2.1 : sanrenの入れ替えのあれ, BitFullSearchの修正，0対策
// ver.2.2 : 玲さんの入力に対応しようとした
// ver.2.3 : 霊さんの入力に対応した．

// rename baken.cpp to simu_gattai.cpp　❓
// ver.1.0 : 関数化したらしい 
// ver.1.1 : 入力をHにした，その他細かいあれ(詳細はgithubに記載予定)
// ver.1.2 : サイトからリアルタイムでスクレイピングにするのを関数化した。あと、スクレイパのスレッド数を最適化した。平均で40秒でスクレイピング終わります

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<sstream> 
#include "FileInput.h"
#include "cal_unpip.h"
#include "baken_format.h"
#include "RealtimeScraper.h"

//race info

int row=1;
int col=3;
int race_num=2;
int num_of_horses=15;


int main(void){
  int i, j, k;
  int n = num_of_horses;
  bool IsSim=false;

  //SantanScraper thread info
  int thread_num = ThreadNumOpt(num_of_horses);
  int mod=num_of_horses%thread_num;
  int step=num_of_horses/thread_num;
  std::vector<std::thread> threads;
  
  if(IsSim)
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
  if(IsSim)
  {
    InputOdds(n, tan, waku, umatan, santan, umaren, sanren, wide);
  }
  else
  {

      for(int i=0;i<thread_num-1;i++)
      {
          //worker_id,row,columun,step,mod,num_of_horses,santan,time,fswitch(10)
          threads.push_back(std::thread(SantanScraper,i,row,col,race_num,step,0,num_of_horses,std::ref(santan),1,false));
      }
    //num_of_horses,umatan,umaren,tan,fswitch,col,row,racenum
      threads.push_back(std::thread(SantanScraper,thread_num-1,row,col,race_num,step,mod,num_of_horses,std::ref(santan),1,false));
      TanBatanRenScraper(num_of_horses,umatan,umaren,tan,false,col,row,race_num);
      
  }

  for (std::thread &th : threads) {
    th.join();
  }

  // 購入リスト
  std::vector<sTansyou> Tan_list;
  std::vector<sUmatan> Umatan_list;
  std::vector<sSantan> Santan_list;
  std::vector<sUmaren> Umaren_list;
  
  // 100当たるときの馬券セットと合成オッズ計算してる
  cal(n, tan, umatan, santan, umaren, Tan_list, Umatan_list, Santan_list, Umaren_list);

  //除外馬の選定、購入馬券の決定
  Select();

  //購入フェーズ

  return 0;
}
