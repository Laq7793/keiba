from selenium import webdriver
from time import sleep
import urllib.request, urllib.error
from math import e, factorial
import sys


def comb(n,r):
    return factorial(n)/factorial(r)/factorial(n-r)


#存在チェック
def checkURL(url):
    try:
        f = urllib.request.urlopen(url)
        print ("OK:" + url )
        f.close()
        return True
    except:
        print ("NotFound:" + url)
        return False


#開催地リスト（よくあるところだけ）
place_list = [1,4,6,7,9,10]
#1:sapporo 4;nigata 6:nakayama 7:tyukyou 9:hansin 10:kokura



driver = webdriver.Chrome('/home/ray/programing/c++/webdriverxx/chromedriver')
root_url = "https://www.keibalab.jp/db/race/"
#url += "/" + "odds.html"
#opening the site
driver.get(root_url)

#サイト調べた結果、2013年以前のデータにはodds.htmlが存在しないことが判明
for year in range(2011,2022):
    for month in range(2,12):
        for day in range(1,31):
            for place in place_list:
                for race in range(1,12):

                    file_name = str(year) + str(month).zfill(2) + str(day).zfill(2) + str(place).zfill(2) + str(race).zfill(2)
                    url = root_url + file_name + "/" + "odds.html"
                    #あまりアクセスが早いと出禁喰らいます。スクレイピングは基本的に寝てる間にやってもらうことにすること！
                    sleep(1)

                    if(True):
                        #uma no kazuwo check
                        try:

                            driver.get(url)
                            
                            file_directry = "./past_odds_data/" + file_name
                            

                            #馬の数
                            horses_txt = str(driver.find_element_by_xpath("//*[@id=\"tab1\"]/div[2]/div[1]/div[1]/div[1]/div[2]/ul/li[2]").text)
                            horses_where_m = horses_txt.rfind('m')
                            horses_where_tou = horses_txt.rfind('頭')
                            horses = int(horses_txt[horses_where_m+1:horses_where_tou])
                            #例外が起きるときは、馬の数をxpathでゲット仕様としたけどうまく行かないときなので、ここでyesを待ち構えないとだめっす。ファイルを開くのもここで
                            print("yes!",url)
                            file = open(file_directry,'w')
                            file.write(str(horses))
                            file.write("\n")

                            #tansyou_fukusyou（ちなみに複勝は3着までにはいる馬を当てる馬券です。
                            
                            
                            
                            #馬番号、単勝、複勝
                            buchikomi = []
                            file.write("\ntanpuku_wakuren\n\n")
                            #範囲に注意。range(i,j)はi-(j-1)
                            for row in range(1,horses+1):
                                umaban = "//*[@id=\"oddsTanTable\"]/tbody/tr["+ str(row) +"]/td[2]"
                                #//*[@id="oddsTanTable"]/tbody/tr[1]/td[1]
                                #//*[@id="oddsTanTable"]/tbody/tr[2]/td[2]
                                tansyou = "//*[@id=\"oddsTanTable\"]/tbody/tr[" + str(row) + "]/td[" + str(4) + "]"
                                fukusyou = "//*[@id=\"oddsTanTable\"]/tbody/tr[" + str(row) + "]/td[5]/i"
                                #buchikomi[column] = driver.find_element_by_xpath(xpath)
                                file.write(driver.find_element_by_xpath(umaban).text)
                                file.write(" ")
                                file.write(driver.find_element_by_xpath(tansyou).text)
                                file.write(" ")
                                file.write(driver.find_element_by_xpath(fukusyou).text)
                                file.write("\n")

                            
                            #馬単
                            url_batan = url+"?kind=utan"

                            driver.get(url_batan)
                            file.write("\nbatan\n")
                            print("getting batan")
                            #フォーメーションボタン、クリック
                            driver.find_element_by_xpath("//*[@id=\"utanSection\"]/ul/li[5]").click()
                            #//*[@id="utanSection"]/ul/li[5]
                            #//*[@id="utanSection"]/ul/li[5]
                            #チェックボックスのクリック
                            column_list = [3,4]
                            for row in range(1,horses+1):
                                for column in column_list:
                                    tmp = "//*[@id=\"utanSection\"]/div[2]/section[5]/div[1]/table/tbody/tr[" +str(row) +"]/td[ " + str(column) +  "]/input"
                                    driver.find_element_by_xpath(tmp).click()
                            
                            #表示ボタンクリック
                            driver.find_element_by_xpath("//*[@id=\"utanSection\"]/div[2]/section[5]/div[1]/div/div/a").click()
                            print("clicked 表示ボタン")

                            #組み合わせが表示されている
                            c_total = driver.find_element_by_xpath("//*[@id=\"utanSection\"]/div[2]/section[5]/div[2]/table/thead/tr[1]/th[2]").text
                            c_total = int(c_total)

                            #組み合わせ数先に表示してあげるという優しさ
                            file.write(str(c_total))
                            file.write("\n")

                            #表のパース
                            for row in range(1,c_total+1):
                                tmp1 = "//*[@id=\"utanSection\"]/div[2]/section[5]/div[2]/table/tbody/tr["+ str(row)+ "]/td[1]/i["+ str(1) +"]"
                                tmp2 = "//*[@id=\"utanSection\"]/div[2]/section[5]/div[2]/table/tbody/tr["+ str(row)+ "]/td[1]/i["+ str(2) +"]"
                                odds = "//*[@id=\"utanSection\"]/div[2]/section[5]/div[2]/table/tbody/tr["+ str(row)+"]/td[2]/i"
                                file.write(driver.find_element_by_xpath(tmp1).text)
                                file.write(">")
                                file.write(driver.find_element_by_xpath(tmp2).text)
                                file.write(" ")
                                file.write(driver.find_element_by_xpath(odds).text)
                                file.write("\n")
                            
                            #三連単、行ってみよう
                            url_sanrentan = url + "?kind=trif"
                            driver.get(url_sanrentan)
                            file.write("\nsanrentan\n")
                            print("gettin sanrentan\n")

                            #フォーメーションボタンクリック
                            driver.find_element_by_xpath("//*[@id=\"trifSection\"]/ul/li[4]").click()

                            #チェックボックス全部押し
                            column_list = [3,4,5]
                            for row in range(1,horses+1):
                                for column in column_list:
                                    tmp = "//*[@id=\"trifSection\"]/div[2]/section[4]/div[1]/table/tbody/tr[" +str(row)+"]/td["+ str(column) +"]/input"
                                    driver.find_element_by_xpath(tmp).click()
                            
                            #表示ボタン押し
                            driver.find_element_by_xpath("//*[@id=\"trifSection\"]/div[2]/section[4]/div[1]/div/div/a").click()
                            
                            #組み合わせ数取得
                            c_total = driver.find_element_by_xpath("//*[@id=\"trifSection\"]/div[2]/section[4]/div[2]/table/thead/tr[1]/th[2]").text
                            print(c_total)
                            c_total = int(c_total)

                            #組み合わせ数先に表示してあげるという優しさ
                            file.write(str(c_total))
                            file.write("\n")
                            
                            #パース
                            for row in range(1,c_total+1):
                                tmp1 = "//*[@id=\"trifSection\"]/div[2]/section[4]/div[2]/table/tbody/tr["+str(row)+"]/td[1]/i[1]"
                                tmp2 = "//*[@id=\"trifSection\"]/div[2]/section[4]/div[2]/table/tbody/tr["+str(row)+"]/td[1]/i[2]"
                                tmp3 = "//*[@id=\"trifSection\"]/div[2]/section[4]/div[2]/table/tbody/tr["+str(row)+"]/td[1]/i[3]"
                                odds = "//*[@id=\"trifSection\"]/div[2]/section[4]/div[2]/table/tbody/tr[" +str(row)+ "]/td[2]/i"
                                file.write(driver.find_element_by_xpath(tmp1).text)
                                file.write(">")
                                file.write(driver.find_element_by_xpath(tmp2).text)
                                file.write(">")
                                file.write(driver.find_element_by_xpath(tmp3).text)
                                file.write(" ")
                                file.write(driver.find_element_by_xpath(odds).text)
                                file.write("\n")


                            
                            #baren
                            url_baren = url + "?kind=uren"
                            driver.get(url_baren)
                            file.write("\nbaren\n")
                            print("getting baren")
                            #フォーメーションボタン、クリック
                            driver.find_element_by_xpath("//*[@id=\"urenSection\"]/ul/li[5]").click()
                            #//*[@id="urenSection"]/ul/li[5]
                            #チェックボックスのクリック
                            column_list = [3,4]
                            for row in range(1,horses+1):
                                for column in column_list:
                                    tmp = "//*[@id=\"urenSection\"]/div[2]/section[5]/div[1]/table/tbody/tr[" +str(row) +"]/td[ " + str(column) +  "]/input"
                                    driver.find_element_by_xpath(tmp).click()
                            
                            #表示ボタンクリック
                            driver.find_element_by_xpath("//*[@id=\"urenSection\"]/div[2]/section[5]/div[1]/div/div/a").click()
                            print("clicked 表示ボタン")

                            c_total = driver.find_element_by_xpath("//*[@id=\"urenSection\"]/div[2]/section[5]/div[2]/table/thead/tr[1]/th[2]").text
                            c_total = int(c_total)

                            #組み合わせ数先に表示してあげるという優しさ
                            file.write(str(c_total))
                            file.write("\n")

                            #表のパース
                            for row in range(1,c_total+1):
                                tmp1 = "//*[@id=\"urenSection\"]/div[2]/section[5]/div[2]/table/tbody/tr["+ str(row)+ "]/td[1]/i["+ str(1) +"]"
                                tmp2 = "//*[@id=\"urenSection\"]/div[2]/section[5]/div[2]/table/tbody/tr["+ str(row)+ "]/td[1]/i["+ str(2) +"]"
                                odds = "//*[@id=\"urenSection\"]/div[2]/section[5]/div[2]/table/tbody/tr["+ str(row)+"]/td[2]/i"
                                file.write(driver.find_element_by_xpath(tmp1).text)
                                file.write("-")
                                file.write(driver.find_element_by_xpath(tmp2).text)
                                file.write(" ")
                                file.write(driver.find_element_by_xpath(odds).text)
                                file.write("\n")
                           
                            

                            

                            #三連複

                            url_sanrenpuku = url + "?kind=trio"
                            driver.get(url_sanrenpuku)
                            file.write("\nsanrenpuku\n")
                            print("gettin sanrenpuku\n")

                            #フォーメーションボタンクリック
                            driver.find_element_by_xpath("//*[@id=\"trioSection\"]/ul/li[4]").click()

                            #チェックボックス全部押し
                            column_list = [3,4,5]
                            for row in range(1,horses+1):
                                for column in column_list:
                                    tmp = "//*[@id=\"trioSection\"]/div[2]/section[4]/div[1]/table/tbody/tr[" +str(row)+"]/td["+ str(column) +"]/input"
                                    driver.find_element_by_xpath(tmp).click()
                            
                            #表示ボタン押し
                            driver.find_element_by_xpath("//*[@id=\"trioSection\"]/div[2]/section[4]/div[1]/div/div/a").click()
                            
                            #組み合わせ数取得
                            c_total = driver.find_element_by_xpath("//*[@id=\"trioSection\"]/div[2]/section[4]/div[2]/table/thead/tr[1]/th[2]").text
                            c_total = int(c_total)

                            #組み合わせ数先に表示してあげるという優しさ
                            file.write(str(c_total))
                            file.write("\n")

                            #パース
                            for row in range(1,c_total+1):
                                tmp1 = "//*[@id=\"trioSection\"]/div[2]/section[4]/div[2]/table/tbody/tr["+str(row)+"]/td[1]/i[1]"
                                tmp2 = "//*[@id=\"trioSection\"]/div[2]/section[4]/div[2]/table/tbody/tr["+str(row)+"]/td[1]/i[2]"
                                tmp3 = "//*[@id=\"trioSection\"]/div[2]/section[4]/div[2]/table/tbody/tr["+str(row)+"]/td[1]/i[3]"
                                odds = "//*[@id=\"trioSection\"]/div[2]/section[4]/div[2]/table/tbody/tr[" +str(row)+ "]/td[2]/i"
                                file.write(driver.find_element_by_xpath(tmp1).text)
                                file.write("-")
                                file.write(driver.find_element_by_xpath(tmp2).text)
                                file.write("-")
                                file.write(driver.find_element_by_xpath(tmp3).text)
                                file.write(" ")
                                file.write(driver.find_element_by_xpath(odds).text)
                                file.write("\n")



                            #最後にワイドーー！！これもいつか使うときが来ると信じて。

                            url_wide = url + "?kind=wide"
                            driver.get(url_wide)
                            file.write("\nwide\n")
                            print("getting wide\n")

                            #フォーメーションボタンクリック
                            driver.find_element_by_xpath("//*[@id=\"wideSection\"]/ul/li[5]").click()
                            #//*[@id="wideSection"]/ul/li[5]

                            #チェックボックス全部押し
                            column_list = [3,4]
                            #//*[@id="wideSection"]/div[2]/section[5]/div[1]/table/tbody/tr[1]/td[3]/input
                            #//*[@id="wideSection"]/div[2]/section[5]/div[1]/table/tbody/tr[1]/td[4]/input
                            for row in range(1,horses+1):
                                for column in column_list:
                                    tmp = "//*[@id=\"wideSection\"]/div[2]/section[5]/div[1]/table/tbody/tr[" +str(row)+"]/td["+ str(column) +"]/input"
                                    driver.find_element_by_xpath(tmp).click()
                            
                            #表示ボタン押し
                            driver.find_element_by_xpath("//*[@id=\"wideSection\"]/div[2]/section[5]/div[1]/div/div/a").click()
                            #//*[@id="wideSection"]/div[2]/section[5]/div[1]/div/div/a
                            #組み合わせ数取得
                            #//*[@id="wideSection"]/div[2]/section[5]/div[2]/table/thead/tr[1]/th[2]
                            c_total = driver.find_element_by_xpath("//*[@id=\"wideSection\"]/div[2]/section[5]/div[2]/table/thead/tr[1]/th[2]").text
                            c_total = int(c_total)

                            #組み合わせ数先に表示してあげるという優しさ
                            file.write(str(c_total))
                            file.write("\n")

                            #//*[@id="wideSection"]/div[2]/section[5]/div[2]/table/tbody/tr[1]/td[1]/i[1]
                            #//*[@id="wideSection"]/div[2]/section[5]/div[2]/table/tbody/tr[1]/td[1]/i[2]
                            #//*[@id="wideSection"]/div[2]/section[5]/div[2]/table/tbody/tr[1]/td[2]/i[3]
                            #//*[@id="wideSection"]/div[2]/section[5]/div[2]/table/tbody/tr[1]/td[2]/i[3]
                            #//*[@id="wideSection"]/div[2]/section[5]/div[2]/table/tbody/tr[5]/td[2]/i[3]
                            #パース
                            for row in range(1,c_total+1):
                                tmp1 = "//*[@id=\"wideSection\"]/div[2]/section[5]/div[2]/table/tbody/tr["+str(row)+"]/td[1]/i[1]"
                                tmp2 = "//*[@id=\"wideSection\"]/div[2]/section[5]/div[2]/table/tbody/tr["+str(row)+"]/td[1]/i[2]"
                                odds = "//*[@id=\"wideSection\"]/div[2]/section[5]/div[2]/table/tbody/tr[" +str(row)+ "]/td[2]/i[3]"
                                file.write(driver.find_element_by_xpath(tmp1).text)
                                file.write("-")
                                file.write(driver.find_element_by_xpath(tmp2).text)
                                file.write(" ")
                                file.write(driver.find_element_by_xpath(odds).text)
                                file.write("\n")
                            
                            
                            #そして、結果。
                            url_result = root_url + file_name + "/raceresult.html"
                            driver.get(url_result)

                            file.write("\nresult\n")
                            print("getting result\n")

                            #//*[@id="tab1"]/div[4]/table/tbody/tr[1]/td[1]
                            #//*[@id="tab1"]/div[4]/table/tbody/tr[1]/td[3]
                            #//*[@id="tab1"]/div[4]/table/tbody/tr[2]/td[1]
                            column_list = [1,3]
                            for row in range(1,horses+1):
                                ranking_tmp = "//*[@id=\"tab1\"]/div[4]/table/tbody/tr[" +str(row)+"]/td[1]"
                                horse_number = "//*[@id=\"tab1\"]/div[4]/table/tbody/tr[" +str(row)+ "]/td[3]"
                                file.write(driver.find_element_by_xpath(ranking_tmp).text)
                                file.write(" ")
                                file.write(driver.find_element_by_xpath(horse_number).text)
                                file.write("\n")

                            file.close()
                        except  Exception as e:
                            tb = sys.exc_info()[2]
                            print("message:{0}".format(e.with_traceback(tb)))
                            break
                    else:
                        #nanikasoriwo
                        #このブレイクで計算時間が12分の1に短縮されるという。神ブレイク
                        break

