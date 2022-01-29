#include "gtest/gtest.h"

int main(int argc, char** argv) {
    int iret;
    testing::InitGoogleTest(&argc, argv);
    iret = RUN_ALL_TESTS();
    return iret;
}
