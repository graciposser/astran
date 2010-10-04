#ifndef COMBINATION_H
#define	COMBINATION_H

#include <vector>
	using std::vector;
#include <sstream>
	using std::ostringstream;
#include <string>
	using std::string;

// -----------------------------------------------------------------------------
// Generates binary combination vectors. For instance, a Combiantion object with
// 2 elements generates:
//
// 0 0
// 0 1
// 1 0
// 1 1
//

class Combination {
	private:
		vector<bool> clsVector;
	public:

		void initialize( const int numElements ) {
			clsVector.clear();
			clsVector.resize( numElements, false );
		} // end method

		// Generate the next combination. Return false if all combinations have
		// been generated (i.e. we are back to 0).
		bool next() {
			for ( int i = 0; i < clsVector.size(); i++ ) {
				if ( clsVector[i] = !clsVector[i] ) // :)
					return true;
			} // end for
			return false;
		} // end method

		// Printable version.
		string toString( const string &separator="" ) const {
			ostringstream oss;
			for ( int i = 0; i < getNumElements(); i++ )
				oss << getElement(i) << separator;
			return oss.str();
		} // end method

		// Gets
		int getNumElements() const { return clsVector.size(); }
		bool getElement( const int i ) const { return clsVector[i]; }
}; // end class

#endif
