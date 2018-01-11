#include<algorithm>
#include<iostream>
#include<string>
#include<vector>
#include<thread>
#include<atomic>
#include<mutex>
using namespace std;
using ull = unsigned long long;

const int MAXTNUM = 16;

class Mailbox{
    int dep, nsol;
    vector<int> cnt;
    vector<bool> val;
    mutex put_side, get_side;
public:
    Mailbox(){get_side.lock();}
    bool put(int _dep, int _nsol, const vector<int>& _cnt, const vector<bool>& _val){
        if (!put_side.try_lock()) return false;
        dep = _dep;
        nsol = _nsol;
        cnt = _cnt;
        val = _val;
        get_side.unlock();
        return true;
    }
    void get(int& _dep, int& _nsol, vector<int>& _cnt, vector<bool>& _val){
        get_side.lock();
        _dep = dep;
        _nsol = nsol;
        _cnt = move(cnt);
        _val = move(val);
        put_side.unlock();
    }
} mailbox;

atomic<int> idle_num;
mutex m_ans;
bool found_ans;
vector<bool> ans;

struct yee{
    int r, bk;
    bool is_pos;
};

int myabs(int x){
    return max(x, -x);
}

void do_set(int i, bool v, int& nsol, vector<int>& cnt, vector<vector<yee>>& ad){
    for (auto& x : ad[i]){
        x.bk = cnt[x.r];
        if (cnt[x.r]){
            if (x.is_pos == v){
                cnt[x.r] = 0;
                nsol --;
            }
            else cnt[x.r] --;
        }
    }
}

void do_reset(int i, int& nsol, vector<int>& cnt, vector<vector<yee>>& ad){
    for (auto& x : ad[i]){
        if (x.bk && cnt[x.r]==0) nsol++;
        cnt[x.r] = x.bk;
    }
}

bool dfs(int i, int& nsol, vector<int>& cnt, vector<bool>& val, vector<vector<yee>>& ad){
    if (!nsol) return true;
    bool foundn = false, foundp = false;
    for (auto& x : ad[i]){
        if (cnt[x.r] == 1){
            if (x.is_pos) foundp = true;
            else foundn = true;
        }
    }
    if (foundn && foundp) return false;
    if (foundn || foundp){
        val[i] = foundp;
        do_set(i, foundp, nsol, cnt, ad);
        bool ret = dfs(i+1, nsol, cnt, val, ad);
        do_reset(i, nsol, cnt, ad);
        return ret;
    }

    val[i] = false;
    do_set(i, false, nsol, cnt, ad);
    if (!mailbox.put(i+1, nsol, cnt, val))
        if (dfs(i+1, nsol, cnt, val, ad)) return true;
    do_reset(i, nsol, cnt, ad);

    val[i] = true;
    do_set(i, true, nsol, cnt, ad);
    if (dfs(i+1, nsol, cnt, val, ad)) return true;
    do_reset(i, nsol, cnt, ad);

    return false;
}

void tmain(vector<vector<yee>> ad){
    int dep, nsol;
    vector<int> cnt;
    vector<bool> val;
    while (true){
        idle_num++;
        mailbox.get(dep, nsol, cnt, val);
        idle_num--;

        if (dfs(dep, nsol, cnt, val, ad)){
            m_ans.lock();
            if (!found_ans){
                found_ans = true;
                ans = move(val);
            }
            m_ans.unlock();
        }
    }
}

int main (){
    idle_num = 0;

    int n, m, nsol = 0;
    cin >> n >> m;
    vector<int> cnt(n);
    vector<bool> val(m+1);
    vector<vector<yee>> ad(m+1);

    for (int i=0;i<n;i++){// yuck code, it should works
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
        if (found) cnt[i] = 0;
        else {
            nsol++;
            cnt[i] = k;
            for (int j=0;j<k;j++)
                ad[myabs(tmp[j])].push_back({i, 0, tmp[j]>0});
        }
    }
    mailbox.put(1, nsol, cnt, val);
    vector<thread> threads;
    for (int i=0;i<MAXTNUM;i++)
        threads.push_back(thread(tmain, ad));

    while (idle_num != MAXTNUM && !found_ans) this_thread::yield();

    if (!found_ans) cout << "No satisfy!" << endl;
    else {
        for (size_t i = 1; i < ans.size(); i++)
            cout << i << " := " << (ans[i]?"true":"false") << endl;
    }
    exit(0);
}
