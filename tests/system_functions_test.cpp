#include <lib/interpreter/interpreter.h>
#include <gtest/gtest.h>


TEST(SystemFunctionsTestSuite, BasicFunctionsTest) {
    std::istringstream input(R"(
       x = abs(-1)
       print(x)
       y = ceil(1.23)
       print(y)
       z = sqrt(6.25)
       print(z)
       
    )");

    std::ostringstream output;
    bool result = interpret(input, output);

    EXPECT_TRUE(result);
    EXPECT_EQ(output.str(), "122.5");
}

TEST(SystemFunctionsTestSuite, CompositionsTest) {
    std::istringstream input(R"(
        println(ceil(parse_num("123.456")))
        print(join(["111", "222", "333"], "AAA"))
    )");

    std::ostringstream output;
    bool result = interpret(input, output);

    EXPECT_TRUE(result);
    EXPECT_EQ(output.str(), "124\n111AAA222AAA333");
}
