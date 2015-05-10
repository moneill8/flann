
#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
#include <sstream>
#include <stdio.h>
#include <time.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

#define NUM_CHECKS 6

using namespace flann;

int main(int argc, char** argv)
{
    if(argc != 5) return -1;

	int nn = atoi(argv[1]);
	
	int trees = atoi(argv[2]);
	
	char* input = argv[3];

	char* output = argv[4];

	int checks[NUM_CHECKS] = {1000,1200,1400,1600,1800,2000};

    Matrix<float> dataset;
    Matrix<float> query;
    load_from_file(dataset, input, "dataset");
    load_from_file(query, input,"query");

    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    // construct an randomized kd-tree index using 4 kd-trees
    Index<L2<float> > index(dataset, flann::KDTreeIndexParams(trees));
    
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
