#include "baken_format.h"
#include<iostream>
#include<fstream>


int ResultComp( std::vector<sSantan>& santan,
                std::vector<sUmaren>& umaren,
                std::vector<sUmatan>& umatan,
                std::vector<sTansyou>& tan,
                std::vector<int> result)
{
    int ret=0;
    std::cout << "結果比較中" << std::endl;
    //サンタン走査
    for(auto itr = santan.begin();itr != santan.end();itr++)
    {
        std::cout << itr->umaban1 << ">" << itr->umaban2 << ">" << itr->umaban3 <<" " << itr->odds << " "<< itr->money<< " ";
        if(itr->umaban1 == result[0])
        {
            if(itr->umaban2 == result[1])
            {
                if(itr->umaban3 == result[2])
                {
                    ret += itr->odds*itr->money;
                    std::cout << "★" <<std::endl;
                    continue;
                }
            }
          
        }
        std::cout <<"☓" <<std::endl;

    }

    //馬連走査
    for(auto itr = umaren.begin();itr != umaren.end();itr++)
    {
        std::cout << itr->umaban1 << "-" << itr->umaban2 << " "<< itr->odds << " " << itr->money<< " ";
        if(itr->umaban1 == result[0]||itr->umaban1 == result[1])
        {
            if(itr->umaban2 == result[0] || itr-> umaban2 == result[1])
            {
                ret += itr->odds*itr->money;
                std::cout << "★" <<std::endl;
                continue;
            }
        }
        std::cout <<"☓" <<std::endl;
    }

    //馬単走査
    for(auto itr = umatan.begin();itr != umatan.end();itr++)
    {
        std::cout << itr->umaban1 << ">" << itr->umaban2 << " " <<  itr->odds << " " << itr->money << " ";
        if(itr->umaban1 == result[0])
        {
            if(itr->umaban2 == result[1])
            {
                ret += itr->odds*itr->money;
                std::cout << "★" << std::endl;
            }
        }
 
            std::cout <<"☓" <<std::endl;
    }

    //単勝走査
    for(auto itr = tan.begin();itr != tan.end();itr++)
    {
        std::cout << itr->umaban << " "<<itr->odds << " "<<itr->money << " "; 
        if(itr->umaban == result[0])
        {
            ret += itr->odds*itr->money;
            std::cout << "★" << std::endl;
        }
        else
        {
            std::cout <<"☓" <<std::endl;
        }
    }

    return ret;

}

int GenResultComp( std::vector<sSantan>& santan,
                std::vector<sUmaren>& umaren,
                std::vector<sUmatan>& umatan,
                std::vector<sTansyou>& tan,
                std::vector<int> result,
                std::ofstream& stream)
{
    int ret=0;
    //サンタン走査
    for(auto itr = santan.begin();itr != santan.end();itr++)
    {
        stream << itr->umaban1 << ">" << itr->umaban2 << ">" << itr->umaban3 <<" " << itr->odds << " "<< itr->money<< " ";
        if(itr->umaban1 == result[0])
        {
            if(itr->umaban2 == result[1])
            {
                if(itr->umaban3 == result[2])
                {
                    ret += itr->odds*itr->money;
                    stream << "★" <<"\n";
                    continue;
                }
            }
          
        }
        stream <<"☓" <<"\n";

    }

    //馬連走査
    for(auto itr = umaren.begin();itr != umaren.end();itr++)
    {
        stream << itr->umaban1 << "-" << itr->umaban2 << " "<< itr->odds << " " << itr->money<< " ";
        if(itr->umaban1 == result[0]||itr->umaban1 == result[1])
        {
            if(itr->umaban2 == result[0] || itr-> umaban2 == result[1])
            {
                ret += itr->odds*itr->money;
                stream << "★" << "\n";
                continue;
            }
        }
        stream <<"☓" <<"\n";
    }

    //馬単走査
    for(auto itr = umatan.begin();itr != umatan.end();itr++)
    {
        stream << itr->umaban1 << ">" << itr->umaban2 << " " <<  itr->odds << " " << itr->money << " ";
        if(itr->umaban1 == result[0])
        {
            if(itr->umaban2 == result[1])
            {
                ret += itr->odds*itr->money;
                stream << "★" << "\n";
            }
        }
 
            stream <<"☓" <<"\n";
    }

    //単勝走査
    for(auto itr = tan.begin();itr != tan.end();itr++)
    {
        stream << itr->umaban << " "<<itr->odds << " "<<itr->money << " "; 
        if(itr->umaban == result[0])
        {
            ret += itr->odds*itr->money;
            stream << "★" << std::endl;
        }
        else
        {
            stream <<"☓" <<std::endl;
        }
    }

    return ret;

}