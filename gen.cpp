#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>

#define sz(a) ((int)(a.size()))
#define fi first
#define se second
#define eb emplace_back
#define pb push_back

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, int> pli;
typedef pair<ll, ll> pll;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

const ll inf=1e9;
const ll infl=1e18;
const int N=2e5+5;
const int mod=1e9+7;

int gen(int lo, int hi)
{
    return rng()%(hi-lo+1)+lo;
}

vector<string> a;

void quay(int p, int n, string &s)
{
    if(p == n+1)
    {
        a.pb(s);
        return;
    }
    for(int i=1; i<=7; i++)
    {
        s += to_string(i);
        quay(p+1, n, s);
        s.pop_back();
    }
}

signed main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    for(int t=1; t<=1; t++)
    {
        ofstream inp("input.INP");

        int n=8;
        for(int i=1; i<=n; i++)
        {
            string s;
            quay(1, i, s);
        }
        for(auto &s : a)
        {
            inp << s << '\n';
        }

        inp.close();
    }

    return 0;
}