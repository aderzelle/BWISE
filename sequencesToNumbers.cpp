#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <unordered_map>



using namespace std;



char revCompChar(char c) {
	switch (c) {
		case 'A': return 'T';
		case 'C': return 'G';
		case 'G': return 'C';
	}
	return 'A';
}



string revComp(const string& s){
	string rc(s.size(),0);
	for (int i((int)s.length() - 1); i >= 0; i--){
		rc[s.size()-1-i] = revCompChar(s[i]);
	}
	return rc;
}



string getCanonical(const string& str){
	return (min(str,revComp(str)));
}



void help(){
	cout<<"./sequenceToNumbers unitigs.fa sequences.fa kmersize"<<endl;
}


int main(int argc, char *argv[]) {
	if(argc<3){
		help();
		exit(0);
	}
	unordered_map<string,pair<uint,uint>> kmerBegToUnitigs;
	unordered_map<string,pair<uint,uint>> kmerEndToUnitigs;
	string unitigFile(argv[1]);
	string seqFile(argv[2]);
	uint  k(stoi(argv[3]));
	string unitig,useless,msp;
	ifstream unitigStream(unitigFile);
	ifstream seqStream(seqFile);
	ofstream out("numbers.txt");
	uint i(0);
	while(not unitigStream.eof()){
		getline(unitigStream,useless);
		getline(unitigStream,unitig);
		kmerBegToUnitigs[getCanonical(unitig.substr(0,k))]={i,unitig.size()};
		kmerEndToUnitigs[getCanonical(unitig.substr(unitig.size()-k))]={i,unitig.size()};
		++i;
	}
	cout<<"unitigs indexed"<<endl;
	vector<int> res;
	while(not seqStream.eof()){
		getline(seqStream,useless);
		getline(seqStream,msp);
		uint i(0);
		//~ cout<<msp.size()<<endl;
		res={};
		while(i+k<=msp.size()){
			pair<uint,uint> value(kmerBegToUnitigs[getCanonical(msp.substr(i,k))]);
			if(value.second!=0){
				res.push_back(value.first);
				i+=value.second-k+1;
				//~ cout<<value.second<<endl;
			}else{
				value=(kmerEndToUnitigs[getCanonical(msp.substr(i,k))]);
				if(value.second!=0){
					res.push_back(-value.first);
					i+=value.second-k+1;
					//~ cout<<value.second<<endl;
				}else{
					cout<<"wtf"<<endl;
					exit(0);
				}
			}
		}
		if(res.size()>1){
			for(uint i(0);i<res.size();++i){
				out<<res[i]<<';';
			}
			out<<endl;
		}else{
			cout<<"oups"<<endl;
		}
	}

    return 0;
}
