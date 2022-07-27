#include "gtest/gtest.h"
#include <string>
#include <unordered_map>
#include <container/UrlParams.h>
#include <exception>
#define TEST_SCALE 1000

class UrlParamsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
    UrlParams p0_;
};

TEST_F(UrlParamsTest, PushBackWork)
{
    for (size_t idx = 0; idx < TEST_SCALE; idx++)
    {
        ASSERT_NO_FATAL_FAILURE(p0_.push_back(std::to_string(idx)));
    }
}

TEST_F(UrlParamsTest, IterationWork)
{
    size_t count = 0;
    for (auto &param : p0_)
    {
        ASSERT_EQ(param, std::to_string(count++));
    };
}

TEST_F(UrlParamsTest, IllegalAccessPrevent)
{
    auto p = p0_.end();
    ASSERT_THROW(p++, std::out_of_range);
}

TEST_F(UrlParamsTest, OnlyMovable)
{

    // move by move constructor
    auto testFn = [this]()
    {
        UrlParams p1_;
        std::string test_string[] = {"test", "test", "test"};
        for (size_t idx = 0; idx < 3; idx++)
        {
            p1_.push_back(test_string[idx]);
        }
        auto p = std::move(p1_);
        auto q = p.begin();
        while (q != p.end())
        {
            q++;
        };

        for (auto param : p)
        {
            EXPECT_EQ(param, "test");
        };
    };
    ASSERT_NO_FATAL_FAILURE(testFn());
}

TEST_F(UrlParamsTest, UseAsMapValue)
{
    std::unordered_map<std::string, UrlParams> map;
    UrlParams p;
    std::string test_string[] = {"test", "test", "test"};
    for (size_t idx = 0; idx < 3; idx++)
    {
        p.push_back(test_string[idx]);
    }

    // normal usage
    map.insert(std::make_pair("p", std::move(p)));
    UrlParams &q = map.find("p")->second;
    for (auto item : q)
    {
        EXPECT_EQ(item, "test");
    }
}

TEST_F(UrlParamsTest, DeconstructorWork)
{
    UrlParams *p = new UrlParams();
    std::string test_string[] = {"test", "test", "test"};
    for (size_t idx = 0; idx < 3; idx++)
    {
        p->push_back(test_string[idx]);
    }
    ASSERT_NO_FATAL_FAILURE(delete p);
}