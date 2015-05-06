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

#include "flann/general.h"
#include "flann/algorithms/nn_index.h"
#include "flann/util/matrix.h"
#include "flann/util/result_set.h"
#include "flann/util/heap.h"
#include "flann/util/allocator.h"
#include "flann/util/random.h"
#include "flann/util/saving.h"

namespace flann
{

//TODO: Figure out what parameters to use
struct GraphIndexParams : public IndexParams
{
    GraphIndexParams(int gnn = 3)
    {
        (*this)["algorithm"] = GRAPH_INDEX;
        (*this)["gnn"] = gnn;
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
    	gnn_ = get_param(params, "gnn", 3);
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
		gnn_ = get_param(params, "gnn", 3);
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
        float epsError = 1+searchParams.eps;
		
		int maxChecks = searchParams.checks;

        graphSearch(result, vec, nodes_, maxChecks, epsError);
    }

protected:

    /**
     * Builds the index
     */
    void buildIndexImpl()
    {
        nodes_.resize(size_);
		createNodes(nodes_);
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
	
	//Add edges for EXACT graph
	//TODO: Approximate graph construction
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
		}
	}
	

    /**
     * Performs maxChecks hill climbing searches in the graph starting from random nodes.
     */
    void graphSearch(ResultSet<DistanceType>& result_set, const ElementType* vec, std::vector<NodePtr> nodes, const int maxChecks, const float epsError) const
    {
		//Don't add same node twice
		int* checked = new int[nodes.size()]();
		NodeTuple* min = new NodeTuple();
		for(int i=0; i < maxChecks; i++) {
			int startIndex = rand_int(nodes.size());
			NodePtr current = nodes[startIndex];
			DistanceType currdist = distance_(current->ele, vec, veclen_);	
			if(checked[current->index] == 0) {
				result_set.addPoint(currdist, current->index);
			}
			checked[current->index] = 1;

			DistanceType mindist = distance_(current->edgeset[0]->dest->ele, vec, veclen_);
			min->dist = mindist;
			min->node = current->edgeset[0]->dest;
			if(min->dist < result_set.worstDist()) {
				result_set.addPoint(min->dist, min->node->index);
			}
			checked[min->node->index] = 1;

			while(1) {	
				for(int j=1; j < current->edgeset.size(); j++) {
					if(checked[current->edgeset[j]->dest->index] == 0) {
						checked[current->edgeset[j]->dest->index] = 1;
						DistanceType d = distance_(current->edgeset[j]->dest->ele, vec, veclen_);
						if(d < min->dist) {
							min->dist = d;
							min->node = current->edgeset[i]->dest;
						}
						if(d < result_set.worstDist()) {
							result_set.addPoint(d, current->edgeset[j]->dest->index);
						}
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
		delete[] checked;
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
