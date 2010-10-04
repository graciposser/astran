#ifndef RCTRANSLATOR_H
#define	RCTRANSLATOR_H

#include <vector>
	using std::vector;
#include <map>
	using std::map;
#include <string>
	using std::string;
#include <exception>
	using std::exception;

#include "Combination.h"

// -----------------------------------------------------------------------------

class RCTranslatorException : public exception {
	private:
		string clsMsg;
	public:
		RCTranslatorException( const string &msg ) : clsMsg(msg) {}

		virtual ~RCTranslatorException() throw() {}
		virtual const char* what() const throw() { return clsMsg.c_str(); }
}; // end class

	
// -----------------------------------------------------------------------------

class RCTranslator {
	public:

		enum TransistorType { PMOS, NMOS };
		enum Voltage { GND, VDD, FLOATING };

	private:

		static const string ERROR_MSG_MODIFY_AFTER_INITIALIZED;
		static const string ERROR_MSG_NOT_INITIALIZED;

		struct Transistor {
			string propName;
			TransistorType propType;
			int propSource;
			int propGate;
			int propDrain;
			float propWidth;

			Transistor() {}
			Transistor( const string &name, const TransistorType type, const int source, const int gate, const int drain, const float &width ) {
				propName = name;
				propType = type;
				propSource = source;
				propGate = gate;
				propDrain = drain;
				propWidth = width;
			} // end method

			void teste();
		}; // end struct

		struct Node {
			string propName;
			bool propIsInput;
			bool propIsOutput;
			vector<int> propTrans;   // Stores the transistors connected via source or drain to this node.
			vector<int> propTrigger; // Stores the transistors connected via gate to this node.

			Node() {}
			Node( const string &name ) {
				propName = name;
				propIsInput = false;
				propIsOutput = false;
			} // end method

			void addTrans( const int trans ) {
				propTrans.push_back(trans);
			} // end method

			void addTrigger( const int trans ) {
				propTrigger.push_back(trans);
			} // end method
		}; // end struct

		// Circuit Graph: Nets (graph nodes) and transistors (graph edges).
		vector<Transistor> clsTransistors;
		vector<Node> clsNodes;

		// Circuit I/Os
		vector<int> clsInputs;
		vector<int> clsOutputs;

		int clsGND;
		int clsVDD;

		// Mapping for translating nets/transistor names to indexes.
		map<string,int> clsNodeIndexes;
		map<string,int> clsTransistorIndexes;

		// Auxiliaries methods.
		int findOrCreateNodeIndex( const string &name );

		// Check consistency to perform the translation. Throws a exception if
		// something goes wrong.
		void checkConsistency();

		//
		// Current state information.
		//

		bool clsInitialized;
		int clsRCTreeCounter;

		Combination clsInputVector;
		vector<Voltage> clsNodeStates;
		vector<int> clsNodeParents;
		vector< vector<int> > clsDownstreamNodes;
		vector< vector<int> > clsUpstreamNodes;

		//
		// Auxiliaries properties and methods for perfoming the translation.
		//

		// Defines node state (vdd or gnd).
		void defineNodeStates();
		
		// Defines node parents. Returns false if a loop is detected, true
		// otherwise.
		bool defineNodeParents( const int root );

		// Defines downstream nodes of each node.
		void defineNodeDownstream( const int root );
		
		// Defines upstream path (path from a node to RC root).
		void defineNodeUpstreamPath( const int root );

		bool isTransistorOn( const int transId ) const;

		void statePropagation( const int sourceNodeId, const Voltage voltage );

		bool step();

	public:

		RCTranslator();

		void AddInput( const string &nodename );
		void AddOutput( const string &nodename );
		void AddTransistor(const string &name, const TransistorType type, const string &source, const string &gate, const string &drain, const float &width );

		void SetGND( const string &nodename );
		void SetVDD( const string &nodename );

		void initialize();
		bool next();
		
		// Returns the oposite transistor terminal. If source = nodeid, returns
		// drain. If drain = nodeid, returns source. Throws an exception
		// otherwise.
		int oppositeTransistorTerminal( const int transId, const int nodeId ) const;		

		// Gets
		const Combination &getInputVector() const { return clsInputVector; }
		int getRCTreeCounter() const { return clsRCTreeCounter; }
		
		const string &getNodeName( const int nodeId ) const { return clsNodes[nodeId].propName; }
		Voltage getNodeState( const int nodeId ) const { return clsNodeStates[nodeId]; }
		int getDriverTransistor( const int nodeId ) const { return clsNodeParents[nodeId]; }
		const vector<int> &getDownstreamNodes( const int nodeId ) const { return clsDownstreamNodes[nodeId]; }
		const vector<int> &getUpstreamNodes( const int nodeId ) const { return clsUpstreamNodes[nodeId]; }
		const vector<int> &getConnectedTransistors( const int nodeId ) const { return clsNodes[nodeId].propTrans; }

		const string &getTransistorName( const int transId ) const { return clsTransistors[transId].propName; }
		TransistorType getTransistorType( const int transId ) const { return clsTransistors[transId].propType; }
		const int getTransistorGate( const int transId ) const { return clsTransistors[transId].propGate; }
		const int getTransistorDrain( const int transId ) const { return clsTransistors[transId].propDrain; }
		const int getTransistorSource( const int transId ) const { return clsTransistors[transId].propSource; }
		const float getTransistorWidth( const int transId ) const { return clsTransistors[transId].propWidth; }
		
		int getOutputNode() const { return clsOutputs.back(); } // guessing we have only one output :)
		
		int getNumNodes() const { return clsNodes.size(); }
		int getNumTransistors() const { return clsTransistors.size(); }
		
		// Ises
		bool isInput( const int n ) const { return clsNodes[n].propIsInput; }

}; // end class

#endif

