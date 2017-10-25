#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <fstream>

#include "iniparser.h"

using namespace std;

TEST_CASE("IniParser returns false on loading not existed file") {
    IniParser parser;
    std::string fileName = "no_such_file.ini";
    REQUIRE_FALSE(parser.load(fileName));
}

TEST_CASE("Loading existed and valid ini file") {
    std::ofstream ofs("valid_ini_file.ini", std::ofstream::out);
    ofs << R"(
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Comments			
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
# Comment
; Comment
  # Comment # # # # ; ; ; ; ;

 ;    Comment " lkjhaskfjhlah lkjhsad f"
         ;    Comment   		

;[someCommentedSection]     
;
;commentedOption = commented value    

###################################################
# Default section
###################################################

defParam1 = defValue1
defParam2 = defValue2
defParam3 = defValue3

[emptySection1]
[emptySection2]

[cASE]
PARAm1 = value1
param2 = lower
PARAM2 = UPPER

[  whitespaces		]           			
  param1            =	value1  
    param2 	 =     value2      		
param3=value3                              

[comments] ; comment # comment 		
param1 = ; comment1    
param2 = value2 ; comment2 ; ; ; ;comment2			
;param3 = value			 	 	
param4 = "value; #4#" ; comment4
param5 =# comment5			

[string]
param1 = value 	1 value     1 value 1 value 1         
param2 = value2      			 dfdfdf     
param3 = "param3   	     "

[bool]
param1 = true
param2 = 1
param3 = enable
param4 = on
param5 = false
param6 = 0
param7 = disable
param8 = off

[int]
param1 = 19872634
param2 = -38384849
param3 = -38384 849
param4 = -38hvgf384849
param5 = true
param6 = 3.4832
param7 = 0

[double]
param1 = 2.3937463876
param2 = -276
param3 = -27nmxcb6
param4 = 309e3
param5 = 0.0000
param6 = 0.0000343 444"
param7 = true
)";
    ofs.close();

    IniParser ini;
    REQUIRE(ini.load("valid_ini_file.ini"));

    SECTION("get all the sections") {
        list<string> sections = {"bool", "case", "comments", "default", "double",
                "emptysection1", "emptysection2", "int", "string", "whitespaces"};
        REQUIRE(ini.sections() == sections);
    }

    SECTION("get all the options of the section") {
        list<string> options = {"param1", "param2", "param4", "param5"};
        REQUIRE(ini.options("comments") == options);
    }

    SECTION("default section") {
        REQUIRE(ini.get("default", "defparam1") == "defValue1");
        REQUIRE(ini.get("default", "defparam2") == "defValue2");
        REQUIRE(ini.get("default", "defparam3") == "defValue3");
        REQUIRE(ini.get("default", "defparam4", "no") == "no");
    }

    SECTION("IniParser is case insensitive") {
        REQUIRE(ini.get("case", "param1") == "value1");
        REQUIRE(ini.get("case", "param2") == "UPPER");
    }

    SECTION("handling the whitespaces") {
        REQUIRE(ini.get("whitespaces", "param1") == "value1");
        REQUIRE(ini.get("whitespaces", "param2") == "value2");
        REQUIRE(ini.get("whitespaces", "param3") == "value3");
    }

    SECTION("handling the comments") {
        REQUIRE(ini.get("comments", "param1") == "");
        REQUIRE(ini.get("comments", "param2") == "value2");
        REQUIRE(ini.get("comments", "param4") == "value; #4#");
        REQUIRE(ini.get("comments", "param5") == "");
    }

    SECTION("handling the string options") {
        REQUIRE(ini.get("string", "param1") == "value 	1 value     1 value 1 value 1");
        REQUIRE(ini.get("string", "param2", "defailt") == "value2      			 dfdfdf");
        REQUIRE(ini.get("string", "param3") == "param3   	     ");
        REQUIRE(ini.get("string", "param5", "default") == "default");
        REQUIRE(ini.get("string", "param6") == "");
    }

    SECTION("handling the boolean options") {
        REQUIRE(ini.getBool("bool", "param1") == true);
        REQUIRE(ini.getBool("bool", "param2") == true);
        REQUIRE(ini.getBool("bool", "param3", true) == true);
        REQUIRE(ini.getBool("bool", "param4") == true);
        REQUIRE(ini.getBool("bool", "param5") == false);
        REQUIRE(ini.getBool("bool", "param6", true) == false);
        REQUIRE(ini.getBool("bool", "param7") == false);
        REQUIRE(ini.getBool("bool", "param8") == false);
        REQUIRE(ini.getBool("bool", "param53", true) == true);
        REQUIRE(ini.getBool("bool", "param53") == false);
    }

    SECTION("handling the integer options") {
        REQUIRE(ini.getInt("int", "param1", 5) == 19872634);
        REQUIRE(ini.getInt("int", "param2") == -38384849);
        REQUIRE(ini.getInt("int", "param3") == -38384);
        REQUIRE(ini.getInt("int", "param4") == -38);
        REQUIRE(ini.getInt("int", "param5", 23) == 23);
        REQUIRE(ini.getInt("int", "param6", 54) == 3);
        REQUIRE(ini.getInt("int", "param7", 74) == 0);
        REQUIRE(ini.getInt("int", "param73", 74) == 74);
        REQUIRE(ini.getInt("int", "param73") == 0);
    }

    SECTION("handling the double options") {
        REQUIRE(ini.getDouble("double", "param1", 5) == 2.3937463876);
        REQUIRE(ini.getDouble("double", "param2") == -276);
        REQUIRE(ini.getDouble("double", "param3") == -27);
        REQUIRE(ini.getDouble("double", "param4") == 309e3);
        REQUIRE(ini.getDouble("double", "param5", 23) == 0.0);
        REQUIRE(ini.getDouble("double", "param6", 54) == 0.0000343);
        REQUIRE(ini.getDouble("double", "param7", 74) == 74);
        REQUIRE(ini.getDouble("double", "param73", 740) == 740);
        REQUIRE(ini.getDouble("double", "param73") == 0.0);
    }
}
