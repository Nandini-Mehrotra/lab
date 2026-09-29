#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
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
    int best = -1, minSize = 100, maxDegree = -1;

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

        if (forwardCheck() && solve())
            return true;

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




///////////


#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
using namespace std;

const int N = 20;

int x[N] = {
    60,23,15,85,71,98,50,20,40,75,
    82,91,12,37,28,66,55,73,88,42
};

int y[N] = {
    200,45,150,90,123,45,220,30,180,155,
    60,120,210,100,77,89,130,140,33,170
};

double distCity(int a, int b) {
    double dx = x[a] - x[b];
    double dy = y[a] - y[b];
    return sqrt(dx * dx + dy * dy);
}

double distanceTour(vector<int> tour) {
    double d = 0;

    for (int i = 0; i < N - 1; i++)
        d += distCity(tour[i], tour[i + 1]);

    d += distCity(tour[N - 1], tour[0]);

    return d;
}

vector<int> randomTour() {
    vector<int> tour;

    for (int i = 0; i < N; i++)
        tour.push_back(i);

    for (int i = N - 1; i > 1; i--) {
        int j = 1 + rand() % i;
        swap(tour[i], tour[j]);
    }

    return tour;
}

int selectParent(vector<vector<int> > &pop) {
    vector<double> fitness(pop.size());
    double total = 0;

    for (int i = 0; i < pop.size(); i++) {
        fitness[i] = 1.0 / distanceTour(pop[i]);
        total += fitness[i];
    }

    double r = ((double)rand() / RAND_MAX) * total;
    double sum = 0;

    for (int i = 0; i < pop.size(); i++) {
        sum += fitness[i];

        if (sum >= r)
            return i;
    }

    return pop.size() - 1;
}

vector<int> crossover(vector<int> p1, vector<int> p2) {
    vector<int> child(N, -1);
    child[0] = 0;

    int a = 1 + rand() % (N - 1);
    int b = 1 + rand() % (N - 1);

    if (a > b)
        swap(a, b);

    for (int i = a; i <= b; i++)
        child[i] = p1[i];

    int pos = 1;

    for (int i = 1; i < N; i++) {
        int city = p2[i];
        bool found = false;

        for (int j = a; j <= b; j++)
            if (child[j] == city)
                found = true;

        if (!found) {
            while (pos >= a && pos <= b)
                pos = b + 1;

            if (pos < N)
                child[pos++] = city;
        }
    }

    return child;
}

void mutate(vector<int> &tour) {
    if (rand() % 100 < 10) {
        int a = 1 + rand() % (N - 1);
        int b = 1 + rand() % (N - 1);
        swap(tour[a], tour[b]);
    }
}

void GA(int popSize, int generations) {
    vector<vector<int> > pop;

    for (int i = 0; i < popSize; i++)
        pop.push_back(randomTour());

    for (int gen = 0; gen < generations; gen++) {
        int best = 0;

        for (int i = 1; i < popSize; i++)
            if (distanceTour(pop[i]) < distanceTour(pop[best]))
                best = i;

        vector<vector<int> > newPop;
        newPop.push_back(pop[best]);

        while (newPop.size() < popSize) {
            int p1 = selectParent(pop);
            int p2 = selectParent(pop);

            vector<int> child = crossover(pop[p1], pop[p2]);

            mutate(child);
            newPop.push_back(child);
        }

        pop = newPop;
    }

    int best = 0;

    for (int i = 1; i < popSize; i++)
        if (distanceTour(pop[i]) < distanceTour(pop[best]))
            best = i;

    cout << "\nPopulation = " << popSize
         << ", Generations = " << generations << endl;

    cout << "Best Tour: ";

    for (int city : pop[best])
        cout << city + 1 << " -> ";

    cout << pop[best][0] + 1 << endl;

    cout << "Total Distance = "
         << fixed << setprecision(2)
         << distanceTour(pop[best]) << endl;
}

int main() {
    srand(1);

    GA(10, 100);
    GA(20, 100);
    GA(30, 100);
    GA(10, 200);
    GA(20, 200);
    GA(30, 200);

    return 0;
}

// Time Complexity: O(G*P^2*N), Space Complexity: O(P*N)
