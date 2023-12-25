/*!	\file	marketingTree.h
	\brief	definition of marketing decision tree using smart pointers for automatic memory managment
	\author	Ethan Rivers
	\date	2023-12-18
	\copyright	Ethan Rivers
*/
#pragma once
#include <memory>
#define TRACK_PATH true

struct Params {
	bool uses_market_testing;
	bool has_positive_rating;
	bool successful_launch;
	bool modest_launch;
	bool failed_launch;

	Params() : uses_market_testing(false), has_positive_rating(false), successful_launch(false),
		modest_launch(false), failed_launch(false) {}

};

// Forward declaration of TreeNode (compiler will now know this struct exists and will be used later)
struct TreeNode;
using TreeNodePtr = std::shared_ptr<TreeNode>;

struct TreeNode {
	TreeNodePtr left_;
	TreeNodePtr middle_;
	TreeNodePtr right_;
	Params params;

	explicit TreeNode(const Params& p, TreeNodePtr leftNode = nullptr, TreeNodePtr rightNode = nullptr, TreeNodePtr middleNode = nullptr) :
		left_(leftNode), right_(rightNode), middle_(middleNode), params(p) {}

	virtual TreeNodePtr process() const = 0;
	virtual ~TreeNode() = default; // ensures derived classes are destroyed 
};



// LEAF NODES
struct FailedLaunchNode final : TreeNode { // keword 'final' prevents class from being inherited
	double value;
	explicit FailedLaunchNode(const Params& p, double val = 0.00) : TreeNode(p), value(val) {} // initalizes base class
	TreeNodePtr process() const override { return nullptr; }
};

struct NegativeSuccessfulLaunchNodeTesting final : TreeNode {
	double value;
	explicit NegativeSuccessfulLaunchNodeTesting(const Params& p, double val = 75'000.00) : TreeNode(p), value(val) {}
	TreeNodePtr process() const override { return nullptr; }
};

struct NegativeModestLaunchNodeTesting final : TreeNode {
	double value;
	explicit NegativeModestLaunchNodeTesting(const Params& p, double val = 50'000.00) : TreeNode(p), value(val) {}
	TreeNodePtr process() const override { return nullptr; }
};

struct PositiveSuccessfulLaunchNodeTesting final : TreeNode {
	double value;
	explicit PositiveSuccessfulLaunchNodeTesting(const Params& p, double val = 150'000.00) : TreeNode(p), value(val) {}
	TreeNodePtr process() const override { return nullptr; }
};

struct PositiveModestLaunchNodeTesting final : TreeNode {
	double value;
	explicit PositiveModestLaunchNodeTesting(const Params& p, double val = 75'000.00) : TreeNode(p), value(val) {}
	TreeNodePtr process() const override { return nullptr; }
};

struct NoTestingSuccessfulLaunchNode final : TreeNode {
	double value;
	explicit NoTestingSuccessfulLaunchNode(const Params& p, double val = 100'000.00) : TreeNode(p), value(val) {}
	TreeNodePtr process() const override { return nullptr; }
};

struct NoTestingModestLaunchNode final : TreeNode {
	double value;
	explicit NoTestingModestLaunchNode(const Params& p, double val = 50'000.00) : TreeNode(p), value(val) {}
	TreeNodePtr process() const override { return nullptr; }
};




// DECISION NODES
struct ProductLaunchStrategyNode final : TreeNode {
	explicit ProductLaunchStrategyNode(const Params& p) : TreeNode(p) {}
	TreeNodePtr process() const override {
		return params.uses_market_testing ? left_ : right_;
	}
};

struct RatingNode final : TreeNode {
	explicit RatingNode(const Params& p) : TreeNode(p) {}
	TreeNodePtr process() const override {
		return params.has_positive_rating ? right_ : left_;
	}
};

struct LaunchOutcomeNode final : TreeNode {
	explicit LaunchOutcomeNode(const Params& p) : TreeNode(p) {}
	TreeNodePtr process() const override {
		if (params.successful_launch)
			return right_;
		else if (params.modest_launch)
			return middle_;
		else
			return left_;
	}
};


/* Generate tree structure */
inline TreeNodePtr create_tree(const Params& p) {

	// LEAF NODES
	auto failed = std::make_shared<FailedLaunchNode>(p);
	auto positiveTestingSuccessNode = std::make_shared<PositiveSuccessfulLaunchNodeTesting>(p);
	auto positiveTestingModestNode = std::make_shared<PositiveModestLaunchNodeTesting>(p);
	auto negativeTestingSuccessNode = std::make_shared<NegativeSuccessfulLaunchNodeTesting>(p);
	auto negativeTestingModestNode = std::make_shared<NegativeModestLaunchNodeTesting>(p);
	auto noTestingSuccessNode = std::make_shared<NoTestingSuccessfulLaunchNode>(p);
	auto noTestingModestNode = std::make_shared<NoTestingModestLaunchNode>(p);

	// DECISION NODES RIGHT PATH
	auto noTestingOutcomeNode = std::make_shared<LaunchOutcomeNode>(p);
	noTestingOutcomeNode->right_ = noTestingSuccessNode;
	noTestingOutcomeNode->middle_ = noTestingModestNode;
	noTestingOutcomeNode->left_ = failed;

	// DECISION NODES LEFT PATH
	auto positiveTestingOutcomeNode = std::make_shared<LaunchOutcomeNode>(p);
	positiveTestingOutcomeNode->right_ = positiveTestingSuccessNode;
	positiveTestingOutcomeNode->middle_ = positiveTestingModestNode;
	positiveTestingOutcomeNode->left_ = failed;

	auto negativeTestingOutcomeNode = std::make_shared<LaunchOutcomeNode>(p);
	negativeTestingOutcomeNode->right_ = negativeTestingSuccessNode;
	negativeTestingOutcomeNode->middle_ = negativeTestingModestNode;
	negativeTestingOutcomeNode->left_ = failed;

	auto ratingNode = std::make_shared<RatingNode>(p);
	ratingNode->right_ = positiveTestingOutcomeNode;
	ratingNode->left_ = negativeTestingOutcomeNode;


	auto root = std::make_shared<ProductLaunchStrategyNode>(p);
	root->right_ = noTestingOutcomeNode;
	root->left_ = ratingNode;

	return root;
}


TreeNodePtr get_result(TreeNodePtr node) {
	unsigned nodeCount = 0;
	if (TRACK_PATH) {
		std::cout << "Displaying travel path...\n";
	}

	while (node && node->process()) {
		if (TRACK_PATH) {
			std::cout << ++nodeCount << "." << typeid(*node).name() + 6 << std::endl;
		}
		node = node->process();
	}
	std::cout << '\n';
	return node;
}
