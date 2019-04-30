/**
 * @copyright
 * Copyright (c) 2019- Aptiv. All rights reserved.
 * NOTICE: Removing this heading is prohibited.
 * All information contained herein is, and remains the property of Aptiv and its suppliers, if any.
 * The intellectual and technical concepts contained herein are proprietary to Aptiv and its suppliers and may be
 * covered by U.S. and Foreign Patents, patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
 * permission is obtained from Aptiv.
 *
 * @brief Code snippets that will cause sanitizers to fire.
 *
 * Code sanitization is an instrumentation technique that helps detect bugs and trace execution paths.
 * The following snippets represent common mistakes in show how sanitizers can help in detecting them.
 * The following code sanitizers are covered: Address Sanitizer (ASan), Leak Sanitizer (LSan),
 * Thread Sanitizer (TSan), and Undefined Behavior Sanitizer (UBSan).
 *
 * Note that Leak Sanitizer is a subset of the Address Sanitizer, in the sense that LSan will be invoked
 * whenever ASan is in use.
 *
 * Many static analysis tools will warn you about the numerous suspicious lines in this files, you may therefore
 * want to ensure that static analysis tools are disabled on this target.  Also, check that this target is not
 * excluded from sanitization via EXCLUDE_FROM_SANITIZE_xxxx properties.
 */

#include <gtest/gtest.h>

#include <thread>

// LSan, ASan
TEST(Sanitizer, memory_leak)
{
    auto* buffer = new char[16];
    sprintf(buffer, "I'm leaking!");
    printf("%s\n", buffer);
}

// ASan, TSan
TEST(Sanitizer, use_after_free)
{
    auto* array = new int[16];
    delete[] array;
    std::cout << array[0] << std::endl; // BOOM
}

// UBSan
TEST(Sanitizer, undefined_behavior)
{
    int k = 0x7fffffff;
    k += 1;
}

TEST(Sanitizer, data_race)
{
    int data;

    std::thread t([&] { data = 1; });
    data = 2;
    t.join();
}