
#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
#include <flann/util/params.h>

#include <stdio.h>
#include <sstream>
#include <time.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

#define NUM_CHECKS 10

using namespace flann;

int main(int argc, char** argv)
{
	if(argc != 5) return -1;

	int nn = atoi(argv[1]);
	
	int gnn = atoi(argv[2]);
	
	char* input = argv[3];

	char* output = argv[4];

	int checks[NUM_CHECKS] = {1,2,3,4,5,6,7,8,9,10};

    Matrix<float> dataset;
    Matrix<float> query;
    load_from_file(dataset, input,"dataset");
    load_from_file(query, input,"query");

    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    // construct a nearest neighbor graph with 10 nearest neighbors
	Index<L2<float> > index(dataset, flann::GraphIndexParams(gnn));

	clock_t trainstart = clock();

    index.buildIndex();

	clock_t trainend = clock() - trainstart;

	unsigned traintime = trainend / CLOCKS_PER_MS;                         

	std::cout << traintime << "\n";

	for(int i=0; i < NUM_CHECKS; i++) {

		// do a knn search, using 128 checks, and e = edgeset size
    	SearchParams p = flann::SearchParams();
		p.checks = checks[i];
		p.e = gnn;
		p.use_heap = FLANN_True;
	
		clock_t teststart = clock();

		index.knnSearch(query, indices, dists, nn, p);

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
