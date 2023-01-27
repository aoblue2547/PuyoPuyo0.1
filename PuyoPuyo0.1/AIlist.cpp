#include "AIlist.hpp"



AIlist::AIlist() :csv(U"AI_list.csv") {
	if (not csv) {
		throw Error{ U"Failed to load `AI_list.csv`" };
	}
	for (size_t row = 1; row < csv.rows(); ++row) {
		AIname.push_back(csv[row][0]);
		filepath.push_back(csv[row][1]);
	}
}


size_t AIlist::size() const{
	return AIname.size();
}


void AIlist::addAI(const String& name, String& path) {
	AIname.push_back(name);
	filepath.push_back(path);
	csv.writeRow(name, path);
	csv.save(U"AI_list.csv");
}


const Array<String>& AIlist::AInameArray() {
	return AIname;
}
