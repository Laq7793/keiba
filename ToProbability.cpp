#include<iostream>
using namespace std;

double Odds2Probability(double odds){;
    double plist[] = {100.0, 78.6, 64.6, 66.2, 60.3, 52.0, 50.9, 44.8, 43.5, 39.2,
                      35.7, 33.0, 32.4, 30.0, 27.6, 24.7, 22.4, 17.6, 13.4, 10.0,
                      7.0, 4.9, 3.4, 2.1, 1.1, 0.3};

    int unko = (int) (10*odds);;;

    switch(unko){;
        case 10:    return plist[0]/100.0;
        case 11:    return plist[1]/100.0;
        case 12:    return plist[2]/100.0;
        case 13:    return plist[3]/100.0;
        case 14:    return plist[4]/100.0;
        case 15:    return plist[5]/100.0;
        case 16:    return plist[6]/100.0;
        case 17:    return plist[7]/100.0;
        case 18:    return plist[8]/100.0;
        case 19:    return plist[9]/100.0;
        case 20 ... 21:    return plist[10]/100.0;
        case 22 ... 23:    return plist[11]/100.0;
        case 24 ... 25:    return plist[12]/100.0;
        case 26 ... 27:    return plist[13]/100.0;
        case 28 ... 29:    return plist[14]/100.0;
        case 30 ... 34:    return plist[15]/100.0;
        case 35 ... 39:    return plist[16]/100.0;
        case 40 ... 49:    return plist[17]/100.0;
        case 50 ... 69:    return plist[18]/100.0;
        case 70 ... 99:    return plist[19]/100.0;
        case 100 ... 149:    return plist[20]/100.0;
        case 150 ... 199:    return plist[21]/100.0;
        case 200 ... 299:    return plist[22]/100.0;
        case 300 ... 499:    return plist[23]/100.0;
        case 500 ... 999:    return plist[24]/100.0;
        case 1000 ... 1000000:    return plist[25]/100.0;
        default:
            printf("ERROR FUNC(OdsToP) : odds = %lf", odds);
            return -1.0;
    };
};

double Popularity2Probability(int popularity){;
    double plist[] = {31.9, 18.8, 13.2, 9.6, 7.2, 5.5, 4.0, 3.0, 2.2,
                      1.6, 1.4, 0.9, 0.7, 0.4, 0.4, 0.2, 0.2, 0.1};
    
    if(popularity>=1 && popularity<=18){;
        return plist[popularity-1];
    }else{;
        printf("ERROR FUNC(PToP) : popularity = %d\n",popularity);
        return -1;
    };
};

int main(){;
    double odds;
    int popularity;
    while(true){;
        cout << "odds:";
        cin >> odds;
        cout << Odds2Probability(odds) << endl;
        cout << "popularity:";
        cin >> popularity;
        cout << Popularity2Probability(popularity) << endl;
    };
};
