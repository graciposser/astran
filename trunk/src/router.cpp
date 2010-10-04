/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   amziesemerj[at]inf.ufrgs.br                                           *
 **************************************************************************/
#include "router.h"

Router::Router(){
	sizeX=sizeY=sizeZ=0;
}



Router::~Router(){}

void Router::test(string r){
	//char* name;
	int tSiz,cSiz,cRT,tRT;
	int j;
	float divC,divT;
	teste=true;
	sizeZ=2;	
	system("echo  > result.txt");
	vector<int> binVector;
	int siz=1000;
	//for(int siz=400;siz<=600;siz+=200){	
		//int siz=100;
		sizeX=siz;
		sizeY=siz;
		tSiz=0;
		cSiz=0;
		tRT+=0;
		cRT=0;
		j=0;
		divC=0;
		divT=0;
		cout << "Size: " << siz << endl;
		for(int nrNets=siz*0.6; nrNets<=siz*0.6; nrNets+=siz*.2){
			//for(int nrNets=siz*0.5; nrNets<=siz*0.5; nrNets+=siz*.2){
				//for(int bin=20;bin!=50;bin+=10){
			//		for(int cap=1;cap!=5;cap++){
			//int nrNets=1;
			netIndex.clear();
					//cout << endl << "Bins= " << bin << endl;
			cout << endl << "Routing test circuit with #Nets=" << nrNets << endl; 
			rt.clear();
			gr.clear();
			rt.setSize(sizeX,sizeY,sizeZ);
			gr.setBinSize(1000,1000);
			gr.setGlobalCapacity(9);
			gr.setSize(int(ceil(sizeX/double(gr.getBinX()))),int(ceil(sizeY/double(gr.getBinY()))),1);
			rt.setLayerCosts(0,	11, 4, 4);
			rt.setLayerCosts(1,	4, 11, 4);
			gr.setLayerCosts(0,	4, 4, 4);
			//gr.setLayerCosts(1,4, 11, 4);
			for(int x=1; x<=nrNets; x++){
				netIndex[intToStr(x)]=x;
				int netSize=2+rand()%5;
				//int netSize=3;
				binVector.clear();
				binVector.resize(int(ceil(sizeX/double(gr.getBinX()))*ceil(sizeY/double(gr.getBinY()))));
				for(int y=0; y<netSize; y++){
					int node=rand()%(sizeX*sizeY);
					if(rt.getNet(node)){ --y; continue;}
					rt.addNodetoNet(netIndex[intToStr(x)], node);
					int bin=floor(rt.getPosX(node)/double(gr.getBinX()))+floor(rt.getPosY(node)/double(gr.getBinY()))*ceil(sizeX/double(gr.getBinX()));
					//cout << "Node: " << node << "Bin: " << bin << endl;
					if(binVector[bin]!=1){
						gr.addNodetoNet(netIndex[intToStr(x)], bin);
						binVector[bin]=1;
					    //cout << "Node2: " << node << "Bin2: " << bin << endl; 
					}			 
					Point p((rt.getPosX(node)+1)*pitchH, (rt.getPosY(node)+1)*pitchV);
				}
			}
			//Global Routing -- Set the regions for detailed routing
			gr.routeNets(500);
			//Detailed Routing
			rt.routeNets(500,gr);
		}
			//	}
			//}
  // }
}



/*
void Router::test(string r){
	//char* name;
	int tSiz,cSiz,cRT,tRT;
	int j;
	float divC,divT;
	teste=true;
	sizeZ=2;	
	system("echo  > result.txt");
	
	int siz=40;
	//for(int siz=400;siz<=1000;siz+=200){	
	//int siz=400;
	sizeX=siz;
	sizeY=siz;
	tSiz=0;
	cSiz=0;
	tRT+=0;
	cRT=0;
	j=0;
	divC=0;
	divT=0;
	cout << "Size: " << siz << endl;
	for(int nrNets=siz*0.3; nrNets<=siz*0.3; nrNets+=siz*.2){
		//for(int nrNets=siz*0.3; nrNets<=siz*0.81; nrNets+=siz*.2){
		//int nrNets=2;
		netIndex.clear();
		cout << endl << "Routing test circuit with #Nets=" << nrNets << endl; 
		rt.clear();
		rt.setSize(sizeX,sizeY,sizeZ);
		rt.setLayerCosts(0,	11, 4, 4);
		rt.setLayerCosts(1,	4, 11, 4);
		for(int x=1; x<=nrNets; x++){
			netIndex[intToStr(x)]=x;
			int netSize=2+rand()%5;
			//int netSize=3;
			for(int y=0; y<netSize; y++){
				int node=rand()%(sizeX*sizeY);
				if(rt.getNet(node)){ --y; continue;}
				rt.addNodetoNet(netIndex[intToStr(x)], node);
				Point p((rt.getPosX(node)+1)*pitchH, (rt.getPosY(node)+1)*pitchV);
			}
		}
		//cout << "ROTDL: " << endl;
		//rotdl(r);
		//tRT=tempoRotdl;
		//cRT=custoRotdl;
		//sprintf(name,"%d",nrNets) ;
		//saveRoutingRotdl("teste");
		//rt.reset();
		//cout << "OURS: " << endl;
		rt.routeNets(2000);
		//tSiz=rt.getTempo();
		//cSiz=rt.getCost();
		//divT+=float(tSiz)/float(tRT);
		//divC+=float(cSiz)/float(cRT);
		//j++;
		//readRoutingRotdl("teste");
	}
	//compT=divT/float(j);
	//compC=divC/float(j); 
	//cout << "Comparacao custo " << siz << ": "<<  compC << endl;
	//cout << "Comparacao tempo " << siz << ": "<<  compT << endl;
	//}
}
*/
bool Router::setup(int sX, int sY, int sZ){
	teste=false;
	sizeX=sX;
	sizeY=sY;
	sizeZ=sZ;
	rt.clear();
	rt.setSize(sizeX,sizeY,sizeZ);
	rt.setLayerCosts(0,	999 , 999, 4);
	rt.setLayerCosts(1,	11, 4, 4);
	rt.setLayerCosts(2,	4, 11, 4);
	
	int pitchH=currentCircuit->getHPitch()*currentCircuit->getRules()->getScale();
	int pitchV=currentCircuit->getVPitch()*currentCircuit->getRules()->getScale();
	
	netIndex.clear();
	int i=1;
	vector<t_net>& nets=currentCircuit->getCellNetlst(currentCircuit->getTopCell())->getNets();
	for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		if(nets_it->name!=currentCircuit->getVddNet() && nets_it->name!=currentCircuit->getGndNet()){
			if(netIndex.find(nets_it->name)==netIndex.end()) netIndex[nets_it->name]=i++;
			Interface* tmp_int=currentCircuit->getInterface(nets_it->name);
			if(tmp_int){
				//					cerr << netIndex[nets_it->name] << " " << tmp_int->pos.getX()+currentCircuit->getLMargin() << " " << tmp_int->pos.getY()+currentCircuit->getBMargin() << endl;
				rt.addNodetoNet(netIndex[nets_it->name], rt.getPos(tmp_int->pos.getX()+currentCircuit->getLMargin() ,tmp_int->pos.getY()+currentCircuit->getBMargin(), 2));
			}
			for(list<t_inst>::iterator nodes_it=nets_it->insts.begin(); nodes_it!=nets_it->insts.end(); nodes_it++){
				Instance* tmp_inst=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstance(nodes_it->targetCellInst);
				if(tmp_inst){
					CLayout* tmp_layout=currentCircuit->getLayout(tmp_inst->getTargetCell());
					if(tmp_layout){
						multimap<string,Pin>::iterator pin_it = tmp_layout->getPins().find(currentCircuit->getCellNetlst(tmp_inst->getTargetCell())->getInout(nodes_it->targetPin));
						if(pin_it!=tmp_layout->getPins().end()){
							int posx=pin_it->second.getX(), posy=pin_it->second.getY();
							if(tmp_inst->getMX())
								posy=tmp_inst->getY()-posy;
							else
								posy+=tmp_inst->getY();
							if(tmp_inst->getMY())
								posx=tmp_inst->getX()-posx;
							else
								posx+=tmp_inst->getX();								
							rt.addNodetoNet(netIndex[nets_it->name], rt.getPos(round(float(posx)/pitchH)+currentCircuit->getLMargin() ,round(float(posy)/pitchV)+currentCircuit->getBMargin(),0));
						}else{
							cout << "Pin " << nodes_it->targetPin << " not found in cell instance " << nodes_it->targetCellInst << endl;
							return false;
						}
					}else{
						cout << "Cell " << tmp_inst->getTargetCell() << " not found" << endl;
						return false;
					}
				}else{
					cout << "Cell instance " << nodes_it->targetCellInst << " not found" << endl;
					return false;
				}
			}
		}
	}
	//	if(netIndex.find("bl")!=netIndex.end()) rt.setBlockageNet(netIndex["bl"]);
	return true;
}

bool Router::route(int effort){
	return false;
	//return rt.routeNets(effort);
}

bool Router::optimize(){
	return rt.optimize(); 
}

bool Router::compactLayout(string lpSolverFile){
	bool global=false;
	if(global){
		sizeX=gr.getSizeX();
		sizeY=gr.getSizeY();	
		sizeZ=gr.getSizeZ();
	}	
	vector<layer_name> rtLayers(5);
	rtLayers[0]=MET1;
	rtLayers[1]=VIA1;
	rtLayers[2]=MET2;
	rtLayers[3]=VIA2;
	rtLayers[4]=MET3;
	rtLayers[5]=VIA3;
	rtLayers[6]=MET4;
	
	int pitchH=currentCircuit->getHPitch()*currentCircuit->getRules()->getScale();
	int pitchV=currentCircuit->getVPitch()*currentCircuit->getRules()->getScale();
	
	vector<Box*> geometries;
	
	compaction cpt(CP_LP);
	cpt.setLPFilename("temp");	
	Rules* currentRules=currentCircuit->getRules();
	cpt.insertConstraint( "ZERO", "MAXM1", CP_EQ, currentRules->getRule(W1M1)*1.5);
	cpt.insertConstraint( "ZERO", "MAXM12", CP_EQ, currentRules->getRule(S1M1M1)*3.5);
	
	vector<CLayout> rtLayout(netIndex.size());
	map<string, int>::iterator netIndex_it;
	for(netIndex_it=netIndex.begin(); netIndex_it!=netIndex.end(); netIndex_it++)
		rtLayout[netIndex_it->second-1].setName(netIndex_it->first);

	string lastx, lastSeg;
	bool adj;
	vector<string> labels(sizeX*sizeY*sizeZ, "");
	int currentNode;
	list<int> tmpList;
	list<int>::iterator list_it;
	for(int z=0; z<sizeZ; z++){
		vector<string> lasty(sizeY,""), lastSegY(sizeY,"");
		for(int y=0; y<sizeY; y++){
			lastx="";
			lastSeg="";
			adj=false;
			for(int x=0; x<sizeX; x++){
				if(global){
					currentNode=gr.getPos(x,y,z);
					if(gr.isSource(currentNode)){
						Point p((x+1)*pitchH,(y+1)*pitchV);
						list<int>::iterator listNets_it;
						list<int> listNets; 
						/*listNets=gr.getListNets(currentNode);
						for(listNets_it=listNets.begin();listNets_it!=listNets.end();listNets_it++){
							if(*listNets_it<0)
								rtLayout[abs(*listNets_it)-1].addLabel(intToStr(abs(currentNode)),p);
							
						}	*/
					}
				}else{	
					currentNode=rt.getPos(x,y,z);
					if(rt.isSource(currentNode)){
						Point p((x+1)*pitchH,(y+1)*pitchV);						
						//rtLayout[rt.getNet(currentNode)-1].addLabel(intToStr(abs(currentNode)),p);
					    /*if(rt.getNet(currentNode)!=0)
							rtLayout[rt.getNet(currentNode)-1].addLabel(intToStr(rt.getBin(abs(currentNode))),p);*/
					}
				}
				
				if(true){
					if(global){
						if(gr.areConnected(currentNode, RT_EASTB)){
							tmpList=gr.getListNodes(currentNode,0);
							for(list_it=tmpList.begin();list_it!=tmpList.end();list_it++)
								rtLayout[abs(*list_it)-1].addPath((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W1M1),pitchH, true, E, rtLayers[z*2]);
						}
					}else{
						
						if(rt.areConnected(currentNode, RT_EAST)){
							rtLayout[rt.getNet(currentNode)-1].addPath((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W1M1),pitchH, true, E, rtLayers[z*2]);
						}
					}
					
				    if(global){
						if(gr.areConnected(currentNode, RT_NORTHB)){
							tmpList=gr.getListNodes(currentNode,1);
							for(list_it=tmpList.begin();list_it!=tmpList.end();list_it++)
								rtLayout[abs(*list_it)-1].addPath((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W1M1),pitchV, true, N, rtLayers[z*2]);
						}
					}else{  
						if(rt.areConnected(currentNode, RT_NORTH)){
							rtLayout[rt.getNet(currentNode)-1].addPath((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W1M1),pitchV, true, N, rtLayers[z*2]);
						}
					}
					if(global){
						if(gr.areConnected(currentNode, RT_UPB)){
							tmpList=gr.getListNodes(currentNode,2);
							for(list_it=tmpList.begin();list_it!=tmpList.end();list_it++){
								Box b=rtLayout[(*list_it)-1].addLayer((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W2VI), currentRules->getRule(W2VI), rtLayers[(z*2)+1]);
								if(z) rtLayout[(*list_it)-1].addEnc(b, currentRules->getRule(E1M1VI), rtLayers[z*2]);
								rtLayout[(*list_it)-1].addEnc(b, currentRules->getRule(E1M2VI), rtLayers[(z+1)*2]);
							}
						}
					}else{
						if(rt.areConnected(currentNode, RT_UP)){
							Box b=rtLayout[rt.getNet(currentNode)-1].addLayer((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W2VI), currentRules->getRule(W2VI), rtLayers[(z*2)+1]);
							if(z) rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M1VI), rtLayers[z*2]);
							rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M2VI), rtLayers[(z+1)*2]);						
						}
					}
				}else{
					if(z%2==1){
						//se conecta com o da direira ou para baixo cria dogleg
						if(x<sizeX-1 && rt.areConnected(currentNode,RT_EAST)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,((y+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((y+1)*pitchV)+currentRules->getRule(W1M1)/2, rtLayers[z*2]));
								labels[currentNode]=intToStr(geometries.size()-1);
								cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + labels[currentNode] + "b", CP_EQ, "x" + labels[currentNode] + "min");
								cpt.insertLPMinVar("x" + labels[currentNode] + "min");
								if(lastx!="")
									cpt.insertConstraint( "x" + lastx + "b", "x" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lastx=labels[currentNode];
							}
							labels[currentNode+1]=labels[currentNode];
						}
						
						if(y<sizeY-1 && rt.areConnected(currentNode,RT_NORTH)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,((y+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((y+1)*pitchV)+currentRules->getRule(W1M1)/2, rtLayers[z*2]));
								labels[currentNode]=intToStr(geometries.size()-1);
								cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + labels[currentNode] + "b", CP_EQ, "x" + labels[currentNode] + "min");
								cpt.insertLPMinVar("x" + labels[currentNode] + "min");
								if(lastx!="")
									cpt.insertConstraint( "x" + lastx + "b", "x" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lastx=labels[currentNode]; //armazena o último segmento
							}
						}
						
						if(y>0 && rt.areConnected(currentNode, RT_DOWN)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,((y+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((y+1)*pitchV)+currentRules->getRule(W1M1)/2, rtLayers[z*2]));
								labels[currentNode]=intToStr(geometries.size()-1);
								cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + labels[currentNode] + "b", CP_EQ, "x" + labels[currentNode] + "min");
								cpt.insertLPMinVar("x" + labels[currentNode] + "min");
								if(lastx!="")
									cpt.insertConstraint( "x" + lastx + "b", "x" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lastx=labels[currentNode]; //armazena o último segmento
							}
							
							geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,y*pitchV, 0, (y+1)*pitchV, rtLayers[z*2]));
							string px=intToStr(geometries.size()-1);
							cpt.insertConstraint( "x" + px + "a", "x" + px + "b", CP_MIN, currentRules->getRule(W1M1));
							cpt.insertConstraint( "x" + px + "a", "x" + px + "b", CP_EQ, "x" + px + "s");
							cpt.insertConstraint( "x" + px + "s", "MAXM1", CP_EQ, "x" + px + "min");
							cpt.insertLPMinVar("x" + px + "min",2);
							
							cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + px + "a", CP_MIN, "ZERO");
							cpt.insertConstraint( "x" + px + "b", "x" + labels[currentNode] + "b", CP_MIN, "ZERO");
							
							cpt.insertConstraint( "x" + labels[rt.getPos(x,y-1,z)] + "a", "x" + px + "a", CP_MIN, "ZERO");
							cpt.insertConstraint( "x" + px + "b", "x" + labels[rt.getPos(x,y-1,z)] + "b", CP_MIN, "ZERO");
							
							if(lastSeg!=""){
								cpt.insertConstraint( "x" + lastSeg + "b", "x" + px + "a", CP_MIN, currentRules->getRule(S1M1M1));
								cpt.insertConstraint( "x" + lastSeg + "b", "x" + px + "a", CP_EQ, "x" + px + "se");
								if(adj){
									cpt.insertConstraint( "x" + px + "se", "MAXM12", CP_EQ, "x" + px + "mine");
									cpt.insertLPMinVar("x" + px + "mine",3);
								}
							}
							
							lastSeg=px;
							adj=true;
						}
						else adj=false;
						
						if(z<sizeZ-1 && rt.areConnected(currentNode,RT_UP)){
							Box b=rtLayout[rt.getNet(currentNode)-1].addLayer((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W2VI), currentRules->getRule(W2VI), rtLayers[(z*2)+1]);
							rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M1VI), rtLayers[z*2]);
							rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M2VI), rtLayers[(z+1)*2]);
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,((y+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((y+1)*pitchV)+currentRules->getRule(W1M1)/2, rtLayers[z*2]));
								labels[currentNode]=intToStr(geometries.size()-1);
								cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + labels[currentNode] + "b", CP_EQ, "x" + labels[currentNode] + "min");
								cpt.insertLPMinVar("x" + labels[currentNode] + "min");
								if(lastx!="")
									cpt.insertConstraint( "x" + lastx + "b", "x" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lastx=labels[currentNode]; //armazena o último segmento
							}
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "a", CP_MAX, ((x+1)*pitchH)-(currentRules->getRule(W2VI)/2)-currentRules->getRule(E1M1VI));
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "b", CP_MIN, ((x+1)*pitchH)+(currentRules->getRule(W2VI)/2)+currentRules->getRule(E1M1VI));
						}
						
						if(z>0 && rt.areConnected(currentNode, RT_DOWN)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,((y+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((y+1)*pitchV)+currentRules->getRule(W1M1)/2, rtLayers[z*2]));
								labels[currentNode]=intToStr(geometries.size()-1);
								cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + labels[currentNode] + "b", CP_EQ, "x" + labels[currentNode] + "min");
								cpt.insertLPMinVar("x" + labels[currentNode] + "min");
								if(lastx!="")
									cpt.insertConstraint( "x" + lastx + "b", "x" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lastx=labels[currentNode]; //armazena o último segmento
							}
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "a", CP_MAX, ((x+1)*pitchH)-(currentRules->getRule(W2VI)/2)-currentRules->getRule(E1M1VI));
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "b", CP_MIN, ((x+1)*pitchH)+(currentRules->getRule(W2VI)/2)+currentRules->getRule(E1M1VI));
						}
						if(rt.isSource(currentNode)){
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "a", CP_MAX, ((x+1)*pitchH));
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "b", CP_MIN, ((x+1)*pitchH));
						}
						
					}
					else{
						if(y<sizeY-1 && rt.areConnected(currentNode,RT_NORTH)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(((x+1)*pitchH)-currentRules->getRule(W1M1)/2, 0, ((x+1)*pitchH)+currentRules->getRule(W1M1)/2, 0, rtLayers[z*2]));
								labels[currentNode]=intToStr(geometries.size()-1);
								cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + labels[currentNode] + "b", CP_EQ, "y" + labels[currentNode] + "min");
								cpt.insertLPMinVar("y" + labels[currentNode] + "min");
								if(lasty[x]!="")
									cpt.insertConstraint( "y" + lasty[x] + "b", "y" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lasty[x]=labels[currentNode]; //armazena o último segmento
							}
							labels[rt.getPos(x,y+1,z)]=labels[currentNode];
						}
						if(x<sizeX-1 && rt.areConnected(currentNode,RT_EAST)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(((x+1)*pitchH)-currentRules->getRule(W1M1)/2, 0, ((x+1)*pitchH)+currentRules->getRule(W1M1)/2, 0, rtLayers[z*2]));
								labels[currentNode]=intToStr(geometries.size()-1);
								cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + labels[currentNode] + "b", CP_EQ, "y" + labels[currentNode] + "min");
								cpt.insertLPMinVar("y" + labels[currentNode] + "min");
								if(lasty[x]!="")
									cpt.insertConstraint( "y" + lasty[x] + "b", "y" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lasty[x]=labels[currentNode];
							}
						}
						
						if(x>0 && rt.areConnected(currentNode, RT_WEST)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(((x+1)*pitchH)-currentRules->getRule(W1M1)/2, 0, ((x+1)*pitchH)+currentRules->getRule(W1M1)/2, 0, rtLayers[z*2]));
								labels[currentNode]=intToStr(geometries.size()-1);
								cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + labels[currentNode] + "b", CP_EQ, "y" + labels[currentNode] + "min");
								cpt.insertLPMinVar("y" + labels[currentNode] + "min");
								if(lasty[x]!="")
									cpt.insertConstraint( "y" + lasty[x] + "b", "y" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lasty[x]=labels[currentNode]; //armazena o último segmento
							}
							
							geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(x*pitchH, 0, (x+1)*pitchH, 0, rtLayers[z*2]));
							string px=intToStr(geometries.size()-1);
							cpt.insertConstraint( "y" + px + "a", "y" + px + "b", CP_MIN, currentRules->getRule(W1M1));
							cpt.insertConstraint( "y" + px + "a", "y" + px + "b", CP_EQ, "y" + px + "s");
							cpt.insertConstraint( "y" + px + "s", "MAXM1", CP_EQ, "y" + px + "min");
							cpt.insertLPMinVar("y" + px + "min",2);
							
							cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + px + "a", CP_MIN, "ZERO");
							cpt.insertConstraint( "y" + px + "b", "y" + labels[currentNode] + "b", CP_MIN, "ZERO");
							
							cpt.insertConstraint( "y" + labels[rt.getPos(x-1,y,z)] + "a", "y" + px + "a", CP_MIN, "ZERO");
							cpt.insertConstraint( "y" + px + "b", "y" + labels[rt.getPos(x-1,y,z)] + "b", CP_MIN, "ZERO");
							
							if(lastSegY[x]!=""){
								cpt.insertConstraint( "y" + lastSegY[x] + "b", "y" + px + "a", CP_MIN, currentRules->getRule(S1M1M1));
								cpt.insertConstraint( "y" + lastSegY[x] + "b", "y" + px + "a", CP_EQ, "y" + px + "se");
								if(rt.areConnected(rt.getPos(x,y-1,z), RT_WEST)){
									cpt.insertConstraint( "y" + px + "se", "MAXM12", CP_EQ, "y" + px + "mine");
									cpt.insertLPMinVar("y" + px + "mine",3);
								}
							}						
							lastSegY[x]=px;
						}
						
						if(z<sizeZ-1 && rt.areConnected(currentNode,RT_UP)){
							Box b=rtLayout[rt.getNet(currentNode)-1].addLayer((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W2VI), currentRules->getRule(W2VI), rtLayers[(z*2)+1]);
							rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M1VI), rtLayers[z*2]);
							rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M2VI), rtLayers[(z+1)*2]);
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(((x+1)*pitchH)-currentRules->getRule(W1M1)/2, 0, ((x+1)*pitchH)+currentRules->getRule(W1M1)/2, 0, rtLayers[z*2]));
								labels[currentNode]=intToStr(geometries.size()-1);
								cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + labels[currentNode] + "b", CP_EQ, "y" + labels[currentNode] + "min");
								cpt.insertLPMinVar("y" + labels[currentNode] + "min");
								if(lasty[x]!="")
									cpt.insertConstraint( "y" + lasty[x] + "b", "y" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lasty[x]=labels[currentNode]; //armazena o último segmento
							}
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "a", CP_MAX, ((y+1)*pitchV)-(currentRules->getRule(W2VI)/2)-currentRules->getRule(E1M1VI));
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "b", CP_MIN, ((y+1)*pitchV)+(currentRules->getRule(W2VI)/2)+currentRules->getRule(E1M1VI));
						}
						
						if(z>0 && rt.areConnected(currentNode, RT_DOWN)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(((x+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((x+1)*pitchV)+currentRules->getRule(W1M1)/2, 0, rtLayers[z*2]));
								labels[currentNode]=intToStr(geometries.size()-1);
								cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + labels[currentNode] + "b", CP_EQ, "y" + labels[currentNode] + "min");
								cpt.insertLPMinVar("y" + labels[currentNode] + "min");
								if(lasty[x]!="")
									cpt.insertConstraint( "y" + lasty[x] + "b", "y" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lasty[x]=labels[currentNode]; //armazena o último segmento
							}
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "a", CP_MAX, ((y+1)*pitchV)-(currentRules->getRule(W2VI)/2)-currentRules->getRule(E1M1VI));
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "b", CP_MIN, ((y+1)*pitchV)+(currentRules->getRule(W2VI)/2)+currentRules->getRule(E1M1VI));
						}
						if(rt.isSource(currentNode)){
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "a", CP_MAX, ((y+1)*pitchV));
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "b", CP_MIN, ((y+1)*pitchV));
						}
					}
				}
			}
		}
	}
	
	cpt.solve(lpSolverFile);
	
	for (unsigned int i =0; i < geometries.size(); i++ ) {		
		int xa = cpt.getVariableVal( "x" + intToStr( i ) + "a" );
		int xb = cpt.getVariableVal( "x" + intToStr( i ) + "b" );
		int ya = cpt.getVariableVal( "y" + intToStr( i ) + "a" );
		int yb = cpt.getVariableVal( "y" + intToStr( i ) + "b" );
		
		if(xa!=-1 && xb!=-1){
			geometries[i]->setWidth(xb-xa);
			geometries[i]->setX((xb+xa)/2);
		}
		if(ya!=-1 && yb!=-1){
			geometries[i]->setHeight(yb-ya);
			geometries[i]->setY((yb+ya)/2);
		}		
	}
	CLayout rt_insts(currentCircuit->getTopCell() + "_RT");

	currentCircuit->insertLayout(rt_insts);
	vector<CLayout>::iterator layout_it;
	for(layout_it=rtLayout.begin(); layout_it!=rtLayout.end(); layout_it++){
		layout_it->merge();
		currentCircuit->insertLayout(*layout_it);
		currentCircuit->getLayout(currentCircuit->getTopCell() + "_RT")->addInstance(layout_it->getName(), layout_it->getName());		
	}
	currentCircuit->getLayout(currentCircuit->getTopCell())->addInstance("ROUTING", currentCircuit->getTopCell() + "_RT");
	currentCircuit->getLayout(currentCircuit->getTopCell())->placeCell("ROUTING", -(pitchH+currentCircuit->getHPitch()), -(pitchV+currentCircuit->getVPitch()), false, false);
	return true;
}

int Router::saveRoutingRotdl(string fileName){return 0;};
bool Router::readRoutingRotdl(string fileName){return false;};
bool Router::rotdl(string path){return false;};
int Router::getNrNets(int x, int y, int z){return 0;};

/*
//Save routing result in Rotdl format
int Router::saveRoutingRotdl(string fileName){
	int net;
	ofstream froute(fileName.c_str());
	
	if (!froute.is_open()){
		cout << "Could not save routing to file: " << fileName << endl;
		return 0;
	}
	
	for(net=1;net<=rt.netlistSize();net++){
		//rt.pathTree(net,froute);
	}
	froute.close();
	return 1;
}*/
/*
//Read routing result file in Rotdl format
bool Router::readRoutingRotdl(string fileName){
    rt.clear();
	
	ifstream file(fileName.c_str());
	if ((!file.is_open())){
		cout << "Could not open routing file: " << fileName <<  endl;
		return false;
	}
	int fileline=0, pxini, pyini, pzini, nrMov, net;
	string str_tmp, str, typeMove, netName;
	while(getline(file,str_tmp)) {
		fileline++;
		if(!str_tmp.size()) continue;
		istrstream line(str_tmp.c_str());
		line >> str;
		if(upcase(str)=="NET"){
			line >> netName;
			while(line >> str){
				net=netIndex[netName];
				if(str=="("){
					line >> pxini;
					line >> pyini;				
					line >> pzini;
					line >> str;
					pzini+=!teste;
				}
				else if(str=="["){
					line >> nrMov;
					line >> typeMove;
					for(int x=0; x<nrMov; x++){
						if(typeMove=="UP"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini, pzini+1)); ++pzini;
						}else if(typeMove=="DOWN"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini, pzini-1)); --pzini;
						}else if(typeMove=="EAST"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini+1, pyini, pzini)); ++pxini;
						}else if(typeMove=="WEST"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini-1, pyini, pzini)); --pxini;
						}else if(typeMove=="NORTH"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini+1, pzini)); ++pyini;
						}else if(typeMove=="SOUTH"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini-1, pzini)); --pyini;
						}
					}
					line >> str; // ]
				}else return false;
			}
			
		}else if(upcase(str)=="ERROR:"){
			line >> str;
			line >> netName;
			cout << "Unable to route net: " << str << netName << endl;
		}else return false;
	}
	rt.showResult();
	file.close();
	return true;
}*/
/*
bool Router::rotdl(string path){
	int pitchH=currentCircuit->getHPitch()*currentCircuit->getRules()->getScale();
	int pitchV=currentCircuit->getVPitch()*currentCircuit->getRules()->getScale();
	
	ofstream frot("temp.rot");
	frot << sizeX << " " << sizeY << " " << sizeZ-1+teste << endl;
	frot << "0 0" << endl;
	frot <<  pitchH << " " << pitchV << endl;
	frot << currentCircuit->getRules()->getCIFVal(MET2) << " "<< currentCircuit->getRules()->getCIFVal(VIA2) << " " << currentCircuit->getRules()->getCIFVal(MET3) << endl;
	frot << currentCircuit->getRules()->getRule(W1M2) << " "<< currentCircuit->getRules()->getRule(W2V2) << " " << currentCircuit->getRules()->getRule(W1M3) << endl;
	
	vector<int> *nodesList;
	vector<int>::iterator nodesList_it;
	map<string, int>::iterator netIndex_it;
	for(netIndex_it=netIndex.begin(); netIndex_it!=netIndex.end(); netIndex_it++){
		nodesList=rt.getNetNodes(netIndex_it->second);
		frot << netIndex_it->second << " " << netIndex_it->first << " " << nodesList->size() << " " << nodesList->size();
		for(nodesList_it=nodesList->begin(); nodesList_it!=nodesList->end(); nodesList_it++){
			if(!teste && rt.getPosZ(*nodesList_it)==0){
				frot << " (" << 0 << " " << rt.getPosX(*nodesList_it) << " " << rt.getPosY(*nodesList_it) << " 1h)";
				rt.connect(netIndex[netIndex_it->first], *nodesList_it, *nodesList_it+sizeX*sizeY);
			}else
				frot << " (" << rt.getPosZ(*nodesList_it)-1+teste << " " << rt.getPosX(*nodesList_it) << " " << rt.getPosY(*nodesList_it) << " 1h)";
		}
		frot << ";" << endl;
	}
	frot.close();
	
	string cmd = "\"" + path + "\" -outputConnections temp.rot > temp.log";
	cout << "Executing rotdl: " << cmd << endl;
	
	time_t start,end;
	time(&start); 
	
	FILE *x = _popen(cmd.c_str(), "r");
	
	if(x==NULL) {
		cout << "\t\tERROR: Failed to execute rotdl!" << endl;
		return false;
	}
	
	_pclose(x);
	
	time(&end); 
	cout << "Runtime = " << difftime(end, start) << "s" << endl;
	
	ifstream file("temp.rot.connections"); // Read
	if(!file){
		cout << "Could not open rounting file: temp.rot.connections" << endl;
		return false;
	}
	
	//Parse the file
	int fileline=0, pxini, pyini, pzini, nrMov, net;
	string str_tmp, str, typeMove, netName;
	while(getline(file,str_tmp)) {
		fileline++;
		if(!str_tmp.size()) continue;
		istrstream line(str_tmp.c_str());
		line >> str;
		if(upcase(str)=="NET"){
			line >> netName;
			while(line >> str){
				net=netIndex[netName];
				if(str=="("){
					line >> pxini;
					line >> pyini;				
					line >> pzini;
					line >> str;
					pzini+=!teste;
				}
				else if(str=="["){
					line >> nrMov;
					line >> typeMove;
					for(int x=0; x<nrMov; x++){
						if(typeMove=="UP"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini, pzini+1)); ++pzini;
						}else if(typeMove=="DOWN"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini, pzini-1)); --pzini;
						}else if(typeMove=="EAST"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini+1, pyini, pzini)); ++pxini;
						}else if(typeMove=="WEST"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini-1, pyini, pzini)); --pxini;
						}else if(typeMove=="NORTH"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini+1, pzini)); ++pyini;
						}else if(typeMove=="SOUTH"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini-1, pzini)); --pyini;
						}
					}
					line >> str; // ]
				}else return false;
			}
			
		}else if(upcase(str)=="ERROR:"){
			line >> str;
			line >> netName;
			cout << "Unable to route net: " << str << netName << endl;
		}else return false;
	}
	rt.showResult();
	return true;
}*/
/*
int Router::getNrNets(int x, int y, int z){
	return sizeX ? rt.getNrNets(rt.getPos(x,y,z)) : 0;
}*/
