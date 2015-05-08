#include <iostream>
#include <vector>
#include <queue>
#include <cstdio>
#include <functional>
#include <set>

#define RANGE 100000
#define N 1000
#define D 20
#define K 5
#define ALPHA 9
#define BETA 9

using namespace std;

typedef long long ll;
typedef vector<ll> vi;
typedef vector<vi> vvi;
typedef pair<ll, ll> ii;

vvi random_instance(int n, int d) {
    vvi points;

    for (int i = 0; i < n; ++i) {
        vi point;

        for (int j = 0; j < d; ++j) {
            point.push_back(rand() % RANGE); 
        }

        points.push_back(point);
    }

    return points;
}

ll dist_sqr(vi one, vi two) {
    ll dist = 0;
    for (int i = 0; i < one.size(); ++i) {
        dist += (one[i] - two[i]) * (one[i] - two[i]);
    }

    return dist;
}

void maybe_add(set<ii> &best, ll i, ll j, ll k, vvi &points) {
    if (i == j) return;
    
    double cur_dist = dist_sqr(points[i], points[j]);
    if (best.size() < k) {
        best.insert(ii(cur_dist, j));
    }
    else if (cur_dist < best.rbegin()->first) {
        set<ii>::iterator it = best.end();
        --it;
        best.erase(it);
        best.insert(ii(cur_dist, j));
    }     
}

//O(n^2d)
vector<set<ll> > exact_kNN(vvi &points, ll k) {
    vector<set<ll> > result(points.size());
    for (ll i = 0; i < points.size(); ++i) {
        set<ii> best;

        for (ll j = 0; j < points.size(); ++j) {
            if (i == j) {
                continue;
            }
     
            maybe_add(best, i, j, k, points);   
        }

        for (set<ii>::iterator it = best.begin(); it != best.end(); ++it) {
            result[i].insert(it->second);
        }
    }

    return result;
}

//O(nda^2b), set a=b=log(n) and get O(n log^3 n)
vvi approx_kNN(vvi &points, ll k, ll alpha, ll beta) {
    vector<set<ii> > best(points.size());
    vector<ll> has_me(points.size());

    for (ll i = 0; i < points.size(); ++i) {
        for (ll j = 0; j < alpha; ++j) {
            ll m = rand()%points.size();
            if (m == i) {
                --j;
                continue;
            }

            best[i].insert(ii(dist_sqr(points[i], points[m]), m));
            has_me[m].insert(i);
        }
    }

    for (ll i = 0; i < beta; ++i) {
        vector<set<ii> > new_best = best;

        for (ll j = 0; j < points.size(); ++j) {
            for(set<ii>::iterator it1 = best[j].begin(); it1 != best[j].end(); ++it1) {
                for (set<ii>::iterator it2 = best[it1->second].begin(); it2 != best[it1->second].end(); ++it2) {
                    maybe_add(new_best[j], j, it2->second, alpha, points);
                }
            }

            for (ll m = 0; m < has_me.size(); ++m) {
                
            }
        }

        best = new_best;
    }

    vvi result(points.size());
    for (ll i = 0; i < points.size(); ++i) {
        ll j = 0;
        for (set<ii>::iterator it = best[i].begin(); it != best[i].end(); ++it) {
            if (j == k) break;
            ++j;

            result[i].push_back(it->second);
        }
    }

    return result;
}

int main() {
    vvi points = random_instance(N, D);
    vvi approx = approx_kNN(points, K, ALPHA, BETA);
    cerr << "approx done" << endl;
    vector<set<ll> > exact = exact_kNN(points, K);

    cerr << "exact done" << endl;
    ll correct = 0;
    for (ll i = 0; i < N; ++i) {
        for (ll j = 0; j < K; ++j) {
            if (exact[i].count(approx[i][j]))
                ++correct;
        }
    }

    cout << (double)correct / (N*K) << endl;

    return 0;
}
