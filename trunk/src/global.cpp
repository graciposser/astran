/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   amziesemerj[at]inf.ufrgs.br                                           *
 **************************************************************************/

#include "global.h"

//Clear data structures
void Global::clear(){
	for(int x=0; x<graph.size(); x++)
		graph[x].net.clear();
    netlist.clear();
	graph.clear();
	costs.clear();
	blockageNet=0;
}

void Global::reset(){
	graph.clear();
	graph.resize(sizeXY*sizeZ);
	for(map<int,t_nets>::iterator nets_it=netlist.begin(); nets_it!=netlist.end(); ++nets_it){
		nets_it->second.netTree.clear();
		nets_it->second.conflict=false;
		for(vector<int>::iterator netTree_it=nets_it->second.nodes.begin();netTree_it!=nets_it->second.nodes.end(); ++netTree_it){
			nets_it->second.netTree.push_back(*netTree_it);
			//graph[*netTree_it].net=-nets_it->first;
			//graph[*netTree_it].nrNets=1;
		}
	}
}
bool Global::isSource(int x){
	for(vector<int>::iterator list_it=graph[x].net.begin();list_it!=graph[x].net.end();list_it++)
		if(*list_it<0) return true;
	return false;
}	

bool Global::netBin(int node,int net){

	for(vector<int>::iterator arc_it=graph[node].net.begin();arc_it!=graph[node].net.end();arc_it++)
		if(abs(*arc_it)==net) 	return true;
	return false;
	
};

//Set the size of the grid 
void Global::setSize(int sX, int sY, int sZ){
	sizeX=sX;
	sizeY=sY;
	sizeZ=sZ;
	sizeXY=sX*sY;
	graph.clear();
	graph.resize(sizeXY*sZ);
	costs.resize(3*sZ);
	direction[RT_EASTB]=+1;
	direction[RT_WESTB]=-1;
	direction[RT_NORTHB]=+sizeX;
	direction[RT_SOUTHB]=-sizeX;
}

//Add a new node to the net
bool Global::addNodetoNet(int net, int node){
		netlist[net].nodes.push_back(node);
		netlist[net].netTree.push_back(node);
		netlist[net].conflict=false;
		graph[node].net.push_back(-net);
		//graph[node].nrNets=1;
		return true;
}

//Set the arc Costs
void Global::setLayerCosts(int layer, int hCost, int vCost, int upCost){
	costs[layer*3]=hCost;
	costs[layer*3+1]=vCost;
	costs[layer*3+2]=upCost;
}

//Calculate manhattan distance between nodes
inline int Global::calcDistance(int node1, int node2){
	return 5*(abs(getPosX(node1)-getPosX(node2))+abs(getPosY(node1)-getPosY(node2))+abs(getPosZ(node1)-getPosZ(node2)));
}

//Clear node congestion history
void Global::clearHistory(){
   	for(int x=0; x<graph.size(); x++){
	    graph[x].arcsNrNets[0].history=0;
		graph[x].arcsNrNets[1].history=0;
	}	
}

bool Global::netSource(int node, int net){
	for(vector<int>::iterator net_it=graph[node].net.begin();net_it!=graph[node].net.end();net_it++)
		if(*net_it==-net) return true;
	return false;
}

//
//Astar Search
bool Global::aStar(map<int,t_nets>::iterator& net, list<int>& targetNodes){	
	static t_tmp actualNode;
	int node;
	int antNode;
	
	++trace_id;
	//Initialize priority queue PQ with the sourceNodes
	priority_queue<t_tmp> pq;
	
	actualNode.costAccumulated=0;
	actualNode.father=-1;
	for(list<int>::iterator nodes_it = net->second.netTree.begin(); nodes_it !=  net->second.netTree.end(); ++nodes_it){
		actualNode.aStarCost=getClosestNodeDistance(*nodes_it, targetNodes);
		actualNode.node=*nodes_it;
		pq.push(actualNode);
		graph[*nodes_it].setFather(trace_id, -1);
	}
	
	//Loop until new sink is found
	while(!(pq.empty() || targetNodes.empty())){
		//Remove lowest cost node from PQ
		actualNode=pq.top();
		pq.pop();
		if(actualNode.father!=-1){
			if(graph[actualNode.node].isVisited(trace_id)) continue;
			graph[actualNode.node].setFather(trace_id, actualNode.father);
			if(netSource(actualNode.node,net->first)) break;
		}
		//		cout << ++visited << ": visitando (" << getPosX(actualNode.node) << " " <<getPosY(actualNode.node)<< " " <<getPosZ(actualNode.node)<< ") " << actualNode.costAccumulated << "+" << actualNode.aStarCost-actualNode.costAccumulated << "=" << actualNode.aStarCost << " vindo de (" << getPosX(actualNode.father) << " " <<getPosY(actualNode.father)<< " " <<getPosZ(actualNode.father)<< ")"<<endl;
		//Test the neighbours and add to queue the valids
		if(getPosX(actualNode.node)+1 < sizeX)
			pqAddto(actualNode,pq, net->first,targetNodes,RT_EASTB);
		if(getPosY(actualNode.node)+1 < sizeY)
			pqAddto(actualNode,pq, net->first,targetNodes,RT_NORTHB);	
		if(getPosX(actualNode.node)) 
			pqAddto(actualNode,pq, net->first,targetNodes,RT_WESTB);
		if(getPosY(actualNode.node))
			pqAddto(actualNode,pq, net->first,targetNodes,RT_SOUTHB);
		//	cout << ++visited << ": visitando (" << getPosX(actualNode.node) << " " <<getPosY(actualNode.node)<< " " <<getPosZ(actualNode.node)<< ") " << actualNode.costAccumulated << "+" << actualNode.aStarCost-actualNode.costAccumulated << "=" << actualNode.aStarCost << " vindo de (" << getPosX(actualNode.father) << " " <<getPosY(actualNode.father)<< " " <<getPosZ(actualNode.father)<< ")"<<endl;
	}
	 	
	//Trace the path back to the source node
	if(actualNode.father!=-1){
		node = actualNode.node;
		targetNodes.remove(node);
		while(graph[node].getFather(trace_id)!=-1){
			antNode=node;
			net->second.netTree.push_back(node); 
			if(!netBin(node,net->first)) {
				graph[node].net.push_back(net->first);
				//cout << node << endl;
			}	
			node = graph[node].getFather(trace_id);
			addRoutingTree(antNode,node,net);
		}
		return true;
	}
	return false;
}

void Global::addRoutingTree(int antNode,int node,map<int,t_nets>::iterator nets_it){
	int distance;
	int dir,tmp;
	arc ar;

	list<int>::iterator arc_it;
	
	//Determine the node origin
	if((node-antNode)<0){
		tmp=antNode;
		antNode=node;
		node=tmp;
	}
	//Direction of the connection
	distance=(node-antNode);
	dir=1;
	if(distance==1) dir=0;
	
	//cout << "Node: " << antNode << "  Nets: " << nets_it->first  << "Dir: " << dir << endl;
	graph[antNode].arcsNrNets[dir].nets.push_back(nets_it->first);
	ar.node=antNode;
	ar.dir=dir;
	nets_it->second.arcs.push_back(ar);
	if(graph[antNode].arcsNrNets[dir].nets.size()>globalCapacity){
		for(arc_it=graph[antNode].arcsNrNets[dir].nets.begin();arc_it!=graph[antNode].arcsNrNets[dir].nets.end();arc_it++){
			if(!netlist[*arc_it].conflict) {
				netlist[*arc_it].conflict=true;
				conflicts++;	
			}	
		}	
	}
}	
/*
int Global::lShapeRouting(){
	int H,V,L,lCost,i,node,next,final;
	list<int> targetNodes;
	list<int> ready;
	list<int>::iterator ready_it,target_it,nextNode,antNode;
	map<int,t_nets>::iterator nets_it;
	list<int>::iterator netTree_it;
	vector<int>::iterator nodes_it;
	list<listNets*>::iterator arcs_it;
	list<int>::iterator arc_it;
	
	
	conflicts=0;
	for(nets_it=netlist.begin();nets_it!=netlist.end();nets_it++){
		//Rip-up net 
		nets_it->second.arcs.clear();
		
		nets_it->second.conflict=false;
		//Create list of target nodes 
		targetNodes.clear();
		for(nodes_it=(nets_it->second).nodes.begin();nodes_it!=(nets_it->second).nodes.end(); ++nodes_it){
			targetNodes.push_back(*nodes_it);
			cout << *nodes_it << endl;
		}
		cout << endl;
		ready.clear();
		ready.push_back(*targetNodes.begin());
		//(nets_it->second).netTree.push_back(*targetNodes.begin());
		targetNodes.erase(targetNodes.begin());
		//Loop until all sinks have been found
		while(!targetNodes.empty()){
			nextNode=targetNodes.begin();
			antNode=ready.begin();
			lCost=calcDistance(*nextNode, (*ready.begin()));
			for(ready_it=ready.begin();ready_it!=ready.end();ready_it++)
				for(target_it=targetNodes.begin();target_it!=targetNodes.end();target_it++){
					if(lCost>calcDistance(*ready_it, *target_it)){
						nextNode=target_it;
						antNode=ready_it;
						lCost=calcDistance(*ready_it, *target_it);
					}	
				}
			H=calcDistanceX(*nextNode,*antNode);
			V=calcDistanceY(*nextNode,*antNode);
			L=calcDistanceZ(*nextNode,*antNode);
			next=*antNode;
			//nets_it->second.netTree.push_back(next);
			//cout << "NextNode: " << *nextNode << " " << "AntNode: " << *antNode << endl;
			final=abs(H);
			node=next;
			for(i=1;i<=final;i++){
				if(H<0){
					node=(next)-i;
					graph[node].arcsNrNets[0].nets.push_back(nets_it->first);
					nets_it->second.arcs.push_back(node);
					(nets_it->second).netTree.push_back(node);
					//cout << "H: " << node << endl;
					if(graph[node].arcsNrNets[0].nets.size()>globalCapacity)
						for(arc_it=(graph[node]).arcsNrNets[0].nets.begin();arc_it!=(graph[node]).arcsNrNets[0].nets.end();arc_it++){
							netlist[*arc_it].conflict=true;	
						}	
				}else{
					graph[node].arcsNrNets[0].nets.push_back(nets_it->first);
					nets_it->second.arcs.push_back(node);
					//cout << "H: " << node << endl;
					if(graph[node].arcsNrNets[0].nets.size()>globalCapacity)
						for(arc_it=graph[node].arcsNrNets[0].nets.begin();arc_it!=graph[node].arcsNrNets[0].nets.end();arc_it++){
							netlist[*arc_it].conflict=true;
				        }
					node=(next)+i;
					(nets_it->second).netTree.push_back(node);
				}
			}
			 
			next=next+H;
			node=next;
			final=abs(V);
			for(i=1;i<=final;i++){
				if(V<0){
					node=(next)-(i*sizeX);
					graph[node].arcsNrNets[1].nets.push_back(nets_it->first);
					nets_it->second.arcs.push_back(node);
					(nets_it->second).netTree.push_back(node);
					//cout << "V: " << node << endl;
					if(graph[node].arcsNrNets[1].nets.size()>globalCapacity)
						for(arc_it=graph[node].arcsNrNets[1].nets.begin();arc_it!=graph[node].arcsNrNets[1].nets.end();arc_it++){
							netlist[*arc_it].conflict=true;	
						}	
				}else{
					graph[node].arcsNrNets[1].nets.push_back(nets_it->first);
					nets_it->second.arcs.push_back(node);
					//cout << "V: " << node << endl;
					if(graph[node].arcsNrNets[1].nets.size()>globalCapacity)
						for(arc_it=graph[node].arcsNrNets[1].nets.begin();arc_it!=graph[node].arcsNrNets[1].nets.end();arc_it++){
							netlist[*arc_it].conflict=true;
						}	
					node=(next)+(i*sizeX);
					(nets_it->second).netTree.push_back(node);
				}
				

			}	
			ready.push_back(*nextNode);
			targetNodes.erase(nextNode);
		}
		
		if(!targetNodes.empty()) break;
		if(nets_it->second.conflict) ++conflicts;
	}
	//update History cost
	for(nets_it=netlist.begin(); nets_it!=netlist.end(); ++nets_it){
		if(nets_it->second.conflict){
			for(arc_it=nets_it->second.arcs.begin();arc_it!=nets_it->second.arcs.end();arc_it++){
				if(graph[node].arcsNrNets[0].nets.size()>globalCapacity)  ++graph[node].arcsNrNets[0].history;
				if(graph[node].arcsNrNets[1].nets.size()>globalCapacity)  ++graph[node].arcsNrNets[1].history;
				if(graph[node].arcsNrNets[2].nets.size()>globalCapacity)  ++graph[node].arcsNrNets[2].history;					
			}
		}
	}	
}	
*/
bool Global::routeNets(int nrAttempts){
	list<int> targetNodes;
	map<int,t_nets>::iterator nets_it;
	list<int>::iterator netTree_it;
	vector<int>::iterator nodes_it;
	vector<int> netsVector;
	list<int>::iterator arc_it,node;
	list<arc>::iterator arcTree_it;
	//Initializations
	clock_t start=clock();
	trace_id=0;
	actualAttempt=0;
	conflicts=0;
	visited=0;
	netlist.erase(blockageNet);
	//globalCapacity=3;
	clearHistory();
	
	
	//for(int l=0;l!=graph.size();l++) if(isSource(l)) cout << "Posição: " << l << endl;
	//Global Routing
	cout << "Routing graph..." << endl;
	//Selecting for source node the one closer to the center of the net 
	/*for(nets_it=netlist.begin(); nets_it!=netlist.end(); ++nets_it){
	 getCenter((nets_it->second).nodes);	
	 }	
	 */
	//lShapeRouting();
	do{		
		//conflicts=0;
		// Loop over all nets
		for(nets_it=netlist.begin();nets_it!=netlist.end();nets_it++){
			//If conflict exists, check it out and do rip-up and re-route. Otherwise jump net.
			if(nets_it->second.conflict){
				//If conflict exists, check it out and do rip-upand re-route. Otherwise jump net.
				nets_it->second.conflict=false;
				--conflicts;
				for(arcTree_it=nets_it->second.arcs.begin();arcTree_it!=nets_it->second.arcs.end(); ++arcTree_it){
					if(graph[arcTree_it->node].arcsNrNets[arcTree_it->dir].nets.size()>globalCapacity){
						nets_it->second.conflict=true;
						break;
					}
				}
				if(nets_it->second.conflict) ++conflicts;
			}
			if(nets_it->second.conflict || !actualAttempt){
				//nets_it->second.conflict=false;
				//Rip-up net --Por enquanto desfaz toda a rede
				for(arcTree_it=nets_it->second.arcs.begin();arcTree_it!=nets_it->second.arcs.end();arcTree_it++){	
					for(arc_it=graph[arcTree_it->node].arcsNrNets[arcTree_it->dir].nets.begin();arc_it!=graph[arcTree_it->node].arcsNrNets[arcTree_it->dir].nets.end();arc_it++)
						if(*arc_it==nets_it->first) {
							graph[arcTree_it->node].arcsNrNets[arcTree_it->dir].nets.erase(arc_it);
							break;
						}			
				for(vector<int>::iterator net_it=graph[arcTree_it->node].net.begin();net_it!=graph[arcTree_it->node].net.end();++net_it)
					if(*net_it==nets_it->first) {
						//*net_it=graph[*node].net.back();	
						//graph[*node].net.pop_back();
						graph[arcTree_it->node].net.erase(net_it);
						break;
					}	
				
				}
				nets_it->second.arcs.clear();
				nets_it->second.netTree.clear();
				
				//Create list of target nodes 
				targetNodes.clear();
				for(nodes_it=(nets_it->second).nodes.begin();nodes_it!=(nets_it->second).nodes.end(); ++nodes_it){
					targetNodes.push_back(*nodes_it);
					//cout << *nodes_it << endl;
				}	
				
				//Insert the first node to the tree
				(nets_it->second).netTree.push_back(*targetNodes.begin());
				targetNodes.erase(targetNodes.begin());
				//Loop until all sinks have been found
				while(!targetNodes.empty() && aStar(nets_it, targetNodes)){}  //Route the tree to the closest node in the graph
				
				if(!targetNodes.empty()) break;
				//if(nets_it->second.conflict) ++conflicts;
				
			}	
		}/*
		int conflicts2=0;
		map<int,t_nets>::iterator nets_it2;
		for(nets_it2=netlist.begin();nets_it2!=netlist.end();nets_it2++) conflicts2+=nets_it2->second.conflict;
		cerr << conflicts << " " << conflicts2 << endl;*/
		//update History cost
		++trace_id;
		for(nets_it=netlist.begin(); nets_it!=netlist.end(); nets_it++)
			if(nets_it->second.conflict)
				for(arcTree_it=nets_it->second.arcs.begin();arcTree_it!=nets_it->second.arcs.end();arcTree_it++){
					if(graph[arcTree_it->node].arcsNrNets[arcTree_it->dir].nets.size()>globalCapacity && graph[arcTree_it->node].arcsNrNets[arcTree_it->dir].idNr!=trace_id){
						++graph[arcTree_it->node].arcsNrNets[arcTree_it->dir].history;
						graph[arcTree_it->node].arcsNrNets[arcTree_it->dir].idNr=trace_id;
					}	
				}
		if(!(++actualAttempt%3)) cerr << "." << conflicts << ".";				
	}while(targetNodes.empty() && conflicts && actualAttempt<nrAttempts);
	//ocupacaoArcos();
	
	runtime=float((clock()-start)/(CLOCKS_PER_SEC/1000))/1000;
	cout << endl << "Runtime = " << runtime << "s" << endl;
	if(!targetNodes.empty()) cout << "Impossible to route net: " << nets_it->first << endl;		
	if(conflicts || !targetNodes.empty()){
		cout <<"Unable to route the circuit after ";
		cout << actualAttempt << " attempts."<< endl;
	}else{
		cout <<"Routing finished in ";
		cout << actualAttempt << " attempts."<< endl;
		showResult();
		string tmpstr;
		//	system(("echo "+ intToStr(finalCost) + " " + floatToStr(runtime) + " >> result.txt").c_str());
	} 
	return(targetNodes.empty() && !conflicts);
}



void Global::ocupacaoArcos(){
	/*map<int,t_nets>::iterator nets_it;
	list<int>::iterator arc_it;
	int um,dois,tres,quatro, cinco,defaults;
	um=0; dois=0;tres=0;quatro=0;cinco=0;defaults=0;;
	for(nets_it=netlist.begin(); nets_it!=netlist.end(); ++nets_it){
		for(arc_it=nets_it->second.arcs.begin();arc_it!=nets_it->second.arcs.end();arc_it++){
			if(graph[*arc_it].arcsNrNets[0].nets.size()>0){
				switch (graph[*arc_it].arcsNrNets[0].nets.size()){
					case 1:
						um++;
						break;
					case 2:
						dois++;
						break;
					case 3:
						tres++;
						break;
					case 4:
						quatro++;
						break;
					case 5:
						cinco++;
						break;
				};	
			}	
			if(graph[*arc_it].arcsNrNets[1].nets.size()>0){
				switch (graph[*arc_it].arcsNrNets[1].nets.size()){
					case 1:
						um++;
						break;
					case 2:
						dois++;
						break;
					case 3:
						tres++;
						break;
					case 4:
						quatro++;
						break;
					case 5:
						cinco++;
						break;
				};					
			}
			if(graph[*arc_it].arcsNrNets[2].nets.size()>0){
				switch (graph[*arc_it].arcsNrNets[2].nets.size()){
					case 1:
						um++;
						break;
					case 2:
						dois++;
						break;
					case 3:
						tres++;
						break;
					case 4:
						quatro++;
						break;
					case 5:
						cinco++;
						break;
				};
			}
		}
	}
	
	cout << "Arcs Usage: ";
	cout << um << " " << dois << " " 	<< tres << " " << quatro << " "	<< cinco << endl; 	*/
}
rt_dir2 Global::dirFromPos(int antPos, int pos){
	rt_dir2 dir;
	
	dir=RT_WESTB;
	if(getDir(antPos,dir)==pos) return dir;
	dir=RT_EASTB;
	if(getDir(antPos,dir)==pos) return dir;
	dir=RT_SOUTHB;
	if(getDir(antPos,dir)==pos) return dir;
	dir=RT_NORTHB;
	if(getDir(antPos,dir)==pos) return dir;
	dir=RT_UPB;
	if(getDir(antPos,dir)==pos) return dir;
	dir=RT_DOWNB;
	if(getDir(antPos,dir)==pos) return dir;
}	

bool Global::optimize(){
	return true;
}

void Global::pqAddto(t_tmp& actualNode, priority_queue<t_tmp>& pq, const int& targetNet, list<int>& targetNodes, rt_dir2 direction){
	t_tmp tmp;
	int lCost, nextNode;
	list<int>::iterator target_it;
	
	nextNode=getDir(actualNode.node, direction);
	if(!(graph[nextNode].isVisited(trace_id))){
		tmp.node=nextNode;
		tmp.father=actualNode.node;
		tmp.costAccumulated=actualNode.costAccumulated+calcCost(nextNode, direction,tmp.father);
		lCost=calcDistance(nextNode, targetNodes.front());
		for(target_it=++targetNodes.begin(); target_it!=targetNodes.end(); ++target_it)
			lCost=min(lCost,calcDistance(nextNode, *target_it));
		tmp.aStarCost=tmp.costAccumulated+lCost;
		pq.push(tmp);
	}	
}

//Put the closest to center node in the vector front
int Global::getCenter(vector<int>& Nodes){
	int x=0,y=0,z=0;
	int centerNode,temp,closestPos;
	vector<int>::iterator target_it;
	for(target_it=Nodes.begin(); target_it!=Nodes.end(); ++target_it){
		x+=getPosX(*target_it);
		y+=getPosY(*target_it);
		z+=getPosZ(*target_it);
	}
	centerNode=getPos(round(x/Nodes.size()),round(y/Nodes.size()), round(z/Nodes.size()));
	//cout << "CenterNode: " << centerNode << endl;
	closestPos=getClosestNodePos(centerNode,Nodes);
	temp=Nodes[0];
	Nodes[0]=Nodes[closestPos];
	Nodes[closestPos]=temp;
	return 0;
}

list<int>::iterator Global::getClosestNode(int node,list<int>& targetNodes){
	int lCost=calcDistance(node, targetNodes.front());
    int tempCost;
    list<int>::iterator closestNode=targetNodes.begin();
    for(list<int>::iterator target_it=targetNodes.begin();target_it!=targetNodes.end();target_it++){
		tempCost=calcDistance(node, *target_it);
		if(lCost>tempCost){
			closestNode=target_it;
			lCost=tempCost;
		}	
    }	
    return closestNode;
}	

int Global::getClosestNodePos(int node,vector<int>& targetNodes){
	int lCost=calcDistance(node, targetNodes[0]);
	int closestNodePos=0;
	for(int pos=0;pos<targetNodes.size();pos++){
		//cout << "CALC: " << calcDistance(node, targetNodes[pos]) << endl;
		if(lCost>calcDistance(node, targetNodes[pos])){
			closestNodePos=pos;
			lCost=calcDistance(node, targetNodes[pos]);
		}	
	}	
	return closestNodePos;
}	

int Global::getClosestNodeDistance(int& node, list<int>& targetNodes){
	int lCost;
	list<int>::iterator target_it;
	lCost=calcDistance(node, targetNodes.front());
	for(target_it=++targetNodes.begin(); target_it!=targetNodes.end(); ++target_it)
		lCost=min(lCost,calcDistance(node, *target_it));
	return lCost;
}

inline int Global::calcCost(int node2, rt_dir2 dir,int father){
	
	switch (dir){
		case RT_WESTB: return (costs[getPosZ(node2)*3] +graph[node2].arcsNrNets[0].history)*(actualAttempt ? max(int(graph[node2].arcsNrNets[0].nets.size()-(globalCapacity-2)),1): 1); break;
		case RT_SOUTHB: return (costs[getPosZ(node2)*3+1] +graph[node2].arcsNrNets[1].history)*(actualAttempt ? max(int(graph[node2].arcsNrNets[1].nets.size()-(globalCapacity-2)),1): 1);break;
		case RT_EASTB: return (costs[getPosZ(node2)*3] +graph[father].arcsNrNets[0].history)*(actualAttempt ? max(int(graph[father].arcsNrNets[0].nets.size()-(globalCapacity-2)),1): 1);break;
		case RT_NORTHB: return (costs[getPosZ(node2)*3+1] +graph[father].arcsNrNets[1].history)*(actualAttempt ? max(int(graph[father].arcsNrNets[1].nets.size()-(globalCapacity-2)),1): 1);break;
	}
	return 0;
}



/*
inline int Global::calcCost(int node2, rt_dir2 dir,int father){
	int tmp;
	if(actualAttempt==0) tmp=0;
	else tmp=1;
	switch (dir){
		case RT_WESTB: return (costs[getPosZ(node2)*3] +graph[node2].arcsNrNets[0].history)* tmp*(graph[node2].arcsNrNets[0].nets.size()+1); break;
		case RT_SOUTHB: return (costs[getPosZ(node2)*3+1] +graph[node2].arcsNrNets[1].history)*  tmp*(graph[node2].arcsNrNets[1].nets.size()+1); break;
		case RT_DOWNB: return (costs[(getPosZ(node2)+1)*3+2] +graph[node2].arcsNrNets[2].history)*tmp*(graph[node2].arcsNrNets[2].nets.size()+1); break;
		case RT_EASTB: return (costs[getPosZ(node2)*3] +graph[father].arcsNrNets[0].history)*  tmp*(graph[father].arcsNrNets[0].nets.size()+1); break;
		case RT_NORTHB: return (costs[getPosZ(node2)*3+1] +graph[father].arcsNrNets[1].history)*  tmp*(graph[father].arcsNrNets[1].nets.size()+1); break;
		case RT_UPB: return (costs[(getPosZ(node2)-1)*3+2] +graph[father].arcsNrNets[2].history)*  tmp*(graph[father].arcsNrNets[2].nets.size()+1); break;
	}
	return 0;
}
*/


//Wirelength errado
void Global::showResult(){
	finalCost=wlCost=pins=0;
	rt_dir2 UP=RT_UPB;
	int mul;
	for(map<int,t_nets>::iterator nets_it=netlist.begin(); nets_it!=netlist.end();++nets_it){
		for(list<int>::iterator nodes_it=nets_it->second.netTree.begin(); nodes_it!=nets_it->second.netTree.end();++nodes_it){
				bool inWire=false;
				mul=areConnected2(*nodes_it, RT_EASTB);
				finalCost+=mul*costs[getPosZ(*nodes_it)*3];	
				if(mul!=0){
					//cout << *nodes_it << " -> " << (*nodes_it)+1 << endl;
					inWire=true;
				}	
				mul=areConnected2(*nodes_it, RT_NORTHB);
				finalCost+=mul*costs[getPosZ(*nodes_it)*3+1];
				if(mul!=0) {
					//cout << *nodes_it << " -> " << (*nodes_it)+sizeX << endl;
					inWire=true;
				}
				mul=areConnected2(*nodes_it, RT_UPB);
				if(areConnected(getDir(*nodes_it, UP), RT_UPB)) 
					++pins;
				else pins+=2;  
			if(inWire) ++wlCost;
			}
			//			cout << "(" << getPosX(*nodes_it) << " " <<getPosY(*nodes_it)<< " " <<getPosZ(*nodes_it)<< ")  --("<< graph[*nodes_it].net << " " << nets_it->second.finalNet <<")-->  (" << graph[*nodes_it].rightIter << " " <<graph[*nodes_it].upIter<< " " <<graph[*nodes_it].upLayerIter << ") " << endl;
		}
	cout << "Cost =" << finalCost << endl;
	cout << "Wirelength =" << wlCost << endl;
	cout << "# Pins =" << pins << endl;
}

bool Global::areConnected(int node1, rt_dir2 dir){
	switch(dir){
		case RT_WESTB:	return getPosX(node1)>0 && graph[node1-1].arcsNrNets[0].nets.size()>0;
		case RT_SOUTHB:	return getPosY(node1)>0 && graph[node1-sizeX].arcsNrNets[1].nets.size()>0;
		case RT_EASTB:	return getPosX(node1)+1<sizeX && graph[node1].arcsNrNets[0].nets.size()>0;
		case RT_NORTHB:	return getPosY(node1)+1<sizeY && graph[node1].arcsNrNets[1].nets.size()>0;
	}
	return false;
}

int Global::areConnected2(int node1, rt_dir2 dir){
	switch(dir){
		case RT_WESTB:	return (getPosX(node1)>0) * graph[node1-1].arcsNrNets[0].nets.size();
		case RT_SOUTHB:	return (getPosY(node1)>0) * graph[node1-sizeX].arcsNrNets[1].nets.size();
		case RT_EASTB:	return (getPosX(node1)+1<sizeX) * graph[node1].arcsNrNets[0].nets.size();
		case RT_NORTHB:	return (getPosY(node1)+1<sizeY) * graph[node1].arcsNrNets[1].nets.size();
	}
	return 0;
}
/*
bool Global::connect(int net, int n1, int n2){		
	//cout << n1 << ", " << getNet(n1) << endl;
	//cout << n2 << ", " << getNet(n2) << endl;
    if(!getNet(n1)){
        //cout << "(" << n1 << ", ";
		graph[n1].net=net;
		graph[n1].nrNets++;
		netlist[net].netTree.push_front(n1);
	}		
	if(!getNet(n2)){
        //cout <<  n2 << ")" << endl;
		graph[n2].net=net;
		graph[n2].nrNets++;
		netlist[net].netTree.push_front(n2);
	}  
	return true; //TODO: TEST TO RETURN FALSE
}		
*/
