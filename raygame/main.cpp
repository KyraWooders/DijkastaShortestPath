/*******************************************************************************************
*
*   raylib [core] example - basic window
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <vector>
#include <algorithm>
#include <iostream>

struct Node;

struct Edge
{
	Node* target;
	float cost;
};

struct Node
{
	Vector2 position;

	float gScore;
	Node* previous;

	std::vector<Edge> connections;
};

std::vector<Node*> dijkstrasSearch(Node* startNode, Node* endNode)
{
	if (startNode == nullptr || endNode == nullptr)
	{
		return std::vector<Node*>();
	}

	if (startNode == endNode)
	{
		std::vector<Node*> singleNodePath;
		singleNodePath.push_back(startNode);
		return singleNodePath;
	}

	startNode->gScore = 0;
	startNode->previous = nullptr;

	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	openList.push_back(startNode);

	while (!openList.empty()) 
	{
		Node* currentNode = openList.front();

		//If we visit the endNode, then we can exit
		//Sorting the openList guarantees the shortest path
		//is found, given no negatives
		if (currentNode == endNode)
		{
			break;
		}

		//Remove currentNode from openList
		openList.erase(openList.begin());
		//Add currentNode to closedList
		closedList.push_back(currentNode);

		//For each Edge e in currentNode's connections
		for (Edge e : currentNode->connections) 
		{
			//If e's target is not in closedList
			if (std::find(closedList.begin(), closedList.end(), e.target) == closedList.end()) 
			{
				float newGScore = currentNode->gScore + e.cost;

				//Have not yet visited the node
				//So calucate the score and update its parent
				//Add it to the openList for processing
				//If e's target is not in openList
				if (std::find(openList.begin(), openList.end(), e.target) == openList.end())
				{
					e.target->gScore = newGScore;
					e.target->previous = currentNode;
					//Insert the target in the correct position so the list stays sorted
					if (openList.empty())
					{
						openList.push_back(e.target);
					}
					else
					{
						for (auto i = openList.begin(); i != openList.end(); i++) 
						{
							if (e.target->gScore < (*i)->gScore) 
							{
								openList.insert(i, e.target);
								break;
							}
						}
					}
				}
				//Node is already in the openList with a valid score
				//So compare the caluclated score with the existing
				//to find the shorter path
				else if (newGScore < e.target->gScore) 
				{
					e.target->gScore = newGScore;
					e.target->previous = currentNode;
				}
			}
		}
	}

	//Create path in reverse from endNode to startNode
	std::vector<Node*> path;
	Node* currentNode = endNode;

	while (currentNode != nullptr) 
	{
		//Add the current node to the beginning of the path
		path.insert(path.begin(), currentNode);
		//Go to the previous node
		currentNode = currentNode->previous;
	}

	return path;
}



int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------
	
	//Nodes
	Node* a = new Node();
	a->position = Vector2{ 500.0f, 300.0f };
	Node* b = new Node();
	b->position = Vector2{ 1000.0f, 300.0f };
	Node* c = new Node();
	c->position = Vector2{ 1000.0f, 600.0f };
	Node* d = new Node();
	d->position = Vector2{ 1000.0f, 900.0f };
	Node* e = new Node();
	e->position = Vector2{ 750.0f, 1200.0f };
	Node* f = new Node();
	f->position = Vector2{ 500.0f, 900.0f };
	//Edges
	a->connections.push_back(Edge{ b, 2 });
	b->connections.push_back(Edge{ c, 3 });
	c->connections.push_back(Edge{ a, 3 });
	c->connections.push_back(Edge{ d, 1 });
	d->connections.push_back(Edge{ e, 4 });
	d->connections.push_back(Edge{ f, 4 });
	f->connections.push_back(Edge{ e, 6 });

	std::vector<Node*> shortestPath = dijkstrasSearch(a, e);

	for (Node* node : shortestPath)
	{
		std::cout << node->gScore << std::endl;
	}

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}