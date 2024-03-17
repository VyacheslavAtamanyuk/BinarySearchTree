#include "lib/BST_Container.h"
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <limits>

template <typename T>
void InOrderIteration(std::vector<T>& result, Node<T>* current_node) {
    if (current_node) {
        InOrderIteration(result, current_node->GetLeft());
        result.push_back(current_node->GetKey());
        InOrderIteration(result, current_node->GetRight());
    }
}

template <typename T>
void PreOrderIteration(std::vector<T>& result, Node<T>* current_node) {
    if (current_node) {
        result.push_back(current_node->GetKey());
        PreOrderIteration(result, current_node->GetLeft());
        PreOrderIteration(result, current_node->GetRight());
    }
}

template <typename T>
void PostOrderIteration(std::vector<T>& result, Node<T>* current_node) {
    if (current_node) {
        PostOrderIteration(result, current_node->GetLeft());
        PostOrderIteration(result, current_node->GetRight());
        result.push_back(current_node->GetKey());
    }
}

TEST(ContainerTestsSuite, InOrder) {
    BST_Container<int, InOrder> container{50, 40, 30, 20, 10, 31, 39, 38, 37, 36, 45, 43,
                                          44, 42, 41, 47, 46, 48, 60, 70, 80, 90, 81, 82,
                                          83, 84, 85, 69, 68, 55, 57, 56, 58, 53, 52, 51};

    std::vector<int> result1;
    for (auto it = container.begin(); it != container.end(); ++it) {
        result1.push_back(*it);
    }

    std::vector<int> result2;
    InOrderIteration(result2, container.GetTree()->GetRoot());

    ASSERT_EQ(result1, result2);
}

TEST(ContainerTestsSuite, PreOrder) {
    BST_Container<int, PreOrder> container{16, 67, 90, 34, 54, 23, 0, -21, 20, 28, 29,
                                           45, 56, 78 , 94, 9, 3, 24, 560, 280, 459, 234,
                                           780, 345, 12345, 98765, 2000, 2024, 2, 34, 12};
    std::vector<int> result1;
    for (auto it = container.begin(); it != container.end(); ++it) {
        result1.push_back(*it);
    }

    std::vector<int> result2;
    PreOrderIteration(result2, container.GetTree()->GetRoot());

    ASSERT_EQ(result1, result2);
}

TEST(ContainerTestsSuite, PostOrder) {
    BST_Container<float, PostOrder> container{-2024, 2024, 567, 3, 8, 23, 45, 90, 314,
                                            3141, 31415, 271, 27, 17, 71, 30, -2026,
                                            -2025, 2098, 2077, 7702, 207, 14, 297, 365};
    std::vector<float> result1;
    for (auto it = container.begin(); it != container.end(); ++it) {
        result1.push_back(*it);
    }

    std::vector<float> result2;
    PostOrderIteration(result2, container.GetTree()->GetRoot());

    ASSERT_EQ(result1, result2);
}

TEST(ContainerTestsSuite, TestsForInitializationByList) {
    std::vector<int> v{1, 2, 3, -1, -2};

    BST_Container<int, InOrder> container(v.begin(), v.end());

    std::vector<int> result;
    for (auto it = container.begin(); it != container.end(); ++it) {
        result.push_back(*it);
    }

    for (auto it = v.begin(); it != v.end(); ++it) {
        ASSERT_TRUE(container.contains(*it));
    }
}

TEST(ContainerTestsSuite, TestsForInitializationByNumber) {
    BST_Container<int, InOrder> container(5);

    ASSERT_EQ(container.size(), 1);
    ASSERT_FALSE(container.empty());
    ASSERT_EQ(*container.begin(), 5);
    ASSERT_EQ(std::distance(container.begin(), container.end()), 1);
}

TEST(ContainerTestsSuite, TestsForIterator) {
    BST_Container<int, InOrder> container{50, 40, 30, 20, 10, 31, 39, 38, 37, 36, 45, 43,
                                          44, 42, 41, 47, 46, 48, 60, 70, 80, 90, 81, 82,
                                          83, 84, 85, 69, 68, 55, 57, 56, 58, 53, 52, 51};

    std::vector<int> result1;
    for (auto it = container.begin(); it != container.cend(); ++it) {
        result1.push_back(*it);
    }

    std::vector<int> result2;
    for (auto it = container.crbegin(); it != container.rend(); ++it) {
        result2.push_back(*it);
    }
    std::reverse(result2.begin(), result2.end());

    ASSERT_EQ(result1, result2);

    auto it1 = result1.end();
    --it1;
    ASSERT_EQ(*it1, 90);

    auto it2 = result2.crend();
    --it2;
    ASSERT_EQ(*it2, 10);
}

TEST(ContainerTestsSuite, TestsForСopy) {
    BST_Container<int, InOrder> container1{100, 94, 3, 8, 96, 52, 25, 228,
                                          786, 34, 69, 96, 420, 35};

    BST_Container<int, InOrder> container2 = container1;

    ASSERT_EQ(container1, container2);

    container1.erase(100);
    ASSERT_TRUE(container2.contains(100));
}

TEST(ContainerTestsSuite, TestsForСopyAndAssignment) {
    BST_Container<int, InOrder> container1{100, 94, 3, 8, 96, 52, 25, 228,
                                           786, 34, 69, 96, 420, 35};

    BST_Container<int, InOrder> container2 = {1, 2, 3, 4, 100, 94};

    container2 = container1;

    ASSERT_EQ(container1, container2);
    ASSERT_EQ(container2, container1);

    std::vector<int> result1;
    for (auto it = container1.begin(); it != container1.cend(); ++it) {
        result1.push_back(*it);
    }

    std::vector<int> result2;
    for (auto it = container2.cbegin(); it != container2.end(); ++it) {
        result2.push_back(*it);
    }

    ASSERT_EQ(result1, result2);
}

TEST(ContainerTestsSuite, TestsForDelete) {
    BST_Container<float, PostOrder> container1{-2024, 2024, 567, 3, 8, 23, 45, 90, 314,
                                              3141, 31415, 271, 27, 17, 71, 30, -2026,
                                              -2025, 2098, 2077, 7702, 207, 14, 297, 365};

    ASSERT_EQ(container1.erase(-2024), 1);
    ASSERT_EQ(container1.erase(666), 0);

    size_t current_size = container1.size();
    container1.erase(555);
    size_t size_after_possible_removal = container1.size();
    ASSERT_EQ(current_size, size_after_possible_removal);

    BST_Container<float, PostOrder> container2 = container1;
    container1.erase(3141);
    ASSERT_FALSE(container1 == container2);
    ASSERT_TRUE(container2.contains(3141));
}

TEST(ContainerTestsSuite, TestsForClear) {
    BST_Container<int, InOrder> container1{100, 94, 3, 8, 96, 52, 25, 228,
                                           786, 34, 69, 96, 420, 35};

    BST_Container<int, InOrder> container2 = container1;

    container1.clear();

    ASSERT_TRUE(container1.empty());
    ASSERT_EQ(std::distance(container1.begin(), container1.end()), 0);
}

TEST(ContainerTestsSuite, TestsForLowerAndUpperBoundAndFind) {
    BST_Container<int, PreOrder> container{26, 27, 90, -234, 56, 34, 555, 234, 78,
                                           87, 900, 239, 239, 239, 239, 901, 109,
                                           57, 179, 239, 34, 5789, 9087, 1234};

    ASSERT_TRUE(container.find(239) != container.end());
    container.erase(239);
    ASSERT_TRUE(container.find(239) == container.end());

    ASSERT_EQ(*container.lower_bound(-234), -234);
    ASSERT_EQ(*container.lower_bound(-233.9), 26);
    ASSERT_EQ(*container.lower_bound(-235), -234);

    ASSERT_EQ(*container.upper_bound(9087.3), 9087);
    ASSERT_EQ(*container.upper_bound(9086.6), 5789);

    ASSERT_TRUE(container.upper_bound(-999999) == container.end());
    ASSERT_TRUE(container.lower_bound(999999) == container.end());
    ASSERT_EQ(*container.upper_bound(999999), 9087);
    ASSERT_EQ(*container.lower_bound(-999999), -234);
}

TEST(ContainerTestsSuite, TestsForDestructor) {
    for (size_t i = 0; i < 1000000; ++i) {
        BST_Container<size_t, PreOrder> container{16, 67, 90, 34, 54, 23, 0, 21, 20, 28, 29,
                                                  45, 56, 78 , 94, 9, 3, 24, 560, 280, 459, 234,
                                                  780, 345, 12345, 98765, 2000, 2024, 2, 34, 12};
    }
}