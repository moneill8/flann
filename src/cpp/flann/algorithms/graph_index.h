/***********************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright 2008-2009  Marius Muja (mariusm@cs.ubc.ca). All rights reserved.
 * Copyright 2008-2009  David G. Lowe (lowe@cs.ubc.ca). All rights reserved.
 *
 * THE BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************/

#ifndef GRAPH_INDEX_H_
#define GRAPH_INDEX_H_

#include <algorithm>
#include <map>
#include <cassert>
#include <cstring>
#include <queue>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include <set>
#include <iostream>
#include <algorithm>

#include "flann/general.h"
#include "flann/algorithms/nn_index.h"
#include "flann/util/matrix.h"
#include "flann/util/result_set.h"
#include "flann/util/heap.h"
#include "flann/util/allocator.h"
#include "flann/util/random.h"
#include "flann/util/saving.h"

using namespace std;

namespace flann
{

//TODO: Figure out what parameters to use
struct GraphIndexParams : public IndexParams
{
    GraphIndexParams(int gnn = 25, bool approx = false, int alpha1 = 8, int alpha2 = 8, int beta = 8)
    {
        (*this)["algorithm"] = GRAPH_INDEX;
        (*this)["gnn"] = gnn;
        (*this)["approx"] = approx;
        (*this)["alpha1"] = alpha1;
        (*this)["alpha2"] = alpha2;
        (*this)["beta"] = beta;
	}
};


/**
 * Single kd-tree index
 *
 * Contains the k-d trees and other information for indexing a set of points
 * for nearest-neighbor matching.
 */
template <typename Distance>
class GraphIndex : public NNIndex<Distance>
{
public:
    typedef typename Distance::ElementType ElementType;
    typedef typename Distance::ResultType DistanceType;

    typedef NNIndex<Distance> BaseClass;

    /**
     * KDTree constructor
     *
     * Params:
     *          params = parameters passed to the kdtree algorithm
     */
    GraphIndex(const IndexParams& params = GraphIndexParams(), Distance d = Distance() ) :
        BaseClass(params, d)
    {
    	gnn_ = get_param(params, "gnn", 25);
	
        approx_ = get_param(params, "approx", false);
        beta_ = get_param(params, "beta", 8);
        alpha1_ = get_param(params, "alpha1", 8);
        alpha2_ = get_param(params, "alpha2", 8);
    }

    /**
     * KDTree constructor
     *
     * Params:
     *          inputData = dataset with the input features
     *          params = parameters passed to the kdtree algorithm
     */
    GraphIndex(const Matrix<ElementType>& inputData, const IndexParams& params = GraphIndexParams(),
                      Distance d = Distance() ) : BaseClass(params, d)
    {
		gnn_ = get_param(params, "gnn", 25);

        approx_ = get_param(params, "approx", false);
        beta_ = get_param(params, "beta", (int)(log(inputData.rows)+1));
        alpha1_ = get_param(params, "alpha1", (int)(log(inputData.rows)+1));
        alpha2_ = get_param(params, "alpha2", (int)(log(inputData.rows)+1));

		setDataset(inputData);
    }

	//TODO: Set up copy constructor
	/*
    GraphIndex(const KDTreeSingleIndex& other) : BaseClass(other),
            leaf_max_size_(other.leaf_max_size_),
            reorder_(other.reorder_),
            vind_(other.vind_),
            root_bbox_(other.root_bbox_)
    {
        if (reorder_) {
            data_ = flann::Matrix<ElementType>(new ElementType[size_*veclen_], size_, veclen_);
            std::copy(other.data_[0], other.data_[0]+size_*veclen_, data_[0]);
        }
        copyTree(root_node_, other.root_node_);
    }*/

    GraphIndex& operator=(GraphIndex other)
    {
        this->swap(other);
        return *this;
    }
    
    /**
     * Standard destructor
     */
    virtual ~GraphIndex()
    {
        freeIndex();
    }
    
    BaseClass* clone() const
    {
        return new GraphIndex(*this);
    }

    using BaseClass::buildIndex;

    void addPoints(const Matrix<ElementType>& points, float rebuild_threshold = 2)
    {
        assert(points.cols==veclen_);
        extendDataset(points);
        buildIndex();
    }

    flann_algorithm_t getType() const
    {
        return GRAPH_INDEX;
    }

	//TODO: Implement this
    template<typename Archive>
    void serialize(Archive& ar)
    {
        ar.setObject(this);

        //if (reorder_) index_params_["save_dataset"] = false;

        ar & *static_cast<NNIndex<Distance>*>(this);

        //ar & reorder_;
       // ar & leaf_max_size_;
        //ar & root_bbox_;
        //ar & vind_;
		
		/*
        if (Archive::is_loading::value) {
            root_node_ = new(pool_) Node();
        }

        ar & *root_node_;

        if (Archive::is_loading::value) {
            index_params_["algorithm"] = getType();
            index_params_["leaf_max_size"] = leaf_max_size_;
            index_params_["reorder"] = reorder_;
        }*/
    }


    void saveIndex(FILE* stream)
    {
        serialization::SaveArchive sa(stream);
        sa & *this;
    }


    void loadIndex(FILE* stream)
    {
        freeIndex();
        serialization::LoadArchive la(stream);
        la & *this;
    }

    /**
     * Computes the inde memory usage
     * Returns: memory used by the index
     */
    int usedMemory() const
    {
        return pool_.usedMemory+pool_.wastedMemory+size_*sizeof(int);  // pool memory and vind array memory
    }

    /**
     * Find set of nearest neighbors to vec. Their indices are stored inside
     * the result object.
     *
     * Params:
     *     result = the result object in which the indices of the nearest-neighbors are stored
     *     vec = the vector for which to search the nearest neighbors
     *     maxCheck = the maximum number of restarts (in a best-bin-first manner)
     */
    void findNeighbors(ResultSet<DistanceType>& result, const ElementType* vec, const SearchParams& searchParams) const
    {
        //Number of graph expansions
		int e = searchParams.e;

		int t = searchParams.t;
		
		int maxChecks = searchParams.checks;

        graphSearch(result, vec, nodes_, maxChecks, e, t);
		
    }

protected:

    /**
     * Builds the index
     */
    void buildIndexImpl()
    {
        srand(time(NULL));
		nodes_.resize(size_);
		createNodes(nodes_);
		
        if (approx_)
            createApproxGraph(nodes_);
        else
            createGraph(nodes_);
    }

private:

    /*--------------------- Internal Data Structures --------------------------*/
	struct Edge;
	typedef Edge* EdgePtr;
	
	struct Node
	{
		std::vector<EdgePtr> edgeset;
		ElementType *ele;
		int index;
	};
	typedef Node* NodePtr;
	
	struct Edge
	{
		NodePtr src, dest;
	};

	typedef struct
	{
		NodePtr node;
		DistanceType dist;
	} NodeTuple;

	struct OrderByDist
	{
		bool operator() (NodeTuple* &n1, NodeTuple* &n2) { return n1->dist < n2->dist; }
	};
    
	//TODO: Serialize via old code
	
    void freeIndex()
    {
        /*
		if (data_.ptr()) {
            delete[] data_.ptr();
            data_ = flann::Matrix<ElementType>();
        }*/
        //if (root_node_) root_node_->~Node();
        pool_.free();
    }
    
	//TODO: implement graph copy
    /*
	void copyTree(NodePtr& dst, const NodePtr& src)
    {
        dst = new(pool_) Node();
        *dst = *src;
        if (src->child1!=NULL && src->child2!=NULL) {
            copyTree(dst->child1, src->child1);
            copyTree(dst->child2, src->child2);
        }
    }*/

	//Allocate the nodes
	void createNodes(std::vector<NodePtr> &nodes) {
		for(int i=0; i<nodes.size(); i++) {
			NodePtr node = new (pool_) Node();
			node->ele = points_[i];
			node->index = i;
			nodes[i] = node;
		}
	}

    typedef pair<DistanceType, int> di;

    // handles adding to priority queue (set) if better
    bool maybe_add(set<di> &best, int i, int j, int max_size, vector<NodePtr> &nodes) {
        if (i == j) return false;
        
        DistanceType d = distance_(nodes[i]->ele, nodes[j]->ele, veclen_);
        if (best.size() < max_size) {
            best.insert(di(d, j));
            return true;
        }
        else if (d < best.rbegin()->first) {
            di cur(d, j);
            if (best.count(cur)) return false;

            typename set<di>::iterator it = best.end();
            --it;
            best.erase(it);
            best.insert(di(d, j));
            return true;
        }

        return false;
    }

    //Add edges for APPROX graph
    void createApproxGraph(vector<NodePtr> &nodes) {
        vector<set<di> > best(nodes.size());
        for (int i = 0; i < nodes.size(); ++i) {
            for (int j = 0; j < gnn_; ++j) {    //TODO: Consider making the starting seed number a variable.
                int m = rand()%nodes.size();

                maybe_add(best[i], i, m, gnn_, nodes);
            }
        }

        for (int i = 0; i < beta_; ++i) {
            vector<vector<int> > candidate_best(nodes.size());

            for (int j = 0; j < nodes.size(); ++j) {
                int count1 = 0;
                for (typename set<di>::iterator it1 = best[j].begin(); it1 != best[j].end() && count1 < alpha1_; ++it1, ++count1) {
                    int count2 = 0;
                    for (typename set<di>::iterator it2 = best[it1->second].begin(); it2 != best[it1->second].end() && count2 < alpha2_; ++it2, ++count2) {
                        // try adding neighbor's neighbors
                        candidate_best[j].push_back(it2->second);
                    }
                    
                    // try making neighbor add me
                    candidate_best[it1->second].push_back(j);
                }
            }

            for (int j = 0; j < nodes.size(); ++j) {
                for (int k = 0; k < candidate_best[j].size(); ++k) {
                    maybe_add(best[j], j, candidate_best[j][k], gnn_, nodes);
                }
            }
        }

        int added = 0;
        for (int i = 0; i < nodes.size(); ++i) {
            int j = 0;
            for (typename set<di>::iterator it = best[i].begin(); it != best[i].end() && j < gnn_; ++it, ++j) {
                EdgePtr e = new (pool_) Edge();
                e->src = nodes[i];
                e->dest = nodes[it->second];
                nodes[i]->edgeset.push_back(e);
                ++added;
            }
        }

        cerr << "added: " << added << endl;
    }

	//Add edges for EXACT graph
	void createGraph(std::vector<NodePtr> &nodes) {
		std::priority_queue<NodeTuple*, std::vector<NodeTuple*>, OrderByDist> current;
		for(int i=0; i<nodes.size(); i++) {
			for(int j=0; j<nodes.size(); j++) {
				//Find neighbors
				if(i==j) continue;
				if(current.size() < gnn_) {
					DistanceType d = distance_(nodes[i]->ele, nodes[j]->ele, veclen_);
					NodeTuple* nt = new NodeTuple();
					nt->node = nodes[j];
					nt->dist = d;
					current.push(nt);
				}
				else {
					DistanceType d = distance_(nodes[i]->ele, nodes[j]->ele, veclen_);
					if(d < current.top()->dist) {
						NodeTuple* old = current.top();
						delete[] old;
						current.pop();
						NodeTuple* nt = new NodeTuple();
						nt->node = nodes[j];
						nt->dist = d;
						current.push(nt);
					}		
				}
			}
			while(!current.empty()) {
				//Add the edges
				NodeTuple* nt = current.top();
				EdgePtr e = new (pool_) Edge();
				e->src = nodes[i];
				e->dest = nt->node;
				nodes[i]->edgeset.push_back(e);
				delete[] nt;
				current.pop();
			}
			//Need to shuffle to use e in search. Otherwise will bias against examples late in nodes vector
			std::reverse(nodes[i]->edgeset.begin(), nodes[i]->edgeset.end());
		}
	}
	

    /**
     * Performs maxChecks hill climbing searches in the graph starting from random nodes.
     */
    void graphSearch(ResultSet<DistanceType>& result_set, const ElementType* vec, const std::vector<NodePtr> &nodes, const int maxChecks, const int e, const int t) const
    {
		//Don't add same node twice
		int* checked = new int[nodes.size()]();
		NodeTuple* min = new NodeTuple();
		for(int i=0; i < maxChecks; i++) {
			int startIndex = rand_int(nodes.size());
			if(checked[startIndex] == 1) continue;
			NodePtr current = nodes[startIndex];
			DistanceType currdist = distance_(current->ele, vec, veclen_);	
			result_set.addPoint(currdist, current->index);
			checked[current->index] = 1;
			
			DistanceType mindist = distance_(current->edgeset[0]->dest->ele, vec, veclen_);
			min->dist = mindist;
			min->node = current->edgeset[0]->dest;
			result_set.addPoint(min->dist, min->node->index);
			checked[min->node->index] = 1;

			for(int k=0; k<t; k++) {	
				for(int j=1; j < e && j < current->edgeset.size(); j++) {
					if(checked[current->edgeset[j]->dest->index] == 0) {
						checked[current->edgeset[j]->dest->index] = 1;
						DistanceType d = distance_(current->edgeset[j]->dest->ele, vec, veclen_);
						if(d < min->dist) {
							min->dist = d;
							min->node = current->edgeset[j]->dest;
						}
						result_set.addPoint(d, current->edgeset[j]->dest->index);
					}
				}
				//< not <= so we can't go back to previous node
				if(min->dist < currdist) {
					current = min->node;
					currdist = min->dist;
					mindist = distance_(current->edgeset[0]->dest->ele, vec, veclen_);
					min->dist = mindist;
					min->node = current->edgeset[0]->dest;
				}
				else break;
			}
		}
		delete[] min;
		//delete[] checked;
	}

    
    void swap(GraphIndex& other)
    {
        BaseClass::swap(other);
		/*
        std::swap(leaf_max_size_, other.leaf_max_size_);
        std::swap(reorder_, other.reorder_);
        std::swap(vind_, other.vind_);
        std::swap(data_, other.data_);
        std::swap(root_node_, other.root_node_);
        std::swap(root_bbox_, other.root_bbox_);
        */
		std::swap(nodes_, other.nodes_);
		std::swap(gnn_, other.gnn_);
		std::swap(pool_, other.pool_);
    }
    
private:

    
    /**
     * Array of k-d trees used to find neighbours.
     */
    std::vector<NodePtr> nodes_;

	/**
	* Number of nearest neighbors used during graph construction.
	*/
	int gnn_;

    /**
     * Run approximate graph building algorithm
     */
    bool approx_;

    int alpha1_;
    int alpha2_;

    int beta_;

    /**
     * Pooled memory allocator.
     *
     * Using a pooled memory allocator is more efficient
     * than allocating memory directly when there is a large
     * number small of memory allocations.
     */
    PooledAllocator pool_;

    USING_BASECLASS_SYMBOLS

};   // class KDTreeSingleIndex

}

#endif //FLANN_KDTREE_SINGLE_INDEX_H_
