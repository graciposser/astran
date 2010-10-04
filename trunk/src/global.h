/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
 *   (amziesemerj,clazz)@inf.ufrgs.br                                      *
 ***************************************************************************/
#ifndef GLOBAL_H
#define GLOBAL_H


/**
 Route using a Global based algorithm
 Detailed router
 @author Adriel Mota Ziesemer Jr.
 */

#include <algorithm>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <iostream>
#include <strstream>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include "util.h"
//#include "pathfinder.h"


using namespace std;

enum rt_dir2 {RT_EASTB, RT_WESTB, RT_NORTHB, RT_SOUTHB, RT_DOWNB, RT_UPB, RT_INVALIDB};

class Global{
protected:
	struct listNets{
		list<int> nets;
		int history;
		int idNr;
	};	
	class Node{
	public:
		vector<int> net;
		int history;
		int father;
		int idNr;
		//[0]-East...[1]-North...[2]-South 
		map<int,listNets> arcsNrNets;
		
		void setFather(int i, int f){father=f;idNr=i;};
		int getFather(int i){ return idNr==i ? father :  0; };
		bool isVisited(int i){return idNr==i;};
		Node(): history(0), idNr(0){};
	};
	//Keep trace of the path in the bfs algorithm 
	struct t_tmp{
		int node;
		int father;
		int aStarCost;
		int costAccumulated;
		bool operator< (const t_tmp& x) const { return x.aStarCost < aStarCost | (x.aStarCost == aStarCost & x.aStarCost-x.costAccumulated < aStarCost-costAccumulated) ;};
		//		bool operator< (const t_tmp& x) const { return x.aStarCost < aStarCost;};
	};	
	struct arc{
		int node;
		int dir;
	};
	//List of the Nets in the graph
	struct t_nets{
		vector<int> nodes;
		list<int> netTree;
		list<arc> arcs;
		bool conflict;
	};
	
	//Capacidade global definida para todos os arcos arbitrariamente
	int globalCapacity;
	
	
	//Pathfinder rt;
	int direction[4];
	vector<int> costs;
	map<int,int> binNetTemp;
	vector<Node> graph;
	map<int,t_nets> netlist;
	int sizeX, sizeY, sizeZ, sizeXY;
	int visited;
	int actualAttempt;
	int trace_id;
	int conflicts;
	int blockageNet;
	int finalCost,wlCost,pins;
	float runtime;
	int lastExpansion;
	int binX,binY;
	
	
public:
	Global(){};
	~Global(){};
	
	int getDir(int& node, rt_dir2& dir){return node+direction[dir];};	
	int getDirN(int& node, rt_dir2 dir){return node+direction[dir];};
	void setBlockageNet(int net){blockageNet=net;};
	bool addNodetoNet(int net, int node);	
	int getNet(int node){return graph[node].net.size();};
	vector<int>* getNetNodes(int net){return &netlist[net].nodes;};
	void setLayerCosts(int layer, int hCost, int vCost, int upCost);
	bool routeNets(int nrAttempts);
	bool isSource(int x);
	bool optimize();
	bool aStar(map<int,t_nets>::iterator& net, list<int>& targetNodes);
	void showResult();
	bool areConnected(int node1, rt_dir2 dir);
	int areConnected2(int node1, rt_dir2 dir);
	bool connect(int net,int n1, int n2);
	int getSizeX(){return sizeX;};
	int getSizeY(){return sizeY;};
	int getSizeZ(){return sizeZ;};
	void setSize(int sX, int sY, int sZ);
	int getPos(int x, int y, int z){return (z*sizeXY)+(y*sizeX)+x;};
	int getPosX(int& pos){return (pos%sizeXY)%sizeX;};
	int getPosY(int& pos){return (pos%sizeXY)/sizeX;};
	int getPosZ(int& pos){return pos/sizeXY;};	
	int getCenter(vector<int>& Nodes);
	void clear();
	void reset();
	int calcDistance(int node1, int node2);
	inline int calcCost(int node2, rt_dir2 dir,int father);
	list<int>::iterator getClosestNode(int node,list<int>& targetNodes);
	int getClosestNodeDistance(int& node, list<int>& targetNodes);
	int getClosestNodePos(int nodePos,vector<int>& targetNodes);
	void pqAddto(t_tmp& actualNode, priority_queue<t_tmp>& pq, const int& targetNet, list<int>& targetNodes, rt_dir2 direction);
	void clearHistory();
	int graphSize(){ return graph.size();};
	int netlistSize(){ return netlist.size();};
	int netTreeFirst(int net) { return *(netlist[net].netTree.begin());};
	rt_dir2 dirFromPos(int antPos, int pos);
	list<int> getListNodes(int node1, int dir){return graph[node1].arcsNrNets[dir].nets;};
	void addRoutingTree(int antNode,int node,map<int,t_nets>::iterator nets_it);
	void ocupacaoArcos();
	bool netSource(int node, int net);
	vector<int>& getListNets(int node){return graph[node].net;};
	bool netBin(int node,int net);
	list<int>& getListArcNets(int node,int dir){return graph[node].arcsNrNets[dir].nets;};
	void setBinSize(int X,int Y){binX=X;binY=Y;};
	int getBinX() {return binX;};
	int getBinY() {return binY;};
	int getBinZ() {return sizeZ;};
	void setGlobalCapacity(int globalC){globalCapacity=globalC;};
	void binNetClear(){binNetTemp.clear();};
};


#endif