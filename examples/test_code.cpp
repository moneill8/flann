#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
#include<set>

#include <stdio.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

using namespace flann;
using namespace std;

void test(vector<set<int> > &truth, int nn, int e, int t, Index<L2<float> > &indexG, Matrix<float> &query) {
    vector<vector<int> > indicesNNGraph;
    vector<vector<float> > distNNGraph;

    clock_t graphteststart = clock();

    flann::SearchParams params;
    params.t = t;
    params.e = e;

    indexG.knnSearch(query, indicesNNGraph, distNNGraph, nn, params);
    clock_t graphtestend = clock() - graphteststart;
    unsigned graphtesttime = graphtestend / CLOCKS_PER_MS;

    int correct = 0;
    for (int i = 0; i < indicesNNGraph.size(); ++i) {
        for (int j = 0; j < indicesNNGraph[i].size(); ++j) {
            if (truth[i].count(indicesNNGraph[i][j]))
                ++correct;
        }
    }

    int total = indicesNNGraph.size() * indicesNNGraph[0].size();

    cout << "e = " << e << ", t = " << t << endl;
    cout << "Graph time: " << graphtesttime << endl;
    cout << "Graph accuracy: " << (double)correct / total << endl;
}
    
int main(int argc, char** argv)
{
    int nn = atoi(argv[1]);

	int gnn = atoi(argv[2]);
	
	int alpha = atoi(argv[3]);
	

    Matrix<float> dataset;
    Matrix<float> query;
    
    //load_from_file(dataset, "dataset.hdf5","dataset");
    //load_from_file(query, "dataset.hdf5","query");

    load_from_file(dataset, "sift100K.h5", "dataset");
    load_from_file(query, "sift100K.h5", "query");
    
    //Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    //Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    vector<vector<int> > indices;
    vector<vector<float> > dist;

    Index<L2<float> > index(dataset, flann::LinearIndexParams());

	index.buildIndex();                                                                                               


	clock_t teststart = clock();

    index.knnSearch(query, indices, dist, nn, flann::SearchParams());

	clock_t testend = clock() - teststart;

	unsigned testtime = testend / CLOCKS_PER_MS;                         


    vector<set<int> > truth(indices.size());
    for (int i = 0; i < indices.size(); ++i) {
        for (int j = 0; j < indices[i].size(); ++j) {
            truth[i].insert(indices[i][j]);
        }
    }

    Index<L2<float> > indexG(dataset, flann::GraphIndexParams(gnn, true, alpha, alpha, alpha));

    clock_t trainstart = clock();
    indexG.buildIndex();
	clock_t trainend = clock() - trainstart;
	unsigned traintime = trainend / CLOCKS_PER_MS;                         

    cout << "Train time: " << traintime << endl;
    cout << "Linear time: " << testtime << endl;

    int e[] = {10, 15, 20, 30, 50, 75, 100};
    int t[] = {1, 3, 5, 10, 20, 40};

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 5; ++j) {
            test(truth, nn, e[i], t[j], indexG, query);
        }
    }

    return 0;
}
