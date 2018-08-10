#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#if defined ( __cplusplus ) && __cplusplus > 199711L /* C++ 11 */
#include <unordered_map>
#include <unordered_set>
#include <memory>
#endif /* C++ 11 */
using namespace std;
#endif /* __PROGTEST__ */

class CDate
{
  public:
     CDate (int y, int m, int d ): year(y), month(m), day(d){}
     int getYear()const{return year;}
     int getMonth()const{return month;}
     int getDay()const{return day;}
    private:
        int year;
        int month;
        int day;
    
};

bool operator<(CDate a, CDate b){
if (a.getYear() < b.getYear()){ return true;}
if (a.getYear() > b.getYear()){ return false;}
if (a.getMonth() < b.getMonth()) { return true;}
if (a.getMonth() > b.getMonth()) { return false;}
if (a.getDay() < b. getDay()) {return true;}
return false;
}

struct Ware{
	Ware(CDate exp,int cnt):expires(exp),count(cnt){};
	CDate expires;
	int count;
        void printWare()const;
};

void Ware::printWare()const{
    //cout << expires.getDay() << ". " << expires.getMonth() << ". " << expires.getYear() << " (" << count << " ks)";
}


//zajisti, ze se bude do setu vkladat od nejmensiho data po nejvetsi => odebirat produkty ze zacatku
bool operator<(const Ware& l, const Ware& r){
	return l.expires < r.expires;
}

class CSupermarket
{
  public:
    // default constructor
      CSupermarket(){}
    // Store   ( name, expiryDate, count )
      CSupermarket & Store(const string& name, const CDate & expiryDate, int count);
    // Sell    ( shoppingList )
      list<pair<string,int> > Sell(list<pair<string,int> > &);
    // Expired ( date )
      list<pair<string,int> > Expired(const CDate&);
      void Print() const;

  private:
    // todo
      map<string, vector<Ware> > warehouse;
      bool findTypo(string &);
      void cleanMap();
};


CSupermarket & CSupermarket::Store(const string& name, const CDate & expiryDate, int count){
    warehouse[name].push_back(Ware(expiryDate,count));
    sort(warehouse[name].begin(),warehouse[name].end());
    return *this;

}

void CSupermarket::Print() const {
    //cout << "---warehouse stock----" << endl;
    
  for(auto elem : warehouse) {
    //cout << elem.first << " "; 
    for(auto it : elem.second){
        it.printWare();
        //cout << "  ";
    }
    //cout << endl;
  }
    //cout << "-----------" << endl;
}

list<pair<string,int> > CSupermarket::Expired(const CDate& theDay){
    list<pair<string,int> > result;
    
    for(auto elem : warehouse) {
        pair <string, int> stuff;
        stuff.first = elem.first;
        stuff.second = 0;
        
        for(auto it : elem.second){
            if (it.expires < theDay){
                stuff.second += it.count;
            }
        }
        if(stuff.second > 0)            
            result.push_back(stuff);
    }
     result.sort( []( const pair<string,int> &a, const pair<string,int> &b ) { return a.second > b.second; } );

    
    return result;

}

list<pair<string,int> > CSupermarket::Sell(list<pair<string,int> > & shList){
    for(auto i = shList.begin(); i != shList.end(); i++){ //cyklus pres list paru, i = pair
        //cout << "looking for " << i->first << endl;
        auto it = warehouse.find(i->first); // it je iterator v mape, pro potreby hledani
        if(it != warehouse.end() || findTypo(i->first)){
           auto j = warehouse[i->first].begin(); //j je iterator po vektoru
           
           while( j != warehouse[i->first].end() && i->second > 0 ){
                 if(i->second <= j->count){
                     j->count -= i->second;
                     //cout << "po odebrani ze skladu:" <<endl; 
                     j->printWare();
                     //cout << endl << endl;
                     i = shList.erase(i);
                     i--;
                     break;
                 }else{
                     i->second -= j->count;
                     j->count = 0;
                     //cout << "po odebrani a vyprazdneni ze skladu:" <<endl; 
                     j->printWare();
                     //cout << endl << endl;
                     j = warehouse[i->first].erase(j);
                     j --;
                 }   
                                 
                 
                 j++;
           } 
        }
    }
    cleanMap();
    return shList;
}

bool CSupermarket::findTypo(string & typo){
    int ambigue = 0;
    auto tmp = warehouse.begin();
    //cout << "Looking for typo" << endl;
    
    
    for(auto it = warehouse.begin(); it != warehouse.end(); it++){
        if(it->first.size() != typo.size() )
            continue;
        
        int typoCounter = 0;
        
        for(size_t i = 0; i < typo.size(); i ++){
            if( it->first[i] != typo[i])
                typoCounter++;
            if(typoCounter > 1)
                break;
        }
        
        if(typoCounter == 1){
            ambigue++;
            if(ambigue > 1)
                return false;
            tmp = it;
        }
                
    }
    if(ambigue == 1){    
        //cout << "Changing " << typo << " to " << tmp->first << endl;
        typo = tmp->first;
        return true;
    }
    
    return false;
}


void CSupermarket::cleanMap(){
    for(auto it = warehouse.begin(); it != warehouse.end(); it++){
        if (it->second.size() == 0){
            //cout << it->first << " je prazdne" << endl;
            warehouse.erase(it);
        }
    
    }

}


#ifndef __PROGTEST__
int main ( void )
{
  CSupermarket s;
  s . Store ( "bread", CDate ( 2016, 4, 30 ), 100 ) .
      Store ( "butter", CDate ( 2016, 5, 10 ), 10 ) .
      Store ( "beer", CDate ( 2016, 8, 10 ), 50 ) .
      Store ( "bread", CDate ( 2016, 4, 25 ), 100 ) .
      Store ( "okey", CDate ( 2016, 7, 18 ), 5 );
  
  s.Print();

  list<pair<string,int> > l0 = s . Expired ( CDate ( 2018, 4, 30 ) );
  //cout << l0.size() << " items will have expired by 4. 30. 2018" <<endl;
  assert ( l0 . size () == 4 );
  assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

  list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
  s . Sell ( l1 );
  assert ( l1 . size () == 2 );
  assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );
  list<pair<string,int> > l2 = s . Expired ( CDate ( 2016, 4, 30 ) );
  //cout << l2.size() << " items will have expired by 4. 30. 2016" <<endl;
  s.Print();
  assert ( l2 . size () == 1 );
  //cout << l2.begin()->first << " "<< l2.begin()->second << endl;
  assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

  list<pair<string,int> > l3 = s . Expired ( CDate ( 2016, 5, 20 ) );
  assert ( l3 . size () == 1 );
  assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

  list<pair<string,int> > l4 { { "bread", 105 } };
  s . Sell ( l4 );
  assert ( l4 . size () == 0 );
  assert ( ( l4 == list<pair<string,int> > {  } ) );

 list<pair<string,int> > l5 = s . Expired ( CDate ( 2017, 1, 1 ) );
  assert ( l5 . size () == 3 );
  assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

   s . Store ( "Coke", CDate ( 2016, 12, 31 ), 10 );
   s.Print();

  list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
  s . Sell ( l6 );
  //cout << l6 . size () << endl;
  for(auto i : l6)
      //cout << i.first << " ";
  //cout << endl;
  assert ( l6 . size () == 3 );
  assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

  list<pair<string,int> > l7 = s . Expired ( CDate ( 2017, 1, 1 ) );
  assert ( l7 . size () == 4 );
   assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

  s . Store ( "cake", CDate ( 2016, 11, 1 ), 5 );
  s.Print();

  list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
  s . Sell ( l8 );
  //cout << l8 . size () << endl;
  assert ( l8 . size () == 2 );
  assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

  list<pair<string,int> > l9 = s . Expired ( CDate ( 2017, 1, 1 ) );
  assert ( l9 . size () == 5 );
  assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );
  s.Print();
  
  list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
  s . Sell ( l10 );
  assert ( l10 . size () == 2 );
  assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );

  list<pair<string,int> > l11 = s . Expired ( CDate ( 2017, 1, 1 ) );
  assert ( l11 . size () == 4 );
  assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );

  s.Print();
  list<pair<string,int> > l12 { { "Cake", 4 } };
  s . Sell ( l12 );
  ////cout << l12 . size () <<" item: " << l12.begin()->first << "(" << l12.begin()->second << ")" << endl;
  assert ( l12 . size () == 0 );
  assert ( ( l12 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l13 = s . Expired ( CDate ( 2017, 1, 1 ) );
  assert ( l13 . size () == 4 );
  assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

  list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
  s . Sell ( l14 );
  assert ( l14 . size () == 1 );
  assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );
/*  */
  return 0;
}
#endif /* __PROGTEST__ */