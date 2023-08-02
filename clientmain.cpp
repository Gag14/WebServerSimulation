#include <iostream>
#include <string>

#include "Socket.h"
int main() 
{
	std::cout << "Main Function" << std::endl;
	Socket sk;
	sk.create();
	std::string localhost = "127.0.0.1";
	sk.connect(localhost, "8080");
	std::cout << "Unbelivable!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

}
