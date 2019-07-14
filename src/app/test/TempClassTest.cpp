/**
 * \copyright
 * Copyright (c) 2019- Aptiv. All rights reserved.
 * NOTICE: Removing this heading is prohibited.
 * All information contained herein is, and remains the property of Aptiv and
 * its suppliers, if any. The intellectual and technical concepts contained
 * herein are proprietary to Aptiv and its suppliers and may be covered by U.S.
 * and Foreign Patents, patents in process, and are protected by trade secret or
 * copyright law. Dissemination of this information or reproduction of this
 * material is strictly forbidden unless prior written permission is obtained
 * from Aptiv.
 *
 * \brief Tests for the temporary test class
 */

#include "app/TempClass.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

std::unordered_set<router::State, router::State_hash, router::State_compare> visited{};
router::State a;
a.passenges.insert(1);
a.passenges.insert(2);
visited.insert(a);

router::State b;
b.passenges.insert(2);
b.passenges.insert(1);

if (visited.find(b) != visited.end())
{
    std::cout << "what?" << '\n';
}

// NOLINTNEXTLINE (fuchsia-default-arguments)
TEST(TempClass, GivenDefaultConstrWhenCreatedThenNoExceptions)
{
    ASSERT_NO_THROW(TempClass());
}

// NOLINTNEXTLINE (fuchsia-default-arguments)
TEST(TempClass, GivenDefaultConstrWhenGetDataThenDefaultVal)
{
    TempClass newTempObj;

    ASSERT_EQ(1, newTempObj.getData());
}

// NOLINTNEXTLINE (fuchsia-default-arguments)
TEST(TempClass, GivenSetDataPositiveWhenGetDataThenLatestValue)
{
    TempClass newTempObj;

    newTempObj.setData(32);
    ASSERT_EQ(32, newTempObj.getData());
}

// NOLINTNEXTLINE (fuchsia-default-arguments)
TEST(TempClass, GivenSetDataNegativeWhenGetDataThenLatestValue)
{
    TempClass newTempObj;

    newTempObj.setData(-72);
    ASSERT_EQ(-72, newTempObj.getData());
}

/**
 * @brief Group of tests for the TempClass
 */
class TempClassWithPriorValue : public ::testing::Test
{
protected:
    void SetUp() override { tempClass_.setData(priorSetValue_); }

    TempClass tempClass_{};
    const int priorSetValue_{24};
};

// NOLINTNEXTLINE (fuchsia-default-arguments)
TEST_F(TempClassWithPriorValue, GivenSetDataPositiveWhenGetDataThenNewVal)
{
    tempClass_.setData(255);
    ASSERT_EQ(255, tempClass_.getData());
}

// NOLINTNEXTLINE (fuchsia-default-arguments)
TEST_F(TempClassWithPriorValue, GivenSetDataNegativeWhenGetDataThenNewVal)
{
    tempClass_.setData(-100);
    ASSERT_EQ(-100, tempClass_.getData());
}