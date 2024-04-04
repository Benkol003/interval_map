#include <map>

template<typename K, typename V>
struct interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {

        if(! (keyBegin<keyEnd) ) return;
        if(m_map.size()==0){
            if(!(val==m_valBegin)){
                m_map.emplace(keyBegin,val);
                m_map.emplace(keyEnd,m_valBegin);
            } else return;
        }else{
            //starting index
            typename std::map<K,V>::iterator keyBeginNextRange = m_map.upper_bound(keyBegin);
            V* keyEndVal(nullptr); //wasnt sure if no-arg constructor was available - c++ will initialise local class instances. would be nice if i could use std::unique_ptr
            if(keyBeginNextRange == m_map.begin() ){
                if(!(val==m_valBegin)) m_map.emplace(keyBegin,val);//iterators arent invalidated by inserts
                delete keyEndVal;
                keyEndVal = new V(m_valBegin);
            }else{
                typename std::map<K,V>::iterator keyBeginPrevRange = (--keyBeginNextRange)++;
                //keyEndVal - is value for any ranges that extend past keyEnd
                if(keyBeginPrevRange->first < keyBegin){
                    delete keyEndVal;
                    keyEndVal = new V(keyBeginPrevRange->second);
                    if(!(*keyEndVal==val)) m_map.emplace(keyBegin,val);
                }else{ 
                    //now keyBeginPrevRange = keyBegin
                     if(keyBeginPrevRange->second == val){
                        delete keyEndVal;
                        keyEndVal = new V(val); //value before keyBeginPrevRange has to be different to value after
                     }else{
                        delete keyEndVal;
                        keyEndVal = new V(keyBeginPrevRange->second);
                        V* valuePrev = nullptr;
                        if(keyBeginPrevRange==m_map.begin()){
                            delete valuePrev;
                            valuePrev = new V(m_valBegin);
                        }
                        else{
                            typename std::map<K,V>::iterator actualPrev = (--keyBeginPrevRange)++;
                            delete valuePrev;
                            valuePrev = new V(actualPrev->second); 
                        }
                        if(*valuePrev==val){
                            m_map.erase(keyBeginPrevRange);
                             
                        }else{
                            //reuse iterator rather than look up again
                            keyBeginPrevRange->second=val;
                        }
                     }
                }

            }

            //walk upto keyEnd
            while(keyBeginNextRange != m_map.end() && keyBeginNextRange->first<keyEnd){
                typename std::map<K,V>::iterator erase_i = keyBeginNextRange;
                delete keyEndVal;
                keyEndVal=new V(keyBeginNextRange->second);
                ++keyBeginNextRange;
                m_map.erase(erase_i);
            }
            
            //at this point keyBeginNextRange might aswell be keyBeginEndRange
            typename std::map<K,V>::iterator keyEndNextRange = keyBeginNextRange; //readability, also garunteed keyEndNextRange >=keyEnd
            if(keyEndNextRange == m_map.end()){
                if(!(*keyEndVal==val)) m_map.emplace(keyEnd,*keyEndVal);
            }else{
                if(keyEnd<keyEndNextRange->first){
                    if(!(*keyEndVal==val)) m_map.emplace(keyEnd,*keyEndVal);
                }else{//keyEndNextRange and keyEnd equal, also therefore no keyEndVal
                    if(val==keyEndNextRange->second) m_map.erase(keyEndNextRange);
                }
            }
        }
	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};