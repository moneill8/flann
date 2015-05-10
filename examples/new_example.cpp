
#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
#include <flann/util/params.h>

#include <stdio.h>
#include <sstream>
#include <time.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

#define NUM_ES 4
#define NUM_TS 10

using namespace flann;

int main(int argc, char** argv)
{
	if(argc != 5) return -1;

	int nn = atoi(argv[1]);
	
	int gnn = atoi(argv[2]);
	
	char* input = argv[3];

	char* output = argv[4];

    Matrix<float> dataset;
    Matrix<float> query;
    load_from_file(dataset, input,"dataset");
    load_from_file(query, input,"query");

    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    // construct a nearest neighbor graph with 10 nearest neighbors
	Index<L2<float> > index(dataset, flann::GraphIndexParams(gnn));

	int ts[NUM_TS] = {25, 50, 75, 100, 200, 300, 400, 500, 600, 700};

	clock_t trainstart = clock();

    index.buildIndex();

	clock_t trainend = clock() - trainstart;

	unsigned traintime = trainend / CLOCKS_PER_MS;                         

	std::cout << traintime << "\n";

	for(int i=0; i < NUM_TS; i++) {
		int e = gnn/NUM_ES;
		for(int j=0; j < NUM_ES; j++) {

			// do a knn search, using 128 checks, and e = edgeset size
			SearchParams p = flann::SearchParams();
			
			p.e = e;
			e = e + gnn/NUM_ES;
			p.t = ts[i];
			p.use_heap = FLANN_True;
		
			clock_t teststart = clock();

			index.knnSearch(query, indices, dists, nn, p);

			clock_t testend = clock() - teststart;

			std::stringstream sstm;
			sstm << output  << ts[i] <<  "_" << p.e;

			flann::save_to_file(indices,output,sstm.str());

			unsigned testtime = testend / CLOCKS_PER_MS;

			std::cout << ts[i] << "," << p.e << "," << testtime << "\n";
		}
	}
    delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();
    
    return 0;
}
