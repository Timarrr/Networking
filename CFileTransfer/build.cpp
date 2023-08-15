#include <cstdlib>
int main()
{
    std::system("gcc client.c -o client.bin");
    std::system("gcc server.c -o server.bin");
}
