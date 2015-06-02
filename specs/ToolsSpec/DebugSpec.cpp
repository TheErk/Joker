/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhCommonUI/PhUI.h"
#include "PhTools/PhDebug.h"

#include "PhSpec.h"

using namespace bandit;

/**
 * @brief Redirect the standard output to a stream buffer
 *
 * The ouput will be redirect to its old value upon object destruction.
 */
class CoutRedirect {
public:
	/**
	 * @brief The CoutRedirect constructor
	 * @param new_buffer A stream buffer
	 */
	CoutRedirect( std::streambuf * new_buffer )
		: old( std::cout.rdbuf( new_buffer ) )
	{
	}

	~CoutRedirect() {
		std::cout.rdbuf( old );
	}
private:
	std::streambuf * old;
};

/**
 * @brief Redirect the error output to a string buffer
 *
 * The ouput will be redirect to its old value upon object destruction.
 */
class CerrRedirect {
public:
	/**
	 * @brief The CerrRedirect constructor
	 * @param new_buffer A stream buffer
	 */
	CerrRedirect( std::streambuf * new_buffer )
		: old( std::cerr.rdbuf( new_buffer ) )
	{
	}

	~CerrRedirect() {
		std::cerr.rdbuf( old );
	}
private:
	std::streambuf * old;
};

go_bandit([](){
	describe("debug_test", []() {
		before_each([&](){
			PhDebug::enable();
			PhDebug::setDisplay(false, false, false, false, false);
			PhDebug::setLogMask(1);
		});

		it("log_in_file", []() {
			QString expected = QString("/Users/") + qgetenv("USER") + QString("/Library/Logs/Phonations/%1.log").arg(APP_NAME);
			AssertThat(PhDebug::logLocation().toStdString(), Equals(expected.toStdString()));
		});

		it("display_in_the_debug", []() {
			std::stringstream buffer;
			CoutRedirect redirect(buffer.rdbuf());

			PhDebug::setDisplay(true, true, true, true, true);
			PHDEBUG << "test with all log parameters";

			PhDebug::setDisplay(false, false, false, false, false);
			PHDEBUG << "test with no log parameters";

			PhDebug::showConsole(false);
			PHDEBUG << "not shown because of showConsole(false)";

			PhDebug::showConsole(true);
			PHDEBUG << "shown because of showConsole(true)";

			PhDebug::disable();
			PHDEBUG << "not shown because disable()";

			PhDebug::enable();
			PHDEBUG << "shown because enable()";

			AssertThat(PhDebug::logMask(), Equals(1));
			PHDBG(0) << "it should be displayed when default log mask is 1";
			int testLogLevel = 31;
			PhDebug::setLogMask(1 << testLogLevel);
			AssertThat(PhDebug::logMask(), Equals(1 << testLogLevel));
			PHDBG(0) << "it should not be displayed when default log mask is " + testLogLevel;
			PHDBG(testLogLevel) << "it should be displayed when default log mask is " + testLogLevel;

			QStringList lines = QString::fromStdString(buffer.str()).split("\n");
			AssertThat(lines.count(), Equals(7));
			AssertThat(QRegExp("\\d\\d/\\d\\d/\\d\\d\\d\\d \\d\\d:\\d\\d:\\d\\d\\.\\d\\d\\d DebugSpec.cpp\toperator\\(\\)\t@[0-9]+\ttest with all log parameters").exactMatch(lines[0]), IsTrue());

			AssertThat(lines[1].toStdString(), Equals("test with no log parameters"));
			AssertThat(lines[2].toStdString(), Equals("shown because of showConsole(true)"));
			AssertThat(lines[3].toStdString(), Equals("shown because enable()"));
			AssertThat(lines[4].toStdString(), Equals("it should be displayed when default log mask is 1"));
			AssertThat(lines[5].toStdString(), Equals("it should be displayed when default log mask is " + testLogLevel));
		});

		it("display_in_the_error", []() {
			std::stringstream buffer;
			CerrRedirect redirect(buffer.rdbuf());

			PHERR << "test with no log parameters";

			PhDebug::setDisplay(true, true, true, true, true);
			PHERR << "test with all log parameters";
			QStringList lines = QString::fromStdString(buffer.str()).split("\n");
			AssertThat(lines.count(), Equals(3));
			AssertThat(lines[0].toStdString(), Equals("test with no log parameters"));
			AssertThat(QRegExp("\\d\\d/\\d\\d/\\d\\d\\d\\d \\d\\d:\\d\\d:\\d\\d\\.\\d\\d\\d DebugSpec.cpp\toperator\\(\\)\t@[0-9]+\ttest with all log parameters").exactMatch(lines[1]), IsTrue());
			AssertThat(lines[2].toStdString(), Equals(""));
		});
	});
});
