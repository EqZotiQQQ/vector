#include <gtest/gtest.h>

#include "mock_objects.h"

#include <vector.h>
#include <iostream>
#include <limits>

using namespace no_std;

TEST(basic_operation, constructor_empty_vector_int) {
    Vector<int> v;
    ASSERT_NE(&v, nullptr);
}

TEST(basic_operation, constructor_non_empty) {
    std::size_t f = 5;
    int s = 2;
    Vector<int> v(f, s);
    ASSERT_NE(&v, nullptr);
    ASSERT_EQ(v[0],2);
    ASSERT_EQ(v[1],2);
    ASSERT_EQ(v[3],2);
    ASSERT_EQ(v.cap(), 5);
    ASSERT_NE(v[7],2);
}

TEST(basic_operation, constructor_non_empty_bad_alloc) {
    std::size_t i64_max = std::numeric_limits<i64>::max();
    int s = 2;
    EXPECT_THROW({
                     try {
                         Vector<int> v(i64_max, s);
                     } catch (const std::bad_alloc &e) {
                         EXPECT_STREQ("std::bad_alloc", e.what());
                         throw;
                     }
                 }, std::bad_alloc);
}

TEST(basic_operation, move_constructor) {
    Vector<int> v1;
    v1.push_back(1,2,3);
    auto v1_addr = &v1;
    Vector<int> v2(std::move(v1));
    ASSERT_EQ(v1_addr, &v2);
    ASSERT_EQ(&v1, nullptr);
    ASSERT_EQ(v2[0],1);
    ASSERT_EQ(v2[1],2);
    ASSERT_EQ(v2[2],3);
    ASSERT_EQ(v2.len(),3);
}

TEST(basic_operation, copy_constructor) {
    Vector<int> v1;
    v1.push_back(1,2,3);
    Vector<int> v2(v1);
    ASSERT_NE(&v1, nullptr);
    ASSERT_NE(&v2, nullptr);
    ASSERT_NE(&v2, &v1);
    ASSERT_EQ(v2[0],1);
    ASSERT_EQ(v2[1],2);
    ASSERT_EQ(v2[2],3);
}

TEST(basic_operation, assignment_operation) {
    Vector<int> v1;
    v1.push_back(1,2,3);
    Vector<int> v2 = v1;
    ASSERT_NE(&v1, nullptr);
    ASSERT_NE(&v2, nullptr);
    ASSERT_NE(&v2, &v1);
    ASSERT_EQ(v2[0],1);
    ASSERT_EQ(v2[1],2);
    ASSERT_EQ(v2[2],3);
}

TEST(basic_operation, assignment_move_operation) {
    Vector<int> v1;
    v1.push_back(1,2,3);
    Vector<int> v2 = std::move(v1);
    ASSERT_EQ(&v1, nullptr);
    ASSERT_NE(&v2, nullptr);
    ASSERT_EQ(v2[0],1);
    ASSERT_EQ(v2[1],2);
    ASSERT_EQ(v2[2],3);
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

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

