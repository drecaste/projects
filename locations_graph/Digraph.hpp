#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <queue>
#include <iostream>



class DigraphException
{
public:
    DigraphException(const std::string& reason): reason_{reason} { }

    std::string reason() const { return reason_; }

private:
    std::string reason_;
};


template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};


template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};


template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    Digraph();

    Digraph(const Digraph& d);

    ~Digraph();

    Digraph& operator=(const Digraph& d);

    std::vector<int> vertices() const;

    std::vector<std::pair<int, int>> edges() const;

    std::vector<std::pair<int, int>> edges(int vertex) const;

    VertexInfo vertexInfo(int vertex) const;

    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    void addVertex(int vertex, const VertexInfo& vinfo);

    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    void removeVertex(int vertex);

    void removeEdge(int fromVertex, int toVertex);

    int vertexCount() const;

    int edgeCount() const;

    int edgeCount(int vertex) const;

    bool isStronglyConnected() const;

    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:

	int edgesCount;
	std::map< int, DigraphVertex< VertexInfo, EdgeInfo > > map;
	void DFTr(std::map< int, DigraphVertex< VertexInfo, EdgeInfo > > map, int v, std::vector<int> &visited) const;
	bool DFT(std::map< int, DigraphVertex< VertexInfo, EdgeInfo > > map, std::vector<int> allVertices, int v) const;

};

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
{
	edgesCount = 0;
}

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
{
	map = d.map;
	edgesCount = d.edgesCount;
}

	
template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
	if(this != &d)
	{
		map.clear();		// delete
		map = d.map;		// copy
		edgesCount = d.edgesCount;	// copy
	}
	return *this;
}

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph()
{
}

template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
	std::vector<int> vertices;

	for(auto iterator = map.begin(); iterator != map.end(); iterator++)
	{
		vertices.push_back(iterator->first);
	}
	return vertices;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
	std::vector< std::pair< int, int > > allEdges;

	for(int i = 0; i < map.size(); i++)
	{
		for(auto iterator = map.at(i).edges.begin(); iterator != map.at(i).edges.end(); iterator++)
		{
			std::pair< int, int> Pair;
			Pair.first = iterator->fromVertex;
			Pair.second = iterator->toVertex;
			allEdges.push_back(Pair);
		}
	}
	return allEdges;
}

template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
	std::vector< std::pair< int, int > > allEdges;

	for(auto iterator = map.at(vertex).edges.begin(); iterator != map.at(vertex).edges.end(); iterator++)
	{
		std::pair<int, int> Pair;
		Pair.first = iterator->fromVertex;
		Pair.second = iterator->toVertex;
		allEdges.push_back(Pair);
	}
	return allEdges;
}

template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
	if(map.find(vertex) != map.end())
	{
		return map.at(vertex).vinfo;
	}
	else
	{
		throw DigraphException("Vertex not found");
	}
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
	if(map.find(fromVertex) != map.end())
	{
		EdgeInfo einfo;
		for(auto iterator = map.at(fromVertex).edges.begin(); iterator != map.at(fromVertex).edges.end(); iterator++)
		{
			if( (fromVertex == iterator->fromVertex)
				&& (toVertex == iterator->toVertex) )
			{
				einfo = iterator->einfo;
			}
		}
		return einfo;
	}

	else
	{
		throw DigraphException("Vertex not found");
	}
}

template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
	if(map.find(vertex) == map.end())
	{
		DigraphVertex < VertexInfo, EdgeInfo > newVertex;
		newVertex.vinfo = vinfo;
		map.emplace(vertex, newVertex);
	}
	else
	{
		throw DigraphException("Vertex already exists, cannot add");
	}
}

template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
	if( (map.find(fromVertex) != map.end()) && (map.find(toVertex) != map.end()) )
	{
		for(auto iterator = map.at(fromVertex).edges.begin(); iterator != map.at(fromVertex).edges.end(); iterator++)
		{
			if( (fromVertex == iterator->fromVertex) && (toVertex == iterator->toVertex) )
			{
				throw DigraphException("Edge already exists");
				return;
			}
		}
		DigraphEdge<EdgeInfo> newEdge;
		newEdge.einfo = einfo;
		newEdge.fromVertex = fromVertex;
		newEdge.toVertex = toVertex;
		map.at(fromVertex).edges.push_back(newEdge);
		edgesCount++;

	}
	else
	{
		throw DigraphException("Vertex not found");
	}
}

template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
	if(map.find(vertex) != map.end())
	{
		map.erase(vertex);
	}
	else
	{
		throw DigraphException("Vertex not found");
	}

}

template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
	if(map.find(fromVertex) != map.end())
	{
		for(auto iterator = map.at(fromVertex).edges.begin(); iterator != map.at(fromVertex).edges.end(); iterator++)
		{
			if( (fromVertex == iterator->fromVertex) && (toVertex == iterator->toVertex) )
			{
				edgesCount--;
				map.at(fromVertex).edges.erase(iterator);
				return;
			}
		}
	}
	else
	{
		throw DigraphException("Vertex not found");
	}
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const
{
	return map.size();
}

template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const
{
	return edgesCount;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
	return map.at(vertex).edges.size();
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::DFTr(std::map< int, DigraphVertex< VertexInfo, EdgeInfo > > map, int v, std::vector<int> &visited) const
{
	visited[v] = 1;
	for(auto iterator = map.at(v).edges.begin(); iterator != map.at(v).edges.end(); iterator++)
	{
		if(visited[iterator->toVertex] == 0)
		{
			DFTr(map, iterator->toVertex, visited);
		}
	}
}

template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::DFT(std::map< int, DigraphVertex< VertexInfo, EdgeInfo > > map, std::vector<int> allVertices, int v) const
{
	std::vector<int> visited;
	for(int j = 0; j < allVertices.size(); j++)
	{
		visited.push_back(0);
	}
	DFTr(map, v, visited);
	
	if(std::find(visited.begin(), visited.end(), 0) != visited.end())
	{
		return false;
	}
	return true;
}

template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
	std::vector<int> allVertices = vertices();
	bool res = true;
	
	for(int i = 0; i < allVertices.size(); i++)
	{
		res = DFT(map, allVertices, allVertices[i]);
		if(res == false)
		{
			return false;
		}
	}
	return true;
}


template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
	class comparison
	{
	public:
		bool operator()(std::pair<int, double> first, std::pair<int, int> second)
		{
		    return first.second < second.second;
		}
	};

	std::priority_queue< std::pair<int, double>, std::vector< std::pair<int, int> >, comparison > pq;
	std::vector<int> allVertices = vertices();
	int max_int = std::numeric_limits<int>::max();
	
	std::map<int, double> dv;
	std::map<int, int> pv;

	dv.emplace(startVertex, 0.0);
	pv.emplace(startVertex, startVertex);
	
	for(int i = 0; i < allVertices.size(); i++)
	{
		if(allVertices[i] != startVertex)
		{
			dv.emplace(allVertices[i], max_int);
			pv.emplace(allVertices[i], 0);
		}
	}

	pq.push(std::make_pair(startVertex, 0));

	while( !( pq.empty() ) )
	{
		int v = pq.top().first;
		pq.pop();

		for(auto iterator = map.at(v).edges.begin(); iterator != map.at(v).edges.end(); iterator++)
		{
			double weightResult = edgeWeightFunc(iterator->einfo);
			int w = iterator->toVertex;
			if(dv.at(w) > (weightResult + dv.at(v)))
			{
				dv.at(w) = weightResult + dv.at(v);
				pv.at(w) = v;
				pq.push(std::make_pair(w, dv.at(w)));
			}
		}
	}
	return pv;
}



#endif // DIGRAPH_HPP
