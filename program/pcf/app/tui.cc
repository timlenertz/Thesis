#include "tui.h"

namespace pcf {

std::string read_line(const std::string& prompt, const std::string& def) {
	std::cout << prompt << ": ";
	std::string line;
	std::getline(std::cin, line);
	if(line.empty()) return def;
	else return line;
}


int read_choice(const std::string& prompt, const choices& chc) {
	std::cout << prompt << ':' << std::endl;
	int i = 1;
	for(const std::string& choice : chc)
		std::cout << " (" << i++ << ") " << choice << std::endl;

	int selection = 0;
	while(selection == 0 || selection > chc.size())
		selection = read_from_input("Choose", 0);
	
	return selection - 1;
}

}