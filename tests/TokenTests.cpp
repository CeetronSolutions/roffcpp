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

#include "gtest/gtest.h"

#include <string>

#include "Token.hpp"

using namespace roff;

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
TEST( TokenTests, SimpleTest )
{
    size_t start = 3u;
    size_t end   = 6u;
    Token  token( Token::Kind::TAG, start, end );

    ASSERT_EQ( Token::Kind::TAG, token.kind() );
    ASSERT_EQ( start, token.start() );
    ASSERT_EQ( end, token.end() );
}
