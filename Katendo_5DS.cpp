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
#include "TicketPurchase.h"
#include<filesystem>//-std=c++17オプションをつけてコンパイルしてください

using std::filesystem::directory_iterator;


long long int total_spent = 0;
long long int total_win = 0;


int main(int argc,char** argv){
    int i, j, k;
    int n;

    //連続シミュレーションのためのパス
    std::string path = "./data2020";

    //ファイル入力ストリーム
    std::ifstream InFile;
    std::ofstream OutFile;
    std::ofstream SOutFile;

    //出力ファイルへのパス
    std::string Opath = "./continuous_sim_result5.txt";
    std::string Spath = "./Syusi5.txt";
    //出力ファイルを開ける
    OutFile.open(Opath);
    SOutFile.open(Spath);


    //初期情報
    int capital = 300000;//資本金
    int Base = 7000;//capital*0.03;//比率
    int spent = 0;
    int refund = 0;//返金額

    for(const auto &file : directory_iterator(path))
    {
        InFile.open(file.path());
        std::string path_name = file.path();
        OutFile << path_name << std::endl;
        SOutFile << path_name << std::endl;
        if(InFile)
        {

            InFile >> n;        // n
            int num_of_horses = n;

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

            //インプット先を決められるのがいいよね、
            GenInputOdds(n, tan, waku, umatan, santan, umaren, sanren, wide,result,InFile);
            
            // 購入リスト
            std::vector<sTansyou> Tan_list;
            std::vector<sUmatan> Umatan_list;
            std::vector<sSantan> Santan_list;
            std::vector<sUmaren> Umaren_list;
            
            // 100当たるときの馬券セットと合成オッズ計算してる
            cal(n, tan, umatan, santan, umaren, Tan_list, Umatan_list, Santan_list, Umaren_list);

            //ここで、独自のアルゴリズムで除外馬を決定するのもあり
            ExcStra1(FirstEx_list,SecondEx_list,ThirdEx_list,tan);
            ExcStra2(FirstEx_list,tan,n);

            //除外馬の選定、購入馬券の決定,返り値は購入金額。spent==0のときは、ちきったということで
            spent = Select(Santan_list,Umaren_list,Umatan_list,Tan_list,
                            FirstEx_list,SecondEx_list,ThirdEx_list,Base);

            //結果の比較
            refund = GenResultComp(Santan_list,Umaren_list,Umatan_list,Tan_list,result,OutFile);

            std::string filename = file.path();
            SOutFile << "capital = " << capital << "\n"
                << "spent = " << spent << "\n"
                << "refund = " << refund << "\n"
                <<"syuusi = " << refund - spent << "\n\n\n";

            OutFile  << "capital = " << capital << "\n"
                << "spent = " << spent << "\n"
                << "refund = " << refund << "\n"
                <<"syuusi = " << refund - spent << "\n\n\n";
            
            capital += refund - spent;
            total_spent += spent;
            total_win += refund;
            //Base = capital*0.03;
            if(capital < 0)
            {
                SOutFile << "no money\n" << total_win << " \n" << total_spent <<"\n" << (double)total_win/total_spent*100 << "%\n" << std::endl;
                OutFile << "no money\n" << total_win << " \n" << total_spent <<"\n" << (double)total_win/total_spent*100 << "%\n"<< std::endl;
                break;
            }
            

            //ちゃんとファイルは閉める
            InFile.close();
            

        }
        else
        {
            std::cout << "file doesn't exist" << std::endl;
        }
    }
      
  
  
    

  return 0;
}