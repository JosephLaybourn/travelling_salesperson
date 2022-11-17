/*!
 * \author Joey Laybourn (j.laybourn@digipen.edu)
 * \file tsp.cpp
 * \date 6-21-20
 * \copyright Copyright (C) "2019" DigiPen Institute of Technology.
 * \brief This file contains the solution for TSP
 */


#include "tsp.h"
#include <fstream>
#include <iostream>
#include <bits/stdc++.h> 

std::vector<bool> visited; 

std::vector<int> final_path;

int final_res = INT_MAX; 

void copyToFinal(std::vector<int> currentPath) 
{ 
    final_path.resize(currentPath.size());
    //std::cout << final_path.size() << std::endl;

    for (unsigned i=0; i<currentPath.size(); i++) 
        final_path[i] = currentPath[i]; 
    final_path[currentPath.size() - 1] = currentPath[0]; 
}

int firstMin(MAP input, int i) 
{ 
    int min = INT_MAX; 
    for (int k = 0; k < int(input.size()); k++) 
        if (input[i][k]<min && i != k) 
            min = input[i][k]; 
    return min; 
} 
  
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
  
void TSPRec(MAP &input, int currentBound, int currentWeight, unsigned level, std::vector<int> &currentPath) 
{ 
    //std::cout << currentPath.size() << std::endl;
    if (level==input.size()) 
    { 
        if (input[currentPath[level-1]][currentPath[0]] != 0) 
        { 
            int curr_res = currentWeight + input[currentPath[level-1]][currentPath[0]]; 

            if (curr_res < final_res) 
            { 
                //std::cout << "wumbology" << std::endl;
                copyToFinal(currentPath); 
                final_res = curr_res; 
            } 
        } 
        return; 
    } 
  
    for (unsigned i=0; i<input.size(); i++) 
    { 
        if (input[currentPath[level-1]][i] != 0 && 
            visited[i] == false) 
        { 
            int temp = currentBound; 
            currentWeight += input[currentPath[level-1]][i]; 

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
  
            if (currentBound + currentWeight < final_res) 
            { 
                currentPath[level] = i; 
                visited[i] = true; 
  
                TSPRec(input, currentBound, currentWeight, level+1, currentPath); 
            } 
  
            currentWeight -= input[currentPath[level-1]][i]; 
            currentBound = temp; 
  
            for(unsigned i = 0; i < visited.size(); ++i)
            {
                visited[i] = false;
            }
            for (unsigned j=0; j<=level-1; j++) 
                visited[currentPath[j]] = true; 
        } 
    } 
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

    std::vector<int> currentPath(input.size() + 1); 

    int currentBound = 0; 
    for(unsigned i = 0; i < currentPath.size(); ++i)
    {
        currentPath[i] = -1;
        visited[i] = false;
    }

    for (unsigned i=0; i<input.size(); i++) 
        currentBound += (firstMin(input, i) + secondMin(input, i)); 

    if(currentBound & 1)
    {
        currentBound /= 2 + 1;
    }
    else
    {
        currentBound /= 2;
    }
    

    visited[0] = true; 
    currentPath[0] = 0; 

    TSPRec(input, currentBound, 0, 1, currentPath); 

    return final_path;
}