/**
 * \copyright
 * Copyright (c) 2019- Aptiv. All rights reserved.
 * NOTICE: Removing this heading is prohibited.
 * All information contained herein is, and remains the property of Aptiv and its suppliers, if any.
 * The intellectual and technical concepts contained herein are proprietary to Aptiv and its suppliers and may be
 * covered by U.S. and Foreign Patents, patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
 * permission is obtained from Aptiv.
 *
 * \brief Main test file for the app
 */

#include "TempClass.hpp"

#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
    TempClass testObject;

    std::cout << "Test object data : " << testObject.getData() << std::endl;

    testObject.setData(12);
    std::cout << "Test object data : " << testObject.getData() << std::endl;

    return EXIT_SUCCESS;
}
