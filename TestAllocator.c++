/** @file TestAllocator.c++
 *  @brief Unit tests to check Allocator.h
 *   
 *  Google test is needed to run these tests that ensure the functions do their assigned job perfectly.
 */
// https://code.google.com/p/googletest/wiki/V1_7_Primer#Basic_Assertions

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator

#include "gtest/gtest.h"

#include "Allocator.h"

// --------------
// TestAllocator1
// --------------

template <typename A>
struct TestAllocator1 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A                  allocator_type;
    typedef typename A::value_type      value_type;
    typedef typename A::difference_type difference_type;
    typedef typename A::pointer         pointer;};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            Allocator<int,    100>,
            Allocator<double, 100> >
        my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const difference_type s = 1;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator1, test_10) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const difference_type s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
    const Allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator4, index) {
    Allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator5, index) {
    Allocator<int, 100> x;
    x.allocate(10);
    x.allocate(5);
    ASSERT_EQ(x[48], -20);}

TEST(TestAllocator6, index) {
    Allocator<int, 100> x;
    x.allocate(10);
    x.allocate(5);
    ASSERT_EQ(x[96], 16);}

TEST(TestAllocator7, index) {
    Allocator<int, 200> x;
    x.allocate(10);
    x.allocate(5);
    ASSERT_EQ(x[196], 116);}

TEST(TestAllocator8, index) {
    Allocator<int, 300> x;
    x.allocate(10);
    x.allocate(5);
    ASSERT_EQ(x[296], 216);}

// --------------
// TestAllocator3
// --------------

template <typename A>
struct TestAllocator3 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A                  allocator_type;
    typedef typename A::value_type      value_type;
    typedef typename A::difference_type difference_type;
    typedef typename A::pointer         pointer;};

typedef testing::Types<
            Allocator<int,    100>,
            Allocator<double, 100> >
        my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const difference_type s = 1;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator3, test_100) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const difference_type s = 5;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator3, test_10) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const difference_type s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

TEST(AllocatorTests, valid){
    Allocator<int, 200> x;
    x[0] = 4;
    x[8] = 4;
    x[12] = -180;
    x[196] = -180;
    ASSERT_TRUE(x.valid());}

TEST(AllocatorTests1, valid){
    Allocator<int, 300> x;
    x[0] = 4;
    x[8] = 4;
    x[12] = -80;
    x[96] = -80;
    x[100] = 4;
    x[108] = 4;
    x[112] = -180;
    x[296] = -180;
    ASSERT_TRUE(x.valid());}

TEST(AllocatorTests2, valid){
    Allocator<int, 400> x;
    x[0] = 4;
    x[8] = 4;
    x[12] = -380;
    x[396] = -380;
    ASSERT_TRUE(x.valid());}

TEST(AllocatorTests3, valid){
    Allocator<int, 500> x;
    x[0] = 4;
    x[8] = 4;
    x[12] = -480;
    x[496] = -480;
    ASSERT_TRUE(x.valid());}

TEST(AllocatorTests4, valid){
    Allocator<int, 300> x;
    x[0] = 4;
    x[8] = 4;
    x[12] = -80;
    x[96] = -80;
    x[100] = 0;
    x[108] = 0;
    x[112] = -180;
    x[296] = -180;
    ASSERT_FALSE(x.valid());}

TEST(TestValid, valid_4) {
    Allocator<double, 32> x;
    ASSERT_TRUE(x.valid());
    x.allocate(2);
    ASSERT_TRUE(x.valid());
}

TEST(TestValid1, valid) {
    Allocator<int, 12> x;
    ASSERT_TRUE(x.valid());
    x.allocate(1);
    ASSERT_TRUE(x.valid());
}

TEST(TestValid2, valid) {
    Allocator<int, 13> x;
    ASSERT_TRUE(x.valid());
    x.allocate(1);
    ASSERT_TRUE(x.valid());
}

TEST(TestValid3, valid) {
    Allocator<int, 12> x;
    ASSERT_TRUE(x.valid());
    ASSERT_THROW(x.allocate(2), std::bad_alloc);
}

TEST(TestAllocate, valid) {
    Allocator<int, 12> x;
    ASSERT_THROW(x.allocate(0), std::bad_alloc);
}

TEST(TestAllocate1, valid) {
    Allocator<double, 16> x;
    ASSERT_THROW(x.allocate(3), std::bad_alloc);
}

TEST(TestAllocate2, valid) {
    Allocator<double, 16> x;
    ASSERT_THROW(x.allocate(9001), std::bad_alloc);
}

TEST(TestAllocate3, valid) {
    Allocator<double, 16> x;
    x.allocate(1);
    ASSERT_TRUE(x.valid());
}

TEST(TestAllocate4, valid) {
    Allocator<double, 16> x;
    ASSERT_THROW(x.allocate(2), std::bad_alloc);
}

TEST(TestAllocate5, valid) {
    Allocator<double, 16> x;
    ASSERT_THROW(x.allocate(9001), std::bad_alloc);
}

TEST(TestAllocate6, valid) {
    Allocator<double, 9000> x;
    ASSERT_THROW(x.allocate(9000), std::bad_alloc);
}

TEST(TestAllocate7, valid) {
    Allocator<int, 12> x;
    ASSERT_THROW(x.allocate(10), std::bad_alloc);
}

TEST(TestAllocate8, valid) {
    Allocator<int, 15> x;
    ASSERT_THROW(x.allocate(14), std::bad_alloc);
}