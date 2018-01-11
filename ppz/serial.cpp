#include<algorithm>
#include<iostream>
#include<string>
#include<vector>
using namespace std;
using ull = unsigned long long;

struct yee{
    int r, bk;
    bool is_pos;
};

int nsol;
vector<int> raw;
vector<vector<yee>> ad;
vector<bool> stk;

int myabs(int x){
    return max(x, -x);
}

void do_set(int i, bool v){
    for (auto& x : ad[i]){
        x.bk = raw[x.r];
        if (raw[x.r]){
            if (x.is_pos == v){
                raw[x.r] = 0;
                nsol --;
            }
            else raw[x.r] --;
        }
    }
}

void do_reset(int i){
    for (auto& x : ad[i]){
        if (x.bk && raw[x.r]==0) nsol++;
        raw[x.r] = x.bk;
    }
}

bool dfs(size_t i = 1){
    if (!nsol) return true;
    bool foundn = false, foundp = false;
    for (auto& x : ad[i]){
        if (raw[x.r] == 1){
            if (x.is_pos) foundp = true;
            else foundn = true;
        }
    }
    if (foundn && foundp) return false;
    if (foundn || foundp){
        stk[i] = foundp;
        do_set(i, foundp);
        bool ret = dfs(i+1);
        do_reset(i);
        return ret;
    }

    stk[i] = false;
    do_set(i, false);
    if (dfs(i+1)) return true;
    do_reset(i);

    stk[i] = true;
    do_set(i, true);
    if (dfs(i+1)) return true;
    do_reset(i);

    return false;
}

int main (){
    int n, m;
    cin >> n >> m;
    raw.resize(n);
    stk.resize(m+1);
    ad.resize(m+1);
    for (int i=0;i<n;i++){
        int k=0;
        int tmp[3];
        for (int j=0;j<3;j++){
            cin >> tmp[k];
            for (int l=0;l<k;l++){
                if (tmp[l] == tmp[k])
                    k--;
            }
            k++;
        }
        bool found = false;
        for (int j=0;j<k;j++){
            for (int l=j+1;l<k;l++){
                if (tmp[j] == (-tmp[l])){
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (found) raw[i] = 0;
        else {
            nsol++;
            raw[i] = k;
            for (int j=0;j<k;j++)
                ad[myabs(tmp[j])].push_back({i, 0, tmp[j]>0});
        }
    }
    bool found = dfs();
    if (!found) cout << "No satisfy!" << endl;
    else {
        for (size_t i = 1; i < stk.size(); i++)
            cout << i << " := " << (stk[i]?"true":"false") << endl;
    }
}
