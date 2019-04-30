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
 * \brief Temporary test class.
 */

class TempClass
{
public:
    TempClass() = default;
    virtual ~TempClass() = default;

    int getData();
    void setData(int newData);

private:
    int memberData_{1};
};