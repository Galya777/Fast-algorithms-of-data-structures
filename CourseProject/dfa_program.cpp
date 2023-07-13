#include <string>
#include <queue>
#include <stack>
#include <iostream>
#include <set>
using namespace std;

const int MAXN = 200000000;


set<int> dfa[MAXN], P[MAXN];
bool acc[MAXN];
int charToInt(char c) {
    return c - 'a';
}

void buildDFA(string s) {
    int n = s.size();
    for (int i = 0; i < n; i++) {
        dfa[i].insert(charToInt(s[i]));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 26; j++) {
            set<int> tmpSet;
            for (auto state : dfa[i]) {
                if (charToInt(s[state]) == j) {
                    tmpSet.insert(state+1);
                }
            }
            if (!tmpSet.empty()) {
                dfa[i].insert(j);
                dfa[i].insert(n+i+1);
                dfa[n+i+1] = tmpSet;
            }
        }
    }
}

void removeUnreachable(int start, string s) {
    set<int> reachable;
    queue<int> q;
    q.push(start);
    reachable.insert(start);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto v : dfa[u]) {
            if (reachable.find(v) == reachable.end()) {
                reachable.insert(v);
                q.push(v);
            }
        }
    }
    for (int i = 0; i <= s.size(); i++) {
        if (reachable.find(i) == reachable.end()) {
            dfa[i].clear();
        }
    }
}

void makeAcyclic(int start) {
    set<int> visited;
    stack<int> path;
    path.push(start);
    visited.insert(start);
    while (!path.empty()) {
        int u = path.top();
        path.pop();
        for (auto v : dfa[u]) {
            if (visited.find(v) == visited.end()) {
                visited.insert(v);
                path.push(v);
            } else {
                dfa[u].erase(v);
            }
        }
    }

    void minimizeDFA(string s, int numStates, int numTransitions, int numFinalStates){
    set<int> F;
    for (int i = 0; i < s.size(); i++) {
        if (i == s.size() - 1) {
            F.insert(i+1);
            acc[i+1] = true;
        } else {
            acc[i+1] = false;
        }
    }

    P[0] = F;
    P[0].insert(0);
    queue<pair<set<int>, int>> q;
    q.push({F, 1});
    q.push({set<int>{0}, 1});

    while (!q.empty()) {
        auto [A, p] = q.front();
        q.pop();
        for (int i = 0; i < 26; i++) {
            set<int> X;
            for (auto x : A) {
                for (auto y : dfa[x]) {
                    if (charToInt(s[y-1]) == i) {
                        X.insert(y);
                    }
                }
            }
            if (X.empty()) continue;
            bool found = false;
            for (int j = 0; j < p; j++) {
                if (X == P[j]) {
                    dfa[distance(P, find(P, P+p, A))].insert(j);
                    found = true;
                    break;
                }
            }
            if (!found) {
                P[p] = X;
                q.push({X, p+1});
                dfa[distance(P, find(P, P+p, A))].insert(p);
                P[p].insert(p);
                p++;
            }
        }
    }
    numStates = p;
    numTransitions = 0;
    for (int i = 0; i <= s.size(); i++) {
        numTransitions += dfa[i].size();
        if (acc[i]) {
            numFinalStates++;
        }
    }
    //"Number of States: " <<
  cout << numStates << endl;
  //"Number of Transitions: " <<
    cout<<  numTransitions << endl;
    //"Number of Final States: " <<
    cout<<  numFinalStates << endl;

}
string longestPrefix(string w) {
    string result = "";
    for (int i = 0; i < w.length(); i++) {
        for (int j = i+1; j < w.length(); j++) {
            string v = w.substr(i, j-i);
            bool foundU = false;
            for (int k = j; k <= w.length() - v.length(); k++) {
                string u = w.substr(k, v.length());
                if (u == v) {
                    foundU = true;
                    break;
                }
            }
            if (foundU) {
                bool isPrefix = (w.substr(0, i) == v);
                bool foundAB = false;
                for (char a = 'a'; a <= 'z'; a++) {
                    if (a == w[i]) continue;
                    for (char b = 'a'; b <= 'z'; b++) {
                        if (b == w[i] || b == a) continue;
                        if (w.find(a + v) != string::npos && w.find(b + v) != string::npos) {
                            foundAB = true;
                            break;
                        }
                    }
                    if (foundAB) break;
                }
                if (isPrefix || foundAB) {
                    result = v;
                }
            }
        }
    }
    return result;
}
    int main(){

    string s;
    cin>>s;
    buildDFA(s);
    reachable(0);
    removeUnreachable(0);
    makeAcyclic(0);

    int numStates = 0;
    int numTransitions = 0;
    int numFinalStates = 0;
    minimizeDFA(s, numStates, numTransitions, numFinalStates);


    string longest = longestPrefix(s);
    int count = 0;
    for (int i = 0; i <= s.length() - longest.length(); i++) {
        string v = s.substr(i, longest.length());
        string u = s.substr(i + longest.length());
        if (u.find(v) != string::npos) {
            count++;
        }
    }
    cout << count << endl;
      return 0;
    }
