#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <sstream>
#include <algorithm>
#include <limits>
#include <functional>

using namespace std;

struct Road {
	int destination;
	int distance;
};

class Graph {
    private:
	vector<string> locations;
	vector<vector<Road> > roads;

    // find location by name, return its index
    // if not found return -1
	int findLocation(string name)
	{
		for (int i = 0; i < (int)locations.size(); i++) {
			if (locations[i] == name) return i;
		}

		return -1;
	}


    // finds a road from one location to another
    // return road's index, or -1 if road does not exist/
	int findRoad(int from, int to)
	{
		for (int i = 0; i < (int)roads[from].size(); i++) {
			if (roads[from][i].destination == to) return i;
		}

		return -1;
	}

    public:
    // read input as an integer
	bool readInt(string message, int &value)
	{
		string line;
		cout << message;

		if (!getline(cin, line)) return false;

		stringstream input(line);
		char extra;

		if (!(input >> value) || (input >> extra)) return false;

		return true;
	}

    // add a new location
	void addLocation()
	{
		string name;
		cout << "Enter location name: ";

		if (!getline(cin, name)) return;

		if (name.empty()) {
			cout << "Name cannot be empty.\n";
			return;
		}

        // check for duplicate locations
		if (findLocation(name) != -1) {
			cout << "Location already exists.\n";
			return;
		}

        // store the new locations
		locations.push_back(name);
        // create an empty road list for the new loc.
		roads.push_back(vector<Road>());

		cout << "Location added.\n";
	}

    // connects two locations with a road
	void connectRoad()
	{
		string first, second;

		cout << "Enter first location: ";
		if (!getline(cin, first)) return;

		cout << "Enter second location: ";
		if (!getline(cin, second)) return;

        // convert location names into indices
		int a = findLocation(first);
		int b = findLocation(second);

		if (a == -1 || b == -1) {
			cout << "Location not found.\n";
			return;
		}

		if (a == b) {
			cout << "Cannot connect a location to itself.\n";
			return;
		}

		int distance;

		while (true) {
			if (!readInt("Enter distance: ", distance)) {
				if (cin.eof()) return;

				cout << "Enter a valid integer.\n";
				continue;
			}

			if (distance > 0) break;

			cout << "Distance must be positive.\n";
		}

        // check for duplicate roads
		int index = findRoad(a, b);

		if (index != -1) {
			roads[a][index].distance = distance;
			roads[b][findRoad(b, a)].distance = distance;

			cout << "Road distance updated.\n";
		} else {
			roads[a].push_back({ b, distance });
			roads[b].push_back({ a, distance });

			cout << "Road connected.\n";
		}
	}

    // display every location and its connected roads
	void displayMap()
	{
		if (locations.empty()) {
			cout << "No locations available.\n";
			return;
		}

		for (int i = 0; i < (int)locations.size(); i++) {
			cout << locations[i] << " -> ";

			if (roads[i].empty()) cout << "No roads";

			for (int j = 0; j < (int)roads[i].size(); j++) {
				int to = roads[i][j].destination;

				cout << locations[to] << " (" << roads[i][j].distance << ")";

				if (j + 1 < (int)roads[i].size()) cout << ", ";
			}

			cout << '\n';
		}
	}

    // search for a location by its name
	void searchLocation()
	{
		string name;
		cout << "Enter location name: ";

		if (!getline(cin, name)) return;

		int index = findLocation(name);

		if (index == -1) {
			cout << "Location not found.\n";
			return;
		}

		cout << "Found: " << locations[index] << '\n';
		cout << "Connected roads:\n";

		for (int i = 0; i < (int)roads[index].size(); i++) {
			int to = roads[index][i].destination;

			cout << locations[to] << " - Distance: " << roads[index][i].distance
			     << '\n';
		}
	}

    // Breadth-First Search: visits location level by level.
	void bfs()
	{
		string name;
		cout << "Enter starting location: ";

		if (!getline(cin, name)) return;

		int start = findLocation(name);

		if (start == -1) {
			cout << "Location not found.\n";
			return;
		}

		vector<bool> visited(locations.size(), false);
		queue<int> q;

		q.push(start);
		visited[start] = true;

		cout << "BFS: ";

		while (!q.empty()) {
			int current = q.front();
			q.pop();

			cout << locations[current] << " ";

			for (int i = 0; i < (int)roads[current].size(); i++) {
				int next = roads[current][i].destination;

				if (!visited[next]) {
					visited[next] = true;
					q.push(next);
				}
			}
		}

		cout << '\n';
	}

    // Depth-First Search: explores one branch before another.
	void dfs()
	{
		string name;
		cout << "Enter starting location: ";

		if (!getline(cin, name)) return;

		int start = findLocation(name);

		if (start == -1) {
			cout << "Location not found.\n";
			return;
		}

		vector<bool> visited(locations.size(), false);
		stack<int> s;

		s.push(start);

		cout << "DFS: ";

		while (!s.empty()) {
			int current = s.top();
			s.pop();

			if (visited[current]) continue;

			visited[current] = true;
			cout << locations[current] << " ";

			for (int i = (int)roads[current].size() - 1; i >= 0; i--) {
				int next = roads[current][i].destination;

				if (!visited[next]) s.push(next);
			}
		}

		cout << '\n';
	}

    // Dijkstra's algorithm finds the minimum-distance route.
	void shortestPath()
	{
		string first, last;

		cout << "Enter starting location: ";
		if (!getline(cin, first)) return;

		cout << "Enter destination: ";
		if (!getline(cin, last)) return;

		int start = findLocation(first);
		int end = findLocation(last);

		if (start == -1 || end == -1) {
			cout << "Location not found.\n";
			return;
		}

		int n = (int)locations.size();
		const long long INF = numeric_limits<long long>::max() / 4;

		vector<long long> dist(n, INF);
		vector<int> parent(n, -1);
		vector<bool> visited(n, false);

		priority_queue<pair<long long, int>, vector<pair<long long, int> >,
			       greater<pair<long long, int> > >
			pq;

		dist[start] = 0;
		pq.push({ 0, start });

		int visitedCount = 0;

		while (!pq.empty()) {
			int current = pq.top().second;
			long long currentDist = pq.top().first;
			pq.pop();

			if (visited[current] || currentDist != dist[current]) continue;

			visited[current] = true;
			visitedCount++;

			if (current == end) break;

			for (int i = 0; i < (int)roads[current].size(); i++) {
				int next = roads[current][i].destination;
				int weight = roads[current][i].distance;

				if (visited[next]) continue;

				long long newDist = currentDist + weight;

				if (newDist < dist[next]) {
					dist[next] = newDist;
					parent[next] = current;
					pq.push({ newDist, next });
				}
			}
		}

		cout << "Locations visited by Dijkstra: " << visitedCount << '\n';

		if (dist[end] == INF) {
			cout << "No route exists.\n";
			return;
		}

		vector<int> path;

		for (int current = end; current != -1; current = parent[current])
			path.push_back(current);

		reverse(path.begin(), path.end());

		cout << "Shortest route: ";

		for (int i = 0; i < (int)path.size(); i++) {
			cout << locations[path[i]];

			if (i + 1 < (int)path.size()) cout << " -> ";
		}

		cout << "\nTotal distance: " << dist[end] << '\n';
	}
};

int main()
{
	Graph city;
	int choice;

	while (true) {
		cout << "\n-- SMART CITY ROUTE FINDER --\n";
		cout << "1. Add location\n";
		cout << "2. Connect road\n";
		cout << "3. Display city map\n";
		cout << "4. Search location\n";
		cout << "5. BFS traversal\n";
		cout << "6. DFS traversal\n";
		cout << "7. Find shortest route\n";
		cout << "0. Exit\n";

		if (!city.readInt("Enter choice: ", choice)) {
			if (cin.eof()) break;

			cout << "Invalid input. Enter a number.\n";
			continue;
		}

		switch (choice) {
			case 1:
				city.addLocation();
				break;
			case 2:
				city.connectRoad();
				break;
			case 3:
				city.displayMap();
				break;
			case 4:
				city.searchLocation();
				break;
			case 5:
				city.bfs();
				break;
			case 6:
				city.dfs();
				break;
			case 7:
				city.shortestPath();
				break;
			case 0:
				cout << "Goodbye!\n";
				return 0;
			default:
				cout << "Invalid choice.\n";
		}
	}

	return 0;
}
