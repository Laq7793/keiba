#include<webdriverxx.h>
#include<webdriverxx/browsers/chrome.h>
#include<chrono>
#include<thread>
#include<cstdlib>
#include<fstream>

using namespace webdriverxx;

#define NUM_OF_THREADS 4

//login information
std::string P_ARS = getenv("P_ARS");
std::string INET_ID = getenv("INET_ID");
std::string USER_NUM = getenv("USER_NUM");
std::string PIN = getenv("PIN");
std::string root_url = "https://www.ipat.jra.go.jp/";

//race information
#define N 16 //馬数
int row=1;
int columun=1;
int race_num=10;
int num_of_horses=N;
int mod=num_of_horses%NUM_OF_THREADS;
int step=num_of_horses/NUM_OF_THREADS;

//money information
int capital_money=100000;



//odds array
int n = num_of_horses;
double tansyou[N+1], batan[N+1][N+1], santan[N+1][N+1][N+1];		// sntn[i][j][k] := i>j>k のオッズ
double baren[N+1][N+1], sanren[N+1][N+1][N+1];   // brn[i][j] := i-j のオッズ
std::string unchi, unpip;       // := unchi, unpip
double v_ini = 999999.9;   //   初期化の値

//worker thread (sanrantan scraper)
void worker(int worker_id,int mod){
    WebDriver chrome = Start(Chrome());
    chrome.Navigate(root_url);
        //.FindElement(ByCss("input[name=q]"))
        /*.SendKeys("Hello, world!")
        .Submit();*/
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
        
        //selecting the race place
        /*std::cout << "row=";
        int row = 1;//int(input())
        std::cout << "column=";
        int columun = 1;//int(input())
        */
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::string baba_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/select-course-race/div/div[2]/div[2]/div[2]/div[" + std::to_string(row) + "]/div["+ std::to_string(columun)+ "]/button";
        chrome.FindElement(ByXPath(baba_xpath)).Click();

        std::this_thread::sleep_for(std::chrono::seconds(1));
        //selecting the race num
        /*std::cout <<"race_num=";
        int race_num = 4;
        */
        std::string race_num_xpath="//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/select-course-race/div/div[2]/div[2]/div[4]/div[" + std::to_string(race_num)+ "]/button";
        chrome.FindElement(ByXPath(race_num_xpath)).Click();

        //inputing the horse number
        /*std::cout << "num_of_horses=";
        int num_of_horses=16;
        */
        //std::cin >> num_of_horses;

        std::this_thread::sleep_for(std::chrono::seconds(1));
        //opening the file
        std::string file_name = "real_time_odds"+std::to_string(worker_id);
        std::ofstream file(file_name);//file(file_name);
        
        //scraping sanrentan
        //まずは移動しないといけない
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
                ////*[@id="main"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-trifecta-basic/div/div/div[2]/div[1]/div[2]/div[1]
                ////*[@id="main"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-trifecta-basic/div/div/div[2]/div[2]/div[2]/div[1]
                std::string nichaku_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-trifecta-basic/div/div/div[2]/div[" +std::to_string(second) +"]/div[2]/div[1]";
                for(int third=1;third<num_of_horses;third++){
                    ////*[@id="main"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-trifecta-basic/div/div/div[2]/div[1]/div[2]/div[2]/div[1]
                    ////*[@id="main"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-trifecta-basic/div/div/div[2]/div[1]/div[2]/div[3]/div[1]
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

    //いきなりmultithread化。三連単をworkerに回収してもらう
    std::thread th0(worker,0,0);
    std::thread th1(worker,1,0);
    std::thread th2(worker,2,0);
    std::thread th3(worker,3,mod);
    WebDriver chrome = Start(Chrome());
    chrome.Navigate(root_url);
        //.FindElement(ByCss("input[name=q]"))
        /*.SendKeys("Hello, world!")
        .Submit();*/
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

        //まとめる。
        std::system("cat real_time_odds0 >> real_time_odds.txt");
        std::system("cat real_time_odds1 >> real_time_odds.txt");
        std::system("cat real_time_odds2 >> real_time_odds.txt");
        std::system("cat real_time_odds3 >> real_time_odds.txt");

        
        //ここからは、計算フェーズです。
/*
         // 初期化
  for(i=0;i<n+1;i++){
    tansyou[i] = v_ini;
    //fuku[i] = v_ini;
    for(j=0;j<n+1;j++){
      batan[i][j] = v_ini;
      baren[i][j] = v_ini;
      //wide[i][j] = v_ini;
      for(k=0;k<n+1;k++){
        santan[i][j][k] = v_ini;
        //sanren[i][j][k] = v_ini;
      }
    }
  }
*/





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

  // 買うやつ
  for(int i=1;i<n+1;i++){

    if(f_buy_tnpt[i]){      // tan_optを選択
      if(f_tan[i]){       // tanがbatan_opt よりも合成オッズが高い
        std::cout << i << " " << tansyou[i] << std::endl;    // 短小を購入
      }

      else{   // batan_optがtanよりも高い
        for(int j=1;j<n+1;j++){
          if(i == j)   continue;

          if(f_batan[i][j]){    // batanがsantanよりも高井
            std::cout << i << ">" << j << " " << batan[i][j] << std::endl;    // 馬単を購入
          }
          else{
            for(int k=1;k<n+1;k++){
              if(i == k || j == k)  continue;
              std::cout << i << ">" << j << ">" << k << " " << santan[i][j][k] << std::endl;;   // santanを購入
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
        }
        else if(f_batan[i][j]){    // batanがsantanよりも高井
          std::cout << i << ">" << j << " " << batan[i][j] << std::endl;    // 馬単を購入
        }
        else{
          for(int k=1;k<n+1;k++){
            if(i == k || j == k)  continue;
            std::cout << i << ">" << j << ">" << k << " " << santan[i][j][k] << std::endl;;   // santanを購入
          }
        }
      }
    }
  }

	// 合成オッズ
  std::cout << "odds = " << odds_opt << std::endl;


        //ここからは、購入フェーズです。
    return 0;
}
