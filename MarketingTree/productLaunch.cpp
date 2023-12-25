/*!	\file	productLaunch.cpp
	\brief	main application
	\author	Ethan Rivers
	\date	2023-12-18
	\copyright	Ethan Rivers
*/

#include <iostream>
#include <iomanip>
#include <locale>
#include <random>
#include "marketingTree.h"
using namespace std;

struct comma_numpunct : public std::numpunct<char> {
protected:
	virtual char do_thousands_sep() const {
		return ','; // Change this to ' ' for space as thousands separator
	}

	virtual std::string do_grouping() const {
		return "\3"; // Groups of 3 digits
	}
};

Params generate_random_params() {
	random_device rd;  // Obtain a random number from hardware
	mt19937 gen(rd()); // Seed the generator
	uniform_int_distribution<> distrib(0, 1); // Define the range

	Params params;
	params.uses_market_testing = distrib(gen) != 0;
	params.uses_market_testing ? params.has_positive_rating = distrib(gen) != 0 : params.has_positive_rating = false; // only has a rating for market testing
	params.successful_launch = distrib(gen) != 0;
	params.modest_launch = distrib(gen) != 0;
	params.failed_launch = distrib(gen) != 0;

	return params;
}

int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	locale comma_locale(locale(), new comma_numpunct());
	cout.imbue(comma_locale); // make current local output numbers with commas 

	Params params = generate_random_params();
	TreeNodePtr root = create_tree(params);
	TreeNodePtr resultNode = get_result(root);

	/*	Example of Runtime Type Information (RTTI) 
		Best pracice would be to just make a virtual method and use polymorphism but 
		in this case I want to demonstrate RTTI.
	*/
	if (auto failed = dynamic_pointer_cast<FailedLaunchNode>(resultNode))
		cout << "Failed Launch with value: $" << fixed << setprecision(2) << failed->value << "\n";
	else if (auto negSuccess = dynamic_pointer_cast<NegativeSuccessfulLaunchNodeTesting>(resultNode))
		cout << "Negative Successful Launch with value: $" << fixed << setprecision(2) << negSuccess->value << "\n";
	else if (auto negModest = dynamic_pointer_cast<NegativeModestLaunchNodeTesting>(resultNode))
		cout << "Negative Modest Launch with value: $" << fixed << setprecision(2) << negModest->value << "\n";
	else if (auto posSuccess = dynamic_pointer_cast<PositiveSuccessfulLaunchNodeTesting>(resultNode))
		cout << "Positive Successful Launch with value: $" << fixed << setprecision(2) << posSuccess->value << "\n";
	else if (auto posModest = dynamic_pointer_cast<PositiveModestLaunchNodeTesting>(resultNode))
		cout << "Positive Modest Launch with value: $" << fixed << setprecision(2) << posModest->value << "\n";
	else if (auto noTestSuccess = dynamic_pointer_cast<NoTestingSuccessfulLaunchNode>(resultNode))
		cout << "No Testing Successful Launch with value: $" << fixed << setprecision(2) << noTestSuccess->value << "\n";
	else if (auto noTestModest = dynamic_pointer_cast<NoTestingModestLaunchNode>(resultNode))
		cout << "No Testing Modest Launch with value: $" << fixed << setprecision(2) << noTestModest->value << "\n";
	else
		cout << "Unexpected or Unknown result....\n";


	return 0;
}