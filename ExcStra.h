#include<vector>

//単勝オッズをもとに計算
void ExcStra1(std::vector<int>& FirstEx,
             std::vector<int>& SecondEx,
             std::vector<int>& ThirdEx,
             std::vector<double>& tan)
{
    
    int threshold1 = 15;
    int threshold2 = 30;
    int threshold3 = 100;
    //とりあえず単勝オッズをもとに除外馬を決定したいところである。
    for(int i=1;i<tan.size()+1;i++)
    {   
        //excluding first 
        std::cout << "first" << std::endl;
        if(tan[i] >threshold1 )
        {
            FirstEx.push_back(i);
            
            std::cout << i << std::endl;
        }

        //excluding second
        std::cout << "sec" << std::endl;
        if(tan[i] > threshold2)
        {
            SecondEx.push_back(i);
            std::cout << i << std::endl;
        }

        //excluding third
        std::cout << "third" << std::endl;
        if(tan[i] > threshold3)
        {
            ThirdEx.push_back(i);
            std::cout << i << std::endl;
        }
    }
}

//一番人気を外す。特に微妙な一番人気ははずしていっていいよ。1番人気でオッズが2を下回ってるやつはゴミや
void ExcStra2(std::vector<int>& FirstEx,std::vector<double>& tan,int n)
{
    double min_odds = tan[1];
    int most_pop_baban = 1;

    for(int i = 1;i<n+1;i++){
        if(min_odds>tan[i])
        {
            min_odds = tan[i];
            most_pop_baban = i;
            std::cout << "min_odds=" << min_odds << std::endl;
        }
    }
    FirstEx.push_back(most_pop_baban);

}

void Shinba()
{

}
