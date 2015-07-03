#include "python_functions.hpp"
#include "../general/system.hpp"


PythonFunctions::PythonFunctions() 
{
	Py_Initialize();
	main_module = bpy::import("__main__");
	main_namespace = main_module.attr("__dict__");
	exec("import sys");
	string append_cmd = "sys.path.append(" + reformPath(TremppiSystem::HOME_PATH / "python") + ")";
	exec(append_cmd);
}


string PythonFunctions::reformPath(const bfs::path & path) 
{
	return "'" + std::regex_replace(path.string(), std::regex("\\\\"), "/") + "'";
}


PythonFunctions & PythonFunctions::getInstance()
{
	static PythonFunctions instance; 

	return instance;
}


void PythonFunctions::exec(const string & command) 
{
	try {
		bpy::exec(command.c_str(), main_namespace);
	}

	catch (bpy::error_already_set const &)
	{
		PyErr_Print();
		throw std::runtime_error("Python exception encountered.");
	}
}


void PythonFunctions::configure(string widget) 
{
	PythonFunctions & python = PythonFunctions::getInstance();
	python.exec("from configure.configure import configure");
	python.exec("configure(" + PythonFunctions::reformPath(bfs::absolute(TremppiSystem::DATA_PATH)) + ", \"" + widget + "\")");
}