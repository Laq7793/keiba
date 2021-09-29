// GOMI


#include<iostream>
#include<vector>
#include<string>
#include<iomanip>
#include<sstream>
#include<fstream>
//#include<filesystem>



void LAQ(std::string dir, std::vector<std::string> &file_list){
	


	return;
}



void Brain_nuts(std::vector<std::string> path, std::vector<std::string> &file_list){

	int year, month, day, race;
	std::vector<std::string> place_list = {"01", "04", "06", "07", "09", "10"};

	for(auto i : path){

		for(year=2011;year<=2022;year++){
			for(month=1;month<=12;month++){
				for(day=1;day<=31;day++){
					for(int j=0;j<place_list.size();j++){
						for(race=1;race<=12;race++){

							std::stringstream ss;
							ss << i;
							ss << year << std::setw(2) << std::setfill('0') << month;
							ss << std::setw(2) << std::setfill('0') << day << place_list[j];
							ss << std::setw(2) << std::setfill('0') << race;

							std::ifstream ifs(ss.str());
							if(ifs){
								file_list.push_back(ss.str());
							}
						
						}
					}
				}
			}
		}
	}
	return;
}


int main(void){
	std::vector<std::string> file_list, path = {"", "data/201101-20110305/", "data/201901050601/"};
	Brain_nuts(path, file_list);
	for(auto i : file_list){
		std::cout << i << std::endl;
	}
	return 0;
}
