#include <cstdlib>
int main()
{
    std::system("g++ client.cpp -o client.bin");
    std::system("g++ server.cpp -o server.bin");
}
