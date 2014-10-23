#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cstdlib>
#include <iostream>

int main(int argc, const char* argv[]) {
	CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

	CppUnit::TextUi::TestRunner runner;
	runner.addTest(registry.makeTest());
	bool ok = runner.run("", false);
	return (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}