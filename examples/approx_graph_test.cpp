
#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
#include <flann/util/params.h>

#include <stdio.h>
#include <sstream>
#include <time.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

#define NUM_ES 8

#define NUM_TS 5

using namespace flann;

int main(int argc, char** argv)
{
	if(argc != 8) return -1;

	int nn = atoi(argv[1]);
	
	int gnn = atoi(argv[2]);
	
	int alpha1 = atoi(argv[3]);

	int alpha2 = atoi(argv[4]);

	int beta = atoi(argv[5]);

	char* input = argv[6];

	char* output = argv[7];

	int ts[NUM_TS] = {4,5,6,7,10};

    Matrix<float> dataset;
    Matrix<float> query;
    load_from_file(dataset, input,"dataset");
    load_from_file(query, input,"query");

    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    // construct a nearest neighbor graph with 10 nearest neighbors
	Index<L2<float> > index(dataset, flann::GraphIndexParams(gnn, true, alpha1, alpha2, beta));

	clock_t trainstart = clock();

    index.buildIndex();

	clock_t trainend = clock() - trainstart;

	unsigned traintime = trainend / CLOCKS_PER_MS;                         

	std::cout << traintime << "\n";


	for(int i=0; i < NUM_TS; i++) {
		int e = (gnn)/3;
		for(int j=0; j < NUM_ES+1; j++) {
		// do a knn search, using 128 checks, and e = edgeset size
			SearchParams p = flann::SearchParams();
			p.e = gnn;
			e = e + gnn/(3*(NUM_ES/2));
			p.t = ts[i];
			p.use_heap = FLANN_True;
			
			clock_t teststart = clock();

			index.knnSearch(query, indices, dists, nn, p);

			clock_t testend = clock() - teststart;

			std::stringstream sstm;
			sstm << output << ts[i] << "_" << e;

			flann::save_to_file(indices,output,sstm.str());

			unsigned testtime = testend / CLOCKS_PER_MS;

			std::cout << ts[i] << "," << e << "," << testtime << "\n";
		}
	}
	delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();
    
    return 0;
}
