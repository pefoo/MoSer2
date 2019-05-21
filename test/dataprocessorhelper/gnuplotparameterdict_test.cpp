#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include "catch2/catch.hpp"

TEST_CASE("Gnu plot parameter dict", "[DataProcessorHelper]") {
  dataprocessorhelper::gnuplot::GnuPlotParameterDict dict{};

  REQUIRE(dict.ToString() == "");

  // add values
  dict.AddParameter("key_1", "someValue with spaces", true);
  dict.AddParameter("_k2", "5", false);
  REQUIRE(dict.ToString() == "\"_k2=5;key_1='someValue with spaces'\"");

  // keys are not overwritten by default
  dict.AddParameter("key_1", "something else", true);
  dict.AddParameter("_k2", "1", false);
  REQUIRE(dict.ToString() == "\"_k2=5;key_1='someValue with spaces'\"");

  // keys are overwritten if stated
  dict.AddParameter("key_1", "something else", true, true);
  dict.AddParameter("_k2", "1", false, true);
  REQUIRE(dict.ToString() == "\"_k2=1;key_1='something else'\"");

  // additional set is applied, keys are not overwritten by default
  dataprocessorhelper::gnuplot::GnuPlotParameterDict additional{};
  additional.AddParameter("add_me", "35", false);
  additional.AddParameter("_k2", "2", false);
  dict.AddParameter(additional);
  REQUIRE(dict.ToString() == "\"_k2=1;add_me=35;key_1='something else'\"");

  // additional set is applied, keys are overwritten if stated
  dataprocessorhelper::gnuplot::GnuPlotParameterDict additional2{};
  additional2.AddParameter("_k2", "2", false);
  dict.AddParameter(additional2, true);
  REQUIRE(dict.ToString() == "\"_k2=2;add_me=35;key_1='something else'\"");
}
