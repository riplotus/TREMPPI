// standalone launcher
#include <tremppi_common/general/system.hpp>


int main(int argc, char ** argv) 
{
	TremppiSystem::standalone = true;
	return tremppi_sign(argc, argv);
}