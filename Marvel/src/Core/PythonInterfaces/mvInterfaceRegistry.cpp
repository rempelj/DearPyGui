#include "mvInterfaceRegistry.h"
#include "mvPythonModule.h"
#include <functional>
#include "Core/mvApp.h"
#include <iostream>
#include <fstream>

namespace Marvel {

	mvInterfaceRegistry* mvInterfaceRegistry::s_instance = nullptr;

	void mvInterfaceRegistry::generateStubFile(const std::string& file)
	{
		std::ofstream stub;
		stub.open(file + "/marvel.pyi");

		stub << "from typing import List, Any\n\n";

		for (const auto& parser : m_parsers["marvel"])
		{
			stub << "def " << parser.first << "(";

			auto elements = parser.second.getElements();

			for (int i = 0; i < elements.size(); i++)
			{
				if (elements[i].type == mvPythonDataType::KeywordOnly || elements[i].type == mvPythonDataType::Optional)
					continue;
				if(i != elements.size() - 1)
					stub << elements[i].name << ": " << PythonDataTypeActual(elements[i].type) <<
					", ";
				else
					stub << elements[i].name << ": " << PythonDataTypeActual(elements[i].type) <<
						") -> " << parser.second.getReturnType() << ":\n\t\"\"\""<<parser.second.getAbout() << "\"\"\"\n\t...\n\n";
			}

			if(elements.size() == 0)
				stub << ") -> " << parser.second.getReturnType() << ":\n\t\"\"\"" << parser.second.getAbout() << "\"\"\"\n\t...\n\n";



		}
		stub.close();

		std::ofstream stub2;
		stub2.open(file + "/marvel_constants.pyi");
		stub2 << "\n";
		for (auto& item : m_constants)
			stub2 << item.first << " = " << item.second << "\n";
		stub2.close();
	}

	std::map<std::string, mvPythonParser>& mvInterfaceRegistry::getPythonInterface(const std::string& name)
	{ 
		return m_parsers[name];
	}

	std::vector<std::pair<std::string, long>>& mvInterfaceRegistry::getConstants()
	{ 
		return m_constants; 
	}

	mvInterfaceRegistry* mvInterfaceRegistry::GetRegistry()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvInterfaceRegistry();

		auto initializer = mvModuleInitializer::getInitializer();
		int startindex = initializer->initializeCoreModules();
		initializer->initializeUserModules(startindex);
		return s_instance;
	}

	mvInterfaceRegistry::mvInterfaceRegistry()
	{
		m_constants = BuildConstantsInterface();
	}

	std::vector<std::pair<std::string, std::string>> mvInterfaceRegistry::getAllCommands()
	{

		std::vector<std::pair<std::string, std::string>> result;

		for (const auto& modu : m_parsers)
		{
			for (const auto& item : modu.second) // actual parser maps
				result.emplace_back(item.first.c_str(), item.second.getDocumentation());
		}

		return result;
	}

	void mvInterfaceRegistry::addModule(const char* name, pyInitFunc initfunc, pyDocFunc docfunc)
	{
		if(docfunc)
			m_parsers[name] = docfunc();
		m_modules.push_back(initfunc);
	}

	pyInitFunc mvInterfaceRegistry::getInitFunc(int i)
	{
		return m_modules[i];
	}

	std::vector<const char*> mvInterfaceRegistry::getPythonInterfaceCommands(const std::string& name)
	{
		std::map<std::string, mvPythonParser>& docmap = m_parsers[name];

		std::vector<const char*> commandvec;

		for (const auto& item : docmap)
			commandvec.emplace_back(item.first.c_str());

		return commandvec;
	}

	std::vector<const char*> mvInterfaceRegistry::getPythonInterfaceDoc(const std::string& name)
	{
		std::map<std::string, mvPythonParser>& docmap = m_parsers[name];

		std::vector<const char*> docvec;

		for (const auto& item : docmap)
			docvec.emplace_back(item.second.getDocumentation());

		return docvec;
	}

	std::vector<const char*> mvInterfaceRegistry::getConstantsCommands()
	{
		std::vector<const char*> commandvec;

		for (const auto& item : m_constants)
			commandvec.emplace_back(item.first.c_str());

		return commandvec;
	}

	std::vector<long> mvInterfaceRegistry::getConstantsValue()
	{
		std::vector<long> commandvec;

		for (const auto& item : m_constants)
			commandvec.push_back(item.second);

		return commandvec;
	}

}