#include <iostream>
#include "HCTree.hpp"
#include "Helper.hpp"

using namespace std;

int main (int argc, char* argv[]){

    // remember if stream is reading from file
    // of stream is writing to file

    FancyInputStream my_input(argv[1]); // pass in first file

    if (my_input.filesize() == 0){ // if the file we have is empty
        FancyOutputStream my_output(argv[2]); // create output file
        return 0;
    }
    // create freq counts
    vector<int> freqCounts(256,0); // initalize all 256 count with 0
    FancyOutputStream my_output(argv[2]); // create output file 

    int bitRead;

    for (int i = 0; i < int(freqCounts.size()); i++){
        bitRead = my_input.read_int(); // first read counts of symbols as int
        freqCounts[i] = bitRead;
    }

    HCTree tree; 
    tree.build(freqCounts); // build our try by passing in all freqs
    tree.printTree(tree.returnRoot());

    int sumofVector = 0;
    for (auto& n : freqCounts) // get total count to write it that many times
    sumofVector += n;

    for(int i = 0; i < sumofVector; i++){
        my_output.write_byte((unsigned char)(tree.decode(my_input)));
    }

return 0;
}
