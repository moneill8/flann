#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
#include<set>

#include <stdio.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

using namespace flann;
using namespace std;

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
    clock_t trainstart = clock();

	index.buildIndex();                                                                                               
	clock_t trainend = clock() - trainstart;

	unsigned traintime = trainend / CLOCKS_PER_MS;                         

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

    vector<vector<int> > indicesNNGraph;
    vector<vector<float> > distNNGraph;

    //Index<L2<float> > indexG(dataset, flann::GraphIndexParams(10, 10));
    Index<L2<float> > indexG(dataset, flann::GraphIndexParams(gnn, true, alpha, alpha, alpha));
    
    indexG.buildIndex();

    indexG.knnSearch(query, indicesNNGraph, distNNGraph, nn, flann::SearchParams());

    int correct = 0;
    for (int i = 0; i < indicesNNGraph.size(); ++i) {
        for (int j = 0; j < indicesNNGraph[i].size(); ++j) {
            if (truth[i].count(indicesNNGraph[i][j]))
                ++correct;
        }
    }

    int total = indices.size() * indices[0].size();

    cout << traintime << "," << testtime << "," << (double)correct / total << endl;

    //flann::save_to_file(indices,"result.hdf5","result");

    /*delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();*/
    
    return 0;
}
