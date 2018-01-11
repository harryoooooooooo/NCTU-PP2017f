#include<algorithm>
#include<iostream>
#include<string>
#include<vector>
using namespace std;
using ull = unsigned long long;

vector<int> raw;

int myabs(int x){
    return max(x, -x);
}

bool getbool(ull s, int r){
    if (s & (1llu << myabs(r))) return true;
    return false;
}

bool test(ull s){
    bool ret = true;
    for (size_t i = 0; i < raw.size(); i+=3){
        bool tmp = false;
        for (size_t j = 0; j < 3; j++)
            tmp |= (getbool(s, raw[i+j]) ^ (raw[i+j] < 0));
        ret &= tmp;
    }
    return ret;
}

int main (){
    int n, m;
    cin >> n >> m;
    n*=3;
    raw.resize(n);
    for (int i=0;i<n;i++)
        cin >> raw[i];
    bool found = false;
    ull s;
    for (s = 0; s < (1llu<<m); s++){
        if (test(s<<1)){
            found = true;
            break;
        }
    }
    s <<= 1;
    if (!found) cout << "No satisfy!" << endl;
    else {
        for (int i = 1; i <= m; i++)
            cout << i << " := " << ((s&(1llu<<i))?"true":"false") << endl;
    }
}
