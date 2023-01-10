/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2023-     Equinor ASA
//
//  roffcpp is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  roffcpp is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

namespace roff
{
class Token
{
public:
    enum class Kind
    {
        ROFF_ASC,
        ROFF_BIN,
        TAG,
        ENDTAG,
        STRING_LITERAL,
        NUMERIC_VALUE,
        BINARY_NUMERIC_VALUE,
        NAME,
        CHAR,
        BOOL,
        BYTE,
        INT,
        FLOAT,
        DOUBLE,
        ARRAY,
        ARRAYBLOB,
    };

    Token( Kind, size_t start, size_t end );

    Kind   kind() const;
    size_t start() const;
    size_t end() const;

    static std::string kindToString( Kind kind );
    static int         binaryTokenSizeInBytes( Kind kind );

private:
    Kind   m_kind;
    size_t m_start;
    size_t m_end;
};
} // namespace roff
