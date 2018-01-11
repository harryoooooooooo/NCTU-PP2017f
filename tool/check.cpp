#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<algorithm>
using namespace std;

int myabs(int x){
    return max(x, -x);
}

int main (int, char** argv){
    fstream td(argv[1], fstream::in), out(argv[2], fstream::in);
    vector<bool> ans(1);
    if (out.peek() == 'N'){
        cout << "OK." << endl;
        return 0;
    }
    for (int ind; out>>ind; ){
        string tmp;
        out >> tmp >> tmp;
        ans.push_back(tmp == "true");
    }
    int n, m;
    td >> n >> m;
    n *= 3;
    bool res = true;
    for (int i=0;i<n;i+=3){
        bool tup = false;
        for (int j=0;j<3;j++){
            int tmp;
            td >> tmp;
            tup |= ans[myabs(tmp)] ^ (tmp<0);
        }
        res &= tup;
    }
    if (res) cout << "check pass!" << endl;
    else {
        cout << "oh nooooooooooooo" << endl;
        return 1;
    }
}
