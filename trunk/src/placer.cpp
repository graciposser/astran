/*
 *  placer.cpp
 *  ICPD
 *
 *  Created by Adriel Mota Ziesemer Jr. on 3/19/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "placer.h"
Placer::Placer(){
	nrSites=0;
	utilization=100;
};

int Placer::getHSize(){
	return nrSites+currentCircuit->getLMargin()+currentCircuit->getRMargin();
}

int Placer::getVSize(){
	return rows.size()*currentCircuit->getRowHeight()+currentCircuit->getBMargin()+currentCircuit->getTMargin();
}

float Placer::getUtilization(){
	return utilization;
}

CLayout* Placer::getLayoutFromInstance(string instanceName){
	Instance *currentInstance=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstance(instanceName);
	if(currentInstance){
		CLayout *currentLayout=currentCircuit->getLayout(currentInstance->getTargetCell());
		if(currentLayout){
			return currentLayout; 
		}else cout << "Instance " << instanceName << " has no cell layout " << currentInstance->getTargetCell() << endl;
	}else cout << "Instance " << instanceName << " not found" << endl;
	return false;
}

bool Placer::incrementalPlacement(Router* rt, string lpSolverFile){
	int hGrid=currentCircuit->getHPitch()*currentCircuit->getRules()->getScale();
	compaction cpt(CP_LP);
	cpt.setLPFilename("temp");	
	Instance *currentInstance;
	CLayout *currentLayout;
	cpt.insertConstraint("ZERO", "XMARGIN", CP_EQ, currentCircuit->getLMargin());
	cpt.insertConstraint("XMARGIN", "WIDTH", CP_EQ, nrSites);
	int lastX, cellPosX, cellPosY, cellPosXend, cellPosYend, conflicts, currentRow=0, nrConflicts=0;
	vector<list<string> >::iterator rows_it;
	list<string>::iterator nodes_it;
	for(rows_it=rows.begin();rows_it!=rows.end();rows_it++){
		string lastNode="XMARGIN";
		lastX=0;
		cellPosY=currentRow*currentCircuit->getRowHeight();
		cellPosYend=cellPosY+currentCircuit->getRowHeight();
		for(nodes_it=rows_it->begin();nodes_it!=rows_it->end();nodes_it++){
			currentInstance=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstance(*nodes_it);
			currentLayout=currentCircuit->getLayout(currentInstance->getTargetCell());
			conflicts=0;
			if(currentInstance->getMY()){
				cellPosXend=currentInstance->getX()/hGrid;
				cellPosX=cellPosXend-currentLayout->getWidth()/hGrid;
				for(int z=0; z<3; z++)
					for(int y=cellPosY; y<cellPosYend; y++)
						for(int x=lastX; x<cellPosXend; x++)
							if(rt->getNrNets(x+currentCircuit->getLMargin(),y+currentCircuit->getBMargin(),z)>1) conflicts++;
				cellConflicts[*nodes_it]+=conflicts;
				if(conflicts) cellSpace[*nodes_it]++;
				if(!cellSpace[*nodes_it])
					cpt.insertConstraint( lastNode, "C_"+ *nodes_it+"END", CP_MIN, "ZERO");
				else{
					cpt.insertConstraint( lastNode, "C_"+ *nodes_it+"END", CP_EQ, "C_"+ *nodes_it+"DIST");
					cpt.insertConstraint( "ZERO", "C_"+ *nodes_it+"DIST", CP_MAX, cellSpace[*nodes_it]);
					cpt.insertLPMinVar("C_"+ *nodes_it+"DIST",-cellConflicts[*nodes_it]);
				}
				cpt.insertConstraint("C_"+ *nodes_it+"END", *nodes_it, CP_EQ, currentLayout->getWidth()/hGrid);
				multimap<string,Pin>::iterator pins_it;
				for(pins_it=currentLayout->getPins().begin(); pins_it!=currentLayout->getPins().end(); pins_it++)
					if(pins_it->first!="bl" && pins_it->first!=currentCircuit->getGndNet() && pins_it->first!=currentCircuit->getVddNet())
						cpt.insertConstraint( "C_"+ *nodes_it+"P_"+pins_it->first, *nodes_it, CP_EQ, (pins_it->second.getX()+hGrid/2)/hGrid);
				lastNode=*nodes_it;
			}else{
				cellPosX=currentInstance->getX()/hGrid;
				cellPosXend=cellPosX+currentLayout->getWidth()/hGrid;
				for(int z=0; z<3; z++)
					for(int y=cellPosY; y<cellPosYend; y++)
						for(int x=lastX; x<cellPosXend; x++)
							if(rt->getNrNets(x+currentCircuit->getLMargin(),y+currentCircuit->getBMargin(),z)>1) conflicts++;
				cellConflicts[*nodes_it]+=conflicts;
				if(conflicts) cellSpace[*nodes_it]++;
				if(!cellSpace[*nodes_it])
					cpt.insertConstraint( lastNode, *nodes_it, CP_MIN, "ZERO");
				else{
					cpt.insertConstraint( lastNode, *nodes_it, CP_EQ, "C_"+ *nodes_it+"_DIST");
					cpt.insertConstraint( "ZERO", "C_"+ *nodes_it+"_DIST", CP_MAX, cellSpace[*nodes_it]);
					cpt.insertLPMinVar("C_"+ *nodes_it+"_DIST",-cellConflicts[*nodes_it]);
				}
				cpt.insertConstraint( *nodes_it, "C_"+ *nodes_it+"_END", CP_EQ, getLayoutFromInstance(*nodes_it)->getWidth()/hGrid);
				for(multimap<string,Pin>::iterator pins_it=currentLayout->getPins().begin(); pins_it!=currentLayout->getPins().end(); pins_it++)
					if(pins_it->first!="bl" && pins_it->first!=currentCircuit->getGndNet() && pins_it->first!=currentCircuit->getVddNet())
						cpt.insertConstraint( *nodes_it, "C_"+ *nodes_it+"_P_"+pins_it->first, CP_EQ, (pins_it->second.getX()-hGrid/2)/hGrid);
				lastNode="C_"+ *nodes_it+"_END";
			}
			lastX=cellPosXend;
			nrConflicts+=conflicts;
		}
		currentRow++;
		cpt.insertConstraint( "ZERO", lastNode, CP_MAX, "WIDTH");
	}
	cout << "# Conflicts = " << nrConflicts << endl;
	vector<t_net>& nets=currentCircuit->getCellNetlst(currentCircuit->getTopCell())->getNets();
	Interface *currentInterface;
	for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		if(nets_it->name!=currentCircuit->getVddNet() && nets_it->name!=currentCircuit->getGndNet()){
			currentInterface=currentCircuit->getInterface(nets_it->name);
			if(currentInterface){
				cpt.insertUpperBound("N_"+ nets_it->name+"_MIN", currentInterface->pos.getX()+currentCircuit->getLMargin());
				cpt.insertLowerBound("N_"+ nets_it->name+"_MAX", currentInterface->pos.getX()+currentCircuit->getLMargin());
			}
			for(list<t_inst>::iterator pins_it=nets_it->insts.begin(); pins_it!=nets_it->insts.end(); pins_it++){
				cpt.insertConstraint("N_"+ nets_it->name+"_MIN", "C_" + pins_it->targetCellInst +"_P_"+ currentCircuit->getCellNetlst(currentCircuit->getCellNetlst(currentCircuit->getTopCell())->getInstances()[pins_it->targetCellInst].subCircuit)->getInout(pins_it->targetPin), CP_MIN, 0);
				cpt.insertConstraint("C_"+ pins_it->targetCellInst+"_P_" + currentCircuit->getCellNetlst(currentCircuit->getCellNetlst(currentCircuit->getTopCell())->getInstances()[pins_it->targetCellInst].subCircuit)->getInout(pins_it->targetPin), "N_" + nets_it->name + "_MAX",CP_MIN, 0);
			}
			cpt.insertConstraint("N_"+ nets_it->name+"_MIN", "N_"+ nets_it->name+"_MAX",CP_EQ, "N_"+ nets_it->name+"_COMPACT");
			cpt.insertLPMinVar("N_"+ nets_it->name+"_COMPACT");
		}
	}
	cpt.solve(lpSolverFile);
	
	map<string, Instance> *cellsInsts=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances();
	for(map<string, Instance>::iterator cellsInst_it=cellsInsts->begin(); cellsInst_it!=cellsInsts->end(); cellsInst_it++)
		cellsInst_it->second.setX((cpt.getVariableVal(cellsInst_it->first)-currentCircuit->getLMargin())*hGrid);
	return true;
}

bool Placer::checkPathWL(){
	Instance *currentInstance;
	CLayout *currentLayout;
	CellNetlst *currentCell;
	multimap<string,Pin>::iterator pin_it;
	Interface *currentInterface;
	int minX, maxX, minY, maxY, x,y, wl=0;
	vector<t_net>& nets=currentCircuit->getCellNetlst(currentCircuit->getTopCell())->getNets();
	for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		if(nets_it->name!=currentCircuit->getVddNet() && nets_it->name!=currentCircuit->getGndNet()){
			for(list<t_inst>::iterator nodes_it=nets_it->insts.begin(); nodes_it!=nets_it->insts.end(); nodes_it++){
				currentInstance=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstance(nodes_it->targetCellInst);
				if(currentInstance){
					currentLayout=currentCircuit->getLayout(currentInstance->getTargetCell());
					if(currentLayout){
						currentCell=currentCircuit->getCellNetlst(currentInstance->getTargetCell());
						if(currentCell){
							pin_it=currentLayout->getPins().find(currentCell->getInout(nodes_it->targetPin));
							if(pin_it!=currentLayout->getPins().end()){
								x=currentInstance->getMY() ? -pin_it->second.getX() : pin_it->second.getX();
								y=currentInstance->getMX() ? -pin_it->second.getY() : pin_it->second.getY();
								x=(currentInstance->getX()+x)/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale());
								y=(currentInstance->getY()+y)/(currentCircuit->getVPitch()*currentCircuit->getRules()->getScale());
								if(nodes_it==nets_it->insts.begin()){
									minX=x;  maxX=x; minY=y; maxY=y;
								}else{
									minX=min(minX,x);  maxX=max(maxX,x); minY=min(minY,y); maxY=max(maxY,y);
								}						
							}else{
								cout << "Pin Nr" << nodes_it->targetPin << " in cell " << currentInstance->getTargetCell() << " not found" << endl;
								return false;
							}
						}else{
							cout << "Cell netlist " << currentInstance->getTargetCell() << " in net " << nets_it->name << " not found" << endl;
							return false;
						}
					}else{
						cout << "Cell layout " << currentInstance->getTargetCell() << " not found" << endl;
						return false;
					}			
				}else return false;
			}
			currentInterface=currentCircuit->getInterface(nets_it->name);
			if(currentInterface){
				minX=min(minX,currentInterface->pos.getX());  maxX=max(maxX,currentInterface->pos.getX()); minY=min(minY,currentInterface->pos.getY()); maxY=max(maxY,currentInterface->pos.getY());
			}
			wl+=(maxX-minX)+(maxY-minY);
		}
	}
	cout << "Semi-perimeter wire length = " << wl << endl;
	return true;
}

bool Placer::checkWL(){
	Instance *currentInstance;
	CLayout *currentLayout;
	CellNetlst *currentCell;
	multimap<string,Pin>::iterator pin_it;
	Interface *currentInterface;
	int minX, maxX, minY, maxY, x,y, wl=0;
	ofstream file;
	file.open("/home/gposser/Desktop/posplace.txt");
	vector<t_net>& nets=currentCircuit->getCellNetlst(currentCircuit->getTopCell())->getNets();
	for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		if(nets_it->name!=currentCircuit->getVddNet() && nets_it->name!=currentCircuit->getGndNet()){
			for(list<t_inst>::iterator nodes_it=nets_it->insts.begin(); nodes_it!=nets_it->insts.end(); nodes_it++){
				currentInstance=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstance(nodes_it->targetCellInst);
				if(currentInstance){
				    file << "Instance: " << currentInstance->getTargetCell() << endl;
					currentLayout=currentCircuit->getLayout(currentInstance->getTargetCell());
					if(currentLayout){
					    file << "Passou em Layout" << endl;
						currentCell=currentCircuit->getCellNetlst(currentInstance->getTargetCell());
						if(currentCell){
							file << "Passou em CurrentCell" << endl;
							pin_it=currentLayout->getPins().find(currentCell->getInout(nodes_it->targetPin));
							file << "Passou onde pega o pin_it" << endl;
							if(pin_it!=currentLayout->getPins().end()){
								file << "Passou em pinos" << endl;
								x=currentInstance->getMY() ? -pin_it->second.getX() : pin_it->second.getX();
								y=currentInstance->getMX() ? -pin_it->second.getY() : pin_it->second.getY();
								x=(currentInstance->getX()+x)/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale());
								y=(currentInstance->getY()+y)/(currentCircuit->getVPitch()*currentCircuit->getRules()->getScale());
								if(nodes_it==nets_it->insts.begin()){
									minX=x;  maxX=x; minY=y; maxY=y;
								}else{
									minX=min(minX,x);  maxX=max(maxX,x); minY=min(minY,y); maxY=max(maxY,y);
								}
							}else{
								file << "foi para o else" << endl;
								cout << "Pin Nr" << nodes_it->targetPin << " in cell " << currentInstance->getTargetCell() << " not found" << endl;
								return false;
							}
						}else{
							cout << "Cell netlist " << currentInstance->getTargetCell() << " in net " << nets_it->name << " not found" << endl;
							return false;
						}
					}else{
						cout << "Cell layout " << currentInstance->getTargetCell() << " not found" << endl;
						return false;
					}
				}else return false;
			}
			currentInterface=currentCircuit->getInterface(nets_it->name);
			if(currentInterface){
				minX=min(minX,currentInterface->pos.getX());  maxX=max(maxX,currentInterface->pos.getX()); minY=min(minY,currentInterface->pos.getY()); maxY=max(maxY,currentInterface->pos.getY());
			}
			wl+=(maxX-minX)+(maxY-minY);
			file << "aaaaaaaa" << endl;
			file << "\t" << (maxX-minX)+(maxY-minY) << endl;
		}
	}
	//cout << "Semi-perimeter wire length = " << wl << endl;
	return true;
}

bool Placer::autoFlip(){
	Instance *currentInstance;
	CLayout *currentLayout;	
	CellNetlst *currentCell;
	multimap<string,Pin>::iterator pin_it;
	int minX, maxX, minY, maxY, x,y, xMY;
	map<string, int> netMinX, netMaxX, instanceMY;
	vector<t_net>& nets=currentCircuit->getCellNetlst(currentCircuit->getTopCell())->getNets();
	for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		if(nets_it->name!=currentCircuit->getVddNet() && nets_it->name!=currentCircuit->getGndNet()){
			for(list<t_inst>::iterator nodes_it=nets_it->insts.begin(); nodes_it!=nets_it->insts.end(); nodes_it++){
				currentInstance=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstance(nodes_it->targetCellInst);
				if(currentInstance){
					currentLayout=currentCircuit->getLayout(currentInstance->getTargetCell());
					if(currentLayout){
						currentCell=currentCircuit->getCellNetlst(currentInstance->getTargetCell());
						if(currentCell){
							pin_it=currentLayout->getPins().find(currentCell->getInout(nodes_it->targetPin));
							if(pin_it!=currentLayout->getPins().end()){
								x=currentInstance->getMY() ? -pin_it->second.getX() : pin_it->second.getX();
								y=currentInstance->getMX() ? -pin_it->second.getY() : pin_it->second.getY();
								x=(currentInstance->getX()+x)/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale());
								y=(currentInstance->getY()+y)/(currentCircuit->getVPitch()*currentCircuit->getRules()->getScale());
								if(nodes_it==nets_it->insts.begin()){
									minX=x;  maxX=x; minY=y; maxY=y;
								}else{
									minX=min(minX,x);  maxX=max(maxX,x); minY=min(minY,y); maxY=max(maxY,y);
								}
							}else return false;
						}else return false;
					}else return false;			
				}else return false;
				instanceMY[nodes_it->targetCellInst]=0;
			}
			netMinX[nets_it->name]=minX;
			netMaxX[nets_it->name]=maxX;
		}
	}
	for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		if(nets_it->name!=currentCircuit->getVddNet() && nets_it->name!=currentCircuit->getGndNet()){
			for(list<t_inst>::iterator nodes_it=nets_it->insts.begin(); nodes_it!=nets_it->insts.end(); nodes_it++){
				currentInstance=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstance(nodes_it->targetCellInst);
				currentLayout=currentCircuit->getLayout(currentInstance->getTargetCell());
				currentCell=currentCircuit->getCellNetlst(currentInstance->getTargetCell());
				pin_it=currentLayout->getPins().find(currentCell->getInout(nodes_it->targetPin));
				x=currentInstance->getMY() ? -pin_it->second.getX() : pin_it->second.getX();
				x=(currentInstance->getX()+x)/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale());
				xMY=(currentInstance->getX()-x)/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale());
				
				if(x==netMinX[nets_it->name] && xMY>x)   //razoes inverter
					instanceMY[nodes_it->targetCellInst]+=xMY-x;
				else if(x==netMaxX[nets_it->name] && xMY<x)
					instanceMY[nodes_it->targetCellInst]+=x-xMY;
				
				if(xMY<netMinX[nets_it->name])			   //quanto perde invertendo
					instanceMY[nodes_it->targetCellInst]-=netMinX[nets_it->name]-xMY;
				else if(xMY>netMaxX[nets_it->name])
					instanceMY[nodes_it->targetCellInst]-=xMY-netMaxX[nets_it->name];
			}
		}
	}
	map<string, Instance>::iterator cellsInst_it;
	for(cellsInst_it=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->begin();cellsInst_it!=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->end();cellsInst_it++){
		if(instanceMY[cellsInst_it->first]>0){
			if(cellsInst_it->second.getMY()){
				cellsInst_it->second.setX(cellsInst_it->second.getX()-currentCircuit->getLayout(cellsInst_it->second.getTargetCell())->getWidth());
				cellsInst_it->second.setMY(false);
			}else{
				cellsInst_it->second.setX(cellsInst_it->second.getX()+currentCircuit->getLayout(cellsInst_it->second.getTargetCell())->getWidth());
				cellsInst_it->second.setMY(true);
			}
		}
	}
	return true;
}


bool Placer::checkPlacement(){
	int x,y;
	bool ok=true;
	int errorMsgs=0;
	int cellsHeight=currentCircuit->getRowHeight()*currentCircuit->getVPitch()*currentCircuit->getRules()->getScale();
	CLayout *l, *currentLayout;	
	list<string>::iterator slots_it;
	int posXi=0, posXf=0, width=0, rowSize;
	int nrRows=rows.size();
	rows.clear();
	rows.resize(nrRows);
	Instance *currentInstance;
	map<string, Instance>::iterator cellsInst_it;
	for(cellsInst_it=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->begin();cellsInst_it!=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->end();cellsInst_it++){
		l=currentCircuit->getLayout(cellsInst_it->second.getTargetCell());
		if(l){
			x=(cellsInst_it->second.getX()-int(cellsInst_it->second.getMY())*l->getWidth())/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale());
			y=(cellsInst_it->second.getY()/(currentCircuit->getVPitch()*currentCircuit->getRules()->getScale()));
			if(y%currentCircuit->getRowHeight()){
				y=floor(float(cellsInst_it->second.getY())/cellsHeight);
				cellsInst_it->second.setY(y*cellsHeight);
				//				cout << "Shifting instance " << cellsInst_it->first << " to fit in row: " << y << endl;
				y*=currentCircuit->getRowHeight();
			}
			int rowNr=abs(y/currentCircuit->getRowHeight())-int(cellsInst_it->second.getMX());
			if(rowNr%2 && !cellsInst_it->second.getMX()){
				//				cout << "Mirroring instance: " << cellsInst_it->first << endl;
				cellsInst_it->second.setY(cellsInst_it->second.getY()+(currentCircuit->getRowHeight()*currentCircuit->getVPitch()*currentCircuit->getRules()->getScale()));
				cellsInst_it->second.setMX(true);
			}
			if(rowNr>=0 && x>=0){
				if(rowNr>=rows.size()) rows.resize(rowNr+1);
				width=max(width, int(x+l->getWidth()/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale())));
				for(slots_it=rows[rowNr].begin(); slots_it!=rows[rowNr].end(); slots_it++){
					currentInstance=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstance(*slots_it);
					if(currentInstance){
						currentLayout=currentCircuit->getLayout(currentInstance->getTargetCell());
						rowSize+=currentLayout->getWidth();
						if(currentLayout){
							posXi=(currentInstance->getX()/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale()))-(int(currentInstance->getMY())*currentLayout->getWidth());
							posXf=posXi+currentLayout->getWidth()/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale());
							if((posXi<x+l->getWidth()/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale())) && (posXf>x)){
								if(errorMsgs++<6) cout << "Instance " << cellsInst_it->first << " in row " << rowNr << " is overlaping cell " << *slots_it << ". Run INCREMENTAL PLACEMENT afterward to correct this problem." << endl;
							}
						}else{
							if(errorMsgs++<6) cout << "Instance " << cellsInst_it->first << " has no cell layout" << *slots_it << endl;
							ok=false;
						}
						if(posXi>=x) break;
					}else{
						if(errorMsgs++<6) cout << "Instance " << cellsInst_it->first << " not found" << endl;
						ok=false;
					}
				}
				rows[rowNr].insert(slots_it,cellsInst_it->first);
			}else{
				if(errorMsgs++<6) cout << "Instance " << cellsInst_it->first << " out of placement area" << endl;
				ok=false;
			}
		}else{
			if(errorMsgs++<6) cout << "Instance " << cellsInst_it->first << " has no cell layout " << cellsInst_it->second.getTargetCell() << endl;
			ok=false;
		}
	}

	vector<list<string> >::iterator rows_it;
	list<string>::iterator nodes_it;
	for(rows_it=rows.begin();rows_it!=rows.end();rows_it++){
		rowSize=0;
		for(nodes_it=rows_it->begin();nodes_it!=rows_it->end();nodes_it++){
			currentInstance=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstance(*nodes_it);
			currentLayout=currentCircuit->getLayout(currentInstance->getTargetCell());
			if(currentInstance && currentLayout) rowSize+=currentLayout->getWidth();
		}
		width=max(width, int(rowSize/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale())));
	}

	if(errorMsgs>5) cout << "WARNING: A total of " << errorMsgs << " errors were found. Just the 5 firsts were shown." << endl;
	if(width > nrSites){
		cout << "Some cells could not be placed inside the placement area. Increasing number of sites from " << nrSites << " to " << width << ". Run PLACE TERMINALS again afterward." << endl;
		nrSites=width;
	}
	if(rows.size() > nrRows){
		cout << "Some cells could not be placed inside the placement area. Increasing number of rows from " << nrRows << " to " << rows.size() << ". Run PLACE TERMINALS again afterward." << endl;
		nrRows=rows.size();
	}
	
	return ok;
}

bool Placer::setup(){
	CellNetlst* cell=currentCircuit->getCellNetlst(currentCircuit->getTopCell());
	if(!cell){
		cout << "ERROR: Could not initialize placement. Top cell " << currentCircuit->getTopCell() << " not found" << endl;
		return false;
	}
	
	CLayout top(currentCircuit->getTopCell());
	currentCircuit->insertLayout(top);
	
	CLayout placement(currentCircuit->getTopCell() + "_PL");
	currentCircuit->insertLayout(placement);
	
	currentCircuit->getLayout(currentCircuit->getTopCell())->addInstance("PLACEMENT", currentCircuit->getTopCell() + "_PL");
	currentCircuit->getLayout(currentCircuit->getTopCell())->placeCell("PLACEMENT", 0, 0, false, false);

	//		for(int c=0; c<cell->getInouts().size(); ++c)
	//			insertInterface(cell->getNetName(cell->getInouts()[c]), N, cell->getIOType(c), 0, 0);
	for(map<string,Inst>::iterator inst_it=cell->getInstances().begin(); inst_it!=cell->getInstances().end(); ++inst_it){
		CLayout *l=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL");
		if(l) l->addInstance(inst_it->first, inst_it->second.subCircuit);
		else {
			cout << "ERROR: Layout for cell " << inst_it->second.subCircuit << "not found" << endl;
			return false;			
		}
	}

	return true;
}

bool Placer::setArea(int n, float u){
	if(!setup()) return false;

	if(n<1) return false;
	rows.clear();
	rows.resize(n);
	int tmp=0;
	
	map<string, Instance>::iterator cellsInst_it;
	for(cellsInst_it=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->begin();cellsInst_it!=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->end();cellsInst_it++){
		if(currentCircuit->getLayout(cellsInst_it->second.getTargetCell()))
			tmp+= currentCircuit->getLayout(cellsInst_it->second.getTargetCell())->getWidth()/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale());
		else {
			cout << "Error: Cell layout not found:" << cellsInst_it->first << endl;
			return false;
		}

	}
	nrSites = ceil((tmp/(u/100.0f))/rows.size());

	if(nrSites){
		utilization= 100*(float(tmp)/(nrSites*rows.size()));
		cout << "Setting place area to " << rows.size()*currentCircuit->getRowHeight()*currentCircuit->getVPitch() << " x " <<  nrSites*currentCircuit->getHPitch() << " (HxW)um  with " << getUtilization() << "% of utilization" << endl;
		return true;
	}else{
		cout << "No cell instance found in the circuit" << endl;
		return false;
	}
}
	
bool Placer::readMangoParrotPlacement(string fileName) {
	ifstream file(fileName.c_str()); // Read
	if ((!file)){
		cout << "Placement file " << fileName << " could not be opened" << endl;
		return(false);
	}
	setup();
	rows.clear();
	//Parse the file
	int fileline=0, actualRow=0, currentPos, largestRow=0;
	string str_tmp,cellName;
	int nrCells;
	while(getline(file,str_tmp)) {
		fileline++;
		if(!str_tmp.size()) continue;
		istrstream line(str_tmp.c_str());
		line >> cellName;
		if(upcase(cellName)=="BANDAS"){
			line >> nrCells;
			rows.resize(nrCells);
		}else{
			currentPos=0;
			while(line >> cellName){
				CLayout *l=getLayoutFromInstance(cellName);
				if(l){
					currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->placeCell( cellName, currentPos, actualRow*currentCircuit->getRowHeight()*currentCircuit->getVPitch()*currentCircuit->getRules()->getScale(), false, false);
					currentPos+=l->getWidth();
				} else return false;
			}
			largestRow=max(largestRow,currentPos);
			actualRow++;
		}
	}
	nrSites=largestRow/(currentCircuit->getHPitch()*currentCircuit->getRules()->getScale());
	return checkPlacement();
}

bool Placer::readBookshelfPlacement(string fileName) {
	int vGrid=currentCircuit->getVPitch()*currentCircuit->getRules()->getScale();
	int hGrid=currentCircuit->getHPitch()*currentCircuit->getRules()->getScale();
	
	ifstream file(fileName.c_str()); // Read
	if ((!file)){
		cout << "Placement file " << fileName << " could not be opened" << endl;
		return(false);
	}
	
	//Parse the file
	int fileline=0;
	string str_tmp,cellName, orientation;
	int posX, posY, offsetx;
	char lixo;
	bool mx, my, left=true;
	
	if(!setup()) return false;
	
	while(getline(file,str_tmp)) {
		fileline++;
		if(!str_tmp.size()) continue;
		istrstream line(str_tmp.c_str());
		line >> cellName;
		if(str_tmp=="# Generated automatically by Z-Place")  //MELHORAR!!!! feito para diferenciar arquivos salvos em .pl no mobralplace e no ICPD, PADRONIZAR URGENTE!!!
			left=false;
		else if(cellName.size() && cellName[0]!='#' && cellName!="UCLA"){
			line >> posX;
			line >> posY;
			line >> lixo;
			line >> orientation;
			Instance *currentInstance=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstance(cellName);
			if(currentInstance && currentCircuit->getLayout(currentInstance->getTargetCell())){
				const int w = currentCircuit->getLayout(currentInstance->getTargetCell())->getWidth();
				//				const int h = currentCircuit->getLayout(currentInstance->getTargetCell())->getHeight();
				
				mx=(orientation=="S" || orientation=="FS");
				my=(orientation=="FS" || orientation=="FN");
				
				//				int offsety = mx ? h : 0;
				//				int offsetx = my ? w : 0;
				
				if(!left) offsetx = ( w / hGrid ) % 2 ? (hGrid/2)-(w/2) : -(w/2);
				else offsetx=0;
				

				int x = offsetx + (posX*hGrid);
				int y = posY*vGrid;				
				currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->placeCell( cellName, x, y, mx, my);
			}else if(currentCircuit->getInterface(cellName)){
				currentCircuit->getInterface(cellName)->pos.setPos(posX, posY);
			} // else
			//	cout << "Instance " << cellName << " not found" << endl;
		}
	}
	if(left) cout << "Mobral placement format with the positions indicating the left side of the cells"<< endl;
	else cout << "Mobral placement format with the positions indicating the center of the cells."<< endl;
	return checkPlacement();
}

bool Placer::writeBookshelfFiles(string fileName, bool dotPLOnly){
	int vGrid=currentCircuit->getVPitch()*currentCircuit->getRules()->getScale();
	int hGrid=currentCircuit->getHPitch()*currentCircuit->getRules()->getScale();
	map<string, Instance>::iterator cellsInst_it;
	map<string, Interface>::iterator interface_it;
	string fname;

	fname = fileName + ".pl";
	ofstream fpl(fname.c_str());
	fpl << "UCLA pl 1.0\n";
	fpl << "# Automatically generated by Jungle Parrot software.\n";
	fpl << "#     by Adriel Mota Ziesemer Junior (amziesemej[at]inf.ufrgs.br) - UFRGS, Brazil.\n\n";
	
	for(interface_it=currentCircuit->getInterfaces()->begin(); interface_it!=currentCircuit->getInterfaces()->end(); interface_it++)
		fpl << interface_it->first << "\t" << interface_it->second.pos.getX() << "\t" << interface_it->second.pos.getY() << " : BLAH" << endl;
	
	for(cellsInst_it=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->begin();cellsInst_it!=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->end();cellsInst_it++){
		fpl << cellsInst_it->first << "\t" << cellsInst_it->second.getX()/hGrid << "\t" << cellsInst_it->second.getY()/vGrid << " : ";
		if(cellsInst_it->second.getMX())
			if(cellsInst_it->second.getMY()) fpl << "FS";
			else fpl << "S";
			else
				if(cellsInst_it->second.getMY()) fpl << "FN";
				else fpl << "N";
		fpl << endl;
	}

	if(dotPLOnly) return true;
	
	fname= fileName + ".nodes";
	ofstream fnodes(fname.c_str());
	fnodes << "UCLA nodes 1.0\n";
	fnodes << "# Automatically generated by Jungle Parrot software.\n";
	fnodes << "#     by Adriel Mota Ziesemer Junior (amziesemej[at]inf.ufrgs.br) - UFRGS, Brazil.\n\n";
	fnodes << "NumNodes : " << currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->size()+currentCircuit->getInterfaces()->size() << "\n";
	fnodes << "NumTerminals : " << currentCircuit->getInterfaces()->size() << endl;
	for(cellsInst_it=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->begin();cellsInst_it!=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->end();cellsInst_it++)
		if (currentCircuit->getLayout(cellsInst_it->second.getTargetCell()))
			fnodes << cellsInst_it->first << "\t" << currentCircuit->getLayout(cellsInst_it->second.getTargetCell())->getWidth()/hGrid << "\t" << currentCircuit->getLayout(cellsInst_it->second.getTargetCell())->getHeight()/vGrid << "\n";
		else{
			cout << "ERROR: Cell not found: " << cellsInst_it->second.getTargetCell() << endl;
			return false;
		}
	
	for(interface_it=currentCircuit->getInterfaces()->begin(); interface_it!=currentCircuit->getInterfaces()->end(); interface_it++)
		fnodes << interface_it->first << "\t1\t1\tterminal" << endl;
	
	
	fname = fileName + ".nets";
	ofstream fnets(fname.c_str());
	fnets << "UCLA nets 1.0\n";
	fnets << "# Automatically generated by Jungle Parrot software.\n";
	fnets << "#     by Adriel Mota Ziesemer Junior (amziesemej[at]inf.ufrgs.br) - UFRGS, Brazil.\n\n";
	int num_pins=0, num_nets=0;
	vector<t_net>& nets=currentCircuit->getCellNetlst(currentCircuit->getTopCell())->getNets();
	for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		if(nets_it->name!=currentCircuit->getVddNet() && nets_it->name!=currentCircuit->getGndNet()){
			num_pins += nets_it->insts.size();
			if(nets_it->insts.size()!=0) num_nets++;  //fix for nets with size == 0 
		}
	}

	fnets << "NumNets : " << num_nets << "\n";
	fnets << "NumPins : " << num_pins << "\n";
	for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		if(nets_it->name!=currentCircuit->getVddNet() && nets_it->name!=currentCircuit->getGndNet()){
			if(currentCircuit->getInterfaces()->find(nets_it->name)!=currentCircuit->getInterfaces()->end()){
				fnets << "NetDegree : " << nets_it->insts.size()+1 << "\n";
				fnets << "    " << nets_it->name << "  B\n";
			}else if(nets_it->insts.size()!=0){ //fix for nets with size == 0 
				fnets << "NetDegree : " << nets_it->insts.size() << "\n";
				// fnets << "#     " << nets_it->name << "  B\n";
			}
			for(list<t_inst>::iterator nodes_it=nets_it->insts.begin(); nodes_it!=nets_it->insts.end(); nodes_it++)
				fnets << "    " << nodes_it->targetCellInst << "  B\n";
		}
	}
	
	fname = fileName + ".scl";
	ofstream fscl(fname.c_str());
	fscl << "UCLA scl 1.0\n";
	fscl << "# Automatically generated by Jungle Parrot software.\n";
	fscl << "#     by Adriel Mota Ziesemer Junior (amziesemej[at]inf.ufrgs.br) - UFRGS, Brazil.\n\n";
	fscl << "NumRows : " << rows.size() << "\n";
	for (int r=0; r<rows.size(); r++)
	{
		fscl << "CoreRow Horizontal\n";
		fscl << " Coordinate : " << currentCircuit->getRowHeight()*r << "\n";
		fscl << " Height : " << currentCircuit->getRowHeight() << "\n";
		fscl << " Sitewidth : 1\n";
		fscl << " Sitespacing : 1\n";
		fscl << " Siteorient : N\n";
		fscl << " Sitesymmetry : Y\n";
		fscl << " SubrowOrigin : 0 Numsites : " << nrSites << "\n";
		fscl << "End\n";
	}
	
	
	fname = fileName + ".wts";
	ofstream fwts(fname.c_str());
	fwts << "UCLA wts 1.0\n";
	fwts << "# Automatically generated by Jungle Parrot software.\n";
	fwts << "#     by Adriel Mota Ziesemer Junior (amziesemej[at]inf.ufrgs.br) - UFRGS, Brazil.\n\n";
	
	for(interface_it=currentCircuit->getInterfaces()->begin(); interface_it!=currentCircuit->getInterfaces()->end(); interface_it++)
		fwts << interface_it->first << "\t0" << endl;
	
	cellsInst_it=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->begin();
	while(cellsInst_it!=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->end()){
		fwts << cellsInst_it->first << " 1\n";
		cellsInst_it++;
	}
	
	fname = fileName + ".aux";
	ofstream faux(fname.c_str());
	faux << "RowBasedPlacement  : " << fileName << ".nodes " << fileName << ".nets " << fileName << ".wts " << fileName << ".pl " << fileName << ".scl\n";	
	return true;
}

bool Placer::writeCadende(string fileName){
	ofstream fcadence(fileName.c_str());
	
	printHeader(fcadence, "# ", "");

	fcadence << "version 5" << endl;
	fcadence << "fileType stdCellPlacement" << endl;
	fcadence << "designName TOP" << endl;
	fcadence << "dbunit 1000" << endl;
	fcadence << "symbolTable" << endl;

	map<string, Instance>::iterator cellsInst_it;
	map<string, int> mapping;
	
	for(cellsInst_it=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->begin();cellsInst_it!=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->end();cellsInst_it++)
		if(mapping.find(cellsInst_it->second.getTargetCell())==mapping.end()) mapping[cellsInst_it->second.getTargetCell()]=mapping.size()+1;
	
	fcadence << "Signature: 0; Symbols: 1; Instances: "<< currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->size() << "; Cells: " << mapping.size() << endl << endl;
	

	for(cellsInst_it=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->begin();cellsInst_it!=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->end();cellsInst_it++)
		fcadence << "0 " << cellsInst_it->first << " " << mapping[cellsInst_it->second.getTargetCell()] << endl;

	for(map<string, int>::iterator mapping_it=mapping.begin(); mapping_it!=mapping.end(); mapping_it++)
		fcadence << mapping_it->second << " " << mapping_it->first << endl;
	
	fcadence << "instLoc" << endl;
	
	int instNr=0;
	for(cellsInst_it=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->begin();cellsInst_it!=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstances()->end();cellsInst_it++)
		fcadence << instNr << " ( " << (cellsInst_it->second.getX()/currentCircuit->getRules()->getScale())*1000 << " " << (cellsInst_it->second.getY()/currentCircuit->getRules()->getScale())*1000 << " 0 1 0 )" << endl;
		
	fcadence << ";" << endl;
	fcadence << "clockSynthesize 0" << endl;
	fcadence << "scanOptimize 0" << endl;
	return true;
}

bool Placer::placeInterfaceTerminals(){
	int nrN=0, nrS=0, nrW=0, nrE=0;
	map<string, Interface>::iterator interface_it;
	for(interface_it=currentCircuit->getInterfaces()->begin(); interface_it!=currentCircuit->getInterfaces()->end(); interface_it++){
		switch(interface_it->second.orient){
			case N: nrN++; break;
			case S: nrS++; break;
			case W: nrW++; break;
			case E: nrE++; break;
		}
	}
	//falta verificar se já posui posição atribuita
	int posN=0, posS=0, posW=0, posE=0;
	int height=(rows.size()*currentCircuit->getRowHeight())-1;
	for(interface_it=currentCircuit->getInterfaces()->begin(); interface_it!=currentCircuit->getInterfaces()->end(); interface_it++){
		switch(interface_it->second.orient){
			case N: interface_it->second.pos.setPos(((currentCircuit->getLMargin()+nrSites+currentCircuit->getRMargin())/nrN)*posN++ - currentCircuit->getLMargin(),height+currentCircuit->getTMargin()); break;
			case S: interface_it->second.pos.setPos(((currentCircuit->getLMargin()+nrSites+currentCircuit->getRMargin())/nrS)*posS++ - currentCircuit->getLMargin(), -currentCircuit->getBMargin()); break;
			case W: interface_it->second.pos.setPos(-currentCircuit->getLMargin(),((currentCircuit->getBMargin()+height+currentCircuit->getTMargin())/nrW)*posW++ - currentCircuit->getBMargin()); break;
			case E: interface_it->second.pos.setPos(nrSites+currentCircuit->getRMargin(),((currentCircuit->getBMargin()+height+currentCircuit->getTMargin())/nrE)*posE++ - currentCircuit->getBMargin()); break;
		}
	}
	return true;
}

bool Placer::optimizeInterfaceTerminals(){
	int nrN=0, nrS=0, nrW=0, nrE=0;
	map<string, Interface>::iterator interface_it;
	for(interface_it=currentCircuit->getInterfaces()->begin(); interface_it!=currentCircuit->getInterfaces()->end(); interface_it++){
		switch(interface_it->second.orient){
			case N: nrN++; break;
			case S: nrS++; break;
			case W: nrW++; break;
			case E: nrE++; break;
		}
	}
	//falta verificar se já posui posição atribuita
	int posN=0, posS=0, posW=0, posE=0;
	int height=(rows.size()*currentCircuit->getRowHeight())-1;
	for(interface_it=currentCircuit->getInterfaces()->begin(); interface_it!=currentCircuit->getInterfaces()->end(); interface_it++){
		switch(interface_it->second.orient){
			case N: interface_it->second.pos.setPos(((currentCircuit->getLMargin()+nrSites+currentCircuit->getRMargin())/nrN)*posN++ - currentCircuit->getLMargin(),height+currentCircuit->getTMargin()); break;
			case S: interface_it->second.pos.setPos(((currentCircuit->getLMargin()+nrSites+currentCircuit->getRMargin())/nrS)*posS++ - currentCircuit->getLMargin(), -currentCircuit->getBMargin()); break;
			case W: interface_it->second.pos.setPos(-currentCircuit->getLMargin(),((currentCircuit->getBMargin()+height+currentCircuit->getTMargin())/nrW)*posW++ - currentCircuit->getBMargin()); break;
			case E: interface_it->second.pos.setPos(nrSites+currentCircuit->getRMargin(),((currentCircuit->getBMargin()+height+currentCircuit->getTMargin())/nrE)*posE++ - currentCircuit->getBMargin()); break;
		}
	}
	return true;
}
