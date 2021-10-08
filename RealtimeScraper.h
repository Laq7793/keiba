#include<iostream>
#include<vector>
#include<cstdlib>
#include<fstream>
#include<webdriverxx.h>
#include<webdriverxx/browsers/chrome.h>
#include "baken_format.h"
#include<thread>
#include<chrono>



std::string P_ARS = getenv("P_ARS");
std::string INET_ID = getenv("INET_ID");
std::string USER_NUM = getenv("USER_NUM");
std::string PIN = getenv("PIN");
std::string root_url = "https://www.ipat.jra.go.jp/";

using namespace webdriverxx;

void TanBatanRenScraper(int num_of_horses,std::vector<std::vector<double>>& batan,std::vector<std::vector<double>>& baren,std::vector<double>& tansyou,bool fswitch,int columun,int row,int race_num)
{
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
    std::this_thread::sleep_for(std::chrono::seconds(2));

    //期間限定の画面
    ////*[@id="main"]/ui-view/div[2]/div[2]/button
    ////*[@id="main"]/ui-view/div[2]/div[2]/button
    std::string xpaht = "//*[@id=\"main\"]/ui-view/div[2]/div[2]/button";
    chrome.FindElement(ByXPath(xpaht)).Click();
    std::this_thread::sleep_for(std::chrono::seconds(2));
        
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
        if(fswitch)
        {
            file << i << " " << chrome.FindElement(ByXPath(odds_xpath)).GetText() << std::endl;
        }
        tansyou[i] = (double)std::stod(chrome.FindElement(ByXPath(odds_xpath)).GetText());
    }
    if(fswitch)
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
            
            if(fswitch)
            {
                file << first << "-" << first+second << " " << chrome.FindElement(ByXPath(odds_xpath)).GetText() << std::endl;
            }
            baren[first][first+second] = std::stod(chrome.FindElement(ByXPath(odds_xpath)).GetText());
        }
    }
    if(fswitch)
        file << "\n";
        
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
            
            if(fswitch)
            {
                file << first << ">" << second << " " << chrome.FindElement(ByXPath(odds_xpath)).GetText() << std::endl;
            }
            odds = chrome.FindElement(ByXPath(odds_xpath)).GetText();
            if(odds!="---")
                batan[first][second] = std::stod(chrome.FindElement(ByXPath(odds_xpath)).GetText());
        }
    }
    file.close();
}


void SantanScraper(int worker_id,int row,int columun,int race_num,int step,int mod,int num_of_horses,std::vector<std::vector<std::vector<double>>>& santan,int time,bool fswitch){
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

        //期間限定
        
        std::string xpaht = "//*[@id=\"main\"]/ui-view/div[2]/div[2]/button";
        ////*[@id="main"]/ui-view/div[2]/div[2]/button
        chrome.FindElement(ByXPath(xpaht)).Click();
        std::this_thread::sleep_for(std::chrono::seconds(2));

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
                    if(fswitch)
                    {
                        file << first << ">" << chrome.FindElement(ByXPath(nichaku_xpath)).GetText()
                        <<">" << chrome.FindElement(ByXPath(sanchaku_xpath)).GetText()
                        << " " << chrome.FindElement(ByXPath(odds_xpath)).GetText() << std::endl;
                    }
                    
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


int ThreadNumOpt(int num_of_horses){
    if(num_of_horses==5||num_of_horses==10||num_of_horses==15||num_of_horses==11)
    {
        return 5;
    }
    else if(num_of_horses==6||num_of_horses==12||num_of_horses==13)
    {
        return 6;
    }
    else if(num_of_horses==7||num_of_horses==14)
    {
        return 7;
    }
    else if(num_of_horses==8||num_of_horses==16||num_of_horses==17)
    {
        return 8;
    }
    else if(num_of_horses==9||num_of_horses==18)
    {
        return 9;
    }
    else
    {
        return -1;
    }
}
