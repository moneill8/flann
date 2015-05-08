#include <flann/flann.hpp>
#include <flann/io/hdf5.h>

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
    
    //load_from_file(dataset, "dataset.hdf5","dataset");
    //load_from_file(query, "dataset.hdf5","query");

    //Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    //Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    vector<vector<int> > indices;
    vector<vector<float> > dist;

    // construct an randomized kd-tree index using 4 kd-trees
    Index<L2<float> > index(dataset, flann::KDTreeIndexParams(4));
    index.buildIndex();                                                                                               

    // do a knn search, using 128 checks
    index.knnSearch(query, indices, dist, nn, flann::SearchParams(128));

    for (int i = 0; i < indices.size(); ++i) {
        for (int j = 0; j < indices[i].size(); ++j) {
            cerr << indices[i][j] << " ";
        }
        cerr << endl;
    }

    //flann::save_to_file(indices,"result.hdf5","result");

    /*delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();*/
    
    return 0;
}
