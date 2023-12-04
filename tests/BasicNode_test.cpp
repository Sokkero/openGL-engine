
#include <gtest/gtest.h>

#include "../src/classes/nodeComponents/BasicNode.h"

using namespace Engine;

TEST(BasicNodeSuite, SetName)
{
    std::string name = "Donald";
    std::shared_ptr<BasicNode> node = std::make_shared<BasicNode>();
    node->setName(name);

    ASSERT_EQ(name, node->getName());
}

TEST(BasicNodeSuite, CallOnChildren)
{
    std::string name = "Donald";
    std::string notName = "Not Donald";

    std::shared_ptr<BasicNode> node = std::make_shared<BasicNode>();
    node->setName(notName);

    std::shared_ptr<BasicNode> nodeChild1 = std::make_shared<BasicNode>();
    std::shared_ptr<BasicNode> nodeChild2 = std::make_shared<BasicNode>();
    std::shared_ptr<BasicNode> nodeChild3 = std::make_shared<BasicNode>();
    node->addChild(nodeChild1);
    node->addChild(nodeChild2);
    node->addChild(nodeChild3);

    node->callOnAllChildrenRecursive([&name](BasicNode* node) { node->setName(name); });

    ASSERT_EQ("Not Donald", node->getName());
    ASSERT_EQ(name, nodeChild1->getName());
    ASSERT_EQ(name, nodeChild2->getName());
    ASSERT_EQ(name, nodeChild3->getName());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
