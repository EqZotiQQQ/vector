#include <gtest/gtest.h>

#include "ADefault.h"
#include "A0_mock.h"
#include "A3_mock.h"
#include "A5_mock.h"

#include <vector.h>
#include <iostream>
#include <limits>

using namespace no_std;

TEST(basic_operation, constructor_empty_vector_int) {
    Vector<int> v;
    ASSERT_NE(&v, nullptr);
}

TEST(basic_operation, constructor_empty_vector_A0) {
    Vector<A0> v;
    ASSERT_NE(&v, nullptr);
}

TEST(basic_operation, constructor_empty_vector_A3) {
    Vector<A3> v;
    ASSERT_NE(&v, nullptr);
}

TEST(basic_operation, constructor_empty_vector_A5) {
    Vector<A5> v;
    ASSERT_NE(&v, nullptr);
}

TEST(basic_operation, constructor_with_size_int) {
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
TEST(basic_operation, constructor_with_size_A5) {
    std::size_t f = 5;
    A5 s{"ass"};
    Vector<A5> v(f, s);
    ASSERT_NE(&v, nullptr);
    ASSERT_EQ(v.cap(), 5);
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
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    auto v1_addr = &v1;
    Vector<int> v2(std::move(v1));
    ASSERT_EQ(v1_addr, &v2);
    ASSERT_EQ(&v1, nullptr);
    ASSERT_EQ(v2[0],1);
    ASSERT_EQ(v2[1],2);
    ASSERT_EQ(v2[2],3);
    ASSERT_EQ(v2.len(),3);
}

TEST(basic_operation, move_constructor_A5) {
    Vector<A5> v1;
    v1.push_back("as");
    v1.push_back("fas");
    v1.push_back("sss");
    auto v1_addr = &v1;
    Vector<A5> v2(std::move(v1));
    ASSERT_EQ(v1_addr, &v2);
    ASSERT_EQ(&v1, nullptr);
    ASSERT_EQ(v2.len(),3);
}

TEST(basic_operation, copy_constructor) {
    Vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
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
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
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
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
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

TEST(push_back, push_back_A5) {
    Vector<A5> v1;
    v1.push_back("2");
    std::cout << '\n';
    v1.push_back("3");
}

TEST(push_back, detect_move_instead_of_copy_after_push_back_with_reallocation_int) {
    Vector<int> v1;
    for (int i = 0; i < 9; i++) {
        v1.push_back(i);
    }
    ASSERT_EQ(v1.len(), 9);
    ASSERT_EQ(v1.cap(), 16);
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(v1[i], i);
    }
}

TEST(push_back, detect_move_instead_of_copy_after_push_back_with_reallocation_A5) {
    Vector<A5> v1;
    std::vector<A5> v2;
    for (int i = 0; i < 10; i++) {
        std::string val = std::to_string(i);
        v1.push_back(val.c_str());
        v2.push_back(val.c_str());
    }
    ASSERT_EQ(v1.len(), 10);
    ASSERT_EQ(v1.cap(), 16);
    for (int i = 0; i < v1.len(); i++) {
        ASSERT_EQ(*v1[i].cstring, *v2[i].cstring);
    }
}
TEST(emplace_back, emplace_back_A5) {
    Vector<A5> v1;
    std::vector<A5> v2;
    for (int i = 0; i < 10; i++) {
        v1.emplace_back(std::to_string(i).c_str());
        v2.emplace_back(std::to_string(i).c_str());
    }
    ASSERT_EQ(v1.len(), 10);
    ASSERT_EQ(v1.cap(), 16);
    for (int i = 0; i < v1.len(); i++) {
        ASSERT_EQ(*v1[i].cstring, *v2[i].cstring);
    }
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

