#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
#include <sstream>
#include <stdio.h>
#include <time.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

#define NUM_CHECKS 15

using namespace flann;

int main(int argc, char** argv)
{
    if(argc != 6) return -1;

	int nn = atoi(argv[1]);
	
	int branching = atoi(argv[2]);

	int trees = atoi(argv[3]);
	
	char* input = argv[4];

	char* output = argv[5];

	int checks[NUM_CHECKS] = {80,100,120,140,160,180,200,250,300,400,500,600,700,800,900};

    Matrix<float> dataset;
    Matrix<float> query;
    load_from_file(dataset, input, "dataset");
    load_from_file(query, input,"query");

    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

	flann::HierarchicalClusteringIndexParams params = flann::HierarchicalClusteringIndexParams(branching, FLANN_CENTERS_RANDOM, trees);

    // construct an randomized kd-tree index using 4 kd-trees
    Index<L2<float> > index(dataset, params);
    
	clock_t trainstart = clock();

	index.buildIndex();                                                                                               

	clock_t trainend = clock() - trainstart;

	unsigned traintime = trainend / CLOCKS_PER_MS;                         

	std::cout << traintime << "\n";

	for(int i=0; i < NUM_CHECKS; i++) {

		clock_t teststart = clock();

    	// do a knn search, using 128 checks
    	index.knnSearch(query, indices, dists, nn, flann::SearchParams(checks[i]));

		clock_t testend = clock() - teststart;

		std::stringstream sstm;
		sstm << output << i;

	   	flann::save_to_file(indices,output,sstm.str());

		unsigned testtime = testend / CLOCKS_PER_MS;                         

		std::cout << checks[i] << "," << testtime << "\n";
	}

    delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();
    
    return 0;
}
