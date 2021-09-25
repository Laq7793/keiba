from builtins import range
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from time import sleep
import urllib.request, urllib.error
from math import e, factorial
import sys
import os

#ログイン情報は環境変数で受け取る（セキュリティー向上の為）
#暗証番号だけは自分で入力

P_ARS = os.environ['P_ARS']
INET_ID= os.environ['INET_ID']
USER_NUM = os.environ['USER_NUM']
PIN = #入れてね
print(P_ARS)


#initializeing
driver = webdriver.Chrome('/home/ray/programing/c++/webdriverxx/chromedriver')
root_url = "https://www.ipat.jra.go.jp/"
driver.get(root_url)

#inputting required information
login_text_box_xpath = "//*[@id=\"top\"]/div[3]/div/table/tbody/tr/td[2]/div/div/form/table[1]/tbody/tr/td[2]/span/input"
click_button_xpath = "//*[@id=\"top\"]/div[3]/div/table/tbody/tr/td[2]/div/div/form/table[1]/tbody/tr/td[3]/p/a"
driver.find_element_by_xpath(login_text_box_xpath).send_keys(INET_ID)
driver.find_element_by_xpath(click_button_xpath).click()

#inputting the required information
user_num_input_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[1]/td[2]/span/input"
pin_input_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[2]/td[2]/span/input"
p_ars_input_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[3]/td[2]/span/input"
click_button_xpath = "//*[@id=\"main_area\"]/div/div[1]/table/tbody/tr[1]/td[3]/p/a"
driver.find_element_by_xpath(user_num_input_xpath).send_keys(USER_NUM)
driver.find_element_by_xpath(pin_input_xpath).send_keys(PIN)
driver.find_element_by_xpath(p_ars_input_xpath).send_keys(P_ARS)
driver.find_element_by_xpath(click_button_xpath).click()

#ここまででログイン完了。続いてオッズ投票へ遷移
sleep(1)
#このスリープを入れないとプログラムが早すぎてエラーが起こる
odds_vote_xpath = "//*[@id=\"main\"]/ui-view/main/div[2]/div[1]/div[2]/button"
driver.find_element_by_xpath(odds_vote_xpath).click()

#馬場選択（ブラウザで見ながら選択）
print("row=")
row = int(input())
print("column=")
columun = int(input())

baba_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/select-course-race/div/div[2]/div[2]/div[2]/div[" + str(row) + "]/div["+ str(columun)+ "]/button"
driver.find_element_by_xpath(baba_xpath).click()

#race選択
print("race_num=")
race_num = int(input())
race_num_xpath="//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/select-course-race/div/div[2]/div[2]/div[4]/div[" + str(race_num)+ "]/button"
driver.find_element_by_xpath(race_num_xpath).click()

#まずは目視で馬の数を確認して、それの入力をお願いしたい。
print("num_of_horses=")
num_of_horses=int(input())

#ファイルを開く。
file_name = "./real_time_odds"
file = open(file_name,"w")

#単勝のスクレイピング
for row in range(1,num_of_horses+1):
    #//*[@id="main"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-winplace-basic/div/div/table/tbody/tr[1]/td[3]/button/odds-num/span/span
    odds_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-winplace-basic/div/div/table/tbody/tr[" +str(row) +"]/td[3]/button/odds-num/span/span"
    file.write(str(row))
    file.write(" ")
    file.write(driver.find_element_by_xpath(odds_xpath).text)
    file.write("\n")


file.write("\n")
sleep(1)
#馬券タイプ遷移
#下方向キーを二回押して馬連に遷移
odds_type_change_xpath = "//*[@id=\"bet-odds-type\"]"
driver.find_element_by_xpath(odds_type_change_xpath).click()
driver.find_element_by_xpath(odds_type_change_xpath).send_keys(Keys.DOWN)
driver.find_element_by_xpath(odds_type_change_xpath).send_keys(Keys.DOWN)
driver.find_element_by_xpath(odds_type_change_xpath).send_keys(Keys.ENTER)

#馬連のスクレイピング
sleep(1)
for first in range(1,num_of_horses+1):
    for second in range(1,num_of_horses+1-(first)):
        odds_xpath="//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-quinella-basic/div/div/div[1]/div["+ str(first) +"]/div/div["+ str(second+1) +"]/div[2]/button/odds-num/span/span"
        file.write(str(first))
        file.write("-")
        file.write(str(first+second))
        file.write(" ")
        file.write(driver.find_element_by_xpath(odds_xpath).text)
        file.write("\n")

file.write("\n")
#馬単のスクレイピング

odds_type_change_xpath = "//*[@id=\"bet-odds-type\"]"
driver.find_element_by_xpath(odds_type_change_xpath).click()
driver.find_element_by_xpath(odds_type_change_xpath).send_keys(Keys.DOWN)
driver.find_element_by_xpath(odds_type_change_xpath).send_keys(Keys.DOWN)
driver.find_element_by_xpath(odds_type_change_xpath).send_keys(Keys.ENTER)
sleep(1)
for first in range(1,num_of_horses+1):
    for second in range(1,num_of_horses+1):
        odds_xpath="//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-exacta-basic/div/div/div[1]/div[" +str(first)+ "]/div/div["+str(second+1) +"]/div[2]/button/odds-num/span/span"
        file.write(str(first))
        file.write(">")
        file.write(str(second))
        file.write(" ")
        file.write(driver.find_element_by_xpath(odds_xpath).text)
        file.write("\n")

#三連単
odds_type_change_xpath = "//*[@id=\"bet-odds-type\"]"
driver.find_element_by_xpath(odds_type_change_xpath).click()
driver.find_element_by_xpath(odds_type_change_xpath).send_keys(Keys.DOWN)
driver.find_element_by_xpath(odds_type_change_xpath).send_keys(Keys.DOWN)
driver.find_element_by_xpath(odds_type_change_xpath).send_keys(Keys.ENTER)
sleep(1)

for first in range(1,num_of_horses+1):
    if(first!=1):
        selector_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/div[2]/div[2]/div[2]/select"
        driver.find_element_by_xpath(selector_xpath).click()
        driver.find_element_by_xpath(selector_xpath).send_keys(Keys.DOWN)
        driver.find_element_by_xpath(selector_xpath).send_keys(Keys.ENTER)
        sleep(1)
    for second in range(1,num_of_horses):
        nichaku_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-trifecta-basic/div/div/div[2]/div[" +str(second) +"]/div[2]/div[1]"
        for third in range(1,num_of_horses):
            sanchaku_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-trifecta-basic/div/div/div[2]/div[1]/div[2]/div["+ str(third+1)+"]/div[1]"
            odds_xpath = "//*[@id=\"main\"]/ui-view/div[2]/ui-view/ui-view/main/div/span/span/bet-odds-type-trifecta-basic/div/div/div[2]/div["+str(second)+"]/div[2]/div["+str(third+1)+"]/div[2]/button/odds-num/span/span"
            file.write(str(first))
            file.write(">")
            file.write(driver.find_element_by_xpath(nichaku_xpath).text)
            file.write(">")
            file.write(driver.find_element_by_xpath(sanchaku_xpath).text)
            file.write(" ") 
            file.write(driver.find_element_by_xpath(odds_xpath).text)
            file.write("\n")
