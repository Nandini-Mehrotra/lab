// labtest
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Option {
    int time, lab;
};

string name[] = {"A", "B", "C"};

vector<Option> domain[3] = {
    {{1,1}, {2,1}},
    {{2,1}, {2,2}, {3,2}, {4,2}},
    {{1,2}, {3,2}, {4,2}}
};

int assigned[3] = {-1, -1, -1};
Option answer[3];

bool conflict(int a, Option x, int b, Option y) {
    if ((a == 0 && b == 1) || (a == 1 && b == 0))
        if (x.time == y.time) return true;

    if ((a == 1 && b == 2) || (a == 2 && b == 1))
        if (x.time == y.time) return true;

    if (x.time == y.time && x.lab == y.lab)
        return true;

    if ((a == 0 && b == 2) || (a == 2 && b == 0))
        if (abs(x.time - y.time) == 1) return true;

    return false;
}

bool safe(int var, Option x) {
    for (int i = 0; i < 3; i++)
        if (assigned[i] != -1 && conflict(var, x, i, answer[i]))
            return false;

    return true;
}

vector<Option> validDomain(int var) {
    vector<Option> v;

    for (Option x : domain[var])
        if (safe(var, x))
            v.push_back(x);

    return v;
}

int degree(int var) {
    int d = 0;

    for (int i = 0; i < 3; i++) {
        if (i == var || assigned[i] != -1) continue;

        if ((var == 0 && i == 1) || (var == 1 && i == 0) ||
            (var == 1 && i == 2) || (var == 2 && i == 1) ||
            (var == 0 && i == 2) || (var == 2 && i == 0))
            d++;
    }

    return d;
}

int selectMRV() {
    int best = -1;
    int minSize = 100;
    int maxDegree = -1;

    for (int i = 0; i < 3; i++) {
        if (assigned[i] != -1) continue;

        int size = validDomain(i).size();
        int deg = degree(i);

        if (size < minSize || (size == minSize && deg > maxDegree)) {
            minSize = size;
            maxDegree = deg;
            best = i;
        }
    }

    return best;
}

void showDomains() {
    for (int i = 0; i < 3; i++) {
        if (assigned[i] != -1) continue;

        vector<Option> v = validDomain(i);

        cout << name[i] << ": ";

        if (v.empty())
            cout << "EMPTY";

        for (Option x : v)
            cout << "(" << x.time << ",L" << x.lab << ") ";

        cout << endl;
    }
}

bool forwardCheck() {
    for (int i = 0; i < 3; i++)
        if (assigned[i] == -1 && validDomain(i).empty())
            return false;

    return true;
}

bool solve() {
    int var = selectMRV();

    if (var == -1)
        return true;

    vector<Option> values = validDomain(var);

    cout << "\nMRV selected: " << name[var] << endl;

    for (Option x : values) {
        assigned[var] = 1;
        answer[var] = x;

        cout << "Assign " << name[var] << " = Time "
             << x.time << ", Lab L" << x.lab << endl;

        cout << "Domains after Forward Checking:\n";
        showDomains();

        if (forwardCheck()) {
            if (solve())
                return true;
        } else {
            cout << "Domain became empty. Backtracking required.\n";
        }

        cout << "Backtracking from " << name[var] << endl;
        assigned[var] = -1;
    }

    return false;
}

int main() {
    cout << "Initial Domains:\n";
    showDomains();

    if (solve()) {
        cout << "\nFinal Schedule:\n";
        cout << "Section\tTime Slot\tLab\n";

        for (int i = 0; i < 3; i++)
            cout << name[i] << "\t"
                 << answer[i].time << "\t\tL"
                 << answer[i].lab << endl;
    } else {
        cout << "No solution exists.\n";
    }

    return 0;
}

// Time Complexity: O(d^n), Space Complexity: O(n*d)
