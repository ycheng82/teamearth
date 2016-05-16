#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

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

	if (argc != 2){
		cout << "You need a vtk file as an argument" << endl;
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
	}

	file.seekg(file_pos);

	while (!file.eof()){
		intensity_value += file.get();
		intensity_value = intensity_value << 8;
		intensity_value += file.get();
		intensity_value = intensity_value << 8;
		intensity_value += file.get();
		voxel_intensity_values[intensity_value] += 1;
		intensity_value = 0;
	}

	for (std::map<int, int>::iterator it = voxel_intensity_values.begin(); it != voxel_intensity_values.end(); ++it){
		cout << "Intensity value [ " << std::hex << it->first << " ]:  " << std::dec << it->second << " voxels" <<  endl;
	}

	file.close();

	return 0;
}