// C++ program to solve Traveling Salesman Problem 
// using Branch and Bound. 
#include "tsp.h"
#include <fstream>
#include <iostream>
#include <bits/stdc++.h> 
  
// visited[] keeps track of the already visited nodes 
// in a particular path 
std::vector<bool> visited; 
  
// final_path[] stores the final solution ie, the 
// path of the salesman. 
std::vector<int> final_path;

// Stores the final minimum weight of shortest tour. 
int final_res = INT_MAX; 
  
// Function to copy temporary solution to 
// the final solution 
void copyToFinal(std::vector<int> currentPath) 
{ 
    final_path.resize(currentPath.size());
    //std::cout << final_path.size() << std::endl;

    for (unsigned i=0; i<currentPath.size(); i++) 
        final_path[i] = currentPath[i]; 
    final_path[currentPath.size() - 1] = currentPath[0]; 
}

// Function to find the minimum edge cost 
// having an end at the vertex i 
int firstMin(MAP input, int i) 
{ 
    int min = INT_MAX; 
    for (int k = 0; k < int(input.size()); k++) 
        if (input[i][k]<min && i != k) 
            min = input[i][k]; 
    return min; 
} 
  
// function to find the second minimum edge cost 
// having an end at the vertex i 
int secondMin(MAP input, unsigned i) 
{ 
    int first = INT_MAX, second = INT_MAX; 
    for (unsigned j=0; j<input.size(); j++) 
    { 
        if (i == j) 
            continue; 
  
        if (input[i][j] <= first) 
        { 
            second = first; 
            first = input[i][j]; 
        } 
        else if (input[i][j] <= second && input[i][j] != first) 
            second = input[i][j]; 
    } 
    return second; 
} 
  
// function that takes as arguments: 
// currentBound -> lower bound of the root node 
// currentWeight-> stores the weight of the path so far 
// level-> current level while moving in the search 
//         space tree 
// currentPath[] -> where the solution is being stored which 
//                would later be copied to final_path[] 
void TSPRec(MAP &input, int currentBound, int currentWeight, unsigned level, std::vector<int> &currentPath) 
{ 
    //std::cout << currentPath.size() << std::endl;
    // base case is when we have reached level N which 
    // means we have covered all the nodes once 
    if (level==input.size()) 
    { 
        // check if there is an edge from last vertex in 
        // path back to the first vertex 
        if (input[currentPath[level-1]][currentPath[0]] != 0) 
        { 
            // curr_res has the total weight of the 
            // solution we got 
            int curr_res = currentWeight + input[currentPath[level-1]][currentPath[0]]; 
  
            // Update final result and final path if 
            // current result is better. 
            if (curr_res < final_res) 
            { 
                //std::cout << "wumbology" << std::endl;
                copyToFinal(currentPath); 
                final_res = curr_res; 
            } 
        } 
        return; 
    } 
  
    // for any other level iterate for all vertices to 
    // build the search space tree recursively 
    for (unsigned i=0; i<input.size(); i++) 
    { 
        // Consider next vertex if it is not same (diagonal 
        // entry in adjacency matrix and not visited 
        // already) 
        if (input[currentPath[level-1]][i] != 0 && 
            visited[i] == false) 
        { 
            int temp = currentBound; 
            currentWeight += input[currentPath[level-1]][i]; 
  
            // different computation of currentBound for 
            // level 2 from the other levels 
            // if (level==1) 
            //   currentBound -= ((firstMin(input, currentPath[level-1]) + firstMin(input, i))/2); 
            //  else
            //    currentBound -= ((secondMin(input, currentPath[level-1]) +  firstMin(input, i))/2); 

            const std::vector<int> &current = input[i];

            for(unsigned j = 0; j < current.size(); j++)
            {
                if(!visited[j] || j == currentPath.size())
                {
                    if(current[j] < currentBound)
                    {
                        currentBound = current[j];
                    }
                }
            }
  
            // currentBound + currentWeight is the actual lower bound 
            // for the node that we have arrived on 
            // If current lower bound < final_res, we need to explore 
            // the node further 
            if (currentBound + currentWeight < final_res) 
            { 
                currentPath[level] = i; 
                visited[i] = true; 
  
                // call TSPRec for the next level 
                TSPRec(input, currentBound, currentWeight, level+1, currentPath); 
            } 
  
            // Else we have to prune the node by resetting 
            // all changes to currentWeight and currentBound 
            currentWeight -= input[currentPath[level-1]][i]; 
            currentBound = temp; 
  
            // Also reset the visited array 
            for(unsigned i = 0; i < visited.size(); ++i)
            {
                visited[i] = false;
            }
            for (unsigned j=0; j<=level-1; j++) 
                visited[currentPath[j]] = true; 
        } 
    } 
} 
  
// This function sets up final_path[]  
void TSP(MAP &input) 
{ 
    std::vector<int> currentPath(input.size() + 1); 
  
    // Calculate initial lower bound for the root node 
    // using the formula 1/2 * (sum of first min + 
    // second min) for all edges. 
    // Also initialize the currentPath and visited array 
    int currentBound = 0; 
    for(unsigned i = 0; i < currentPath.size(); ++i)
    {
        currentPath[i] = -1;
        visited[i] = false;
    }
  
    // Compute initial bound 
    for (unsigned i=0; i<input.size(); i++) 
        currentBound += (firstMin(input, i) + secondMin(input, i)); 
  
    // Rounding off the lower bound to an integer 
    //  currentBound = (currentBound&1)? currentBound/2 + 1 : currentBound/2; 

    if(currentBound & 1)
    {
        currentBound /= 2 + 1;
    }
    else
    {
        currentBound /= 2;
    }
    
  
    // We start at vertex 1 so the first vertex 
    // in currentPath[] is 0 
    visited[0] = true; 
    currentPath[0] = 0; 
  
    // Call to TSPRec for currentWeight equal to 
    // 0 and level 1 
    TSPRec(input, currentBound, 0, 1, currentPath); 
} 

MAP createMap(const char* filename)
{
    std::fstream input(filename);
    int count;
    input >> count;
    //--count;
    visited.resize(count);
    MAP output(count);
    
    for(int i = 0; i < count; ++i)
    {
        output[i].resize(count);
        
        for(int j = 0; j < count; ++j)
        {
            output[i][j] = 0;
        }
    }
    for(int i = 0; i < count - 1; ++i)
    {
        for(int j = i + 1; j < count; ++j)
        {
            int value;
            input >> value;
            //std::cout << "NOW PUTTING " << value << " INTO POSITION " << i << ", " << j << std::endl;
            //std::cout << "NOW PUTTING " << value << " INTO POSITION " << j << ", " << i << std::endl;
            output[i][j] = value;
            output[j][i] = value;
        }
    }
    return output;
}

std::vector<int> SolveTSP(char const* filename)
{
    MAP input = createMap(filename);

    // for(unsigned i = 0; i < input.size(); ++i)
    // {
    //     for(unsigned j = 0; j < input.size(); ++j)
    //     {
    //         std::cout << input[i][j] << ", ";
    //     }
    //     std::cout << std::endl;
    // }

    TSP(input);

    // for(unsigned i = 0; i < final_path.size(); ++i)
    // {
    //     std::cout << final_path[i] << ", ";
    // }
    //std::cout << std::endl;
    //std::cout << final_res << std::endl;
    return final_path;
}