# A_star

A* Search algorithm is one of the best and popular technique used in graph traversals.
For this project, the graph is interpreted as a road network for mobile robots in factory floor, retail stores ,etc.  The characteristics of the graph are as follows:

* Nodes: Locations of interest or intermediate waypoints in an environment. 
* Edges: Desirable paths between nodes.
* Arrows: Directions of paths e.g. a path can allow traffic in one direction or two directions. 

For example, 

![alt text](https://github.com/TuanMinhNguyen15/A_star/raw/main/images/example_graph.PNG)

In the above figure, the locations of interest are the meat station, vegetable station, dairy station and pharmacy station. The intermediate waypoints are the four intersection nodes at the middle of the graph. Notice that a path connecting any two nodes is also indicated with the direction of traffic such that a mobile robot is only allowed to traverse in the direction of the arrow. 


## Demo

The c++ file `A_star.cpp` demonstrates the A* path planner for the following graph:

![alt text](https://github.com/TuanMinhNguyen15/A_star/raw/main/images/demo_graph.PNG)

This graph structure is chosen because it can be served as the building block in constructing an extensive road network for mobile robots where the robots need stay on the right lane when going north, and must cross to the left lane whenver it needs to go south. Notice that the left and right lanes only allow traffic in one direction, while the paths connecting the two lanes allow robots to travel in both directions. 

**Caveat:**

The A* algorithm here serves as a global path planner which provides a robot with the most efficient path to reach its destination, and DOES NOT consider obstacle avoidance. After knowing where to go, a robot can then track the computed path and, if needed, implement other local planners to avoid obstacles or other robots in its way. The implementation of local path planner for avoiding obstacles is outside the scope of this project. 

*Sample Cases:*

1/ Case 1
```
Enter start node: A
Enter goal node: E
Path is found!
A -> B -> D -> F -> E
Continue? [y/n]: y
B -> D -> F -> E
Continue? [y/n]: y
D -> F -> E
Continue? [y/n]: y
F -> E
Continue? [y/n]: y
E
Continue? [y/n]: y
GOAL IS REACHED!!! Restart? [y/n]: n
```

2/ Case 2
```
Enter start node: A
Enter goal node: C
Path is found!
A -> B -> D -> C
Continue? [y/n]: y
B -> D -> C
Continue? [y/n]: y
D -> C
Continue? [y/n]: n
Change goal node? Restart? Exit? [new_goal/restart/exit]: new_goal
Enter goal node: E
Path is found!
D -> F -> E
Continue? [y/n]: y
F -> E
Continue? [y/n]: y
E
Continue? [y/n]: y
```



