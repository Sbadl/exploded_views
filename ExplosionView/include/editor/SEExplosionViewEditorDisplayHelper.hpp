#pragma once 
#include "SAMSON.hpp"
#include "SEExplosionViewAppFunctions.hpp"

/*********************************************************************************************************************/
/* Helper Functions for displaying various objects																	 */
/*********************************************************************************************************************/
namespace displayHelper {

	void displayLine(SBPosition3 start, SBPosition3 end, std::string text);

	void displayCylinder(SBVector3 vec, SBPosition3 start);

	void displayArrow(SBPosition3 start, SBPosition3 end, const vector<float> groupColor);
}

namespace color {

	const int numberOfColors = 22;

	const vector<float> red{ 0.9f, 0.1f, 0.3f, 1.0f };
	const vector<float> green{ 0.23f, 0.7f, 0.3f, 1.0f };
	const vector<float> yellow{ 1.0f, 1.0f, 0.1f, 1.0f };
	const vector<float> blue{ 0.0f, 0.5f, 0.78f, 1.0f };
	const vector<float> orange{ 0.96f, 0.5f, 0.18f, 1.0f };
	const vector<float> purple{ 0.56f, 0.11f, 0.7f, 1.0f };
	const vector<float> cyan{ 0.27f, 0.94f, 0.94f, 1.0f };
	const vector<float> magenta{ 0.94f, 0.19f, 0.9f, 1.0f };
	const vector<float> lime{ 0.82f, 0.96f, 0.23f, 1.0f };
	const vector<float> pink{ 0.98f, 0.74f, 0.74f, 1.0f };
	const vector<float> teal{ 0.0f, 0.5f, 0.5f, 1.0f };
	const vector<float> lavender{ 0.9f, 0.74f, 1.0f, 1.0f };
	const vector<float> brown{ 0.66f, 0.43f, 0.15f, 1.0f };
	const vector<float> beige{ 1.0f, 0.98f, 0.78f, 1.0f };
	const vector<float> maroon{ 0.5f, 0.0f, 0.0f, 1.0f };
	const vector<float> mint{ 0.66f, 1.0f, 0.76f, 1.0f };
	const vector<float> olive{ 0.5f, 0.5f, 0.0f, 1.0f };
	const vector<float> coral{ 1.0f, 0.84f,0.7f, 1.0f };
	const vector<float> navy{ 0.0f, 0.0f, 0.5f, 1.0f };
	const vector<float> grey{ 0.5f, 0.5f, 0.5f, 1.0f };
	const vector<float> white{ 1.0f, 1.0f, 1.0f, 1.0f };
	const vector<float> black{ 0.0f, 0.0f, 0.0f, 1.0f };
}