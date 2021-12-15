#include "HCTree.hpp"
#include <stack>
using namespace std;

void HCTree::printTree(HCNode* curr){

	if(curr){
		cout << curr->symbol << curr->count << endl;
		printTree(curr->c0);
		printTree(curr->c1);
	}
}
HCNode* HCTree::returnRoot(){
            return root;
        }

void helperDelete(HCNode * temp){
	if (temp == nullptr){ // if we traverse into nullptr then return
        return;
    }

    helperDelete(temp->c0); // traverse all the way left
    helperDelete(temp->c1); // traverse all the way right
    
    delete (temp); // delete the current node
}

HCTree::~HCTree(){ // deconstructor to delete our tree USE PA 2 BST REMOVAL 
helperDelete(root);
root = nullptr;
}

void HCTree::build(const vector<int>& freqs){

	priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;// This is given to us
	// First argument tells us what type the queue will be storing
	// Second argument tell us to use a vector container to store it's elements
	// Third argument is a custom function that will determine priorities for us. This was given to us
	for (unsigned int count = 0; count < freqs.size(); count++){ // go through all 255-256??? ascii characters
		if (freqs[count] == 0){ // don't do anything and leave if statement
			continue;
		}
		else{
			HCNode* leaf = new HCNode(freqs[count],count); // pass byte and freq count of that byte
			leaves[count] = leaf; // push this leaf to our vector of leaves
			pq.push(leaf); // push our leaf to priority queue
			
		}
	}

	// create the tree from forest of nodes
	while(pq.size() > 1){

		HCNode* lowest;
		HCNode* secondLowest;

		lowest = pq.top(); // lowest in priority queue
		pq.pop(); // pop lowest
		secondLowest = pq.top(); // get second lowest
		pq.pop(); // pop second lowest

		int totalFreq;
		totalFreq = lowest->count + secondLowest->count; // find total freq to use as parent

		unsigned char totalSymbol = secondLowest->symbol; // symbol of our parent. Just use lowest

		HCNode* totalNode = new HCNode(totalFreq,totalSymbol); // create parent use total freq

		totalNode->c0 = lowest; // set left child to be lowest
		totalNode->c1 = secondLowest; // set right child to be secondLowest

		lowest->p = totalNode; // set parent of our leaves
		secondLowest->p = totalNode; // set parent of our leaves
		pq.push(totalNode); // push our parent back onto PQ

	}

	root = pq.top(); // once only 1 left set root to be the last node in PQ. 

}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{

	stack<int> path; // stack that contains path up the tree to root
	HCNode* curr = leaves[symbol]; // find the node with our symbol and set it to curr
	while(curr != root){ // traverse curr up until it doesn't equal root
		if(curr->p->c1 == curr){ // if curr is on right side of parent
			path.push(1);
		}
		if(curr->p->c0 == curr){ // if curr is on left side of parent
			path.push(0);
		}
		curr = curr->p; // traverse up
	}

	while(path.size() != 0){ // while there is something in the stack
		out.write_bit(path.top()); // write the bit at the top since we'll encode from top to bottom
		path.pop(); // pop that bit
	}
}

unsigned char HCTree::decode(FancyInputStream & in) const{
	
	// to decode we need to move to find the leaf
	HCNode * curr = root;
	while(curr->c0 && curr->c1){ // repeat while we have 2 children (Basically we are parents)
		int bit = in.read_bit();
		if(bit == 1){
			curr = curr->c1; // traverse right if we read 1
		}
		else if (bit == 0){
			curr = curr->c0; // traverse left isntead;
		}
	}
	return curr->symbol;

}
