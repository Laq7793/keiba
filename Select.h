#include<iostream>
#include<vector>
#include<string>
#include "baken_format.h"
#include "ToProbability.h"

int my_ceil(double m){
    int ret;
    m/=10;
    m/=10;
    ret = (int)m;
    ret+=1;
    ret*=100;
    return ret;
}



int Select(std::vector<sSantan>& Santan,
                std::vector<sUmaren>& Umaren,
                std::vector<sUmatan>& Umatan,
                std::vector<sTansyou>& tan,
                std::vector<int>& FirstEx,
                std::vector<int>& SecondEx,
                std::vector<int>& ThirdEx,
                int BaseMoney)
{
  

  double MaxOdds = 0;
  double MinimumMoney=0;
  double denominator=0;
  double SynthOddsInv = 0;
  int spent=0;



  //一着外し
  if(FirstEx.size()!=0){
    for(auto itr = Santan.begin();itr != Santan.end();){
      for(int j=0;j<FirstEx.size();j++){
          if(itr->umaban1 == FirstEx[j]){
              itr = Santan.erase(itr);
              break;
          }
          if(j==FirstEx.size()-1){
              ++itr;
          }
      }
  }

   for(auto itr = Umatan.begin();itr != Umatan.end();){
      for(int j=0;j<FirstEx.size();j++){
          if(itr->umaban1 == FirstEx[j]){
              itr = Umatan.erase(itr);
              break;
          }
          if(j==FirstEx.size()-1){
              ++itr;
          }
      }
  }
  /*
    for(auto itr = Umaren.begin();itr != Umaren.end();){
      for(int j=0;j<FirstEx.size();j++){
          if(itr->umaban1 == FirstEx[j]){
              itr = Umaren.erase(itr);
              break;
          }
          if(j==FirstEx.size()-1){
              ++itr;
          }
      }
  }
  */
    for(auto itr = tan.begin();itr != tan.end();){
        for(int j=0;j<FirstEx.size();j++){
          if(itr->umaban == FirstEx[j]){
              itr = tan.erase(itr);
              break;
          }
          if(j==FirstEx.size()-1){
              ++itr;
          }
      }
    }
  }
  //二着抜かし
  if(SecondEx.size()!=0){
  for(auto itr = Santan.begin();itr != Santan.end();){
      for(int j=0;j<SecondEx.size();j++){
          if(itr->umaban2 == SecondEx[j]){
              itr = Santan.erase(itr);
              break;
          }
          if(j==SecondEx.size()-1){
              ++itr;
          }
      }
  }

   for(auto itr = Umatan.begin();itr != Umatan.end();){
      for(int j=0;j<SecondEx.size();j++){
          if(itr->umaban2 == SecondEx[j]){
              itr = Umatan.erase(itr);
              break;
          }
          if(j==SecondEx.size()-1){
              ++itr;
          }
      }
  }

    for(auto itr = Umaren.begin();itr != Umaren.end();){
      for(int j=0;j<SecondEx.size();j++){
          if(itr->umaban2 == SecondEx[j]){
              itr = Umaren.erase(itr);
              break;
          }
          if(j==SecondEx.size()-1){
              ++itr;
          }
      }
  }
  }

  //三着外し
if(ThirdEx.size()!=0){
  for(auto itr = Santan.begin();itr != Santan.end();){
      for(int j=0;j<ThirdEx.size();j++){
          if(itr->umaban3 == ThirdEx[j]){
              itr = Santan.erase(itr);
              break;
          }
          if(j==ThirdEx.size()-1){
              ++itr;
          }
      }
  }
}


  //基準金額(一番でかいオッズ）を探す&&合成オッズの計算
  //tansyou
  for(auto itr = Umatan.begin();itr != Umatan.end();itr++)
  {
      SynthOddsInv += 1.0/itr->odds;
      if(itr->odds > MaxOdds)
      {
          MaxOdds = itr->odds;
      }
  }
  
  //baren
  for(auto itr = Umaren.begin();itr != Umaren.end();itr++)
  {
      SynthOddsInv += 1.0/itr->odds;
      if(itr->odds > MaxOdds)
      {
          MaxOdds = itr->odds;
      }
  }
  //batan

  for(auto itr = Umatan.begin();itr != Umatan.end();itr++)
  {
      SynthOddsInv += 1.0/itr->odds;
      if(itr->odds > MaxOdds)
      {
          MaxOdds = itr->odds;
      }
  }

  //santan
  for(auto itr = Santan.begin();itr != Santan.end();itr++)
  {
      SynthOddsInv += 1.0/itr->odds;
      if(itr->odds > MaxOdds)
      {
          MaxOdds = itr->odds;
      }
  }

  //基準金額の設定
    for(auto itr = tan.begin();itr != tan.end();itr++){
          denominator += MaxOdds/itr->odds;
    }

  for(auto itr = Umaren.begin();itr != Umaren.end();itr++){
          denominator += MaxOdds/itr->odds;
  }
  //batan
  for(auto itr = Umatan.begin();itr != Umatan.end();itr++){
        denominator += MaxOdds/itr->odds;
  }
  //santan
  for(auto itr = Santan.begin();itr != Santan.end();itr++){
        denominator += MaxOdds/itr->odds;
  }


  std::cout << "BaseMoney" << BaseMoney <<std::endl;
  std::cout << "denominator=" << denominator << std::endl;
  MinimumMoney = BaseMoney/denominator;
  std::cout << "MinimumMoney=" << MinimumMoney << std::endl;

  //メビウスの方程式
  //tansyou
  for(auto itr = tan.begin();itr != tan.end();itr++){
        int kingaku= my_ceil(MinimumMoney*MaxOdds/itr->odds);
        itr->money = kingaku;
        spent += kingaku;
  }
  //umaren
  for(auto itr = Umaren.begin();itr != Umaren.end();itr++){
        int kingaku = my_ceil(MinimumMoney*MaxOdds/itr->odds);
        itr->money = kingaku;
        spent += kingaku;
  }
  //batan
  for(auto itr = Umatan.begin();itr != Umatan.end();itr++){
        int kingaku = my_ceil(MinimumMoney*MaxOdds/itr->odds);
        itr->money = kingaku;
        spent += kingaku;
  }
  //sanren
  for(auto itr = Santan.begin();itr != Santan.end();itr++){
        int kingaku = my_ceil(MinimumMoney*MaxOdds/itr->odds);
        itr->money = kingaku;
        spent += kingaku;
  }

  //結局何を買うのかの出力

  //単勝
  std::cout << "tansyou" << std::endl;
  for(auto i = tan.begin();i!=tan.end();i++){
      std::cout << i-> umaban << " " << i->odds << " " << i->money << std::endl;
  }

  //baren
  std::cout << "baren" << std::endl;
  for(auto i= Umaren.begin();i!=Umaren.end();i++){
      std::cout << i-> umaban1 << "-" << i-> umaban2 << " " << i->odds << " " << i->money << std::endl; 
  }

  //batan
    std::cout << "batan" << std::endl;
    for(auto i= Umatan.begin();i!=Umatan.end();i++){
        std::cout << i-> umaban1 << ">" <<i-> umaban2 << " " << i->odds << " " << i->money << std::endl; 
    }

    //sanrentan
    std::cout << "sanrentan" << std::endl;
    for(auto i= Santan.begin();i!=Santan.end();i++){
        std::cout << i-> umaban1 << ">" <<i-> umaban2 << ">" << i->umaban3 <<" " << i->odds << " " << i->money << std::endl; 
    }

    //その情報出力
    std::cout << "買った馬券の合成オッズ＝"<<1.0/SynthOddsInv << std::endl;
    std::cout << "SynthOdds*0.85*spent = " << 1.0/SynthOddsInv*0.85*BaseMoney << std::endl;
    std::cout << "Recovery rate = " <<1.0/SynthOddsInv*0.85*BaseMoney/spent << std::endl;

    double Rrate = 1.0/SynthOddsInv*0.85*BaseMoney/spent;
    if(Rrate > 1.1){
        return spent;
    }
    else{
        return 0;
    }

}