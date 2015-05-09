#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
#include<set>

#include <stdio.h>

using namespace flann;
using namespace std;

int main(int argc, char** argv)
{
    int nn = 3;

    Matrix<float> dataset;
    Matrix<float> query;
    
    load_from_file(dataset, "dataset.hdf5","dataset");
    load_from_file(query, "dataset.hdf5","query");

    //load_from_file(dataset, "sift100K.h5", "dataset");
    //load_from_file(query, "sift100K.h5", "query");
    
    //Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    //Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    vector<vector<int> > indices;
    vector<vector<float> > dist;

    Index<L2<float> > index(dataset, flann::LinearIndexParams());
    index.buildIndex();                                                                                               

    index.knnSearch(query, indices, dist, nn, flann::SearchParams());

    vector<set<int> > truth(indices.size());
    for (int i = 0; i < indices.size(); ++i) {
        for (int j = 0; j < indices[i].size(); ++j) {
            truth[i].insert(indices[i][j]);
        }
    }

    vector<vector<int> > indicesNNGraph;
    vector<vector<float> > distNNGraph;

    cerr << "rows: " << dataset.rows << endl;
    //Index<L2<float> > indexG(dataset, flann::GraphIndexParams(10, 10));
    Index<L2<float> > indexG(dataset, flann::GraphIndexParams(100, 100, true, 6, 6, 6));
    
    cerr << "building index" << endl;
    indexG.buildIndex();
    cerr << "index built" << endl;

    indexG.knnSearch(query, indicesNNGraph, distNNGraph, nn, flann::SearchParams());

    int correct = 0;
    for (int i = 0; i < indicesNNGraph.size(); ++i) {
        for (int j = 0; j < indicesNNGraph[i].size(); ++j) {
            if (truth[i].count(indicesNNGraph[i][j]))
                ++correct;
        }
    }

    int total = indices.size() * indices[0].size();

    cerr << (double)correct / total << endl;

    //flann::save_to_file(indices,"result.hdf5","result");

    /*delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();*/
    
    return 0;
}
