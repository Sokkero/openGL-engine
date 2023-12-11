find_package(GTest REQUIRED)

add_executable(tests BasicNode_test.cpp ../src/classes/nodeComponents/BasicNode.cpp ../src/classes/nodeComponents/BasicNode.h)

target_link_libraries(tests
        PRIVATE
        GTest::GTest)

add_test(engineTests tests)

