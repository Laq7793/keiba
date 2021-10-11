// ver.1.0 : santan, batan, tan の合成オッズの最適化
// ver.2.0 : BitFullSearchによる baren を含めた最適化をしたと思ってた
// ver.2.1 : sanrenの入れ替えのあれ, BitFullSearchの修正，0対策
// ver.2.2 : 玲さんの入力に対応しようとした
// ver.2.3 : 霊さんの入力に対応した．

// rename baken.cpp to simu_gattai.cpp　❓
// ver.1.0 : 関数化したらしい 
// ver.1.1 : 入力をHにした，その他細かいあれ(詳細はgithubに記載予定)
// ver.1.2 : サイトからリアルタイムでスクレイピングにするのを関数化した。あと、スクレイパのスレッド数を最適化した。平均で40秒でスクレイピング終わります
// ver.1.3 : 除外馬を決める（ExcStra)。馬の除外＆＆メビウスの方程式で均等振り分け(Select)。結果との比較、あたり馬券の可視化（ResultComp)
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<sstream> 
#include "FileInput.h"
#include "cal_unpip.h"
#include "baken_format.h"
#include "RealtimeScraper.h"
#include "ResultComp.h"
#include "ExcStra.h"
#include "Select.h"

//race info

int row=1;
int col=3;
int race_num=2;
int num_of_horses=15;


int main(void){
  int i, j, k;
  int n = num_of_horses;
  bool IsSim=true;

  //SantanScraper thread info
  int thread_num = ThreadNumOpt(num_of_horses);
  int mod=num_of_horses%thread_num;
  int step=num_of_horses/thread_num;
  std::vector<std::thread> threads;

  //その他
  int capital = 100000;//資本金
  int Base = capital*0.1;
  int spent = 0;
  int refund = 0;//返金額
  
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
  std::vector<int> result(num_of_horses);

  //除外馬のベクター。自分で除外馬を決めてもよし、独自のアルゴリズムを組むのもよし
  std::vector<int> FirstEx_list ;
  std::vector<int> SecondEx_list;
  std::vector<int> ThirdEx_list;

  // オッズのインプット
  if(IsSim)
  {
    InputOdds(n, tan, waku, umatan, santan, umaren, sanren, wide,result);
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

  //ここで、独自のアルゴリズムで除外馬を決定するのもあり
  ExcStra1(FirstEx_list,SecondEx_list,ThirdEx_list,tan);

  //除外馬の選定、購入馬券の決定,返り値は購入金額。spent==0のときは、ちきったということで
  spent = Select(Santan_list,Umaren_list,Umatan_list,Tan_list,
                 FirstEx_list,SecondEx_list,ThirdEx_list,Base);

  //購入フェーズ||結果照合
  if(IsSim)
  {
    if(spent!=0){
      refund = ResultComp(Santan_list,Umaren_list,Umatan_list,Tan_list,result);
    }
  }
  else
  {
    //リアルタイムガチ馬券購入
  }

  if(IsSim)
  {
    std::cout << "capital = " << capital << "\n"
        << "spent = " << spent << "\n"
        << "refund = " << refund << "\n"
        <<"syuusi = " << refund - spent << "\n";
  }
  

  return 0;
}
