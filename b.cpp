#include <iostream>
#include <algorithm>
using namespace std;

string encrypt_string(string x, int n, int k)
{
    if (k == 0)
    {
        rotate(x.begin(), x.begin() + n, x.end());
    }
    else
    {
        rotate(x.rbegin(), x.rbegin() + n, x.rend());
    }
    return x;
}

int main()
{
    string x;
    cin >> x;
    int n, k;
    cin >> k >> n;

    string anss = encrypt_string(x, n, k);
    cout << anss << endl;
    return 0;
}
