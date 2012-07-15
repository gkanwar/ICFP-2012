// See search.h for documentation.

template <class Node>
Edge<Node>::Edge( Node target, float cost ) {
	this->target = target;
	this->cost = cost;
}

template <class Node>
std::vector<Node> reconstructPath( Node start, Node end, std::map<Node, Node> pathMap ) {
	std::vector<Node> path;
	Node current = end;
	while( current != start ) {
		path.insert( path.begin(), current );
		current = pathMap[ current ];
	}
	path.insert( path.begin(), start );
	return path;
}
