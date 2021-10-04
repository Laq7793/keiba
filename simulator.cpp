#include<webdriverxx.h>
#include<webdriverxx/browsers/chrome.h>
#include<chrono>
#include<thread>
#include<cstdlib>
#include<fstream>
#include<set>

using namespace webdriverxx;

#define NUM_OF_THREADS 4

//login information(dont forget to source ~/.profile)
std::string P_ARS = getenv("P_ARS");

std::string INET_ID = getenv("INET_ID");
std::string USER_NUM = getenv("USER_NUM");
std::string PIN = getenv("PIN");
std::string root_url = "https://www.ipat.jra.go.jp/";

//race information
#define N 16 //馬数
int row=1;
int columun=1;
int race_num=12;
int num_of_horses=N;
int mod=num_of_horses%NUM_OF_THREADS;
int step=num_of_horses/NUM_OF_THREADS;

//money information
int capital_money=100000;
int actual_upper_spent=0;//上位分に使った実際の金額
int deposit = 3000;//頭金の基準
int challnage_index=3; //2:慎重　3:セメ　4:ハイリスク、ハイリターン
int actual_deposit=0;//頭金に使った実際の金額
int odds_boundary = 15;//これが上位と下位の分かれ目になります。
double upper_probability = 0.9; //これが上位が勝つ確率.odds_boundaryによっても変化する、またこの数字は確実なものではないので注意すること。
double lower_probability = 1-upper_probability; //下位がかつ確率
double upper_synth_odds=0;//上位の合成オッズ
double lower_synth_odds=0;//下位の合成オッズ
int money_upper=0;//上位にかける金
double expected_return = 0;//期待返金額
int purchase_price = 0;//合計購入金額
double recovery_rate = 0; //回収率

//ticket_selecting configuration
std::vector<int> second_excluding{3,7,9,13,11};//二着外し。絶対何か外さんと動かん。
std::vector<int> third_excluding{};//３着外し
std::vector<int> first_excluding{4};//1着外し。かなり怖いけど。
//ticket_information
struct sTansyou{
    int umaban;
    double odds;
    int money;
    double probability;
  };

  struct sUmaren{
    
    int umaban1;
    int umaban2;
    double odds;
    int money;
    double probability;
  };

  struct sUmatan{
    
    int umaban1;
    int umaban2;
    double odds;
    int money;
    double probability;
  };

  struct sSantan{
    
    int umaban1;
    int umaban2;
    int umaban3;
    double odds;
    int money;
    double probability;
  };

  

  //100円単位で切り上げ関数
  int my_ceil(double m){
    int ret;
    m/=10;
    m/=10;
    ret = (int)m;
    ret+=1;
    ret*=100;
    return ret;
  }

//odds array
int n = num_of_horses;
double tansyou[N+1], batan[N+1][N+1], santan[N+1][N+1][N+1];		// sntn[i][j][k] := i>j>k のオッズ
double baren[N+1][N+1]; // brn[i][j] := i-j のオッズ

//worker thread (sanrantan scraper)
void worker(int worker_id,int mod){
    WebDriver chrome = Start(Chrome());
    chrome.Navigate(root_url);

        std::string login_text_box_xpath = "//*[@id=\"top\"]/div[3]/div/table/tbody/tr/td[2]/div/div/form/table[1]/tbody/tr/td[2]/span/input";
        std::string click_button_xpath = "//*[@id=\"top\"]/div[3]/div/table/tbody/tr/td[2]/div/div/form/table[1]/tbody/tr/td[3]/p/a";
        chrome.FindElement(ByXPath(login_text_box_xpath)).SendKeys(INET_ID);
        chrome.FindElement(ByXPath(click_button_xpath)).Click();

        //inputting the required information
        std::string user_num_input_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[1]/td[2]/span/input";
        std::string pin_input_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[2]/td[2]/span/input";
        std::string p_ars_input_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[3]/td[2]/span/input";
        click_button_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[1]/td[3]/p/a";
        chrome.FindElement(ByXPath(user_num_input_xpath)).SendKeys(USER_NUM);
        chrome.FindElement(ByXPath(pin_input_xpath)).SendKeys(PIN);
        chrome.FindElement(ByXPath(p_ars_input_xpath)).SendKeys(P_ARS);
        chrome.FindElement(ByXPath(click_button_xpath)).Click();
        std::this_thread::sleep_for(std::chrono::seconds(1));

        //login complete.going to odds vote screen
        std::string odds_vote_xpath = "//*[@id=\"main\"]/ui-view/main/div[2]/div[1]/div[2]/button";
        chrome.FindElement(ByXPath(odds_vote_xpath)).Click();
        
    
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::string baba_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/select-course-race/div/div[2]/div[2]/div[2]/div[" + std::to_string(row) + "]/div["+ std::to_string(columun)+ "]/button";
        chrome.FindElement(ByXPath(baba_xpath)).Click();

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::string race_num_xpath="//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/select-course-race/div/div[2]/div[2]/div[4]/div[" + std::to_string(race_num)+ "]/button";
        chrome.FindElement(ByXPath(race_num_xpath)).Click();


        std::this_thread::sleep_for(std::chrono::seconds(1));
        //opening the file
        std::string file_name = "real_time_odds"+std::to_string(worker_id);
        std::ofstream file(file_name);//file(file_name);
        
        //scraping sanrentan
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::string odds_type_change_xpath = "//*[@id=\"bet-odds-type\"]";
        chrome.FindElement(ByXPath(odds_type_change_xpath)).Click();
        for(int i=0;i<6;i++)
            chrome.FindElement(ByXPath(odds_type_change_xpath)).SendKeys(keys::Down);
        chrome.FindElement(ByXPath(odds_type_change_xpath)).SendKeys(keys::Enter);

        std::string selector_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/div[2]/div[2]/div[2]/select";
                chrome.FindElement(ByXPath(selector_xpath)).Click();
                for(int i=0;i<step*worker_id;i++)
                    chrome.FindElement(ByXPath(selector_xpath)).SendKeys(keys::Down);
                chrome.FindElement(ByXPath(selector_xpath)).SendKeys(keys::Enter);
                std::this_thread::sleep_for(std::chrono::seconds(1));

        
        for(int first=step*worker_id+1;first<step*worker_id+step+mod+1;first++){
            if(first%step!=1){
                std::string selector_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/div[2]/div[2]/div[2]/select";
                chrome.FindElement(ByXPath(selector_xpath)).Click();
                chrome.FindElement(ByXPath(selector_xpath)).SendKeys(keys::Down);
                chrome.FindElement(ByXPath(selector_xpath)).SendKeys(keys::Enter);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            for(int second = 1;second<num_of_horses;second++){
                std::string nichaku_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-trifecta-basic/div/div/div[2]/div[" +std::to_string(second) +"]/div[2]/div[1]";
                for(int third=1;third<num_of_horses;third++){
                    std::string sanchaku_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-trifecta-basic/div/div/div[2]/div[1]/div[2]/div["+ std::to_string(third+1)+"]/div[1]";
                    std::string odds_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-trifecta-basic/div/div/div[2]/div["+std::to_string(second)+"]/div[2]/div["+std::to_string(third+1)+"]/div[2]/button/odds-num/span/span";
                    file << first << ">" << chrome.FindElement(ByXPath(nichaku_xpath)).GetText()
                    <<">" << chrome.FindElement(ByXPath(sanchaku_xpath)).GetText()
                    << " " << chrome.FindElement(ByXPath(odds_xpath)).GetText() << std::endl;
                    int nichaku_int = std::stoi(chrome.FindElement(ByXPath(nichaku_xpath)).GetText());
                    int sanchaku_int = std::stoi(chrome.FindElement(ByXPath(sanchaku_xpath)).GetText());
                    std::string odds=chrome.FindElement(ByXPath(odds_xpath)).GetText();
                    if(odds!="---")
                        santan[first][nichaku_int][sanchaku_int] = std::stod(chrome.FindElement(ByXPath(odds_xpath)).GetText());
                }
            }
        }
        file.close();
}



int main()
{

    //multithread化。三連単をworkerに回収してもらう。ここね、スレッド数が固定されてるとmod=3のときとかに遅くなるので動的生成したほうがいいかもしれない。

    std::thread th0(worker,0,0);
    std::thread th1(worker,1,0);
    std::thread th2(worker,2,0);
    std::thread th3(worker,3,mod);
    
    /*
    std::thread th0(worker,0,0);
    std::thread th1(worker,1,0);
    std::thread th2(worker,2,0);
    std::thread th3(worker,3,0);
    std::thread th4(worker,4,mod);
*/


    //main thread は単勝、馬単、馬連をスクレイピングしてもらう
    WebDriver chrome = Start(Chrome());
    chrome.Navigate(root_url);
    std::string login_text_box_xpath = "//*[@id=\"top\"]/div[3]/div/table/tbody/tr/td[2]/div/div/form/table[1]/tbody/tr/td[2]/span/input";
    std::string click_button_xpath = "//*[@id=\"top\"]/div[3]/div/table/tbody/tr/td[2]/div/div/form/table[1]/tbody/tr/td[3]/p/a";
    chrome.FindElement(ByXPath(login_text_box_xpath)).SendKeys(INET_ID);
    chrome.FindElement(ByXPath(click_button_xpath)).Click();

    //inputting the required information
    std::string user_num_input_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[1]/td[2]/span/input";
    std::string pin_input_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[2]/td[2]/span/input";
    std::string p_ars_input_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[3]/td[2]/span/input";
    click_button_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[1]/td[3]/p/a";
    chrome.FindElement(ByXPath(user_num_input_xpath)).SendKeys(USER_NUM);
    chrome.FindElement(ByXPath(pin_input_xpath)).SendKeys(PIN);
    chrome.FindElement(ByXPath(p_ars_input_xpath)).SendKeys(P_ARS);
    chrome.FindElement(ByXPath(click_button_xpath)).Click();
    std::this_thread::sleep_for(std::chrono::seconds(1));
        
    //login complete.going to odds vote section（オッズ投票画面への遷移）
    std::string odds_vote_xpath = "//*[@id=\"main\"]/ui-view/main/div[2]/div[1]/div[2]/button";
    chrome.FindElement(ByXPath(odds_vote_xpath)).Click();
    std::this_thread::sleep_for(std::chrono::seconds(1));    
        
    //馬場選択画面への遷移   
    std::string baba_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/select-course-race/div/div[2]/div[2]/div[2]/div[" + std::to_string(row) + "]/div["+ std::to_string(columun)+ "]/button";
    chrome.FindElement(ByXPath(baba_xpath)).Click();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    //レース番号選択
    std::string race_num_xpath="//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/select-course-race/div/div[2]/div[2]/div[4]/div[" + std::to_string(race_num)+ "]/button";
    chrome.FindElement(ByXPath(race_num_xpath)).Click();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    //opening the file
    std::string file_name = "real_time_odds";
    std::ofstream file("real_time_odds.txt");//file(file_name);

    //scraping tansyou odds
    for(int i=1;i<num_of_horses+1;i++){
        std::string odds_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-winplace-basic/div/div/table/tbody/tr[" +std::to_string(i) +"]/td[3]/button/odds-num/span/span";
        file << i << " " << chrome.FindElement(ByXPath(odds_xpath)).GetText() << std::endl;
        tansyou[i] = (double)std::stod(chrome.FindElement(ByXPath(odds_xpath)).GetText());
    }
    file << "\n";

    //scraping umaren
    std::string odds_type_change_xpath = "//*[@id=\"bet-odds-type\"]";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    chrome.FindElement(ByXPath(odds_type_change_xpath)).Click();
    chrome.FindElement(ByXPath(odds_type_change_xpath)).SendKeys(keys::Down);
    chrome.FindElement(ByXPath(odds_type_change_xpath)).SendKeys(keys::Down);
    chrome.FindElement(ByXPath(odds_type_change_xpath)).SendKeys(keys::Enter);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    for(int first = 1;first < num_of_horses+1;first++){
        for(int second=1;second<num_of_horses+1-first;second++){
            std::string odds_xpath="//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-quinella-basic/div/div/div[1]/div["+ std::to_string(first) +"]/div/div["+ std::to_string(second+1) +"]/div[2]/button/odds-num/span/span";
            file << first << "-" << first+second << " " << chrome.FindElement(ByXPath(odds_xpath)).GetText() << std::endl;
               baren[first][first+second] = std::stod(chrome.FindElement(ByXPath(odds_xpath)).GetText());
        }
    }
    std::cout << "\n";
        
    //scraping umatan
    std::this_thread::sleep_for(std::chrono::seconds(1));
    odds_type_change_xpath = "//*[@id=\"bet-odds-type\"]";
    chrome.FindElement(ByXPath(odds_type_change_xpath)).Click();
    chrome.FindElement(ByXPath(odds_type_change_xpath)).SendKeys(keys::Down);
    chrome.FindElement(ByXPath(odds_type_change_xpath)).SendKeys(keys::Down);
    chrome.FindElement(ByXPath(odds_type_change_xpath)).SendKeys(keys::Enter);
    std::this_thread::sleep_for(std::chrono::seconds(1));


    std::string odds;
    for(int first=1;first<num_of_horses+1;first++){
        for(int second=1;second<num_of_horses+1;second++){
            std::string odds_xpath="//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-exacta-basic/div/div/div[1]/div[" +std::to_string(first)+ "]/div/div["+std::to_string(second+1) +"]/div[2]/button/odds-num/span/span";
            file << first << ">" << second << " " << chrome.FindElement(ByXPath(odds_xpath)).GetText() << std::endl;
            odds = chrome.FindElement(ByXPath(odds_xpath)).GetText();
            if(odds!="---")
                batan[first][second] = std::stod(chrome.FindElement(ByXPath(odds_xpath)).GetText());
        }
    }
    file << "\n";

    //スレッドがジョインするのを待つ

    th0.join();
    th1.join();
    th2.join();
    th3.join();

    // th0.join();
    // th1.join();
    // th2.join();
    // th3.join();
    // th4.join();
    

    //まとめる。linuxコマンドをプログラムの中から呼び出している。
    std::system("cat real_time_odds0 >> real_time_odds.txt");
    std::system("cat real_time_odds1 >> real_time_odds.txt");
    std::system("cat real_time_odds2 >> real_time_odds.txt");
    std::system("cat real_time_odds3 >> real_time_odds.txt");



    

  // batan_optの計算
    std::vector<std::vector<bool>> f_batan(n+1, std::vector<bool>(n+1, false));   // := 馬単を選択したか
    double batan_opt[n+1][n+1];    // := 馬単と三連単の良いほう
    for(int i=1;i<n+1;i++){
        for(int j=1;j<n+1;j++){
            if(i == j)  continue;

            double sansum = 0.0;   // := 三連単の逆数の和
            for(int k=1;k<n+1;k++){
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
    for(int i=1;i<n+1;i++){
        double batansum = 0.0;  // := batan_optの和
        for(int j=1;j<n+1;j++){
            if(i == j)  continue;    
            batansum += 1.0/batan_opt[i][j];
        }

    if(1.0/batansum <= tansyou[i]){   // 短小選択
      tan_opt[i] = tansyou[i];
      f_tan[i] = true;
    }
    else{    // !tan
      tan_opt[i] = 1.0/batansum;
    }
  }



  // baren, btn_opt比較
  std::vector<std::vector<bool>> f_baren(n+1, std::vector<bool>(n+1, false));   // := btn_optとbrnでbrnのほうが高井か
  for(int i=1;i<n+1;i++){
    for(int j=i+1;j<n+1;j++){
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
      for(int j=0;j<nouse.size();j++){
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


//下位グループと上位グループに分ける
  std::set<int> upper_group;
  std::set<int> lower_group;
  for(int i=1;i<=N;i++){
      if(tansyou[i]>=odds_boundary){
          lower_group.insert(i);
      }
      else{
          upper_group.insert(i);
      }
  }

  int n_tansyou=0,n_umaren=0,n_umatan=0,n_santan=0;

    std::vector<sTansyou> Tansyou;
    std::vector<sUmaren> Umaren;
    std::vector<sUmatan> Umatan;
    std::vector<sSantan> Santan;
  

  // 買うやつ
    for(int i=1;i<n+1;i++){
      //下位グループであれば問答無用で単勝
        if(lower_group.count(i)){
            struct sTansyou new_tan1 = {i,tansyou[i],0};
            Tansyou.push_back(new_tan1);
            n_tansyou++;
            std::cout << i << " " << tansyou[i] << std::endl;
        }
        else{
            if(f_buy_tnpt[i]){      // tan_optを選択
                if(f_tan[i]){       // tanがbatan_opt よりも合成オッズが高い
                    std::cout << i << " " << tansyou[i] << std::endl;    // 短小を購入
                    struct sTansyou new_tan2 = {i,tansyou[i],0};
                    Tansyou.push_back(new_tan2);
                    n_tansyou++;
                }
                else{   // batan_optがtanよりも高い
                    for(int j=1;j<n+1;j++){
                        if(i == j)   continue;

                        if(f_batan[i][j]){    // batanがsantanよりも高井
                            std::cout << i << ">" << j << " " << batan[i][j] << std::endl;    // 馬単を購入
                            struct sUmatan new_umatan = {i,j,batan[i][j],0};
                            Umatan.push_back(new_umatan);
                            n_umatan++;
                        }
                        else{
                            for(int k=1;k<n+1;k++){
                                if(i == k || j == k)  continue;
                                std::cout << i << ">" << j << ">" << k << " " << santan[i][j][k] << std::endl;;   // santanを購入
                                struct sSantan new_santan = {i,j,k,santan[i][j][k],0};
                                Santan.push_back(new_santan);
                                n_santan++;
                            }
                        }
                    }
                }
            }
            else{   // baren混合を購入
                for(int j=1;j<n+1;j++){
                    if(i == j)  continue;

                    if(f_buy_brn[i][j]){        // barenを買う
                        if(i > j) continue;   // 重複対策
                        std::cout << i << "=" << j << " " << baren[i][j] << std::endl;
                        struct sUmaren new_umaren = {i,j,baren[i][j],0};
                        Umaren.push_back(new_umaren);
                        n_umaren++;
                    }
                else if(f_batan[i][j]){    // batanがsantanよりも高井
                    std::cout << i << ">" << j << " " << batan[i][j] << std::endl;    // 馬単を購入
                    struct sUmatan new_umatan = {i,j,batan[i][j],0};
                    Umatan.push_back(new_umatan);
                    n_umatan++;
                }
                else{
                    for(int k=1;k<n+1;k++){
                        if(i == k || j == k)  continue;
                        std::cout << i << ">" << j << ">" << k << " " << santan[i][j][k] << std::endl;;   // santanを購入
                        struct sSantan new_santan = {i,j,k,santan[i][j][k],0};
                        Santan.push_back(new_santan);
                        n_santan++;
                    }
                }
                }
            }
    
        }   
    }

	// 合成オッズ
  std::cout << "odds = " << odds_opt << std::endl;

  //ここまでで、合成オッズ的に考えて最適な馬券が選択された。しかし、１）確率を考えていない、２）下位の１になりそうにもない馬にも、歪みを利用して三連単で流しとかしてる＝溝に金を捨てる
  //可能性がクソ高い。
  //以上を踏まえてのストラテジーだが、

  //1)odds_boundary=15よりしたは全部単勝で買う(下位グループ）。
  //2)下位グループの合成オッズ＝lower_synth_oddsを計算
  //3)下位で均等配分（メビウスの方程式）分配源はdeposit
  //4)upper_money = 2*deposit*lower_synth_odds -deposit　　で上位にかける金を決める2倍にして、-depositしてるのは上位グループで引っかからなかったときにかけた金の半分が帰ってくるように
  //5)上位で、例えば18番人気が2着の三連単流しなどを消す。
  //6)上位合成オッズ= upper_synth_odds算出　＆＆　メビウスの方程式で均等配分
  //7)予想返金金額　expected_return = upper_money*upper_synth_odds*upper_probability(0.9) + lower_money*lower*synth_odds*lower_probability(0.1)　を算出
  //8)合計購入金額　purchase_price = upper_money + lower_moneyとして、
  //9) expected_return > purchase_priceなら、かける。

  lower_synth_odds = 0.0;//思い出しよう
  double lower_synth_odds_inv = 0; 
  double lower_groups_max_odds = 0;
  double minimum_money=0;
  double dominator=0;

  //下位グループの中で、単勝オッズが最大のものを探す
  for(auto itr=lower_group.begin();itr!=lower_group.end();itr++){
      if(tansyou[*itr]>lower_groups_max_odds){
          lower_groups_max_odds = tansyou[*itr];
      }
  }
  //基準となる最低金額を決定
  for(int i=0;i<n_tansyou;i++){
      if(lower_group.count(Tansyou[i].umaban)){
          dominator += lower_groups_max_odds/Tansyou[i].odds;
      }
  }
  minimum_money = deposit/dominator;
  //メビウスの方程式適用
  for(int i=0;i<n_tansyou;i++){
      //合成オッズの計算
      if(lower_group.count(Tansyou[i].umaban)){
          lower_synth_odds_inv += 1.0/(double)Tansyou[i].odds;
          int kingaku = my_ceil(minimum_money*lower_groups_max_odds/Tansyou[i].odds);
          Tansyou[i].money = kingaku;
          actual_deposit+=kingaku;
      }
      //
  }

  lower_synth_odds = 1.0/lower_synth_odds_inv;
  money_upper = challnage_index*actual_deposit*lower_synth_odds - actual_deposit;
  //ここで、絶対に来ないであろう馬を三連、馬連、馬単から抜いておく。特に三連から。しかし慎重に。
  //とりあえず飛ばす。
  //二着抜かし
  if(second_excluding.size()!=0){
  for(auto itr = Santan.begin();itr != Santan.end();){
      for(int j=0;j<second_excluding.size();j++){
          if(itr->umaban2 == second_excluding[j]){
              itr = Santan.erase(itr);
              n_santan--;
              break;
          }
          if(j==second_excluding.size()-1){
              ++itr;
          }
      }
  }

   for(auto itr = Umatan.begin();itr != Umatan.end();){
      for(int j=0;j<second_excluding.size();j++){
          if(itr->umaban2 == second_excluding[j]){
              itr = Umatan.erase(itr);
              n_umatan--;
              break;
          }
          if(j==second_excluding.size()-1){
              ++itr;
          }
      }
  }

    for(auto itr = Umaren.begin();itr != Umaren.end();){
      for(int j=0;j<second_excluding.size();j++){
          if(itr->umaban2 == second_excluding[j]){
              itr = Umaren.erase(itr);
              n_umaren--;
              break;
          }
          if(j==second_excluding.size()-1){
              ++itr;
          }
      }
  }
  }

  //一着外し
  if(first_excluding.size()!=0){
    for(auto itr = Santan.begin();itr != Santan.end();){
      for(int j=0;j<first_excluding.size();j++){
          if(itr->umaban1 == first_excluding[j]){
              itr = Santan.erase(itr);
              n_santan--;
              break;
          }
          if(j==first_excluding.size()-1){
              ++itr;
          }
      }
  }

   for(auto itr = Umatan.begin();itr != Umatan.end();){
      for(int j=0;j<first_excluding.size();j++){
          if(itr->umaban1 == first_excluding[j]){
              itr = Umatan.erase(itr);
              n_umatan--;
              break;
          }
          if(j==first_excluding.size()-1){
              ++itr;
          }
      }
  }

    for(auto itr = Umaren.begin();itr != Umaren.end();){
      for(int j=0;j<first_excluding.size();j++){
          if(itr->umaban1 == first_excluding[j]){
              itr = Umaren.erase(itr);
              n_umaren--;
              break;
          }
          if(j==first_excluding.size()-1){
              ++itr;
          }
      }
  }
  }


  //上位合成オッズの計算
  double upper_synth_odds_inv=0;
  double upper_group_max_odds=0;
  minimum_money=0;
    dominator=0;


  //基準金額を探す
  //tansyou
  for(int i=0;i<n_tansyou;i++){
      if(upper_group.count(Tansyou[i].umaban)){
          upper_synth_odds_inv+=1.0/Tansyou[i].odds;
      }
      if(Tansyou[i].odds>upper_group_max_odds){
          upper_group_max_odds=Tansyou[i].odds;
      }
  }


  //baren
  for(int i=0;i<n_umaren;i++){
      if(upper_group.count(Umaren[i].umaban1)){
          upper_synth_odds_inv+=1.0/Umaren[i].odds;
      }
      if(Umaren[i].odds>upper_group_max_odds){
          upper_group_max_odds=Umaren[i].odds;
      }
  }

  //batan
  for(int i=0;i<n_umatan;i++){
      if(upper_group.count(Umatan[i].umaban1)){
          upper_synth_odds_inv+=1.0/Umatan[i].odds;
      }
      if(Umatan[i].odds > upper_group_max_odds){
          upper_group_max_odds = Umatan[i].odds;
      }
  }

  //santan
  for(int i=0;i<n_santan;i++){
      if(upper_group.count(Santan[i].umaban1)){
          upper_synth_odds_inv+=1.0/Santan[i].odds;
          //std::cout << "segufo sagasi3.3.1" << std::endl; 
      }
      if(Santan[i].odds > upper_group_max_odds){
          upper_group_max_odds = Santan[i].odds;
      }
  }

  //基準金額の設定
    for(int i=0;i<n_tansyou;i++){
      if(upper_group.count(Tansyou[i].umaban)){
          dominator += upper_group_max_odds/Tansyou[i].odds;
      }
    }

  for(int i=0;i<n_umaren;i++){
      if(upper_group.count(Umaren[i].umaban1)){
          dominator += upper_group_max_odds/Umaren[i].odds;
      }

  }
  //batan
  for(int i=0;i<n_umatan;i++){
      if(upper_group.count(Umatan[i].umaban1)){
          dominator += upper_group_max_odds/Umatan[i].odds;
      }
  }
  //santan
  for(int i=0;i<n_santan;i++){
      if(upper_group.count(Santan[i].umaban1)){
          dominator += upper_group_max_odds/Santan[i].odds;
      }
  }
  std::cout << "money_upper=" << money_upper <<std::endl;
  std::cout << "dominator=" << dominator << std::endl;
  minimum_money = money_upper/dominator;
  std::cout << "minimum_money=" << minimum_money << std::endl;

  //メビウスの方程式

  //tansyou
  for(int i=0;i<n_tansyou;i++){
      //合成オッズの計算
      if(upper_group.count(Tansyou[i].umaban)){
          int kingaku = my_ceil(minimum_money*upper_group_max_odds/Tansyou[i].odds);
          Tansyou[i].money = kingaku;
          actual_upper_spent+=kingaku;
      }
      //
  }
  //umaren
  for(int i=0;i<n_umaren;i++){
      if(upper_group.count(Umaren[i].umaban1)){
          int kingaku = my_ceil(minimum_money*upper_group_max_odds/Umaren[i].odds);
          Umaren[i].money = kingaku;
          actual_upper_spent += kingaku;
      }
  }
  //batan
  for(int i=0;i<n_umatan;i++){
      if(upper_group.count(Umatan[i].umaban1)){
          int kingaku = my_ceil(minimum_money*upper_group_max_odds/Umatan[i].odds);
          Umatan[i].money = kingaku;
          actual_upper_spent += kingaku;
      }
  }
  //sanren
  for(int i=0;i<n_santan;i++){
      if(upper_group.count(Santan[i].umaban1)){
          int kingaku = my_ceil(minimum_money*upper_group_max_odds/Santan[i].odds);
          Santan[i].money = kingaku;
          actual_upper_spent += kingaku;
      }
  }
  

  //単勝
  std::cout << "tansyou" << std::endl;
  for(auto i = Tansyou.begin();i!=Tansyou.end();i++){
      std::cout << i-> umaban << " " << i->odds << " " << i->money << std::endl;
  }

  bool henkou_flag=false;
  //baren
  std::cout << "baren" << std::endl;
  for(auto i= Umaren.begin();i!=Umaren.end();i++){
      if(lower_group.count(i->umaban1)){
          //馬連の購入馬券の中に下位グループのものが含まれている。単勝に変更
          //i = Umaren.erase(i);
      }
      else{
          //そのままでおｋ
      }
      std::cout << i-> umaban1 << "-" << i-> umaban2 << " " << i->odds << " " << i->money << std::endl; 
  }

  //batan
    std::cout << "batan" << std::endl;
    for(auto i= Umatan.begin();i!=Umatan.end();i++){
        if(lower_group.count(i->umaban1)){

        }
        else{

        }
        std::cout << i-> umaban1 << ">" <<i-> umaban2 << " " << i->odds << " " << i->money << std::endl; 
    }

    //sanrentan
    std::cout << "sanrentan" << std::endl;
    for(auto i= Santan.begin();i!=Santan.end();i++){
        if(lower_group.count(i->umaban1)){

        }
        std::cout << i-> umaban1 << ">" <<i-> umaban2 << ">" << i->umaban3 <<" " << i->odds << " " << i->money << std::endl; 
    }

    //情報出力
    std::cout << "lower_synth_odds=" << lower_synth_odds << std::endl;
    std::cout << "expected_lower_return = " << actual_deposit*lower_synth_odds*lower_probability << " Yen" << std::endl;
    std::cout << "upper_monery=" << money_upper <<" Yen" << std::endl; 
    std::cout << "actual_upper_spent=" << actual_upper_spent << " yen" << std::endl;
    purchase_price = actual_upper_spent + actual_deposit;
    std::cout << "total money spent = " << purchase_price << " yen" << std::endl;
    upper_synth_odds = 1.0/upper_synth_odds_inv;
    std::cout << "upper_synth_odds=" << upper_synth_odds << std::endl;
    expected_return = actual_upper_spent*upper_synth_odds*upper_probability + actual_deposit*lower_synth_odds*lower_probability;
    std::cout << "expected_return= " << expected_return << " yen" << std::endl;
    recovery_rate = expected_return/(double)purchase_price;
    std::cout << "recovery_rate = " << recovery_rate  << std::endl;
    //

    //ここからは、購入フェーズです。注意点としては、一度の購入で50馬券までしか買えないため、個々もマルチスレッド化するのがいい気がします。
    //具体的には、単勝購入スレッド*1、馬連購入スレッド＊１、馬単購入スレッド*１,三連単購入スレッド*2とかがいいかなと。
    return 0;

}
