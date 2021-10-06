#include<iostream>
#include<vector>
#include<string>
#include "baken_format.h"


void cal(int n,
         std::vector<double> tansyou, 
         std::vector<std::vector<double>> umatan, 
         std::vector<std::vector<std::vector<double>>> santan, 
         std::vector<std::vector<double>> umaren, 
         std::vector<sTansyou>& tan_list, 
         std::vector<sUmatan>& umatan_list, 
         std::vector<sSantan>& santan_list, 
         std::vector<sUmaren>& umaren_list)
{


    std::vector<std::vector<bool>> f_umatan(n+1, std::vector<bool>(n+1, false));   // := 馬単を選択したか
    double umatan_opt[n+1][n+1];    // := 馬単と三連単の良いほう
    for(int i=1;i<n+1;i++){
        for(int j=1;j<n+1;j++){
            if(i == j)  continue;

            double sansum = 0.0;   // := 三連単の逆数の和
            for(int k=1;k<n+1;k++){
                if(i == k || j == k)    continue;
            sansum += 1.0/santan[i][j][k];
            }

            if(1.0/sansum <= umatan[i][j]){   // 馬単のほうが高井
                umatan_opt[i][j] = umatan[i][j];
                f_umatan[i][j] = true;
            }
            else{   // 三連単の合計のほうが高い
                umatan_opt[i][j] = 1.0/sansum;
            }
        }
    }    



    // tan_optの計算
    std::vector<bool> Istan(n+1, false);     // := 短小，btn_optで短小が高井か
    double tan_opt[n+1];    // := umatan_optと短小の良いほう
    for(int i=1;i<n+1;i++){
        double umatansum = 0.0;  // := umatan_optの和
        for(int j=1;j<n+1;j++){
            if(i == j)  continue;    
            umatansum += 1.0/umatan_opt[i][j];
        }

    if(1.0/umatansum <= tansyou[i]){   // 短小選択
      tan_opt[i] = tansyou[i];
      Istan[i] = true;
    }
    else{    // !tan
      tan_opt[i] = 1.0/umatansum;
    }
  }



  // umaren, btn_opt比較
  std::vector<std::vector<bool>> f_umaren(n+1, std::vector<bool>(n+1, false));   // := btn_optとbrnでbrnのほうが高井か
  for(int i=1;i<n+1;i++){
    for(int j=i+1;j<n+1;j++){
      if(1.0/umatan_opt[i][j] + 1.0/umatan_opt[j][i] >= 1.0/umaren[i][j]){   // umarenのほうが高い
        f_umaren[i][j] = true;
        f_umaren[j][i] = true;
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
    std::vector<int> use, nouse;    // := umaren を使う使わないグループ
    std::vector<bool> f_tnpt_unchi(n+1, false);                                       // := OOを選んだか
    std::vector<std::vector<bool>> f_btnpt_unchi(n+1, std::vector<bool>(n+1, false));
    std::vector<std::vector<bool>> f_brn_unchi(n+1, std::vector<bool>(n+1, false));


    // グループ作成
    for(int i=0;i<n;i++){
      if(bit & (1<<i)){   // i番目のbitが1 <=> i番目の馬連使てがんばる
        use.push_back(i+1);
      }
      else{
        nouse.push_back(i+1);
      }
    }
    
    // 馬連を使う
    for(int i=0;i<use.size();i++){

      // 2番目がuseのとき
      for(int j=0;j<use.size();j++){
        if(use[i] == use[j])  continue;

        if(f_umaren[use[i]][use[j]]){      // umarenのほうが高い
          if(use[i] > use[j])  continue;  // 二重加算の防止
          odds += 1.0/umaren[use[i]][use[j]];
          f_brn_unchi[use[i]][use[j]] = true;
          f_brn_unchi[use[j]][use[i]] = true;
        }
        else{                   // umatan_optのほうが高い
          odds += 1.0/umatan_opt[use[i]][use[j]];
          f_btnpt_unchi[use[i]][use[j]] = true;
        } 
      }

      // 2番目がnouseのとき   //umaren使わない
      for(int j=0;j<nouse.size();j++){
        if(use[i] == nouse[j])  continue;   //  多分いらない
        odds += 1.0/umatan_opt[use[i]][nouse[j]];
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
      for(int i=1;i<n+1;i++){
        f_buy_tnpt[i] = f_tnpt_unchi[i];
        for(int j=1;j<n+1;j++){
          f_buy_brn[i][j] = f_brn_unchi[i][j];
          f_buy_btnpt[i][j] = f_btnpt_unchi[i][j];
        }
      }
    }
  }

  // output
  // 馬連買ってなかったらそれ実質tan_opt
  for(int i=1;i<n+1;i++){
    if(f_buy_tnpt[i]) continue;
    int cnt = 0;
    for(int j=1;j<n+1;j++){
      if(f_buy_brn[i][j]){
        cnt++;
      }
    }
    if(cnt == 0){
      f_buy_tnpt[i] = true;
    }
  }

  // 買うやつ
    for(int i=1;i<n+1;i++){
      //下位グループであれば問答無用で単勝
        //if(lower_group.count(i)){
            // struct sTansyou new_tan1 = {i,tansyou[i],0};
            // tan_list.push_back(new_tan1);
            
            // std::cout << i << " " << tansyou[i] << std::endl;
        //}
        //else{
            if(f_buy_tnpt[i]){      // tan_optを選択
                if(Istan[i]){       // tanがumatan_opt よりも合成オッズが高い
                    std::cout << i << " " << tansyou[i] << std::endl;    // 短小を購入
                    struct sTansyou new_tan2 = {i,tansyou[i],0};
                    tan_list.push_back(new_tan2);
                    
                }
                else{   // umatan_optがtanよりも高い
                    for(int j=1;j<n+1;j++){
                        if(i == j)   continue;

                        if(f_umatan[i][j]){    // umatanがsantanよりも高井
                            std::cout << i << ">" << j << " " << umatan[i][j] << std::endl;    // 馬単を購入
                            struct sUmatan new_umatan = {i,j,umatan[i][j],0};
                            umatan_list.push_back(new_umatan);
                            
                        }
                        else{
                            for(int k=1;k<n+1;k++){
                                if(i == k || j == k)  continue;
                                std::cout << i << ">" << j << ">" << k << " " << santan[i][j][k] << std::endl;;   // santanを購入
                                struct sSantan new_santan = {i,j,k,santan[i][j][k],0};
                                santan_list.push_back(new_santan);
                                
                            }
                        }
                    }
                }
            }
            else{   // umaren混合を購入
                for(int j=1;j<n+1;j++){
                    if(i == j)  continue;

                    if(f_buy_brn[i][j]){        // umarenを買う
                        if(i > j) continue;   // 重複対策
                        std::cout << i << "=" << j << " " << umaren[i][j] << std::endl;
                        struct sUmaren new_umaren = {i,j,umaren[i][j],0};
                        umaren_list.push_back(new_umaren);
                       
                    }
                else if(f_umatan[i][j]){    // umatanがsantanよりも高井
                    std::cout << i << ">" << j << " " << umatan[i][j] << std::endl;    // 馬単を購入
                    struct sUmatan new_umatan = {i,j,umatan[i][j],0};
                    umatan_list.push_back(new_umatan);
                    
                }
                else{
                    for(int k=1;k<n+1;k++){
                        if(i == k || j == k)  continue;
                        std::cout << i << ">" << j << ">" << k << " " << santan[i][j][k] << std::endl;;   // santanを購入
                        struct sSantan new_santan = {i,j,k,santan[i][j][k],0};
                        santan_list.push_back(new_santan);
                        
                    }
                }
                }
            }
    
        //}   
    }

	// 合成オッズ
  std::cout << "odds = " << odds_opt << std::endl;

}