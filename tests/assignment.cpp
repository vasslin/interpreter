#include <lib/interpreter/interpreter.h>
#include <gtest/gtest.h>


TEST(CompoundAssignmentTestSuite, BasicCompoundAssignment) {
    std::istringstream input(R"(
        x = 10
        x += 5
        print(x)
        y = 20
        y -= 3
        print(y)
        z = 4
        z *= 3
        print(z)
    )");

    std::ostringstream output;
    bool result = interpret(input, output);

    EXPECT_TRUE(result);
    EXPECT_EQ(output.str(), "151712");
}

TEST(CompoundAssignmentTestSuite, StringCompoundAssignment) {
    std::istringstream input(R"(
        s = "Hello,"
        s += " Itmo"
        print(s)
    )");

    std::ostringstream output;
    bool result = interpret(input, output);

    EXPECT_TRUE(result);
    EXPECT_EQ(output.str(), "Hello, Itmo");
}

TEST(CompoundAssignmentTestSuite, ListCompoundAssignment) {
    std::istringstream input(R"(
        arr = [1, 2, 3]
        arr += [4, 5]
        print(len(arr))
    )");

    std::ostringstream output;
    bool result = interpret(input, output);

    EXPECT_TRUE(result);
    EXPECT_EQ(output.str(), "5");
}