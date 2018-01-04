#include<algorithm>
#include<iostream>
#include<string>
#include<vector>
using namespace std;

vector<int> raw;
vector<bool> stk;

int myabs(int x){
    return max(x, -x);
}

bool test(){
    bool ret = true;
    for (size_t i = 0; i < raw.size(); i+=3){
        bool tmp = false;
        for (size_t j = 0; j < 3; j++)
            tmp |= (stk[myabs(raw[i+j])] ^ (raw[i+j] < 0));
        ret &= tmp;
    }
    return ret;
}

bool dfs(size_t i = 1){
    if (i == stk.size())
        return test();
    stk[i] = false;
    if (dfs(i+1)) return true;
    stk[i] = true;
    if (dfs(i+1)) return true;
    return false;
}

int main (){
    int n, m;
    cin >> n >> m;
    n*=3;
    raw.resize(n);
    stk.resize(m+1);
    for (int i=0;i<n;i++)
        cin >> raw[i];
    bool found = dfs();
    if (!found) cout << "No satisfy!" << endl;
    else {
        for (size_t i = 1; i < stk.size(); i++)
            cout << i << " := " << (stk[i]?"true":"false") << endl;
    }
}
