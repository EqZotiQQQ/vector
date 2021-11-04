#include <gtest/gtest.h>

#include <vector.h>
#include <iostream>

using namespace no_std;

TEST(basic_operation, constructor_empty) {
    Vector<int> v;
    ASSERT_NE(&v, nullptr);
}

TEST(basic_operation, constructor_non_empty) {
    std::size_t f = 5;
    int s = 2;
    Vector<int> v(f, s);
    ASSERT_NE(&v, nullptr);
}

TEST(basic_operation, move_constructor) {
    Vector<int> v1;
    auto v1_addr = &v1;
    Vector<int> v2(std::move(v1));
    ASSERT_EQ(v1_addr, &v2);
    ASSERT_EQ(&v1, nullptr);
}

TEST(basic_operation, copy_constructor) {
    Vector<int> v1;
    Vector<int> v2(v1);
    ASSERT_NE(&v1, nullptr);
    ASSERT_NE(&v2, nullptr);
}

TEST(basic_operation, assignment_operation) {
    Vector<int> v1;
    Vector<int> v2 = v1;
    ASSERT_NE(&v1, nullptr);
    ASSERT_NE(&v2, nullptr);
}

TEST(basic_operation, assignment_move_operation) {
    Vector<int> v1;
    Vector<int> v2 = std::move(v1);
    ASSERT_EQ(&v1, nullptr);
    ASSERT_NE(&v2, nullptr);
}

TEST(push_back, push_back_test) {
    Vector<int> v1;
    v1.push_back(2);
    v1.push_back(3);
    ASSERT_EQ(v1.len(), 2);
    ASSERT_EQ(v1.cap(), 4);
    ASSERT_EQ(v1[0], 2);
    ASSERT_EQ(v1[1], 3);
}

TEST(push_back, push_back_fold_expr) {
    Vector<int> v1;
    v1.push_back(2,3,4);
    ASSERT_EQ(v1.len(), 3);
    ASSERT_EQ(v1.cap(), 4);
    ASSERT_EQ(v1[0], 2);
    ASSERT_EQ(v1[1], 3);
    ASSERT_EQ(v1[2], 4);
}

TEST(push_back, push_back_fold_expr_realloc) {
    Vector<int> v1;
    v1.push_back(1,2,3,4,5,6,7,8,9);
    ASSERT_EQ(v1.len(), 9);
    ASSERT_EQ(v1.cap(), 16);
    ASSERT_EQ(v1[0], 1);
    ASSERT_EQ(v1[1], 2);
    ASSERT_EQ(v1[2], 3);
    ASSERT_EQ(v1[7], 8);
}

TEST(push_front, push_front) {
    Vector<int> v1;
    v1.push_back(1,2,3);
    v1.push_front(10,20,30);
    ASSERT_EQ(v1[0],10);
    ASSERT_EQ(v1[1],20);
    ASSERT_EQ(v1[2],30);
    ASSERT_EQ(v1[3],1);
    ASSERT_EQ(v1[4],2);
    ASSERT_EQ(v1[5],3);
    ASSERT_EQ(v1.len(),6);
}

TEST(pop_back, pop_back) {
    Vector<int> v1;
    v1.push_back(1,2,3);
    v1.push_front(10,20,30);
    ASSERT_EQ(v1[0],10);
    ASSERT_EQ(v1[1],20);
    ASSERT_EQ(v1[2],30);
    ASSERT_EQ(v1[3],1);
    ASSERT_EQ(v1[4],2);
    v1.pop_back();
    ASSERT_EQ(v1.len(),5);
}

TEST(pop_front, pop_front) {
    Vector<int> v1;
    v1.push_back(1,2,3);
    v1.push_front(10,20,30);
    v1.pop_front();
    ASSERT_EQ(v1.len(),5);
    ASSERT_EQ(v1[0],20);
    ASSERT_EQ(v1[1],30);
    ASSERT_EQ(v1[2],1);
    ASSERT_EQ(v1[3],2);
    ASSERT_EQ(v1[4],3);
}



int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

