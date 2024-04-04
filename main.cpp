#include <iostream>
#include <cassert>
#include <map>
#include <stdexcept>
#include <cstdlib>

#include "interval_map.hpp"

class KVal{
    public:

    KVal(int v){
        value=v;
    }

    KVal(KVal const& v){
        value=v.value;
    }
    
    bool operator <(KVal const & a) const {
        return value<a.value;
    }

    KVal& operator=(const KVal&) = default;
    private:
        int value;
        bool operator==(int);
};

class VVal{
    public:
    VVal(char v){
        value=v;
    }

    VVal(VVal const& v){
        value=v.value;
    }
    
    bool operator==(VVal const & a) const {
        return value==a.value;
    }

    VVal& operator=(const VVal&) = default;

    char getV()const{
        return value;
    }

    private:
        char value;
};



std::string mapRep(interval_map<KVal,VVal> const& map){
    std::string s; s.reserve(20);
    for(int i=0;i<20;++i) s.push_back(map[KVal(i)].getV());
    return s;
}

bool check(interval_map<KVal,VVal> const& map){
    auto i = map.m_map.begin(); if (i == map.m_map.end()) return true;
    auto j = i; ++i; if (i == map.m_map.end()) return true;
    for(;i!=map.m_map.end();++i,++j){
        if(((i->second)==(j->second) )) return false;
    }
    return true;
}

int main(){
    //randomised testing
    interval_map<KVal,VVal> map('A');
    std::string s(20,'A');
    char vals[] = {'A','B','C'};
    int count = 0;
    while(true){
        ++count;
        std::cout<<count<<std::endl;
        int i1 = rand() % 20;
        int i2 = rand() % 20;
        int ci = rand() % 3;

        interval_map<KVal,VVal> old_map = map;
        map.assign(i1,i2,vals[ci]);

        for(int j=i1;j<i2;++j){
            s[j]=vals[ci];
        }

        if(!check(map)) throw std::exception();
        if(!(s == mapRep(map)))throw std::exception();
    }

}