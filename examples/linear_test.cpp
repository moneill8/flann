
#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
#include <flann/util/params.h>

#include <stdio.h>

#include <time.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

using namespace flann;

int main(int argc, char** argv)
{
	if(argc != 4) return -1;

	int nn = atoi(argv[1]);
	
	char* input = argv[2];

	char* output = argv[3];


    Matrix<float> dataset;
    Matrix<float> query;
    load_from_file(dataset, input,"dataset");
    load_from_file(query, input,"query");

    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    // construct a linear index
	Index<L2<float> > index(dataset, flann::LinearIndexParams());

	clock_t trainstart = clock();

    index.buildIndex();

	clock_t trainend = clock() - trainstart;

	// do a knn search
    SearchParams p = flann::SearchParams();
	p.use_heap = FLANN_True;
	
	clock_t teststart = clock();

	index.knnSearch(query, indices, dists, nn, p);

	clock_t testend = clock() - teststart;

    flann::save_to_file(indices,output,"result");


	unsigned traintime = trainend / CLOCKS_PER_MS;                         
	unsigned testtime = testend / CLOCKS_PER_MS;

	std::cout << traintime << "," << testtime << "\n";

    delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();
    
    return 0;
}
