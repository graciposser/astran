/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   amziesemerj[at]inf.ufrgs.br                                           *
 ***************************************************************************/
#include "autocell.h"

AutoCell::AutoCell(){
	currentCell=NULL;
	rt=NULL;	
	state=0;
	nrTracks=2;
}

AutoCell::~AutoCell() {
	clear();
}

void AutoCell::clear(){
	elements.clear();
	inoutPins.clear();
	if(rt!=NULL) delete(rt);
}

Element* AutoCell::createElement(int vcost){
	Element tmp;
	tmp.ativeCntP=rt->createNode();
	tmp.ativeCntN=rt->createNode();
	tmp.linkP.type=tmp.linkN.type=GAP;
	tmp.gapP=tmp.gapN=false;
	//AUMENTAR O PESO  PARA ROTEAMENTEO EM METAL NA VERTICAL SOBRE A DIFUSAO SE FOR POLY
	for(unsigned int x=0; x<nrCntsP; x++){
		tmp.diffCntsP.push_back(rt->createNode());
		if(tmp.diffCntsP.size()!=1) rt->addArc(tmp.diffCntsP.back(),tmp.diffCntsP[tmp.diffCntsP.size()-2],vcost);
		if(elements.size() && pDif_endY-currentRules->getRule(E1DFCT)-currentRules->getRule(W2CT)-currentRules->getRule(E1M1CT)-x*(currentRules->getRule(W2CT)/2+currentRules->getRule(E1M1CT)+currentRules->getRule(S1M1M1)+currentRules->getRule(W1M1)/2)>=supWidth+currentRules->getRule(S1M1M1)) 
			rt->addArc(tmp.diffCntsP.back(),elements.back().diffCntsP[x],4);	//if it' not the first, connect to the last diffcnt
	}
	
	for(unsigned int x=0; x<nrCntsN; x++){
		tmp.diffCntsN.push_back(rt->createNode());
		if(tmp.diffCntsN.size()!=1) rt->addArc(tmp.diffCntsN.back(),tmp.diffCntsN[tmp.diffCntsN.size()-2],vcost);
		if(elements.size() && nDif_iniY+currentRules->getRule(E1DFCT)+currentRules->getRule(W2CT)+currentRules->getRule(E1M1CT)+x*(currentRules->getRule(W2CT)/2+currentRules->getRule(E1M1CT)+currentRules->getRule(S1M1M1)+currentRules->getRule(W1M1)/2)<=height-supWidth-currentRules->getRule(S1M1M1)) 
			
			rt->addArc(tmp.diffCntsN.back(),elements.back().diffCntsN[x],4);	//if it' not the first, connect to the last diffcnt
	}
	
	tmp.inoutCnt=rt->createNode();
	for(unsigned int x=0; x<nrTracks; x++){
		tmp.interMet.push_back(rt->createNode());
		tmp.interPol.push_back(rt->createNode());
		if(x!=0) {
			rt->addArc(tmp.interMet[x], tmp.interMet[x-1],5);
			rt->addArc(tmp.interPol[x], tmp.interPol[x-1],5);
		}
		rt->addArc(tmp.interPol[x], tmp.interMet[x],20);
		if(elements.size()){ 
			rt->addArc(tmp.interMet[x], elements.back().interMet[x],4);	//if it' not the first, connect to the last diffcnt
			rt->addArc(tmp.interPol[x], elements.back().interPol[x],5);	//if it' not the first, connect to the last diffcnt
		}
	}
	tmp.outPolP=rt->createNode();
	tmp.outPolN=rt->createNode();
	
	//conecta em poly por fora somente se couber
	if(elements.size()){ 
		if(posCntsDiffP.back()-currentRules->getRule(E1DFCT)-currentRules->getRule(S1DFP1)-currentRules->getRule(W2P1)-currentRules->getRule(S1P1P1)/2>=0)
			rt->addArc(tmp.outPolP, elements.back().outPolP,6);
		if(posCntsDiffN.back()+currentRules->getRule(E1DFCT)+currentRules->getRule(S1DFP1)+currentRules->getRule(W2P1)+currentRules->getRule(S1P1P1)/2<=height)
			rt->addArc(tmp.outPolN, elements.back().outPolN,6);
	}
	
	rt->addArc(tmp.diffCntsP[0],tmp.interMet[0],vcost);
	rt->addArc(tmp.diffCntsN[0],tmp.interMet[nrTracks-1],vcost);
	
	elements.push_back(tmp);
	return &elements.back();
}

bool AutoCell::calcArea(Circuit* c){
	currentCircuit=c;
	currentRules=currentCircuit->getRules();
	
	cout << "Calculating cell area..." << endl;
	state=0;
	
	int x;
	vGrid=currentRules->getIntValue(currentCircuit->getVPitch());
	hGrid=currentRules->getIntValue(currentCircuit->getHPitch());
	height=currentCircuit->getRowHeight()*vGrid;
	supWidth=max(currentRules->getIntValue(currentCircuit->getSupplyVSize())/2,currentRules->getRule(W2CT)/2+currentRules->getRule(E1M1CT));
	
	pDif_iniY = max(currentRules->getRule(S1ININ)/2+currentRules->getRule(E1INDF),currentRules->getRule(S1DFP1)+currentRules->getRule(E1P1DF));
	pDif_endY = (posIOs*vGrid)-(currentRules->getRule(W2VI)/2)-currentRules->getRule(E1M1VI)-currentRules->getRule(S1M1M1)-currentRules->getRule(E1M1CT)+currentRules->getRule(E1DFCT);
	pSize=pDif_endY-pDif_iniY;
	
	nDif_iniY = ((posIOs+nrTracks-1)*vGrid)+(currentRules->getRule(W2VI)/2)+currentRules->getRule(E1M1VI)+currentRules->getRule(S1M1M1)+currentRules->getRule(E1M1CT)-currentRules->getRule(E1DFCT);
	nDif_endY = height-max(currentRules->getRule(S1IPIP)/2+currentRules->getRule(E1IPDF),currentRules->getRule(S1DFP1)+currentRules->getRule(E1P1DF));
	nSize=nDif_endY-nDif_iniY;
	
	nrCntsP=1+((pDif_endY-(currentRules->getRule(S1ININ)/2+currentRules->getRule(E1INDF)))-currentRules->getRule(W2CT)-2*currentRules->getRule(E1DFCT))/(currentRules->getRule(W2CT)/2+currentRules->getRule(E1M1CT)+currentRules->getRule(S1M1M1)+currentRules->getRule(W1M1)/2); 
	nrCntsN=1+((height-(currentRules->getRule(S1IPIP)/2+currentRules->getRule(E1IPDF))-nDif_iniY)-currentRules->getRule(W2CT)-2*currentRules->getRule(E1DFCT))/(currentRules->getRule(E1DFCT)+currentRules->getRule(W2CT)+currentRules->getRule(E1M1CT)+currentRules->getRule(S1M1M1)+currentRules->getRule(W1M1)/2-currentRules->getRule(W2CT)/2-currentRules->getRule(E1DFCT)); 
	
	trackPos.clear();
	trackPos.push_back(posIOs*vGrid);
	for(x=1; x<nrTracks;x++)
		trackPos.push_back(trackPos[x-1]+vGrid);
	
	posCntsDiffP.clear();
	posCntsDiffP.push_back(pDif_endY-currentRules->getRule(E1DFCT));
	for(x=1; x<nrCntsP; x++)
		posCntsDiffP.push_back(posCntsDiffP[x-1]-currentRules->getRule(W1M1)/2-currentRules->getRule(W2CT)/2-currentRules->getRule(S1M1M1)-currentRules->getRule(E1M1CT));
	
	posCntsDiffN.clear();
	posCntsDiffN.push_back(nDif_iniY+currentRules->getRule(E1DFCT));
	
	for(x=1; x<nrCntsN; x++)
		posCntsDiffN.push_back(posCntsDiffN[x-1]+currentRules->getRule(W1M1)/2+currentRules->getRule(W2CT)/2+currentRules->getRule(S1M1M1)+currentRules->getRule(E1M1CT));	
	return state++;
}

bool AutoCell::selectCell(string c){	
	if(state<1) return 0;
	state=1;
	
	if(currentCell=currentCircuit->getCellNetlst(c)){
		cout << "Selecting cell netlist: " << currentCell->getName() << endl;
		currentNetList.clear();
		currentNetList=currentCircuit->getFlattenCell(c);
		state=2;
	}
	return state==2;		
}

bool AutoCell::foldTrans(){
	cout << "Applying folding..." << endl;
	if(state<2) return 0;
	state=2;
	cout << "- Number of transistors before folding: " << currentCell->size() << " -> P(" << currentCell->pSize() << ") N(" << currentCell->nSize() << ")" << endl;
	if(currentNetList.folding(float(pSize)/currentRules->getScale(),float(nSize)/currentRules->getScale())){
		cout << "- Number of transistors after folding: " << currentNetList.size() << " -> P(" << currentNetList.pSize() << ") N(" << currentNetList.nSize() << ")" << endl;
		state=3;
	}
	return state==3;
}

bool AutoCell::placeTrans(bool ep, int saquality, int nrAttempts, int wC, int gmC, int rC, int congC, int ngC){
	cout << "Placing transistors..." << endl;
	if(state<3) return 0;
	state=3;
	if(currentNetList.transPlacement(ep, saquality, nrAttempts,  wC,  gmC,  rC,  congC,  ngC)){
		
		// CONSTRUCT THE ROUTING GRAPH
		
		int COST_CNT_INSIDE_DIFF=10, COST_CNT_OUTSIDE_DIFF=15, COST_CNT_BORDER_DIFF=15;
		
		Element *tmp, *lastelement;
		bool gapP, gapN, decCostIOP, decCostION;
		
		clear();
		rt = new Pathfinder2();
		
		vdd=rt->createNode();
		gnd=rt->createNode();
		//	currentCell->print();
		map<string,unsigned int>::iterator inoutPins_it;
		vector<int>::iterator inouts_it=currentNetList.getInouts().begin();
		while(inouts_it!=currentNetList.getInouts().end()){
			//		cerr << currentCircuit->getGndNet()  << " - " <<  currentCircuit->getVddNet() << " - " << currentNetList.getNetName(*inouts_it) << endl;
			if(currentNetList.getNetName(*inouts_it)==currentCircuit->getVddNet()){
				rt->addNodetoNet(*inouts_it,vdd);
			}else if(currentNetList.getNetName(*inouts_it)==currentCircuit->getGndNet()){
				rt->addNodetoNet(*inouts_it,gnd);
			}else{
				inoutPins[currentNetList.getNetName(*inouts_it)]=rt->createNode();
				rt->addNodetoNet(*inouts_it,inoutPins[currentNetList.getNetName(*inouts_it)]);
			}
			inouts_it++;
		}
//		cout << vdd << "-" << gnd << endl;
		//cria elemento para roteamento lateral
		tmp=createElement(20);
		for(unsigned int x=0; x<nrTracks; x++)
			rt->addArc(tmp->inoutCnt, tmp->interMet[x],550);
		
		//conecta sinais de entrada e saida com o nó inoutCnt do elemento
		inoutPins_it=inoutPins.begin();
		while(inoutPins_it!=inoutPins.end()){
			rt->addArc(tmp->inoutCnt,inoutPins_it->second,0);
			inoutPins_it++;
		}
		
		decCostIOP=decCostION=true;
		vector<t_net2>::iterator eulerPathP_it=currentNetList.getOrderingP().begin(), eulerPathN_it=currentNetList.getOrderingN().begin(),lastP_it, lastN_it;
		
		while(eulerPathP_it!=currentNetList.getOrderingP().end() && eulerPathN_it!=currentNetList.getOrderingN().end()){
			gapP=false;
			gapN=false;
			
			if(eulerPathP_it!=currentNetList.getOrderingP().begin() && eulerPathP_it->link!=-1 && tmp->linkP.type!=GAP){
				if((lastP_it->type==SOURCE && (
											   (eulerPathP_it->type==SOURCE && currentNetList.getTrans(lastP_it->link).drain!=currentNetList.getTrans(eulerPathP_it->link).source) ||
											   (eulerPathP_it->type==DRAIN && currentNetList.getTrans(lastP_it->link).drain!=currentNetList.getTrans(eulerPathP_it->link).drain))) ||
				   (lastP_it->type==DRAIN && (
											  (eulerPathP_it->type==SOURCE && currentNetList.getTrans(lastP_it->link).source!=currentNetList.getTrans(eulerPathP_it->link).source) ||
											  (eulerPathP_it->type==DRAIN && currentNetList.getTrans(lastP_it->link).source!=currentNetList.getTrans(eulerPathP_it->link).drain)))||
				   eulerPathP_it->link==-1)
					gapP=true;
			}
			if(eulerPathN_it!=currentNetList.getOrderingN().begin() && eulerPathN_it->link!=-1 && tmp->linkN.type!=GAP){
				if((lastN_it->type==SOURCE && (
											   (eulerPathN_it->type==SOURCE && currentNetList.getTrans(lastN_it->link).drain!=currentNetList.getTrans(eulerPathN_it->link).source) ||
											   (eulerPathN_it->type==DRAIN && currentNetList.getTrans(lastN_it->link).drain!=currentNetList.getTrans(eulerPathN_it->link).drain))) ||
				   (lastN_it->type==DRAIN && (
											  (eulerPathN_it->type==SOURCE && currentNetList.getTrans(lastN_it->link).source!=currentNetList.getTrans(eulerPathN_it->link).source) ||
											  (eulerPathN_it->type==DRAIN && currentNetList.getTrans(lastN_it->link).source!=currentNetList.getTrans(eulerPathN_it->link).drain)))||
				   eulerPathN_it->link==-1)
					gapN=true;
			}
			
			if(gapP || gapN || eulerPathP_it==currentNetList.getOrderingP().begin() || eulerPathN_it==currentNetList.getOrderingN().begin()){
				lastelement=tmp;
				tmp = createElement(5);
				
				//conecta aos pinos de entrada e saida
				for(unsigned int x=0; x<nrTracks; x++)
					if((!decCostIOP && x==0) || (!decCostION && x==nrTracks-1)) rt->addArc(tmp->inoutCnt, tmp->interMet[x],500);
					else rt->addArc(tmp->inoutCnt, tmp->interMet[x],496);
				decCostIOP=decCostION=false;
				
				//conecta sinais de entrada e saida com o nó inoutCnt do elemento
				inoutPins_it=inoutPins.begin();
				while(inoutPins_it!=inoutPins.end()){
					rt->addArc(tmp->inoutCnt,inoutPins_it->second,0);
					inoutPins_it++;
				}
				
				rt->addArc(tmp->diffCntsP.back(),vdd,0);
				rt->addArc(tmp->diffCntsN.back(),gnd,0);
				
			}
			
			if(tmp->linkP.type==GAP && eulerPathP_it->link!=-1){ // nao é GAP na difusao P
				if(gapP) tmp->linkP=*eulerPathP_it;
				tmp->linkP=*eulerPathP_it;
				//			tmp->ativeCntP=rt->createNode();
				if(gapP) tmp->gapP=true;
				
				if(eulerPathP_it->type==SOURCE){
					rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).source,tmp->ativeCntP);
					//decrementa custo do contato de IO do poly na 1a track
					if(currentNetList.getNet(currentNetList.getTrans(eulerPathP_it->link).source).trans.size()>2)
						decCostIOP=true;
					if(lastelement->linkP.type!=GAP && rt->getNet(lastelement->ativeCntP)==rt->getNet(tmp->ativeCntP) && currentNetList.getNet(currentNetList.getTrans(eulerPathP_it->link).source).trans.size()>=2)  //VERIFICAR POSTERIORMENTE ==2?
						rt->addArc(tmp->ativeCntP,lastelement->ativeCntP,0); //tira a ponte entre a difusao atual e ela mesma 
					
				}
				else{
					rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).drain,tmp->ativeCntP);
					if(currentNetList.getNet(currentNetList.getTrans(eulerPathP_it->link).drain).trans.size()>2)
						decCostIOP=true;
					if(lastelement->linkP.type!=GAP && rt->getNet(lastelement->ativeCntP)==rt->getNet(tmp->ativeCntP) && currentNetList.getNet(currentNetList.getTrans(eulerPathP_it->link).drain).trans.size()>=2) //VERIFICAR POSTERIORMENTE ==2?
						rt->addArc(tmp->ativeCntP,lastelement->ativeCntP,0); //tira a ponte entre a difusao atual e ela mesma 
				}
				//VER MINIMO E MAXIMO ENTRE TRANSISTOR ATUAL E PROXIMO
				int penality=0;
				for(unsigned int x=0; x<tmp->diffCntsP.size(); x++){
					if(posCntsDiffP[x]-currentRules->getRule(W2CT)-currentRules->getRule(E1DFCT)>=pDif_endY-currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width)){
						rt->addArc(tmp->diffCntsP[x], tmp->ativeCntP,COST_CNT_INSIDE_DIFF);
					}else if(posCntsDiffP[x]+currentRules->getRule(E1DFCT) <= pDif_endY-currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width)-currentRules->getRule(E1P1DF)-currentRules->getRule(S1DFP1)){
						penality++;
						rt->addArc(tmp->diffCntsP[x], tmp->ativeCntP,COST_CNT_OUTSIDE_DIFF+(7*penality));
					}else{
						penality++;
						rt->addArc(tmp->diffCntsP[x], tmp->ativeCntP,COST_CNT_BORDER_DIFF+(7*penality));
					}
				}
			}
			
			if(tmp->linkN.type==GAP && eulerPathN_it->link!=-1){ // nao é GAP na difusao N
				tmp->linkN=*eulerPathN_it;
				//			tmp->ativeCntN=rt->createNode();
				if(gapN) tmp->gapN=true;
				
				if(eulerPathN_it->type==SOURCE){
					rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).source,tmp->ativeCntN);
					//decrementa custo do contato de IO do poly na 1a track
					if(currentNetList.getNet(currentNetList.getTrans(eulerPathN_it->link).source).trans.size()>2)
						decCostION=true;
					if(lastelement->linkN.type!=GAP && rt->getNet(lastelement->ativeCntN)==rt->getNet(tmp->ativeCntN) && currentNetList.getNet(currentNetList.getTrans(eulerPathN_it->link).source).trans.size()>=2) //VERIFICAR POSTERIORMENTE ==2?
						rt->addArc(tmp->ativeCntN,lastelement->ativeCntN,0); //tira a ponte entre a difusao atual e ela mesma
				}
				else{
					rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).drain,tmp->ativeCntN);
					//decrementa custo do contato de IO do poly na 1a track
					if(currentNetList.getNet(currentNetList.getTrans(eulerPathN_it->link).drain).trans.size()>2)
						decCostION=true;
					if(lastelement->linkN.type!=GAP && rt->getNet(lastelement->ativeCntN)==rt->getNet(tmp->ativeCntN) && currentNetList.getNet(currentNetList.getTrans(eulerPathN_it->link).drain).trans.size()>=2) //VERIFICAR POSTERIORMENTE ==2?
						rt->addArc(tmp->ativeCntN,lastelement->ativeCntN,0); //tira a ponte entre a difusao atual e ela mesma 
				}
				int penality=0;
				for(unsigned int x=0; x<nrCntsN; x++){
					if(posCntsDiffN[x]+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT) <= nDif_iniY+currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width)){
						rt->addArc(tmp->diffCntsN[x], tmp->ativeCntN,COST_CNT_INSIDE_DIFF);
					}else if(posCntsDiffN[x]-currentRules->getRule(E1DFCT) >= nDif_iniY+currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width)+currentRules->getRule(E1P1DF)+currentRules->getRule(S1DFP1)){
						penality++;
						rt->addArc(tmp->diffCntsN[x], tmp->ativeCntN,COST_CNT_OUTSIDE_DIFF+(7*penality));
					}else{
						penality++;
						rt->addArc(tmp->diffCntsN[x], tmp->ativeCntN,COST_CNT_BORDER_DIFF+(7*penality));
					}
				}
			}
			
			//desenha gate do transistor se nao for GAP
			lastelement=tmp;
			tmp = createElement(20);
			
			//conecta aos pinos de entrada e saida
			for(unsigned int x=0; x<nrTracks; x++){
				if((!decCostIOP && x==0) || (!decCostION && x==nrTracks-1)) rt->addArc(tmp->inoutCnt, tmp->interMet[x],500);
				else rt->addArc(tmp->inoutCnt, tmp->interMet[x],496);
			}
			decCostIOP=decCostION=false;
			
			if(eulerPathP_it->link!=-1){ // nao é GAP na difusao P
				tmp->linkP=*eulerPathP_it;
				tmp->linkP.type=GATE;
				//			tmp->ativeCntP=rt->createNode();
				rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).gate,tmp->ativeCntP);
				rt->addArc(tmp->ativeCntP,tmp->interPol[0],5);
				rt->lockArc(tmp->interPol[0], tmp->interMet[0], currentNetList.getTrans(eulerPathP_it->link).gate);
				if(pDif_endY-currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width)-currentRules->getRule(E1P1DF)-currentRules->getRule(S1P1P1)-currentRules->getRule(W2P1)-currentRules->getRule(S1DFP1)<0)
					rt->remArcs(tmp->outPolP);
				else
					rt->addArc(tmp->ativeCntP,tmp->outPolP,5);
			}
			else tmp->linkP.type=GAP;
			
			inoutPins_it=inoutPins.begin();
			while(inoutPins_it!=inoutPins.end()){
				rt->addArc(tmp->inoutCnt,inoutPins_it->second,0);
				inoutPins_it++;
			}
			
			if(eulerPathN_it->link!=-1){ // nao é GAP na difusao N
				tmp->linkN=*eulerPathN_it;
				tmp->linkN.type=GATE;
				//			tmp->ativeCntN=rt->createNode();
				rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).gate,tmp->ativeCntN);
				rt->addArc(tmp->ativeCntN,tmp->interPol[nrTracks-1],5);
				rt->lockArc(tmp->interPol[nrTracks-1], tmp->interMet[nrTracks-1], currentNetList.getTrans(eulerPathN_it->link).gate);
				if(nDif_iniY+currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width)+currentRules->getRule(E1P1DF)+currentRules->getRule(S1P1P1)+currentRules->getRule(W2P1)+currentRules->getRule(S1DFP1)>height)
					rt->remArcs(tmp->outPolN);
				else
					rt->addArc(tmp->ativeCntN,tmp->outPolN,5);
			}
			else tmp->linkN.type=GAP;
			
			lastelement=tmp;
			tmp = createElement(5);
			
			if(eulerPathP_it->link!=-1){ // nao é GAP na difusao P
				tmp->linkP=*eulerPathP_it;
				//			tmp->ativeCntP=rt->createNode();
				if(eulerPathP_it->type==DRAIN){
					tmp->linkP.type=SOURCE;
					rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).source,tmp->ativeCntP);
					//decrementa custo do contato de IO 
					if(currentNetList.getNet(currentNetList.getTrans(eulerPathP_it->link).source).trans.size()>2)
						decCostIOP=true;
					
				}
				else{
					tmp->linkP.type=DRAIN;
					rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).drain,tmp->ativeCntP);
					//decrementa custo do contato de IO 
					if(currentNetList.getNet(currentNetList.getTrans(eulerPathP_it->link).drain).trans.size()>2)
						decCostIOP=true;
				}
				
				int penality=0;
				for(unsigned int x=0; x<tmp->diffCntsP.size(); x++){
					if(posCntsDiffP[x]-currentRules->getRule(W2CT)-currentRules->getRule(E1DFCT)>=pDif_endY-currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width)){
						rt->addArc(tmp->diffCntsP[x], tmp->ativeCntP,COST_CNT_INSIDE_DIFF);
					}else if(posCntsDiffP[x]+currentRules->getRule(E1DFCT) <= pDif_endY-currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width)-currentRules->getRule(E1P1DF)-currentRules->getRule(S1DFP1)){
						penality++;
						rt->addArc(tmp->diffCntsP[x], tmp->ativeCntP,COST_CNT_OUTSIDE_DIFF+(7*penality));
					}else{
						penality++;
						rt->addArc(tmp->diffCntsP[x], tmp->ativeCntP,COST_CNT_BORDER_DIFF+(7*penality));
					}
				}
			}
			else {
				tmp->linkP.type=GAP; 
			}

			rt->addArc(tmp->diffCntsP.back(),vdd,0);  // Create a link to VDD;
			rt->addArc(tmp->diffCntsN.back(),gnd,0);  // Create a link to GND;
			
			if(eulerPathN_it->link!=-1){ // nao é GAP na difusao N
				tmp->linkN=*eulerPathN_it;
				//			tmp->ativeCntN=rt->createNode();
				if(eulerPathN_it->type==SOURCE){
					tmp->linkN.type=DRAIN;
					rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).drain,tmp->ativeCntN);
					//decrementa custo do contato de IO 
					if(currentNetList.getNet(currentNetList.getTrans(eulerPathN_it->link).drain).trans.size()>2)
						decCostION=true;
				}
				else{
					tmp->linkN.type=SOURCE;
					rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).source,tmp->ativeCntN);
					//decrementa custo do contato de IO 
					if(currentNetList.getNet(currentNetList.getTrans(eulerPathN_it->link).source).trans.size()>2)
						decCostION=true;
				}
				
				int penality=0;
				for(unsigned int x=0; x<nrCntsN; x++){
					if(posCntsDiffN[x]+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT) <= nDif_iniY+currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width)){
						rt->addArc(tmp->diffCntsN[x], tmp->ativeCntN,COST_CNT_INSIDE_DIFF);
					}else if(posCntsDiffN[x]-currentRules->getRule(E1DFCT) >= nDif_iniY+currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width)+currentRules->getRule(E1P1DF)+currentRules->getRule(S1DFP1)){
						penality++;
						rt->addArc(tmp->diffCntsN[x], tmp->ativeCntN,COST_CNT_OUTSIDE_DIFF+(7*penality));
					}else{
						penality++;
						rt->addArc(tmp->diffCntsN[x], tmp->ativeCntN,COST_CNT_BORDER_DIFF+(7*penality));
					}
				}
			}
			else tmp->linkN.type=GAP;
			
			//conecta aos pinos de entrada e saida
			for(unsigned int x=0; x<nrTracks; x++)
				if((!decCostIOP && x==0) || (!decCostION && x==nrTracks-1)) rt->addArc(tmp->inoutCnt, tmp->interMet[x],498);
				else rt->addArc(tmp->inoutCnt, tmp->interMet[x],500);
			
			inoutPins_it=inoutPins.begin();
			while(inoutPins_it!=inoutPins.end()){
				rt->addArc(tmp->inoutCnt,inoutPins_it->second,0);
				inoutPins_it++;
			}
			
			lastP_it=eulerPathP_it++;
			lastN_it=eulerPathN_it++;
		}
		
		//cria elemento lateral para roteamento
		tmp=createElement(20);
		for(unsigned int x=0; x<nrTracks; x++)
			rt->addArc(tmp->inoutCnt, tmp->interMet[x],550);
		
		//conecta sinais de entrada e saida com o nó inoutCnt do elemento
		inoutPins_it=inoutPins.begin();
		while(inoutPins_it!=inoutPins.end()){
			rt->addArc(tmp->inoutCnt,inoutPins_it->second,0);
			inoutPins_it++;
		}
		state++;
		return true;
	}else
		return false;	
}

bool AutoCell::route(int mCost, int pCost, int cCost, int ioCost){
	cout << "Routing cell..." << endl;
	showIOCost();
	if(state<4) return 0;
	state=4;
	
	if(rt->routeNets(8000)&&rt->optimize()&&rt->optimize()){
		/*		compaction cpt(CP_LP);
		 cpt.setLPFilename("temp");
		 list<Element>::iterator lastelements_it;
		 int el=0;
		 for(list<Element>::iterator elements_it=elements.begin(); elements_it!=elements.end(); elements_it++){
		 if(elements_it!=elements.begin()){
		 for(int x=0; x<nrTracks;x++){
		 cpt.insertUpperBound("E"+(el-1)+"_"+x+" + "+"E"+el+"_"+x,"M"+el);
		 }
		 cpt.insertLPMinVar("M"+el);
		 }
		 ++el;
		 lastelements_it=elements_it;
		 }
		 */		
		//			rt->showResult();
		//		eletricalOtimization(currentCell,rt);
		state=5;
	}
	else{
		cout << "Unable to route this circuit" << endl;
	}
	return state==5;
}

bool AutoCell::compact(int mPriority, int pPriority, int gsPriority, int wPriority, string lpSolverFile){
	cout << "Compacting layout..." << endl;
	if(state<5) return 0;
	state=5;
	int x;
	
	vector<Box*> geometries;
	map<string,int> IOgeometries;
	
	int transWidthP=0, transWidthN=0;
	currentLayout.clear();
	currentLayout.setName(currentCell->getName());
	compaction cpt(CP_LP);
	cpt.setLPFilename("temp");
	
	
    string v1, v2, v3, vDiff, diffTowerP="", diffTowerN=""; // v2 armazena as posicoes x do ultimo metal e v3 do ultimo poly
	string polyEdgeP="", limiteCntsP="", lastDiffP="";
	string polyEdgeN="", limiteCntsN="", lastDiffN="";
    string compacDiffP="", compacDiffN="", pPolyCnt="", nPolyCnt="", pPolyGate="", nPolyGate="", lastIO="";
	unsigned int diffTowerP_width, diffTowerN_width, polyWidthP=0, polyWidthN=0;
	
	list<Element>::iterator lastElements_it;
	vector<string> metTracksPos(nrTracks), polTracksPos(nrTracks);
	vector<string> lastMetTracksPos(nrTracks), lastPolTracksPos(nrTracks);
	string outPolP="", outPolN="";
	for(x=0; x<nrTracks;x++){
		lastMetTracksPos[x]="";
		metTracksPos[x]="";
		lastPolTracksPos[x]="";
		polTracksPos[x]="";
	}
	
	vector<string> pTracksPos(nrCntsP), nTracksPos(nrCntsN);
	vector<string> pDiffPos(nrCntsP), nDiffPos(nrCntsN);
	vector<string> pCntPos(nrCntsP), nCntPos(nrCntsN);
	vector<string> lastPTracksPos(nrCntsP), lastNTracksPos(nrCntsN);
	bool gapP=false, gapN=false;
	for(x=0; x<nrCntsP;x++){
		pTracksPos[x]="";
		pDiffPos[x]="";
		pCntPos[x]="";
		lastPTracksPos[x]="";
	}
	for(x=0; x<nrCntsN;x++){
		nTracksPos[x]="";
		nDiffPos[x]="";
		nCntPos[x]="";
		lastNTracksPos[x]="";
	}
	
	cpt.insertConstraint( "ZERO", "HGRID", CP_EQ, hGrid/2);
	
	for(list<Element>::iterator elements_it=elements.begin(); elements_it!=elements.end(); elements_it++){
		//	elements_it->print();
		
		for(x=0; x<nrCntsP;x++){
			//gera os metais para roteamento sobre a difusao P
			//conecta as trilhas de metal horizontalmente
			if(rt->getNet(elements_it->diffCntsP[x])!=-1){
				if(pTracksPos[x]=="" || !rt->areConnected(elements_it->diffCntsP[x],lastElements_it->diffCntsP[x])){
					geometries.push_back(&currentLayout.addPolygon(0,posCntsDiffP[x]-currentRules->getRule(W2CT)/2+currentRules->getRule(W1M1)/2,0,posCntsDiffP[x]-currentRules->getRule(W2CT)/2-currentRules->getRule(W1M1)/2,MET1));
					geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->diffCntsP[x])));
					pTracksPos[x]=intToStr(geometries.size()-1);
					cpt.insertConstraint( "x" + pTracksPos[x] + "a", "x" + pTracksPos[x] + "b", CP_EQ, "x" + pTracksPos[x] + "min");
					cpt.insertLPMinVar("x" + pTracksPos[x] + "min");
					//insere regras de distância mínima entre metais
					if(lastPTracksPos[x]!="")
						cpt.insertConstraint( "x" + lastPTracksPos[x] + "b", "x" + pTracksPos[x] + "a", CP_MIN, currentRules->getRule(S1M1M1));
				}
			}
			
			//verifica se conecta em metal 1 com o contato adjacente
			if(x>0 && rt->areConnected(elements_it->diffCntsP[x],elements_it->diffCntsP[x-1])){
				geometries.push_back(&currentLayout.addPolygon(0, posCntsDiffP[x], 0,posCntsDiffP[x-1],MET1));
				geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->diffCntsP[x])));
				string lastMetP=intToStr(geometries.size()-1);
				cpt.insertConstraint( "x" + lastMetP + "a", "x" + lastMetP + "b", CP_EQ, currentRules->getRule(W1M1));					
				cpt.insertConstraint( "x" + pTracksPos[x] + "a", "x" + lastMetP + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + lastMetP + "b", "x" + pTracksPos[x] + "b", CP_MIN, 0);
				cpt.insertConstraint( "x" + pTracksPos[x-1] + "a", "x" + lastMetP + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + lastMetP + "b", "x" + pTracksPos[x-1] + "b", CP_MIN, 0);
			}
			//verifica se conecta em metal 1 com o VDD
			if(rt->areConnected(elements_it->diffCntsP[x],vdd)){
				geometries.push_back(&currentLayout.addPolygon(0, posCntsDiffP[x], 0,0,MET1));
				geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->diffCntsP[x])));
				string lastMetP=intToStr(geometries.size()-1);
				cpt.insertConstraint( "x" + lastMetP + "a", "x" + lastMetP + "b", CP_EQ, currentRules->getRule(W1M1));
				cpt.insertConstraint( "x" + pTracksPos[x] + "a", "x" + lastMetP + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + lastMetP + "b", "x" + pTracksPos[x] + "b", CP_MIN, 0);
			}						
		}
		
		for(x=0; x<nrCntsN;x++){
			//gera os metais para roteamento sobre a difusao N
			//conecta as trilhas de metal horizontalmente
			if(rt->getNet(elements_it->diffCntsN[x])!=-1){
				if(nTracksPos[x]=="" || !rt->areConnected(elements_it->diffCntsN[x],lastElements_it->diffCntsN[x])){
					geometries.push_back(&currentLayout.addPolygon(0,posCntsDiffN[x]+currentRules->getRule(W2CT)/2+currentRules->getRule(W1M1)/2,0,posCntsDiffN[x]+currentRules->getRule(W2CT)/2-currentRules->getRule(W1M1)/2,MET1));
					geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->diffCntsN[x])));
					nTracksPos[x]=intToStr(geometries.size()-1);
					cpt.insertConstraint( "x" + nTracksPos[x] + "a", "x" + nTracksPos[x] + "b", CP_EQ, "x" + nTracksPos[x] + "min");
					cpt.insertLPMinVar("x" + nTracksPos[x] + "min");
					//insere regras de distância mínima entre metais
					if(lastNTracksPos[x]!="")
						cpt.insertConstraint( "x" + lastNTracksPos[x] + "b", "x" + nTracksPos[x] + "a", CP_MIN, currentRules->getRule(S1M1M1));
				}
			}
			
			//verifica se conecta em metal 1 com o contato adjacente
			if(x>0 && rt->areConnected(elements_it->diffCntsN[x],elements_it->diffCntsN[x-1])){
				geometries.push_back(&currentLayout.addPolygon(0, posCntsDiffN[x], 0,posCntsDiffN[x-1],MET1));
				geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->diffCntsN[x])));
				string lastMetN=intToStr(geometries.size()-1);
				cpt.insertConstraint( "x" + lastMetN + "a", "x" + lastMetN + "b", CP_EQ, currentRules->getRule(W1M1));					
				cpt.insertConstraint( "x" + nTracksPos[x] + "a", "x" + lastMetN + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + lastMetN + "b", "x" + nTracksPos[x] + "b", CP_MIN, 0);
				cpt.insertConstraint( "x" + nTracksPos[x-1] + "a", "x" + lastMetN + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + lastMetN + "b", "x" + nTracksPos[x-1] + "b", CP_MIN, 0);
			}
			//verifica se conecta em metal 1 com o GND
			if(rt->areConnected(elements_it->diffCntsN[x],gnd)){
				geometries.push_back(&currentLayout.addPolygon(0, posCntsDiffN[x], 0, height,MET1));
				geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->diffCntsN[x])));
				string lastMetN=intToStr(geometries.size()-1);
				cpt.insertConstraint( "x" + lastMetN + "a", "x" + lastMetN + "b", CP_EQ, currentRules->getRule(W1M1));
				cpt.insertConstraint( "x" + nTracksPos[x] + "a", "x" + lastMetN + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + lastMetN + "b", "x" + nTracksPos[x] + "b", CP_MIN, 0);
			}
		}
		
		for(x=0; x<nrTracks;x++){
			//conecta os metais das trilhas
			//conecta as trilhas de metal e poly horizontalmente
			if(rt->getNet(elements_it->interMet[x])!=-1){
				if(metTracksPos[x]=="" || !rt->areConnected(elements_it->interMet[x],lastElements_it->interMet[x])){
					geometries.push_back(&currentLayout.addPolygon(0,trackPos[x]-(currentRules->getRule(W1M1)/2),0,trackPos[x]+(currentRules->getRule(W1M1)/2),MET1));
					geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->interMet[x])));
					metTracksPos[x]=intToStr(geometries.size()-1);
					cpt.insertConstraint( "x" + metTracksPos[x] + "a", "x" + metTracksPos[x] + "b", CP_EQ, "x" + metTracksPos[x] + "min");
					cpt.insertLPMinVar("x" + metTracksPos[x] + "min",mPriority);
					cpt.insertConstraint( "ZERO", "x" + metTracksPos[x] + "a", CP_MIN, currentRules->getRule(S1M1M1)/2);
					cpt.insertConstraint( "x" + metTracksPos[x] + "b" , "WIDTH", CP_MIN, currentRules->getRule(S1M1M1)/2); //colocar no final!!
					//insere regras de distância mínima entre metais
					if(lastMetTracksPos[x]!="")
						cpt.insertConstraint( "x" + lastMetTracksPos[x] + "b", "x" + metTracksPos[x] + "a", CP_MIN, currentRules->getRule(S1M1M1));				
				}
			}
			
			//conecta os polys das trilhas		
			//conecta as trilhas de poly
			if(rt->getNet(elements_it->interPol[x])!=-1){
				if(polTracksPos[x]=="" || !rt->areConnected(elements_it->interPol[x],lastElements_it->interPol[x])){
/*					if(x==0)
						geometries.push_back(&currentLayout.addPolygon(0,pDif_endY+currentRules->getRule(E1P1DF)+currentRules->getRule(S2P1P1),0,pDif_endY+currentRules->getRule(E1P1DF)+currentRules->getRule(S2P1P1)+currentRules->getRule(W2P1),POLY));
					else if(x==nrTracks-1)
						geometries.push_back(&currentLayout.addPolygon(0,nDif_iniY-currentRules->getRule(E1P1DF)-currentRules->getRule(S2P1P1)-currentRules->getRule(W2P1),0,nDif_iniY-currentRules->getRule(E1P1DF)-currentRules->getRule(S2P1P1),POLY));
					else
*/						geometries.push_back(&currentLayout.addPolygon(0,trackPos[x]-(currentRules->getRule(W1M1)/2),0,trackPos[x]+(currentRules->getRule(W1M1)/2),POLY));
					polTracksPos[x]=intToStr(geometries.size()-1);
					cpt.insertConstraint( "x" + polTracksPos[x] + "a", "x" + polTracksPos[x] + "b", CP_EQ, "x" + polTracksPos[x] + "min");
					cpt.insertLPMinVar("x" + polTracksPos[x] + "min",pPriority);
					cpt.insertConstraint( "ZERO", "x" + polTracksPos[x] + "a", CP_MIN, currentRules->getRule(S1P1P1)/2);
					//insere regras de distância mínima entre os polys
					if(lastPolTracksPos[x]!="")
						cpt.insertConstraint( "x" + lastPolTracksPos[x] + "b", "x" + polTracksPos[x] + "a", CP_MIN, currentRules->getRule(S1P1P1));
				}
			}
			
			//conecta os polys com os metais das vias
			if(rt->areConnected(elements_it->interMet[x],elements_it->interPol[x])){
				geometries.push_back(&currentLayout.addPolygon(0,trackPos[x]-(currentRules->getRule(W2CT)/2),0,trackPos[x]+(currentRules->getRule(W2CT)/2),CONT));
				string tmpCnt=intToStr(geometries.size()-1);
				cpt.insertConstraint( "x" + tmpCnt + "a", "x" + tmpCnt + "b", CP_EQ, currentRules->getRule(W2CT));
				
				geometries.push_back(&currentLayout.addPolygon(0,trackPos[x]-(currentRules->getRule(W2CT)/2)-currentRules->getRule(E1P1CT),0,trackPos[x]+(currentRules->getRule(W2CT)/2)+currentRules->getRule(E1P1CT),POLY));
				string tmpPol=intToStr(geometries.size()-1);
				cpt.insertConstraint( "x" + tmpPol + "a", "x" + tmpCnt + "a", CP_EQ, currentRules->getRule(E1P1CT));
				cpt.insertConstraint( "x" + tmpCnt + "b", "x" + tmpPol + "b", CP_EQ, currentRules->getRule(E1P1CT));
				
				cpt.insertConstraint( "x" + polTracksPos[x] + "a", "x" + tmpPol + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + tmpPol + "b", "x" + polTracksPos[x] + "b", CP_MIN, 0);
				
				if(x==0 && pPolyGate!="") cpt.insertConstraint( "x" + pPolyGate + "b", "x" + tmpPol + "a", CP_MIN,  currentRules->getRule(S1P1P1));
				if(x==0 && !rt->areConnected(elements_it->ativeCntP,elements_it->interPol[x])) pPolyCnt= tmpPol;
				if(x==nrTracks-1 && nPolyGate!="") cpt.insertConstraint( "x" + nPolyGate + "b", "x" + tmpPol + "a", CP_MIN,  currentRules->getRule(S1P1P1));
				if(x==nrTracks-1 && !rt->areConnected(elements_it->ativeCntN,elements_it->interPol[x])) nPolyCnt= tmpPol;
				
				geometries.push_back(&currentLayout.addPolygon(0,trackPos[x]-(currentRules->getRule(W2CT)/2)-currentRules->getRule(E1M1CT),0,trackPos[x]+(currentRules->getRule(W2CT)/2)+currentRules->getRule(E1M1CT),MET1));
				geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->interMet[x])));
				string tmpMet=intToStr(geometries.size()-1);
				cpt.insertConstraint( "x" + tmpMet + "a", "x" + tmpCnt + "a", CP_EQ, currentRules->getRule(E1M1CT));
				cpt.insertConstraint( "x" + tmpCnt + "b", "x" + tmpMet + "b", CP_EQ, currentRules->getRule(E1M1CT));
				
				cpt.insertConstraint( "x" + metTracksPos[x] + "a", "x" + tmpMet + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + tmpMet + "b", "x" + metTracksPos[x] + "b", CP_MIN, 0);
			}
			//se for entrada/saida, alinha o metal1 com a grade
			if(rt->areConnected(elements_it->interMet[x],elements_it->inoutCnt)){
				geometries.push_back(&currentLayout.addPolygon(0,trackPos[x]-(currentRules->getRule(W2VI)/2)-currentRules->getRule(E1M1VI),0,trackPos[x]+(currentRules->getRule(W2VI)/2)+currentRules->getRule(E1M1VI),MET1));
				geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->interMet[x])));
				string tmp=intToStr(geometries.size()-1);
				cpt.insertConstraint( "HGRID", "x" + tmp + "g", CP_EQ_VAR_VAL,  "x" + tmp + "gpos", hGrid);
				cpt.forceIntegerVar("x" + tmp + "gpos");
				cpt.insertConstraint( "x" + tmp + "a", "x" + tmp + "g", CP_EQ, currentRules->getRule(E1M1VI)+currentRules->getRule(W2VI)/2);
				cpt.insertConstraint( "x" + tmp + "g", "x" + tmp + "b", CP_EQ, currentRules->getRule(E1M1VI)+currentRules->getRule(W2VI)/2);
				cpt.insertConstraint( "x" + tmp + "g", "WIDTH", CP_MIN, hGrid/2);
				cpt.insertConstraint( "ZERO", "x" + tmp + "g", CP_MIN, hGrid/2);
				
				cpt.insertConstraint( "x" + metTracksPos[x] + "a", "x" + tmp + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + tmp + "b", "x" + metTracksPos[x] + "b", CP_MIN, 0);
				
				IOgeometries[currentNetList.getNetName(rt->getNet(elements_it->inoutCnt))]=geometries.size()-1;
				if(lastIO!="") cpt.insertConstraint("x" + lastIO + "b", "x" + tmp + "a", CP_MIN, 0);
				lastIO=tmp;
			}						
		}
		
		//roteamento vertical dos metais e polys das trilhas
		for(x=1; x<nrTracks;x++){
			if(rt->areConnected(elements_it->interMet[x-1],elements_it->interMet[x])){				
				geometries.push_back(&currentLayout.addPolygon(0,trackPos[x]+(currentRules->getRule(W1M1)/2),0,trackPos[x-1]-(currentRules->getRule(W1M1)/2),MET1));
				geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->interMet[x])));
				string tmp=intToStr(geometries.size()-1);
				cpt.insertConstraint( "x" + tmp + "a", "x" + tmp + "b", CP_EQ, currentRules->getRule(W1M1));
				cpt.insertConstraint( "x" + metTracksPos[x] + "a", "x" + tmp + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + tmp + "b",  "x" + metTracksPos[x] + "b", CP_MIN, 0);
				cpt.insertConstraint( "x" + metTracksPos[x-1] + "a", "x" + tmp + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + tmp + "b", "x" + metTracksPos[x-1] + "b", CP_MIN, 0);
			}
		}
		
		for(x=1; x<nrTracks;x++){
			if(rt->areConnected(elements_it->interPol[x-1],elements_it->interPol[x])){				
				geometries.push_back(&currentLayout.addPolygon(0,trackPos[x],0,trackPos[x-1],POLY));
				string tmp=intToStr(geometries.size()-1);
				cpt.insertConstraint( "x" + tmp + "a", "x" + tmp + "b", CP_EQ, currentRules->getRule(W2P1));
				cpt.insertConstraint( "x" + polTracksPos[x] + "a", "x" + tmp + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + tmp + "b", "x" + polTracksPos[x] + "b", CP_MIN, 0);
				cpt.insertConstraint( "x" + polTracksPos[x-1] + "a", "x" + tmp + "a", CP_MIN, 0);
				cpt.insertConstraint( "x" + tmp + "b",  "x" + polTracksPos[x-1] + "b", CP_MIN, 0);
			}
		}
		
		if(rt->areConnected(elements_it->diffCntsP[0],elements_it->interMet[0])){
			geometries.push_back(&currentLayout.addPolygon(0, posCntsDiffP[0]-currentRules->getRule(W2CT)/2, 0,trackPos[0]+(currentRules->getRule(W1M1)/2),MET1));
			geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->interMet[0])));
			string tmp=intToStr(geometries.size()-1);
			
			cpt.insertConstraint( "x" + metTracksPos[0] + "a", "x" + tmp + "a", CP_MIN, 0);					
			cpt.insertConstraint( "x" + tmp + "b", "x" + metTracksPos[0] + "b", CP_MIN, 0);					
			cpt.insertConstraint( "x" + tmp + "a", "x" + tmp + "b", CP_EQ, currentRules->getRule(W1M1));
			
			//conecta no joelho que vai pro 1o contato da difusao
			cpt.insertConstraint( "x" + pTracksPos[0] + "a", "x" + tmp + "a", CP_MIN, 0);
			cpt.insertConstraint( "x" + tmp + "b", "x" + pTracksPos[0] + "b", CP_MIN, 0);
		}
		
		if(rt->areConnected(elements_it->diffCntsN[0],elements_it->interMet[nrTracks-1])){
			geometries.push_back(&currentLayout.addPolygon(0, posCntsDiffN[0]+currentRules->getRule(W2CT)/2, 0,trackPos[nrTracks-1]-(currentRules->getRule(W1M1)/2),MET1));
			geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->diffCntsN[0])));
			string tmp=intToStr(geometries.size()-1);
			cpt.insertConstraint( "x" + metTracksPos[nrTracks-1] + "a", "x" + tmp + "a", CP_MIN, 0);					
			cpt.insertConstraint( "x" + tmp + "b", "x" + metTracksPos[nrTracks-1] + "b", CP_MIN, 0);					
			cpt.insertConstraint( "x" + tmp + "a", "x" + tmp + "b", CP_EQ, currentRules->getRule(W1M1));
			
			//conecta no joelho que vai pro 1o contato da difusao
			cpt.insertConstraint( "x" + nTracksPos[0] + "a", "x" + tmp + "a", CP_MIN, 0);
			cpt.insertConstraint( "x" + tmp + "b", "x" + nTracksPos[0] + "b", CP_MIN, 0);
		}
		
		if(elements_it->gapP) gapP=true;
		if(elements_it->gapN) gapN=true;
		
		string tmp;
		
		switch(elements_it->linkP.type){
			case GAP:
				gapP=true;
				break;		
			case SOURCE: 
			case DRAIN:
				diffTowerP_width=pDif_endY;
				if(lastDiffP!="" && !gapP) diffTowerP_width-=transWidthP;
				
				for(x=0; x<elements_it->diffCntsP.size();x++)
					if(rt->areConnected(elements_it->diffCntsP[x],elements_it->ativeCntP))
						diffTowerP_width= min(diffTowerP_width,posCntsDiffP[x]-currentRules->getRule(W2CT)-currentRules->getRule(E1DFCT));
				
				//controla a posicao do outpol
				if(outPolP!="")	cpt.insertConstraint( "y" + outPolP + "b", "ZERO", CP_MIN,  -(diffTowerP_width-currentRules->getRule(S1DFP1)));				
				
				geometries.push_back(&currentLayout.addPolygon(0,pDif_endY,0,diffTowerP_width,PDIF));
				diffTowerP=intToStr(geometries.size()-1);
				
				cpt.insertConstraint( "x" + diffTowerP + "a", "x" + diffTowerP + "b", CP_MIN, currentRules->getRule(W2DF)); //CORRIGIR REGRA
				//insere regra de distancia entre gate e a difftower
                if((polyEdgeP!="") && (diffTowerP_width<pDif_endY-transWidthP)){// || rt->areConnected(lastElements_it->ativeCntP,lastElements_it->outPolP))){
					cpt.insertConstraint( "x" + polyEdgeP + "b", "x" + diffTowerP + "a", CP_MIN, currentRules->getRule(S1DFP1));
				}
				//faz a difusao anterior terminar depois da diffTowerP;
				if(lastDiffP!="" && !gapP){
					cpt.insertConstraint( "x" + diffTowerP + "b", "x" + lastDiffP + "b", CP_MIN, 0);
					cpt.insertConstraint( "x" + lastDiffP + "a", "x" + diffTowerP + "a", CP_MIN, 0);
				}
				
				limiteCntsP="";
				//cria boundbox para contatos
				tmp=intToStr(geometries.size()) + "_limcnt_";
				for(x=0; x<elements_it->diffCntsP.size();x++){ // desenha contatos na difusao
					if(rt->areConnected(elements_it->diffCntsP[x],elements_it->ativeCntP)){
						geometries.push_back(&currentLayout.addPolygon(0, posCntsDiffP[x]+currentRules->getRule(E1DFCT),0,posCntsDiffP[x]-currentRules->getRule(W2CT)-currentRules->getRule(E1DFCT),PDIF));
						vDiff=intToStr(geometries.size()-1);
						geometries.push_back(&currentLayout.addPolygon(0, posCntsDiffP[x], 0,posCntsDiffP[x]-currentRules->getRule(W2CT),CONT));
						v1= intToStr(geometries.size()-1);
						pCntPos[x]=v1;
						//						geometries[strToInt(diffTowerP)]->setY(pDif_endYposCntsDiffP[x]-currentRules->getRule(W2CT)-currentRules->getRule(E1DFCT));
						
						cpt.insertConstraint( "x" + vDiff + "a", "x" + diffTowerP + "a", CP_MIN, 0);
						cpt.insertConstraint(  "x" + diffTowerP + "b", "x" + vDiff + "b", CP_MIN, 0);
						
						//minimisa o tamanho da difusao entre contatos
						if(compacDiffP!="" && polyEdgeP!=""){
							cpt.insertConstraint("x" + v1 + "b", "x" + compacDiffP + "b", CP_MIN, 0);
							cpt.insertConstraint("x" + compacDiffP + "a", "x" + compacDiffP + "b", CP_EQ, "x" + compacDiffP + "min");
							cpt.insertLPMinVar("x" + compacDiffP + "min",gsPriority);                                            
						}
						compacDiffP=intToStr(geometries.size()) + "_compacDiffP_";
				        cpt.insertConstraint("x" + compacDiffP + "a", "x" + v1 + "b", CP_MIN, 0);
						
						if(polyEdgeP!=""){
							//se contato nao estiver acima do poly anterior
							if(posCntsDiffP[x]+currentRules->getRule(E1DFCT) > polyWidthP-currentRules->getRule(S1DFP1)){
								//contato esta dentro da difusao, entao cria boundbox para contatos 
								cpt.insertConstraint( "x" + tmp + "a", "x" + v1 + "a", CP_MIN, 0);
								cpt.insertConstraint(  "x" + v1 + "b", "x" + tmp + "b", CP_MIN, 0);          
								limiteCntsP=tmp;
								//se contato estiver na altura do poly anterior (w do transistor)                                                                
								if(posCntsDiffP[x]-currentRules->getRule(W2CT)-currentRules->getRule(E1DFCT)<pDif_endY-transWidthP){
									cpt.insertConstraint( "x" + polyEdgeP + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFP1));
									cpt.insertConstraint( "x" + diffTowerP + "a", "x" + diffTowerP + "b", CP_MIN, 2*currentRules->getRule(E1DFCT)+currentRules->getRule(W2CT));
								}
							}else cpt.insertConstraint( "x" + polyEdgeP + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFP1));
						}
                        //regra de distancia entre difusoes do contato atual com o anterior(mesmo nivel, acima e abaixo)
						if(lastDiffP!=""){
							if(gapP || (polyEdgeP!="" && posCntsDiffP[x]+currentRules->getRule(E1DFCT) < pDif_endY-transWidthP-currentRules->getRule(E1P1DF)-currentRules->getRule(S1DFP1))){
								if(pDiffPos[x]!="")
									cpt.insertConstraint( "x" + pDiffPos[x] + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
								if(x>=1 && pDiffPos[x-1]!="")
									cpt.insertConstraint( "x" + pDiffPos[x-1] + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
								if(x<elements_it->diffCntsP.size()-1 && pDiffPos[x+1]!="")
									cpt.insertConstraint( "x" + pDiffPos[x+1] + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
								if(gapP) cpt.insertConstraint( "x" + lastDiffP + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
							}
							else
								cpt.insertConstraint("x" + pDiffPos[x] + "b", "x" + vDiff + "a", CP_MIN,  currentRules->getRule(S1CTCT)-2*currentRules->getRule(E1DFCT));
						}
						//coloca o enclosure de difusao no contato
						cpt.insertConstraint( "x" + v1 + "a", "x" + v1 + "b", CP_EQ, currentRules->getRule(W2CT));
						cpt.insertConstraint( "x" + vDiff + "a", "x" + v1 + "a", CP_EQ, currentRules->getRule(E1DFCT));
						cpt.insertConstraint( "x" + v1 + "b", "x" + vDiff + "b", CP_EQ, currentRules->getRule(E1DFCT));
						cpt.insertConstraint( "x" + vDiff + "b", "WIDTH", CP_MIN, currentRules->getRule(S1DFDF)/2);
						cpt.insertConstraint("ZERO", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFDF)/2);
						
						//coloca a cabeca de metal no contato
						geometries.push_back(&currentLayout.addEnc(*geometries.back(),currentRules->getRule(E1M1CT),MET1));
						geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->diffCntsP[x])));
						v2=intToStr(geometries.size()-1);
						cpt.insertConstraint( "x" + v2 + "a", "x" + v1 + "a", CP_EQ, currentRules->getRule(E1M1CT));
						cpt.insertConstraint( "x" + v1 + "b", "x" + v2 + "b", CP_EQ, currentRules->getRule(E1M1CT));
						
						//coloca a cabeca de metal dentro da trilha do contato sobre a difusao
						cpt.insertConstraint( "x" + pTracksPos[x] + "a", "x" + v2 + "a", CP_MIN, 0);
						cpt.insertConstraint( "x" + v2 + "b", "x" + pTracksPos[x] + "b", CP_MIN, 0);						
						pDiffPos[x]=vDiff;
					}
				}
				//se existe contatos, entao insere regra de distancia para o poly anterior caso exista
				if(limiteCntsP!= "" && polyEdgeP!=""){
					cpt.insertConstraint( "x" + polyEdgeP + "b", "x" + limiteCntsP + "a", CP_MIN, currentRules->getRule(S1CTP1));
					polyEdgeP="";
				}
				
				gapP=false;
				//			lastDiffP=diffTowerP;
				break;
			case GATE:
				//desenha o gate
                transWidthP=int(ceil((currentNetList.getTrans(elements_it->linkP.link).width*currentRules->getScale())/2)*2);
                polyWidthP=pDif_endY-transWidthP-currentRules->getRule(E1P1DF);
				if(currentRules->getIntValue(currentNetList.getTrans(elements_it->linkP.link).length)<currentRules->getRule(W2P1)) cout << "WARNING: Gate length of transistor " << currentNetList.getTrans(elements_it->linkP.link).name << " is smaller than the minimum of the technology"<< endl;
				geometries.push_back(&currentLayout.addPolygon(0, pDif_endY+currentRules->getRule(E1P1DF), 0, polyWidthP,POLY));
				v3=intToStr(geometries.size()-1);
				cpt.insertConstraint( "x" + v3 + "a", "x" + v3 + "b", CP_EQ, int(ceil((currentNetList.getTrans(elements_it->linkP.link).length*currentRules->getScale())/2)*2));                                                                                

				if(pPolyCnt!="")
					cpt.insertConstraint(  "x" + pPolyCnt + "b", "x" + v3 + "a", CP_MIN, currentRules->getRule(S1P1P1));                                                                                
				pPolyGate=v3;
				
				if(polyEdgeP!="")
					cpt.insertConstraint("x" + polyEdgeP + "b", "x" + v3 + "a", CP_MIN, currentRules->getRule(S1P1P1));
				//				else{					
				for(x=0; x<elements_it->diffCntsP.size();x++){
					if(pDiffPos[x]!="" && (posCntsDiffP[x]-currentRules->getRule(W2CT)-currentRules->getRule(E1DFCT)<pDif_endY-transWidthP || rt->areConnected(elements_it->ativeCntP,elements_it->outPolP)))
						cpt.insertConstraint( "x" + pDiffPos[x] + "b", "x" + v3 + "a", CP_MIN, currentRules->getRule(S1DFP1));
					if(posCntsDiffP[x]-currentRules->getRule(W2CT)-currentRules->getRule(E1DFCT)>=pDif_endY-transWidthP)
						cpt.insertConstraint( "x" + pCntPos[x] + "b", "x" + v3 + "a", CP_MIN, currentRules->getRule(S1CTP1));
				}
				//		}
				
				//desenha difusao
				geometries.push_back(&currentLayout.addPolygon(0,pDif_endY, 0, pDif_endY-transWidthP,PDIF));
				string vDiff=intToStr(geometries.size()-1);
				
				//if there is no contact between 2 gates and they have different widths
				if(polyEdgeP!="") {
					if(diffTowerP_width<pDif_endY-transWidthP)
				 		cpt.insertConstraint( "x" + lastDiffP + "b", "x" + v3 + "a", CP_MIN, currentRules->getRule(S1DFP1));
					if(diffTowerP_width>pDif_endY-transWidthP)
				 		cpt.insertConstraint( "x" + polyEdgeP + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFP1));
				}
				//faz a difusao começar dentro do diffTowerP anterior;
				if(diffTowerP!="")
			  	    cpt.insertConstraint( "x" + vDiff + "a", "x" + diffTowerP + "a", CP_EQ, 0);
				
				cpt.insertConstraint( "x" + vDiff + "a", "x" + v3 + "a", CP_MIN, currentRules->getRule(E1DFP1));
				cpt.insertConstraint( "x" + v3 + "b", "x" + vDiff + "b", CP_MIN, currentRules->getRule(E1DFP1));
				cpt.insertConstraint( "x" + vDiff + "a", "x" + vDiff + "b", CP_EQ, "x" + vDiff + "min");
				cpt.insertLPMinVar("x" + vDiff + "min");				
				polyEdgeP=v3;
				
				if(rt->areConnected(elements_it->ativeCntP,elements_it->interPol[0])){
					if(rt->getNrFinalArcs(elements_it->interPol[0])>1){
						geometries.push_back(&currentLayout.addPolygon(0, pDif_endY, 0,trackPos[0],POLY));

						string ponte=intToStr(geometries.size()-1);
						
						cpt.insertConstraint( "x" + ponte + "a", "x" + ponte + "b", CP_EQ,  currentRules->getRule(W2P1));					
						cpt.insertConstraint( "x" + v3 + "a", "x" + ponte + "a", CP_MIN, 0);					
						cpt.insertConstraint( "x" + ponte + "b","x" + v3 + "b", CP_MIN, 0);					
						cpt.insertConstraint( "x" + polTracksPos[0] + "a", "x" + ponte + "a", CP_MIN, 0);					
						cpt.insertConstraint( "x" + ponte + "b", "x" + polTracksPos[0] + "b", CP_MIN, 0);					
					}
				}
				//conecta no outPol
				if(rt->areConnected(elements_it->ativeCntP,elements_it->outPolP)){
					geometries.push_back(&currentLayout.addPolygon(0, 0, 0, 0, POLY));
					string ponte=intToStr(geometries.size()-1);
					
					cpt.insertConstraint( "x" + v3 + "a", "x" + ponte + "a", CP_EQ, 0);					
					cpt.insertConstraint( "x" + ponte + "b","x" + v3 + "b", CP_EQ, 0);					
					//se existe um outPol e esta conectado no anterior
					if(outPolP!="" && rt->areConnected(lastElements_it->outPolP,elements_it->outPolP))	{
						cpt.insertConstraint( "x" + ponte + "b", "x" + outPolP + "b", CP_MIN, 0);				
						cpt.insertConstraint(  "y" + ponte + "a", "y" + outPolP + "a" , CP_MIN, 0);
					}
					else{
						geometries.push_back(&currentLayout.addPolygon(0,0,0,0,POLY));
						outPolP=intToStr(geometries.size()-1);
						cpt.insertConstraint( "x" + outPolP + "a", "x" + outPolP + "b", CP_EQ, "x" + outPolP + "min");
						cpt.insertLPMinVar("x" + outPolP + "min");
					}
					cpt.insertConstraint( "ZERO", "y" + ponte + "b", CP_EQ, pDif_endY);
					cpt.insertConstraint( "y" + ponte + "a", "y" + outPolP + "a" , CP_MIN, 0);
					cpt.insertConstraint( "y" + ponte + "a", "y" + ponte + "b", CP_EQ, "x" + ponte + "min");
					cpt.insertLPMinVar("x" + ponte + "min");
					
					cpt.insertConstraint( "x" + outPolP + "a", "x" + ponte + "a", CP_MIN, 0);					
					cpt.insertConstraint( "y" + outPolP + "a", "y" + outPolP + "b", CP_EQ, currentRules->getRule(W2P1));				
				}
				else {
					//controla a posicao do outpol
					if(outPolP!="")	cpt.insertConstraint( "y" + outPolP + "b", "ZERO", CP_MIN,  -(polyWidthP-currentRules->getRule(S1P1P1)));				
					if(!rt->areConnected(elements_it->outPolP,lastElements_it->outPolP)) outPolP="";
				}
				lastDiffP=vDiff;
				gapP=false;
				break;
				
		}

		switch(elements_it->linkN.type){
			case GAP:
				gapN=true;
				break;		
			case SOURCE: 
			case DRAIN:
				diffTowerN_width=nDif_iniY;
				if(lastDiffN!="" && !gapN) diffTowerN_width+=transWidthN;
				for(x=0; x<elements_it->diffCntsN.size();x++) // desenha contatos na difusao
					if(rt->areConnected(elements_it->diffCntsN[x],elements_it->ativeCntN)){
						diffTowerN_width= max(diffTowerN_width,posCntsDiffN[x]+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT));
					}
				
				geometries.push_back(&currentLayout.addPolygon(0,nDif_iniY,0,diffTowerN_width,NDIF));
				diffTowerN=intToStr(geometries.size()-1);
				cpt.insertConstraint( "x" + diffTowerN + "a", "x" + diffTowerN + "b", CP_MIN, currentRules->getRule(W2DF)); //CORRIGIR REGRA
				//insere regra de distancia entre gate e a difftower
				if(outPolN!="")	cpt.insertConstraint( "ZERO", "y" + outPolN + "a", CP_MIN,  diffTowerN_width+currentRules->getRule(S1DFP1));				
				
                if(polyEdgeN!="" && (diffTowerN_width>nDif_iniY+transWidthN)) // || rt->areConnected(lastElements_it->ativeCntN,lastElements_it->outPolN)))
					cpt.insertConstraint( "x" + polyEdgeN + "b", "x" + diffTowerN + "a", CP_MIN, currentRules->getRule(S1DFP1));
				
				//faz a difusao anterior terminar depois da diffTower;
				if(lastDiffN!="" && !gapN){
					cpt.insertConstraint( "x" + diffTowerN + "b", "x" + lastDiffN + "b", CP_MIN, 0);
					cpt.insertConstraint( "x" + lastDiffN + "a", "x" + diffTowerN + "a", CP_MIN, 0);
				}
				
				limiteCntsN="";
				//cria boundbox para contatos
				tmp=intToStr(geometries.size()) + "_limcnt_";
				for(x=0; x<elements_it->diffCntsN.size();x++){ // desenha contatos na difusao
					if(rt->areConnected(elements_it->diffCntsN[x],elements_it->ativeCntN)){
						geometries.push_back(&currentLayout.addPolygon(0,posCntsDiffN[x]-currentRules->getRule(E1DFCT),0,posCntsDiffN[x]+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT),NDIF));
						vDiff=intToStr(geometries.size()-1);
						geometries.push_back(&currentLayout.addPolygon(0, posCntsDiffN[x], 0,posCntsDiffN[x]+currentRules->getRule(W2CT),CONT));
						v1= intToStr(geometries.size()-1);
						nCntPos[x]=v1;
						
						cpt.insertConstraint( "x" + vDiff + "a", "x" + diffTowerN + "a", CP_MIN, 0);
						cpt.insertConstraint(  "x" + diffTowerN + "b", "x" + vDiff + "b", CP_MIN, 0);
						
						//minimisa o tamanho da difusao entre contatos
						if(compacDiffN!="" && polyEdgeN!=""){
							cpt.insertConstraint("x" + v1 + "b", "x" + compacDiffN + "b", CP_MIN, 0);
							cpt.insertConstraint("x" + compacDiffN + "a", "x" + compacDiffN + "b", CP_EQ, "x" + compacDiffN + "min");
							cpt.insertLPMinVar("x" + compacDiffN + "min",gsPriority);                              
						}
						compacDiffN=intToStr(geometries.size()) + "_compacDiffN_";
				        cpt.insertConstraint("x" + compacDiffN + "a", "x" + v1 + "b", CP_MIN, 0);
						
						if(polyEdgeN!=""){
							//se contato nao estiver acima do poly anterior
							if(posCntsDiffN[x]-currentRules->getRule(E1DFCT) < polyWidthN+currentRules->getRule(S1DFP1)){
								//contato esta dentro da difusao, entao cria boundbox para contatos 
								cpt.insertConstraint( "x" + tmp + "a", "x" + v1 + "a", CP_MIN, 0);
								cpt.insertConstraint(  "x" + v1 + "b", "x" + tmp + "b", CP_MIN, 0);       
								limiteCntsN=tmp;                   
								//se contato estiver na altura do poly anterior (w do transistor)                                                                
								if(posCntsDiffN[x]+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT)>nDif_iniY+transWidthN){
									cpt.insertConstraint( "x" + polyEdgeN + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFP1));
									cpt.insertConstraint( "x" + diffTowerN + "a", "x" + diffTowerN + "b", CP_MIN, 2*currentRules->getRule(E1DFCT)+currentRules->getRule(W2CT));
								}
							}else cpt.insertConstraint( "x" + polyEdgeN + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFP1)); 							
                        }
						
	                    //regra de distancia entre difusoes do contato atual com o anterior(mesmo nivel, acima e abaixo)
						if(lastDiffN!=""){
							if(gapN ||  (polyEdgeN!="" && posCntsDiffN[x]-currentRules->getRule(E1DFCT) >= nDif_iniY+transWidthN+currentRules->getRule(E1P1DF)+currentRules->getRule(S1DFP1))){
								if(nDiffPos[x]!="")
									cpt.insertConstraint( "x" + nDiffPos[x] + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
								if(x>=1 && nDiffPos[x-1]!="")
									cpt.insertConstraint( "x" + nDiffPos[x-1] + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
								if(x<elements_it->diffCntsN.size()-1 && nDiffPos[x+1]!="")
									cpt.insertConstraint( "x" + nDiffPos[x+1] + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
								if(gapN) cpt.insertConstraint( "x" + lastDiffN + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
							}
							else
								cpt.insertConstraint("x" + nDiffPos[x] + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1CTCT)-2*currentRules->getRule(E1DFCT));
						}
						//coloca o contato a uma certa distancia da difusao
						cpt.insertConstraint( "x" + v1 + "a", "x" + v1 + "b", CP_EQ, currentRules->getRule(W2CT));
						cpt.insertConstraint( "x" + vDiff + "a", "x" + v1 + "a", CP_EQ, currentRules->getRule(E1DFCT));
						cpt.insertConstraint( "x" + v1 + "b", "x" + vDiff + "b", CP_EQ, currentRules->getRule(E1DFCT));
						cpt.insertConstraint( "x" + vDiff + "b", "WIDTH", CP_MIN, currentRules->getRule(S1DFDF)/2);
						cpt.insertConstraint("ZERO", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFDF)/2);
						
						//coloca a cabeca de metal no contato
						geometries.push_back(&currentLayout.addEnc(*geometries.back(),currentRules->getRule(E1M1CT),MET1));
						geometries.back()->setNet(currentNetList.getNetName(rt->getNet(elements_it->diffCntsN[x])));
						v2=intToStr(geometries.size()-1);
						cpt.insertConstraint( "x" + v2 + "a", "x" + v1 + "a", CP_EQ, currentRules->getRule(E1M1CT));
						cpt.insertConstraint( "x" + v1 + "b", "x" + v2 + "b", CP_EQ, currentRules->getRule(E1M1CT));
						
						//coloca a cabeca de metal dentro da trilha do contato sobre a difusao
						cpt.insertConstraint( "x" + nTracksPos[x] + "a", "x" + v2 + "a", CP_MIN, 0);
						cpt.insertConstraint( "x" + v2 + "b", "x" + nTracksPos[x] + "b", CP_MIN, 0);						
						nDiffPos[x]=vDiff;
					}
				}
				//se existe contatos, entao insere regra de distancia para o poly anterior caso exista
				if(limiteCntsN!= "" && polyEdgeN!=""){
					cpt.insertConstraint( "x" + polyEdgeN + "b", "x" + limiteCntsN + "a", CP_MIN, currentRules->getRule(S1CTP1));
					polyEdgeN="";
				}
				gapN=false;
				//			lastDiffN=diffTowerN;
				break;
			case GATE:
				//desenha o gate
				transWidthN=int(ceil((currentNetList.getTrans(elements_it->linkN.link).width*currentRules->getScale())/2)*2);
				polyWidthN=nDif_iniY+transWidthN+currentRules->getRule(E1P1DF);
				geometries.push_back(&currentLayout.addPolygon(0, nDif_iniY-currentRules->getRule(E1P1DF), 0, polyWidthN,POLY));
				v3=intToStr(geometries.size()-1);
				if(currentRules->getIntValue(currentNetList.getTrans(elements_it->linkN.link).length)<currentRules->getRule(W2P1)) cout << "WARNING: Gate length of transistor " << currentNetList.getTrans(elements_it->linkN.link).name << " is smaller than the minimum of the technology"<< endl;
				cpt.insertConstraint( "x" + v3 + "a", "x" + v3 + "b", CP_EQ,int(ceil((currentNetList.getTrans(elements_it->linkN.link).length*currentRules->getScale())/2)*2));

				if(nPolyCnt!="")
					cpt.insertConstraint(  "x" + nPolyCnt + "b", "x" + v3 + "a", CP_MIN, currentRules->getRule(S1P1P1));                                                                                
				nPolyGate=v3;
				
				if(polyEdgeN!="")
					cpt.insertConstraint("x" + polyEdgeN + "b", "x" + v3 + "a", CP_MIN, currentRules->getRule(S1P1P1));
				//else{					
				for(x=0; x<elements_it->diffCntsN.size();x++){
					if(nDiffPos[x]!="" && (posCntsDiffN[x]+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT)>nDif_iniY+transWidthN || rt->areConnected(elements_it->ativeCntN,elements_it->outPolN)))
						cpt.insertConstraint( "x" + nDiffPos[x] + "b", "x" + v3 + "a", CP_MIN, currentRules->getRule(S1DFP1));
					if(posCntsDiffN[x]+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT)<=nDif_iniY+transWidthN)
						cpt.insertConstraint( "x" + nCntPos[x] + "b", "x" + v3 + "a", CP_MIN, currentRules->getRule(S1CTP1));
					//	}
				}
				
				//desenha difusao
				geometries.push_back(&currentLayout.addPolygon(0,nDif_iniY, 0, nDif_iniY+transWidthN,NDIF));
				string vDiff=intToStr(geometries.size()-1);
				
				//if there is no contact between 2 gates and they have different widths
				if(polyEdgeN!="") {
					if(diffTowerN_width>nDif_iniY+transWidthN)
				 		cpt.insertConstraint( "x" + lastDiffN + "b", "x" + v3 + "a", CP_MIN, currentRules->getRule(S1DFP1));
					if(diffTowerN_width<nDif_iniY+transWidthN)
				 		cpt.insertConstraint( "x" + polyEdgeN + "b", "x" + vDiff + "a", CP_MIN, currentRules->getRule(S1DFP1));
				}
				
				//faz a difusao começar dentro do diffTowerN anterior;
				if(diffTowerN!="")
			  	    cpt.insertConstraint( "x" + vDiff + "a", "x" + diffTowerN + "a", CP_EQ, 0);
				
				cpt.insertConstraint( "x" + vDiff + "a", "x" + v3 + "a", CP_MIN, currentRules->getRule(E1DFP1));
				cpt.insertConstraint( "x" + v3 + "b", "x" + vDiff + "b", CP_MIN, currentRules->getRule(E1DFP1));
				cpt.insertConstraint( "x" + vDiff + "a", "x" + vDiff + "b", CP_EQ, "x" + vDiff + "min");
				cpt.insertLPMinVar("x" + vDiff + "min");
				
				polyEdgeN=v3;
				
				string ponte;
				if(rt->areConnected(elements_it->ativeCntN,elements_it->interPol[nrTracks-1])){
					if(rt->getNrFinalArcs(elements_it->interPol[nrTracks-1])>1){
						geometries.push_back(&currentLayout.addPolygon(0, nDif_iniY, 0,trackPos[nrTracks-1],POLY));
						ponte=intToStr(geometries.size()-1);
						
						cpt.insertConstraint( "x" + ponte + "a", "x" + ponte + "b", CP_EQ,  currentRules->getRule(W2P1));					
						cpt.insertConstraint( "x" + v3 + "a", "x" + ponte + "a", CP_MIN, 0);					
						cpt.insertConstraint( "x" + ponte + "b","x" + v3 + "b", CP_MIN, 0);					
						cpt.insertConstraint( "x" + polTracksPos[nrTracks-1] + "a", "x" + ponte + "a", CP_MIN, 0);					
						cpt.insertConstraint( "x" + ponte + "b", "x" + polTracksPos[nrTracks-1] + "b", CP_MIN, 0);					
					}
				}
				//conecta no outPol
				if(rt->areConnected(elements_it->ativeCntN,elements_it->outPolN)){
					geometries.push_back(&currentLayout.addPolygon(0, 0, 0, 0, POLY));
					ponte=intToStr(geometries.size()-1);
					
					cpt.insertConstraint( "x" + v3 + "a", "x" + ponte + "a", CP_EQ, 0);					
					cpt.insertConstraint( "x" + ponte + "b","x" + v3 + "b", CP_EQ, 0);					
					
					if(outPolN!="" && rt->areConnected(lastElements_it->outPolN,elements_it->outPolN))	{
						cpt.insertConstraint( "x" + ponte + "b", "x" + outPolN + "b", CP_MIN, 0);				
						cpt.insertConstraint( "y" + outPolN + "b", "y" + ponte + "b", CP_MIN, 0);
					}
					else{
						geometries.push_back(&currentLayout.addPolygon(0,0,0,0,POLY));
						outPolN=intToStr(geometries.size()-1);
						cpt.insertConstraint( "x" + outPolN + "a", "x" + outPolN + "b", CP_EQ, "x" + outPolN + "min");
						cpt.insertLPMinVar("x" + outPolN + "min");
					}
					cpt.insertConstraint( "ZERO", "y" + ponte + "a", CP_EQ, nDif_iniY);
					cpt.insertConstraint(  "y" + outPolN + "b" ,"y" + ponte + "b", CP_MIN, 0);
					cpt.insertConstraint( "y" + ponte + "a", "y" + ponte + "b", CP_EQ, "y" + ponte + "min");
					cpt.insertLPMinVar("y" + ponte + "min");
					
					cpt.insertConstraint( "x" + outPolN + "a", "x" + ponte + "a", CP_MIN, 0);					
					cpt.insertConstraint( "y" + outPolN + "a", "y" + outPolN + "b", CP_EQ, currentRules->getRule(W2P1));	
				}
				else {
					//controla a posicao do outpol
					if(outPolN!="")	cpt.insertConstraint( "ZERO", "y" + outPolN + "a", CP_MIN,  polyWidthN+currentRules->getRule(S1P1P1));				
					if(!rt->areConnected(elements_it->outPolN,lastElements_it->outPolN)) outPolN="";
				}
 
				lastDiffN=vDiff;
				gapN=false;
				break;
		}
		
		lastElements_it=elements_it;
		lastMetTracksPos=metTracksPos;
		lastPolTracksPos=polTracksPos;
		lastPTracksPos=pTracksPos;		
		lastNTracksPos=nTracksPos;		
	}
	
	cpt.insertConstraint( "ZERO", "WIDTH", CP_MIN, 0);					
	cpt.insertConstraint( "ZERO", "WIDTH", CP_EQ_VAR_VAL,  "WIDTH_gpos", hGrid);
	cpt.forceIntegerVar("WIDTH_gpos");
	cpt.insertLPMinVar("WIDTH",wPriority);
	
	if(!cpt.solve(lpSolverFile))
		return false;
	
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
	
	//Aqui duplica as camadas de M1 relacionadas aos pinos de I/O
	list <Box>::iterator net_it;
	for ( net_it = currentLayout.layers[MET1].begin(); net_it != currentLayout.layers[MET1].end(); net_it++ )
		if(currentNetList.isIO(net_it->getNet())) currentLayout.addEnc(*net_it,  0 , MET1P); 
	
	for (map<string,int>::iterator IOgeometries_it=IOgeometries.begin(); IOgeometries_it != IOgeometries.end(); IOgeometries_it++ ) {
		Pin p;
		p.setX(geometries[IOgeometries_it->second]->getX());
		p.setY(geometries[IOgeometries_it->second]->getY());
		p.setLayer(MET1P);
		currentLayout.setPin(IOgeometries_it->first,p);
		currentLayout.addLabel(IOgeometries_it->first,p);
	}
	
	list <Box>::iterator layer_it;
	for ( layer_it = currentLayout.layers[PDIF].begin(); layer_it != currentLayout.layers[PDIF].end(); layer_it++ )
		&currentLayout.addEnc(*layer_it,  currentRules->getRule(E1IPDF) , PSEL);
	for ( layer_it = currentLayout.layers[NDIF].begin(); layer_it != currentLayout.layers[NDIF].end(); layer_it++ )
		&currentLayout.addEnc(*layer_it,  currentRules->getRule(E1INDF) , NSEL);

	int width=cpt.getVariableVal("WIDTH");

	bool btP=false, btN=false;
	list<int> btIntervalsP,btIntervalsN;
	btIntervalsP.push_back(currentRules->getIntValue(0.45));
	btIntervalsP.push_back(width-currentRules->getIntValue(0.45));
	btIntervalsN=btIntervalsP;
	for ( layer_it = currentLayout.layers[PDIF].begin(); layer_it != currentLayout.layers[PDIF].end(); layer_it++ ){
		if(layer_it->getY1() < currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT)+currentRules->getRule(S1DFDF)){
			for(list<int>::iterator btIntervals_it=btIntervalsP.begin(); btIntervals_it!=btIntervalsP.end(); ++btIntervals_it){
				list<int>::iterator ini, end;
				ini=btIntervals_it;
				end=++btIntervals_it;
				if(layer_it->getX1()>*ini && layer_it->getX2()<*end){
					btIntervalsP.insert(end, layer_it->getX1());
					btIntervalsP.insert(end, layer_it->getX2());
				}
				else if(layer_it->getX1()<*ini && layer_it->getX2()>*end){
					btIntervalsP.erase(ini);
					btIntervalsP.erase(end);
				}				
				else if(layer_it->getX1()<*ini && layer_it->getX2()>*ini) 
					*ini=layer_it->getX2();
				else if(layer_it->getX1()<*end && layer_it->getX2()>*end) 
					*end=layer_it->getX1();
			}
		}
	}
	for(list<int>::iterator btIntervals_it=btIntervalsP.begin(); btIntervals_it!=btIntervalsP.end(); ++btIntervals_it){
		list<int>::iterator ini, end;
		ini=btIntervals_it;
		end=++btIntervals_it;
		if(*ini+currentRules->getRule(W2CT)+2*currentRules->getRule(E1DFCT)+2*currentRules->getRule(S1DFDF)<=*end){
			btP=true;
			currentLayout.addPolygon(*ini+currentRules->getRule(S1DFDF)-currentRules->getRule(E1INDF), 0, *end-currentRules->getRule(S1DFDF)+currentRules->getRule(E1INDF), currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT)+currentRules->getRule(E1INDF), NSEL);
			currentLayout.addPolygon(*ini+currentRules->getRule(S1DFDF), 0, *end-currentRules->getRule(S1DFDF), currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT), NDIF);
			for(int cnt=*ini+currentRules->getRule(S1DFDF)+currentRules->getRule(E1DFCT); cnt<=*end-currentRules->getRule(S1DFDF)-currentRules->getRule(E1DFCT)-currentRules->getRule(W2CT); cnt+=currentRules->getRule(W2CT)+currentRules->getRule(S1CTCT))
				currentLayout.addPolygon(cnt, currentRules->getRule(S1CTCT)/2, cnt+currentRules->getRule(W2CT), currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT), CONT);				
		}
	}
	
	for ( layer_it = currentLayout.layers[NDIF].begin(); layer_it != currentLayout.layers[NDIF].end(); layer_it++ ){
		if(layer_it->getY2() > height-(currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT)+currentRules->getRule(S1DFDF))){
			for(list<int>::iterator btIntervals_it=btIntervalsN.begin(); btIntervals_it!=btIntervalsN.end(); ++btIntervals_it){
				list<int>::iterator ini, end;
				ini=btIntervals_it;
				end=++btIntervals_it;
				if(layer_it->getX1()>*ini && layer_it->getX2()<*end){
					btIntervalsN.insert(end, layer_it->getX1());
					btIntervalsN.insert(end, layer_it->getX2());
				}
				else if(layer_it->getX1()<*ini && layer_it->getX2()>*end){
					btIntervalsN.erase(ini);
					btIntervalsN.erase(end);
				}				
				else if(layer_it->getX1()<*ini && layer_it->getX2()>*ini) 
					*ini=layer_it->getX2();
				else if(layer_it->getX1()<*end && layer_it->getX2()>*end) 
					*end=layer_it->getX1();
			}
		}
	}
	for(list<int>::iterator btIntervals_it=btIntervalsN.begin(); btIntervals_it!=btIntervalsN.end(); ++btIntervals_it){
		list<int>::iterator ini, end;
		ini=btIntervals_it;
		end=++btIntervals_it;
		if(*ini+currentRules->getRule(W2CT)+2*currentRules->getRule(E1DFCT)+2*currentRules->getRule(S1DFDF)<=*end){
			btN=true;
			currentLayout.addPolygon(*ini+currentRules->getRule(S1DFDF)-currentRules->getRule(E1INDF), height-(currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT)+currentRules->getRule(E1INDF)), *end-currentRules->getRule(S1DFDF)+currentRules->getRule(E1INDF), height, PSEL);
			currentLayout.addPolygon(*ini+currentRules->getRule(S1DFDF), height-(currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT)), *end-currentRules->getRule(S1DFDF), height, PDIF);
			for(int cnt=*ini+currentRules->getRule(S1DFDF)+currentRules->getRule(E1DFCT); cnt<=*end-currentRules->getRule(S1DFDF)-currentRules->getRule(E1DFCT)-currentRules->getRule(W2CT); cnt+=currentRules->getRule(W2CT)+currentRules->getRule(S1CTCT))
				currentLayout.addPolygon(cnt, height-(currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)), cnt+currentRules->getRule(W2CT), height-currentRules->getRule(S1CTCT)/2, CONT);				
		}
	}
	
	if(!btP) cout << "Could not insert bodye ties to the P transistors" << endl;
	if(!btN) cout << "Could not insert bodye ties to the N transistors" << endl;

	currentLayout.setWidth(width);
	currentLayout.setHeight(height);
	
	//Draw supply strips
	
	currentLayout.addPath(0, 0, currentRules->getRule(S1IPIP), width, false, E, NSEL);
	currentLayout.addPath(0, height, currentRules->getRule(S1IPIP), width, false, E, PSEL);
		
	currentLayout.addPolygon(0, currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT)+currentRules->getRule(S1DFDF)-currentRules->getRule(E1INDF), width, pDif_endY+currentRules->getRule(E1IPDF), PSEL);
	currentLayout.addPolygon(0, height-(currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E1DFCT)+currentRules->getRule(S1DFDF)-currentRules->getRule(E1INDF)), width, nDif_iniY-currentRules->getRule(E1INDF), NSEL);
	
	currentLayout.addPolygon(0, 0, width, supWidth, MET1).setNet(currentCircuit->getVddNet());
	currentLayout.addPolygon(0, 0, width, supWidth, MET1P);
	currentLayout.addPolygon(0, height-supWidth, width, height, MET1).setNet(currentCircuit->getGndNet());
	currentLayout.addPolygon(0, height-supWidth, width, height, MET1P);
	currentLayout.addPolygon(0, 0, width, height, CELLBOX);
	
	currentLayout.addPolygon(currentRules->getRule(S1DFDF)/2-currentRules->getRule(E1WNDP), currentRules->getRule(S1ININ)/2+currentRules->getRule(E1INDF)-currentRules->getRule(S1DNWN), width-currentRules->getRule(S1DFDF)/2+currentRules->getRule(E1WNDP), pDif_endY+currentRules->getRule(E1WNDP), NWEL);
		
//	currentLayout.merge();
	currentCircuit->insertLayout(currentLayout);
	cout << "Cell Size (W x H): " << float(currentLayout.getWidth())/currentRules->getScale() << " x " << float(currentLayout.getHeight())/currentRules->getScale() << endl;
	state=6;
	return state==6;
}

void AutoCell::showIOCost(){	
	for(list<Element>::iterator elements_it=elements.begin(); elements_it!=elements.end(); elements_it++){
		elements_it->print();
	}
}
