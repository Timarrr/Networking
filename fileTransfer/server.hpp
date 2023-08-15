#ifndef SERVER_HPP
#define SERVER_HPP
struct file_s
{
	std::string name;
	std::string content;
	uint32_t size;
};
const std::string getFile(int argc, const char **argv)
{
	if(argc <= 1)
	{
		std::cout << "no file path specified\n";
		return "";
	}
	std::ifstream fileToBeTransfered(argv[1], std::ios::binary);
	if(!fileToBeTransfered.is_open())
	{
		std::cout << "failed to open file " << std::string(argv[1]) << '\n';
		return "";
	}
	else
	{
		std::string fileContent;
		std::string line;
		while(std::getline(fileToBeTransfered, line, '\0'))
		{
			fileContent += line;
		}
		std::cout << fileContent << '\n';
		return fileContent;
	}
};

#endif
