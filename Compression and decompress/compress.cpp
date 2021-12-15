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

    int byteRead;

    while((byteRead = my_input.read_byte()) != -1){ // while the bit I'm reading is not -1
        unsigned char symbol = (unsigned char)(byteRead); // type cast it
        //cout << symbol;
        freqCounts[symbol]++; // increment the count of that symbol
    }
    HCTree tree; 
    tree.build(freqCounts); // build our try by passing in all freqs

    tree.printTree(tree.returnRoot());
    
    for(unsigned int i = 0; i< freqCounts.size(); i++){ // first write out header
        my_output.write_threebyte(freqCounts[i]);
    }

    my_input.reset();

    while( (byteRead = my_input.read_byte()) != -1){ // while the bit I'm reading is not -1
        tree.encode((unsigned char)(byteRead), my_output);
    }
return 0;
}
