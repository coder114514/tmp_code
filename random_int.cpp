#include<iostream>
#include<random>

typedef unsigned int       u32;
typedef unsigned long long u64;

std::mt19937 MT;
u32 rand4(){ return MT() & 15; }
template <typename T>
T rand(T l, T r){
    typedef typename std::make_unsigned<T>::type U;
    U u = r - l, g = 15, w = 0;
    if(g == u) return l + rand4();
    if(g >  u){
        U eu = u + 1, s = g / eu, t = s * eu;
        do{
            w = rand4();
        }while(w >= t);
        w /= s;
    } else {
        U eg = g + 1, s = u / eg;
        do{
            w = rand((U)0, s) * eg + rand4();
        }while(w >= u);
    }
    return l + w;
}

const int test_num = 10000000;
const int test_range = 10;

int C[test_range];
int main(){
    for(int i = 0; i < test_num; ++i)
        ++C[rand(1, test_range) - 1];
    for(int i = 0; i < test_range; ++i)
        std::cout << i + 1 << ":" << C[i]*100.0/test_num << "%" << std::endl;
    return 0;
}
