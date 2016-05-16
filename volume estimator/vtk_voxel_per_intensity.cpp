#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

//
//these two split functions are only for checking if the file ends in .vtk
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


int main(int argc, char ** argv){

	string file_argument;
	vector<string> split_str;
	fstream file;
	map<int,int> voxel_intensity_values;
	int intensity_value = 0;
	int file_pos = 0;

	//Check arguments
	if (argc != 2){
		cout << "You need a vtk file as an argument" << endl;
		return -1;
	}
	else{
		file_argument = argv[1];
		split_str = split(file_argument,'.');
		if ( split_str[1].compare("vtk")){
			cout << "This is not a vtk file!" << endl;
			
			return 0;
		}
	}

	
	file.open(file_argument);
	if (file.is_open()){

		cout << "File is open" << endl;
		ifstream infile{file_argument};
		string file_contents{ istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };
		int line_break = 0;

		//This for loop is to determine where to position the file read pointer to start counting voxels that are mapped to a particular intensity value
		for (int i = 0;; i++){
			if (file_contents[i] == 10){
				line_break++;
			}
			if (line_break == 9){
				file_pos = i+1;
				break;
			}
		}
	}
	else{
		cout << "File not open" << endl;
		return -1;
	}

	//We position the file read pointer right after the header
	file.seekg(file_pos);


	//Using bit shifting to get the intensity value, we increment by one per intensity value.
	//The intensity value has only 3 bytes, and an int holds 4 bytes. You get one byte of the intensity value, store it into the int, then shift by 8 bits.
	// Repeat the process until you get all 3 bytes of the intensity value.
	while (!file.eof()){
		intensity_value += file.get();
		intensity_value = intensity_value << 8;
		intensity_value += file.get();
		intensity_value = intensity_value << 8;
		intensity_value += file.get();
		voxel_intensity_values[intensity_value] += 1;
		intensity_value = 0;
	}

	//Using iterators, print out all intensity values and the number of voxels with that particular intensity value;
	for (std::map<int, int>::iterator it = voxel_intensity_values.begin(); it != voxel_intensity_values.end(); ++it){
		cout << "Intensity value [ " << std::hex << it->first << " ]:  " << std::dec << it->second << " voxels" <<  endl;
	}


	
	file.close();
	return 0;
}