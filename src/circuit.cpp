/***************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                         *
 *   amziesemerj[at]inf.ufrgs.br                                            *
 ****************************************************************************/
#include <vector>
	using std::vector;
#include "circuit.h"

Circuit::Circuit(){
	hGrid=1.4f;
	vGrid=1.3f;
	supplyHSize=2.5f;
	supplyVSize=3.6f;
	vddNet="VCC";
	gndNet="GND";
	cellsHeight=10;
	c2lMargin=c2rMargin=c2tMargin=c2bMargin=0;
	topCell="";
	topNetlist =NULL;
	
	//parameters to PrintGP
	optimize = "area";
	sizingtype = "gate";
	technology ="350nm"; 
	Cload = 8.609274e-16; //  1.3273e-14 (350nm)
	constrArea= 1;
	constrDelay= 4.29113e-9;
	constrCin = 4; //15 (350nm)
	
}

bool Circuit::setMargins(float l, float r, float t, float b){
	c2lMargin=round(l/getHPitch());
	c2rMargin=round(r/getHPitch());
	c2tMargin=round(t/getVPitch());
	c2bMargin=round(b/getVPitch());
	cout << "Margins: L=" << c2lMargin*getHPitch() << "um  R=" << c2rMargin*getHPitch() << "um  T=" << c2tMargin*getVPitch() << "um  B=" << c2bMargin*getVPitch() << "um"  << endl;
	return true;
};

CellNetlst* Circuit::getCellNetlst(string n){
	map<string,CellNetlst>::iterator tmp=cellNetlsts.find(n);
	if(tmp!=cellNetlsts.end())
		return &tmp->second;
	else
		return 0;
}

string* Circuit::getEquation(string n){
	map<string,string>::iterator tmp=equations.find(n);
	if(tmp!=equations.end())
		return &tmp->second;
	else
		return 0;
}

Interface* Circuit::getInterface(string n){
	map<string,Interface>::iterator tmp=interfaces.find(n);
	if(tmp!=interfaces.end())
		return &tmp->second;
	else
		return 0;
}

bool Circuit::insertLayout(CLayout& tmp){
	map<string, CLayout>::iterator cells_it=layouts.find(tmp.getName());
	//	if(cells_it!=layouts.end())
	//		cout << "Cell layout " << tmp.getName() << " already exists. Overwriting..." << endl;
	layouts[tmp.getName()]=tmp;
	return true;
}

CLayout* Circuit::getLayout(string n){
	map<string,CLayout>::iterator tmp=layouts.find(n);
	if(tmp!=layouts.end())
		return &tmp->second;
	else
		return 0;
}

bool Circuit::insertCell(CellNetlst& tmp){
	map<string,CellNetlst>::iterator cells_it=cellNetlsts.find(tmp.getName());
	//	if(cells_it!=cellNetlsts.end())
	//		cout << "Cell netlist " << tmp.getName() << " already exists. Overwriting..." << endl;
	cellNetlsts[tmp.getName()]=tmp;
	return true;
}

bool Circuit::insertEquation(string cellName, string& tmp){
	map<string, string>::iterator cells_it=equations.find(cellName);
	//	if(cells_it!=equations.end())
	//		cout << "Cell equation " << cellName << " already exists. Overwriting..." << endl;
	equations[cellName]=tmp;
	return true;
}

bool Circuit::insertInterface(string name, direction orient, IOType type, int posx, int posy){
	Interface tmp;
	tmp.orient=orient;
	tmp.ioType=type;
	tmp.pos.setX(posx);
	tmp.pos.setY(posy);
	interfaces[name]=tmp;
	return true;
}

void clear(){
}
/*
bool Circuit::promoteCell2Top(string cellName){
	CLayout instances(currentCircuit->getTopCell() + "_PL");
	insertLayout(instances);
	
	CellNetlst* cell=getCellNetlst(cellName);
	if(cell){
		for(int c=0; c<cell->getInouts().size(); ++c)
			insertInterface(cell->getNetName(cell->getInouts()[c]), N, cell->getIOType(c), 0, 0);
		for(map<string,Inst>::iterator inst_it=cell->getInstances().begin(); inst_it!=cell->getInstances().end(); ++inst_it){
			list<string> inouts;
			for(int c=0; c<inst_it->second.ports.size(); ++c)
				inouts.push_back(cell->getNetName(inst_it->second.ports[c]));
			insertInstance(currentCircuit->getTopCell() + "_PL", inst_it->first, inst_it->second.subCircuit, inouts);
		}
		return true;
	}
	return false;
}
*/

/*
 void Circuit::printCells(){
 map<string,Cell>::iterator cells_it;
 for(cells_it=cells.begin(); cells_it!=cells.end(); cells_it++)
 cells_it->second.print();
 }
 */
 
bool Circuit::setTopCell(string t){
	int n;
	if(cellNetlsts.find(t)==cellNetlsts.end()){
		cout << "ERROR: Top not found: " << t << endl;
		return false;
	}else{
		topCell=t;
		topNetlist=getCellNetlst(topCell);
		map<string,Inst>& instances=cellNetlsts[t].getInstances();
		for(map<string,Inst>::iterator instance_it=instances.begin(); instance_it!=instances.end(); ++instance_it){
			if(instance_it->second.m!=1 && cellNetlsts.find(instance_it->second.subCircuit+"_"+intToStr(instance_it->second.m))==cellNetlsts.end()){
				//cout << "Removing resized cell from top hierarchy: " << instance_it->second.subCircuit << endl;
				CellNetlst newCell;
				newCell.setName(instance_it->second.subCircuit+"_"+intToStr(instance_it->second.m));
				map<string, CellNetlst>::iterator target=cellNetlsts.find(instance_it->second.subCircuit);
				vector<string> targetIOs;
				if(target==cellNetlsts.end()){
					cout << "ERROR: Resized cell not found: " << instance_it->second.subCircuit << endl;
					return false;
				}else{
					for(n=0;n<target->second.getInouts().size();++n){
						newCell.insertInOut(target->second.getInout(n));
						targetIOs.push_back(target->second.getInout(n));
					}
				}
				newCell.insertInstance(instance_it->first, instance_it->second.subCircuit, targetIOs, instance_it->second.m); // Insert a new cell instance
				instance_it->second.m=1;
				instance_it->second.subCircuit=newCell.getName();
				insertCell(newCell);
			}
		}		
	}
	return true;
}
 

CellNetlst Circuit::getFlattenCell(string name){
	CellNetlst tmp;
	int n;
	if(cellNetlsts.find(name)==cellNetlsts.end()){
		cout << "ERROR: Cell not found: " << name << endl;
	}else{
		tmp=cellNetlsts[name];
		map<string,Inst>& instances=tmp.getInstances();
		for(map<string,Inst>::iterator instance_it=instances.begin(); instance_it!=instances.end(); ++instance_it){
			if(instance_it==instances.begin()) cout << "Removing cell hierarchy in " << name << endl;
			CellNetlst currentCell=getFlattenCell(instance_it->second.subCircuit);
			currentCell.appendNetNameSuffix(instance_it->first);
			currentCell.multiply(instance_it->second.m);
			if(instance_it->second.ports.size()!=currentCell.getInouts().size()){
				cout << "ERROR: Call for cell " << instance_it->second.subCircuit << " have different number of IOs in subcircuit " << name << endl;
				return tmp;
			}
			for(n=0;n<instance_it->second.ports.size();++n)
				currentCell.getNet(currentCell.getInouts().at(n)).name=tmp.getNetName(instance_it->second.ports[n]);
			for(n=0;n<currentCell.size();++n){
				Trans& currentTrans=currentCell.getTrans(n);
				currentTrans.name=currentTrans.name+"_"+instance_it->first;
				tmp.insertTrans(currentTrans.name, currentCell.getNetName(currentTrans.drain), currentCell.getNetName(currentTrans.gate), currentCell.getNetName(currentTrans.source), currentTrans.type, currentTrans.length, currentTrans.width);
			}
		}
	}	
	return tmp;
}

bool Circuit::printInterface(string net){
	map<string, Interface>::iterator interface_it=interfaces.find(net);
	if(interface_it!=interfaces.end()){
		cout << "Interface: " <<  net << " => Orientation: ";
		switch(interface_it->second.orient){
			case N: cout << "N"; break;
			case S: cout << "S"; break;
			case W: cout << "W"; break;
			case E: cout << "E"; break;
		}
		cout << "; Type: ";
		switch(interface_it->second.ioType){
			case IOTYPE_INPUT: cout << "INPUT"; break;
			case IOTYPE_OUTPUT: cout << "OUTPUT"; break;
			case IOTYPE_INOUT: cout << "INOUT"; break;
		}
		cout << "; Position: (" << interface_it->second.pos.getX() << "," <<  interface_it->second.pos.getY() << ")";
		cout << endl;
		return true;
	}
	else
		return false;
}

bool Circuit::printInstance(CLayout* l, string instance){
	Instance *currentInstance = l->getInstance(instance);
	if(currentInstance){
		cout << "Instance: " <<  instance << " => " << currentInstance->getTargetCell() << endl;
		currentInstance->print();
		cout << "Pins Assignment (Global Net->Pin): ";
/*		map<string, list<Net> >::iterator netList_it;
		for(netList_it=netList.begin(); netList_it!=netList.end(); netList_it++)
			for(tmp_it=netList_it->second.begin(); tmp_it!=netList_it->second.end(); tmp_it++)
				if(tmp_it->targetCellInst==instance)
					cout << " (" << netList_it->first << "->" << tmp_it->targetPin << ") ";
		cout << endl;
*/		return true;
	}
	else cout << "Instance " << instance << " not found";
	return false;
}

bool Circuit::printNet(string net){
/*	map<string, list<Net> >::iterator netList_it=netList.find(net);
	list<Net>::iterator tmp_it;
	if(netList_it!=netList.end()){
		cout << "Net: " <<  netList_it->first << " -> ";
		for(tmp_it=netList_it->second.begin(); tmp_it!=netList_it->second.end(); tmp_it++)
			cout << "(" << tmp_it->targetCellInst << "," << tmp_it->targetPin << ") ";
		cout << endl;
		return true;
	}
	else
*/		return false;
}

bool Circuit::writeCellSizesFile(string filename){
	bool ok=true;
	ofstream file;
	file.open(filename.c_str()); // Write
	
	if ((!file)){
		cout << "ERROR:File <" << filename << "> could not be created" << endl;
		ok=false;
	}else{
		map<string,CellNetlst>::iterator cells_it;
		for(cells_it=cellNetlsts.begin(); cells_it!=cellNetlsts.end(); cells_it++){
			CLayout* l=getLayout(cells_it->first);
			if(l){
				file << cells_it->first << endl;
				file << l->getWidth()/currentRules->getIntValue(getHPitch()) << endl;
				file << l->getHeight()/currentRules->getIntValue(getVPitch()) << endl;
			}else{
				cout << "Cell Layout " << cells_it->first << " not found"<< endl;
				ok=false;
			}
		}
	}
	file.close();
	return ok;
}

/*
 bool Circuit::checkCells(){
 bool ok=true;
 map<string,Cell>::iterator cells_it=cells.begin();
 
 while(ok && cells_it!=cells.end()){
 ok && cells_it->second.check();
 cells_it++;
 }
 return ok;
 }
 */

void Circuit::calculateCellsPins(){
	int halfPinSize=currentRules->getRule(E1M1VI)+currentRules->getRule(W2VI)/2;
	int hGrid=currentRules->getIntValue(getHPitch()), vGrid=currentRules->getIntValue(getVPitch());
	for(map<string, CellNetlst>::iterator cellNetlsts_it=cellNetlsts.begin(); cellNetlsts_it!=cellNetlsts.end(); ++cellNetlsts_it){
		map<string, CLayout>::iterator layouts_it=layouts.find(cellNetlsts_it->first);
		if(layouts_it!=layouts.end()){
			layouts_it->second.getPins().clear();
			for(int x=hGrid/2; x<=layouts_it->second.getWidth(); x+=hGrid){
				for(int y=vGrid; y<=layouts_it->second.getHeight()-vGrid; y+=vGrid){
					string connectedNet="none";
					string aroundNet="none";
					bool fail=false;
					int finalX, finalY;
					for(map <layer_name , list<Box> >::iterator layers_it = layouts_it->second.layers.begin(); layers_it != layouts_it->second.layers.end(); layers_it++){
						if(layers_it->first==MET1){
							for(list <Box>::iterator layer_it = layers_it->second.begin(); layer_it != layers_it->second.end(); layer_it++){
								int dx=max(layer_it->getX1(),x-halfPinSize) - min(layer_it->getX2(),x+halfPinSize);
								int dy=max(layer_it->getY1(),y-halfPinSize) - min(layer_it->getY2(),y+halfPinSize);
								if(dx<=-(2*halfPinSize-1) && dy<=-(2*halfPinSize-1)){
									if(connectedNet=="none"){
										connectedNet=layer_it->getNet();
										finalX=x;
										finalY=y;
									}
									if(connectedNet!=layer_it->getNet() || (aroundNet!="none" && aroundNet!=connectedNet)) fail=true;									
								}else if((dx<0 && dy<0) || 
										 (dx<=0 && dy>=0 && dy<currentRules->getRule(S1M1M1)-1) ||
										 (dx>=0 && dy<=0 && dx<currentRules->getRule(S1M1M1)-1) ||
										 (dx>=0 && dy>=0 && sqrt(float(dx*dx+dy*dy))<currentRules->getRule(S1M1M1)-1)){
									if(aroundNet=="none") aroundNet=layer_it->getNet();
									if(aroundNet!=layer_it->getNet() || (connectedNet!="none" && aroundNet!=connectedNet)) fail=true;
								}								
							}
						}
					}
					Pin p;
					p.setPos(x,y);
					p.setLayer(MET1);
					if(!fail && connectedNet!="none" && connectedNet!=""){
						layouts_it->second.getPins().insert(make_pair(connectedNet,p));
						layouts_it->second.addLabel(connectedNet, Point(x, y));
					} else if(!(connectedNet=="none" && aroundNet=="none")){
						layouts_it->second.getPins().insert(make_pair("bl",p));
//						layouts_it->second.addLabel("bl", Point(x, y));
					}
				} 
			}
		}
	}		
}


// -----------------------------------------------------------------------------

bool Circuit::isPrimaryInput( const string &netname ) {
    for ( map<string, Interface>::iterator it = interfaces.begin(); it != interfaces.end(); it++ ) {
		if ( it->second.ioType == IOTYPE_INPUT && it->first == netname )
			return true;
    } // end for
	return false;
} // end method

// -----------------------------------------------------------------------------

bool Circuit::isPrimaryOutput( const string &netname ) {
    for ( map<string, Interface>::iterator it = interfaces.begin(); it != interfaces.end(); it++ ) {
		if ( it->second.ioType == IOTYPE_OUTPUT && it->first == netname )
			return true;
    } // end for
	return false;
} // end method

// -----------------------------------------------------------------------------

bool Circuit::isPrimaryInput( const Inst &inst ) {
	const vector<int> &ports = inst.ports;
	for ( int i =0; i < ports.size(); i++ ) {
		if ( isPrimaryInput( getTopNetlist()->getNetName(ports[i]) ) )
			return true;
	} // end for   
	
	return false;
} // end method

// -----------------------------------------------------------------------------

bool Circuit::isPrimaryOutput( const Inst &inst ) {
	const vector<int> &ports = inst.ports;
	for ( int i =0; i < ports.size(); i++ ) {
		if ( isPrimaryOutput( getTopNetlist()->getNetName(ports[i]) ) )
			return true;
	} // end for   
	
	return false;
} // end method
