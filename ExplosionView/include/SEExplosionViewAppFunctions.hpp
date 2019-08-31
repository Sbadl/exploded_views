#pragma once
#include "SAMSON.hpp"
#include "SBMStructuralModel.hpp"
#include "SBMStructuralModelNodeRoot.hpp"


/*********************************************************************************************************************/
/* EASE FUNCTIONS																									 */
/*********************************************************************************************************************/
/*Ease Functions
Sources:
http://upshots.org/actionscript/jsas-understanding-easing
http://www.gizma.com/easing/

t is the current time(or position) of the tween.
b is the beginning value of the property.
c is the change between the beginning and destination value of the property.
d is the total time of the tween.
*/
namespace ease {
	double cubicEaseIn(double t, int b, int c, int d);

	double cubicEaseOut(double t, int b, int c, int d);

	double cubicEaseInOut(double t, int b, int c, int d);

	double handleEase(double explosionValue, int intervalMin, int intervalMax, int easeIndex);

}


/*********************************************************************************************************************/
/* VECTOR OEPRATIONS																								 */
/*********************************************************************************************************************/
namespace vectorOps {

	//constants for simpler use
	const SBQuantity::length Ql_1 = SBQuantity::length(1.0);
	const SBQuantity::length Ql_0 = SBQuantity::length(0.0);

	double calcLengthBetweenTwoVectors(SBPosition3 a, SBPosition3 b);
	SBPosition3	calcCentre(SBNodeIndexer nodeIndexer);									///< calculates the centre of the nodes in nodeIndexer
	SBQuantity::length calcVectorLength(SBPosition3 p);									///< calculates the distance between p and the origin

	void moveModel(SBPosition3 moveVector, SBNodeIndexer indexer);						///< moves all Atoms in the indexer by adding moveVector to each Atom in the indexer
}


/*********************************************************************************************************************/
/* MATRIX OPERATIONS																								 */
/*********************************************************************************************************************/
namespace matrixOps {

	//abc = row, 123 = col
	struct mat3x3
	{
		SBQuantity::length a1;
		SBQuantity::length a2;
		SBQuantity::length a3;

		SBQuantity::length b1;
		SBQuantity::length b2;
		SBQuantity::length b3;

		SBQuantity::length c1;
		SBQuantity::length c2;
		SBQuantity::length c3;

		// default constructor
		mat3x3() :
			a1(vectorOps::Ql_0), a2(vectorOps::Ql_0), a3(vectorOps::Ql_0),
			b1(vectorOps::Ql_0), b2(vectorOps::Ql_0), b3(vectorOps::Ql_0),
			c1(vectorOps::Ql_0), c2(vectorOps::Ql_0), c3(vectorOps::Ql_0) {
		}

		// parameterized constructor
		mat3x3(SBQuantity::length a1, SBQuantity::length a2, SBQuantity::length a3,
			SBQuantity::length b1, SBQuantity::length b2, SBQuantity::length b3,
			SBQuantity::length c1, SBQuantity::length c2, SBQuantity::length c3)
			: a1(a1), a2(a2), a3(a3),
			b1(b1), b2(b2), b3(b3),
			c1(c1), c2(c2), c3(c3)
		{
		}

		mat3x3 operator+ (mat3x3 m) {
			return mat3x3(
				a1 + m.a1, a2 + m.a2, a3 + m.a3,
				b1 + m.b1, b2 + m.b2, b3 + m.b3,
				c1 + m.c1, c2 + m.c2, c3 + m.c3
			);
		}

		mat3x3 operator* (double s) {
			return mat3x3(
				s*a1, s*a2, s*a3,
				s*b1, s*b2, s*b3,
				s*c1, s*c2, s*c3);
		}
	};

	mat3x3 posXpos(SBPosition3 a, SBPosition3 b);					///< calculates the matrix a*b'  (b gets transposed)

	void convertToArray(mat3x3 in, double out[3][3]);				///< converts a mat3x3 to a double[3][3]
}

/*********************************************************************************************************************/
/* HELPER TYPES																										 */
/*********************************************************************************************************************/
namespace hTypes {
	typedef	tuple<SBPosition3, SBPosition3, SBQuantity::length> positionsLengthTuple;

	typedef tuple<SBPosition3, SBPosition3, SBPosition3> positionTriple;

	typedef	tuple<SBAtom*, SBAtom*, SBQuantity::length> atomsLengthTuple;

	//Source: http://www.techiedelight.com/sort-map-values-cpp/
	typedef pair<SBNode*, SBQuantity::length> nodeLengthPair;

	typedef pair<SBPosition3, SBPosition3> positionPair;
}


/*********************************************************************************************************************/
/* DEBUGGING																										 */
/*********************************************************************************************************************/
namespace debug {

	void debugPosition(SBPosition3 p);

}