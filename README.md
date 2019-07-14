#Carpool Router
## Setup
This project uses cmake and conan, which can be set up with following commands.
`pip install cmake`  
`pip install conan`  
Conan is used to fetch dependencies, in this project, `gtest` and `boost`is used. Packages are fetched from `conan-center`
### gtest
Only used for testing purpose.
In this sample I only wrote limited amount of tests to get the program to work, potentially a lot more can be tested.
### boost
Used for hashing function, for checking visited state in graph search

## Compile
Ideally cmake would take care of everything.
In project root folder  
`mkdir build && cd build`  
`cmake ..`  
`make`
should do the job, including sorting out conan dependencies

## JSON-parsing
Used thirdparty library rapidjson from Tencent.
With a factory, read json input as a string, creates a vector of `Request` objects.  
Some limitations:  
Run time parsing, potentially we could do compile time parse as see from CppCon 2017 "constexpr all the things", and at compile time turn json into objects. But considering the usecase, where request are also generated from some other functions, this should be good enough.  
Json as string: user needs to input the json element as a string.  
Tuple as array: tuples are not supported with rapidjson, so replaced with array

## Algorithms
If no request is given, either finish previous route, or stay put if no current route. (We could also greedily move to the center of the map in this case, but I didn't implement this).  
If there is request, fill out the request information in the router so that I can search:  
Request by ID  
List of pickup requests ID by coordinate  
List of dropoff requests ID by coordinate  
All in O(1) in average case.

`State` serves as graph search node, contains vehicle location, who's in the car and who's been dropped off. Also keeps track of the route.  
Neighbor are generated from `State` and the 4 moves, by moving and perform pickups and dropoffs. If a `State` is at a location that's been visit with the same passengers and dropoffs, it will not be visited again, as the previous visit is guaranteed to be the fast route.

I tested out BFS/Dijkstra first but it was taking too much time for longer routes. Then used A* for better performance.
For heuristic, I used the minimum distance to fulfill the request furthest from finish.