#include "trailblazer.h"
#include "queue.h"
#include "set.h"
#include "map.h"
#include "pqueue.h"
#include "point.h"
#include <limits>
#include "math.h"
using namespace std;

// this makes Path an alias for the type Vector<Vertex*>
typedef Vector<Vertex*> Path;

const double INF = numeric_limits<double>::max();
// the minimum difference for an alternate route
const double SUFFICIENT_DIFFERENCE = 0.2;

/**
 * @brief breadthFirstSearch - returns the quickest path (least number of jumps) from start to end
 * does not take into account path weights, simply the amount of jumps needed to reach the end node
 *
 * essentially dijkstra's algorithm but all paths are weighted the same amount
 * we can skip upwards of a cycle though by differentiating from dijkstra's and BFS,
 * as BFS does the end check at the neighbor of the end node rather than when the end node is popped out of the queue
 */
Path breadthFirstSearch(RoadGraph& graph, Vertex* start, Vertex* end) {
    // path to be returned
    Path validPath;

    // initial path containing only start
    Path startPath;
    start->visited = true;
    start->setColor(GREEN);
    startPath.push_back(start);

    // list containign choices
    queue<Path> list({startPath});

    // while there are still choices left to make
    while(!list.empty() && validPath.isEmpty())
    {
        // the path at the front of the queue, our current choice
        Path currentPath = list.front();
        // the vertex at the end of the path that we chose
        Vertex* current = currentPath.get(currentPath.size()-1);
        list.pop();

        // for each of the vertex's neighbors
        for(Edge* edge: current->edges)
        {
            // we are trying to construct new paths by appending
            // currentVertex to each of its neighbors
            // and then adding that new path back into the queue
            // thus we must backtrack back to the original path that we popped out of the queue
            // at each iteration
            Path backtrackingPath = currentPath;

            // if we have reached the end node, then we are done
            // as the first time we see this node is the quickest path
            // (least number of jumps) needed to reach it
            if(edge->end == end)
            {
                backtrackingPath.push_back(edge->end);
                validPath = backtrackingPath;
            }

            // otherwise, if we have reached a node that we havent seen before
            // mark it as seen and add it to the current path
            // then push that path onto the queue to be processed later
            else if(!edge->end->visited)
            {
                edge->end->visited = true;
                edge->end->setColor(GREEN);
                backtrackingPath.push_back(edge->end);
                list.push(backtrackingPath);
            }
        }
    }
    return validPath;
}

/**
 * @brief getHeuristicValue - returns a heuristic based on the passed in road graph from start to the goal
 */
double getHeuristicValue(RoadGraph& graph, Vertex* start, Vertex* goal) {
    return graph.getCrowFlyDistance(start,goal) / graph.getMaxRoadSpeed();
}

/**
 * @brief dijkstraPathFinder - helper function for aStar and dijkstra
 * returns the most efficient weighted path between start and end
 * optionally using a heuristic value
 * ignores edges found in filter
 */
Path dijkstraPathFinder(RoadGraph& graph, Vertex* start, Vertex* end, bool bUsingHeuristic, Set<Edge*> filter) {

    // essentiially enables or disables the heuristic feature based on the boolean
    double heuristicMultiplier = bUsingHeuristic ? 1.0 : 0.0;

    // fastest weighted path to the end
    Path endPath;

    PriorityQueue<Path> queue;
    Path startPath({start});
    queue.add(startPath,0);

    // vertexs we have seen thus far, if we encounter a vertex which is not in here
    // then the cost is essentially INF and we must assign a new cost based on the
    // path it took to get here
    Set<Vertex*> encounteredVertexes;
    encounteredVertexes.add(start);

    // while there are still options left to explore and we havent visited the
    // end vertex, endPath is only not empty if we have found a valid path
    while(!queue.isEmpty() && endPath.isEmpty())
    {
        Path currentPath = queue.dequeue();
        Vertex* currentVert = currentPath.get(currentPath.size()-1);

        // we are done if the vertex popped out of the queue (the current most efficient operation we can do)
        // is the end vertex
        if(currentVert == end)
        {
            currentVert->setColor(GREEN);
            endPath = currentPath;
            continue;
        }

        // otherwise, this is the first time we are visiting this vertex, mark it as such
        // this also represents the fastest path to any vertex currentVert
        currentVert->visited = true;
        currentVert->setColor(GREEN);

        //explore its neighbors
        for(Edge* edge: currentVert->edges)
        {

            // this path needs to be wiped after each iteration (since we don't want this edge + the next edge to
            // be considered a valid path)
            Path backtrackingPath = currentPath;
            double costToNextNode = currentVert->cost + edge->cost;


            // if the edge does not exist in the filter
            // and we have yet to encounter this vertex at all (does not exist in encountered edges, thus its cost is INF)
            // or our current path is a more efficient way to reach a previously ENCOUNTERED (not visited, as we should
            // never encounter a visited node here) vertex.
            if(!filter.contains(edge) && (!encounteredVertexes.contains(edge->end) || edge->end->cost > costToNextNode))
            {
                // update the vertex's information and add it to our current path
                encounteredVertexes.add(edge->end);
                edge->end->cost = costToNextNode;
                backtrackingPath.add(edge->end);
                queue.add(backtrackingPath,costToNextNode + (getHeuristicValue(graph,edge->end,end) * heuristicMultiplier));
                edge->end->setColor(YELLOW);
             }
        }
    }
    return endPath;
}

/**
 * @brief dijkstrasAlgorithm - invokes the pathFinder method with an empty filter
 * and heuristic flag off
 */
Path dijkstrasAlgorithm(RoadGraph& graph, Vertex* start, Vertex* end) {
    Set<Edge*> filter;
    return dijkstraPathFinder(graph,start,end,false,filter);
}

/**
 * @brief aStar - invokes the pathFinder method with an empty filter
 * and heuristic flag on
 */
Path aStar(RoadGraph& graph, Vertex* start, Vertex* end) {
    Set<Edge*> filter;
    return dijkstraPathFinder(graph,start,end,true,filter);
}

/**
 * @brief calculateDifference - calculates the difference between two
 * paths in percent (0.00 -> 1.00)
 */
double calculateDifference(Path one, Path two) {
    double numDifferent = 0.00;
    for(int i = 0; i < one.size(); i++)
    {
        if(i < two.size() && one[i] != two[i])
        {
            numDifferent++;
        }
    }
    // if the sizes are not equal, then there are abs(size-size) additional differences
    numDifferent += abs(one.size()-two.size());

    return numDifferent / one.size();
}

/**
 * @brief alternativeRoute - runs aStar, findign the fastest path from start to end
 * then for each edge in the fastest path we have found, we filter that edge and run aStar once again to get a different path
 * @return the fastest path found which is also sufficiently different (specified by SUFFICIENT DIFFERENCE)
 */
Path alternativeRoute(RoadGraph& graph, Vertex* start, Vertex* end) {
    // most optimal path
    Path fastestPath = aStar(graph,start,end);

    // storage for our comparison computations
    // each path found with edge X removed is compared to alternativeCost
    // if the path we found is lower in value than the current value of alternativeCost
    // while still being sufficiently different, it is the new optimal alternative path
    Path alternativePath;
    double alternativeCost = INF;

    // filter which only contains one edge at a time, the one we are currently trying to remove
    Set<Edge*> filter;

    // for each vertex in the fastest path
    for(int i = 0; i < fastestPath.size()-2; i++)
    {
        // filter that edge
        Edge* edge = graph.getEdge(fastestPath[i],fastestPath[i+1]);
        filter.add(edge);

        // compute a new path
        Path currentAlternativePath = dijkstraPathFinder(graph,start,end,true,filter);

        if(!currentAlternativePath.isEmpty())
        {
            double currentAlternativePathCost = currentAlternativePath.get(currentAlternativePath.size()-1)->cost;
            double currentAlternativePathDifference = calculateDifference(fastestPath,currentAlternativePath);
            // if the cost and difference of our new path makes it a valid alternative path which is
            // more efficient than the current alternative path, then it is out new most efficient alternative path
            if(currentAlternativePathCost < alternativeCost && currentAlternativePathDifference >= SUFFICIENT_DIFFERENCE)
            {
                alternativePath = currentAlternativePath;
            }
        }
        filter.remove(edge);
    }
    return alternativePath;
}

