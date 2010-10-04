#include <queue>
	using std::queue;
#include <algorithm>
	using std::find;
#include <iostream>
	using std::cerr;
	using std::cerr;

#include "RCTranslator.h"

const string RCTranslator::ERROR_MSG_MODIFY_AFTER_INITIALIZED =
	"You cannot change the RC network after calling initialize().";
const string RCTranslator::ERROR_MSG_NOT_INITIALIZED =
	"You must call initialize() before calling next().";

// -----------------------------------------------------------------------------

RCTranslator::RCTranslator() {
	clsGND = -1;
	clsVDD = -1;

	clsInitialized = false;
	clsRCTreeCounter = 0;
} // end if

// -----------------------------------------------------------------------------

int RCTranslator::findOrCreateNodeIndex( const string &name ) {
	int index = -1;

	map<string,int>::iterator it = clsNodeIndexes.find(name);
	if ( it != clsNodeIndexes.end() )
		index = it->second;
	else {
		index = clsNodes.size();

		clsNodeIndexes[ name ] = index;
		clsNodes.push_back( Node( name ) );
	} // end else

	return index;
} // end method

// -----------------------------------------------------------------------------

void RCTranslator::AddTransistor(const string &name, const TransistorType type, const string& source, const string& gate, const string& drain, const float &width ) {
	if ( clsInitialized )
		throw RCTranslatorException( ERROR_MSG_MODIFY_AFTER_INITIALIZED );

	const int sourceNodeId = findOrCreateNodeIndex( source );
	const int gateNodeId = findOrCreateNodeIndex( gate );
	const int drainNodeId = findOrCreateNodeIndex( drain );
	//const double widthNodeId = findOrCreateNodeIndex( width );

	// Check if the transistor already exists.
	map<string,int>::iterator it = clsTransistorIndexes.find(name);
	if ( it != clsTransistorIndexes.end() )
		throw RCTranslatorException( "Transistor already exists." );

	// Create the transistor.
	const int transId = clsTransistors.size();
	clsTransistorIndexes[ name ] = transId;
	clsTransistors.push_back( Transistor(name, type, sourceNodeId, gateNodeId, drainNodeId, width) );

	// Connectivity.
	clsNodes[sourceNodeId].addTrans( transId );
	clsNodes[drainNodeId].addTrans( transId );

	clsNodes[gateNodeId].addTrigger( transId );
} // end method

// -----------------------------------------------------------------------------

void RCTranslator::AddInput( const string &nodename ) {
	if ( clsInitialized )
		throw RCTranslatorException( ERROR_MSG_MODIFY_AFTER_INITIALIZED );

	const int index = findOrCreateNodeIndex(nodename);
	if ( !clsNodes[index].propIsInput ) {
		clsNodes[index].propIsInput = true;
		clsInputs.push_back(index);
	} // end if
} // end method

// -----------------------------------------------------------------------------

void RCTranslator::AddOutput( const string &nodename ) {
	if ( clsInitialized )
		throw RCTranslatorException( ERROR_MSG_MODIFY_AFTER_INITIALIZED );

	const int index = findOrCreateNodeIndex(nodename);
	if ( !clsNodes[index].propIsOutput ) {
		clsNodes[index].propIsOutput = true;
		clsOutputs.push_back(index);
	} // end if
} // end method

// -----------------------------------------------------------------------------

void RCTranslator::SetGND( const string &nodename ) {
	if ( clsInitialized )
		throw RCTranslatorException( ERROR_MSG_MODIFY_AFTER_INITIALIZED );

	if ( clsGND != -1 )
		throw RCTranslatorException( "Trying to redefine GND node." );

	clsGND = findOrCreateNodeIndex(nodename);
} // end method

// -----------------------------------------------------------------------------

void RCTranslator::SetVDD( const string &nodename ) {
	if ( clsInitialized )
		throw RCTranslatorException( ERROR_MSG_MODIFY_AFTER_INITIALIZED );

	if ( clsVDD != -1 )
		throw RCTranslatorException( "Trying to redefine VDD node." );

	clsVDD = findOrCreateNodeIndex(nodename);
} // end method

// -----------------------------------------------------------------------------

void RCTranslator::checkConsistency() {
	// We can't deal with cells with more than one components (e.g. if there is
	// a node which is not input connected to a transistor gate).
	for ( int i = 0; i < clsTransistors.size(); i++ ) {
		if ( find( clsInputs.begin(), clsInputs.end(),  clsTransistors[i].propGate ) == clsInputs.end() )
			throw RCTranslatorException( "Only input nodes (nets) should be connected to transistor gates." );
	} // end for

	// Only one input is supported.
	if ( clsOutputs.size() > 1 )
		throw RCTranslatorException( "Only cells with one output are supported." );

	// The cell must have at least one input and one output.
	if ( clsInputs.size() == 0 )
		throw RCTranslatorException( "Cell must have at least one input." );

	if ( clsOutputs.size() == 0 )
		throw RCTranslatorException( "Cell must have at least one output." );
} // end method

// -----------------------------------------------------------------------------

bool RCTranslator::isTransistorOn( const int transId ) const {
	const Transistor &trans = clsTransistors[transId];

	switch ( trans.propType ) {
		case NMOS: return clsNodeStates[trans.propGate] == VDD;
		case PMOS: return clsNodeStates[trans.propGate] == GND;

		default:
			throw RCTranslatorException( "Invalid transistor type!" );
	} // end switch
} // end method

// -----------------------------------------------------------------------------

int RCTranslator::oppositeTransistorTerminal( const int transId, const int nodeId ) const {
	const Transistor &trans = clsTransistors[transId];
	
	if ( nodeId == trans.propSource ) 
		return trans.propDrain;
		
	if ( nodeId == trans.propDrain )
		return trans.propSource;
		
	throw RCTranslatorException( "Node does not connected to source or drain." );
} // end method

// -----------------------------------------------------------------------------

void RCTranslator::statePropagation( const int sourceNodeId, const Voltage voltage ) {
	queue<int> opened;
	vector<bool> visited( clsNodes.size(), false );

	opened.push( sourceNodeId );
	while ( !opened.empty() ) {
		int n = opened.front();
		opened.pop();

		if ( visited[n] )
			continue;

		visited[n] = true;

		if ( clsNodeStates[n] == FLOATING )
			clsNodeStates[n] = voltage;
		else
			throw RCTranslatorException( "Short circuit!" );

		const Node &node = clsNodes[n];
		for ( int i = 0; i < node.propTrans.size(); i++ ) {
			const int transId = node.propTrans[i];

			if ( isTransistorOn( transId ) ) {
				const Transistor &trans = clsTransistors[transId];

				opened.push( trans.propDrain );
				opened.push( trans.propSource );
			} // end if
		} // end for
	} // end while
} // end method

// -----------------------------------------------------------------------------

void RCTranslator::defineNodeStates() {
	clsNodeStates.clear();
	clsNodeStates.resize( clsNodes.size(), FLOATING );

	// Define the state of each input node.
	for ( int i = 0; i < clsInputs.size(); i++ )
		clsNodeStates[ clsInputs[i] ] = ( clsInputVector.getElement(i) ) ? VDD : GND;

	// Propagate the node state.
	statePropagation( clsVDD, VDD );
	statePropagation( clsGND, GND );
} // end method

// -----------------------------------------------------------------------------

bool RCTranslator::defineNodeParents( const int root ) {
	clsNodeParents.clear();
	clsNodeParents.resize( clsNodes.size(), -1 );

	queue<int> opened;
	vector<bool> cross( clsTransistors.size(), false ); // transistors that already have been cross.

	opened.push( root );
	while ( !opened.empty() ) {
		int n = opened.front();
		opened.pop();

		//cerr << "Expanding node " << clsNodes[n].propName << "\n";

		const Node &node = clsNodes[n];
		for ( int i = 0; i < node.propTrans.size(); i++ ) {
			const int transId = node.propTrans[i];

			if ( !cross[transId] && isTransistorOn( transId ) ) {
				const Transistor &trans = clsTransistors[transId];
				const int neighbour = trans.propDrain == n ? trans.propSource : trans.propDrain;

				//cerr << "\tNeighbour: " << trans.propName << "\t" << clsNodes[neighbour].propName << "\n";

				if ( clsNodeParents[neighbour] != -1 )
					return false;

				clsNodeParents[neighbour] = transId;
				opened.push( neighbour );
			} // end if

			cross[transId] = true;
		} // end for
	} // end while

	//cerr << "\n\n";

	return true;
} // end method

// -----------------------------------------------------------------------------

void RCTranslator::defineNodeDownstream( const int root ) {
	const int numNodes = clsNodes.size();

	clsDownstreamNodes.clear();
	clsDownstreamNodes.resize( numNodes );
	
	for ( int k = 0; k < numNodes; k++ ) {
		if ( clsNodeParents[k] == -1 )
			continue;

		int n = k;
		while ( n != root ) {
			clsDownstreamNodes[n].push_back(k);
			n = oppositeTransistorTerminal( clsNodeParents[n], n );
		} // end while
	} // end for
} // end method

// -----------------------------------------------------------------------------

void RCTranslator::defineNodeUpstreamPath( const int root ) {
	const int numNodes = clsNodes.size();

	clsUpstreamNodes.clear();
	clsUpstreamNodes.resize( numNodes );
		
	for ( int k = 0; k < numNodes; k++ ) {
		if ( clsNodeParents[k] == -1 )
			continue;

		int n = k;
		while ( n != root ) {
			clsUpstreamNodes[k].push_back(n);
			n = oppositeTransistorTerminal( clsNodeParents[n], n );
		} // end while
	} // end for
} // end method

// -----------------------------------------------------------------------------

bool RCTranslator::step() {
	defineNodeStates();

	// Debug
	//cerr << "\n\nTruth Table\n";
	//cerr << clsInputVector.toString() << "\t" << clsNodeStates[ clsOutputs.front() ] << "\n";

	const int output = clsOutputs.front();
	if ( clsNodeStates[output] == FLOATING )
		throw RCTranslatorException( "Floating output for input " + clsInputVector.toString() );

	const int root = clsNodeStates[output] == VDD ? clsVDD : clsGND;
	if ( defineNodeParents( root ) ) {
		defineNodeDownstream( root );
		defineNodeUpstreamPath( root );

		/*
		cerr << "RC Tree for input vector " << clsInputVector.toString() << "\n";
		for ( int k = 0; k < clsNodes.size(); k++ ) {
			cerr << "\tNode: " << clsNodes[k].propName << "\n";
			if ( clsNodeParents[k] == -1 ) {
				cerr << "\t\tSkiping... Floating node.\n";
			} else {
				cerr << "\t\tDriver: " << clsNodes[clsNodeParents[k]].propName << "\n";
				cerr << "\t\tDownstream Cap:";
				for ( int i = 0; i < clsDownstreamNodes[k].size(); i++ ) {
					cerr << " " << clsNodes[clsDownstreamNodes[k][i]].propName;
				} // end for
				cerr << "\n";
			} // end else
		} // end for
		*/

		return true;
	} else {
		//cerr << "\t[INFO] Input vector " + clsInputVector.toString() + " generates a RC loop.\n";
		return false;
	} // end else
} // end method

// -----------------------------------------------------------------------------

bool RCTranslator::next() {
	if ( !clsInitialized )
		throw RCTranslatorException( ERROR_MSG_NOT_INITIALIZED );

	if ( clsRCTreeCounter > 0 )
		if ( !clsInputVector.next() )
			return false;

	while ( !step() ) {
		if ( !clsInputVector.next() )
			return false;
	} // end while

	clsRCTreeCounter++;
	return true;
} // end method

// -----------------------------------------------------------------------------

void RCTranslator::initialize() {
	checkConsistency();
	clsInputVector.initialize( clsInputs.size() );

	clsInitialized = true;
} // end method
