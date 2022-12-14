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

#include "Token.hpp"

#include <cassert>

using namespace roff;

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
Token::Token( Token::Kind kind, size_t start, size_t end )
    : m_kind( kind )
    , m_start( start )
    , m_end( end )
{
    assert( start <= end );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
Token::Kind Token::kind() const
{
    return m_kind;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
size_t Token::start() const
{
    return m_start;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
size_t Token::end() const
{
    return m_end;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::string Token::kindToString( Token::Kind kind )
{
    switch ( kind )
    {
        case Token::Kind::ROFF_BIN:
            return "roff-bin";
        case Token::Kind::ROFF_ASC:
            return "roff-asc";
        case Token::Kind::TAG:
            return "tag";
        case Token::Kind::ENDTAG:
            return "endtag";
        case Token::Kind::CHAR:
            return "char";
        case Token::Kind::BOOL:
            return "bool";
        case Token::Kind::BYTE:
            return "byte";
        case Token::Kind::INT:
            return "int";
        case Token::Kind::FLOAT:
            return "float";
        case Token::Kind::DOUBLE:
            return "double";
        case Token::Kind::ARRAY:
            return "array";
        case Token::Kind::STRING_LITERAL:
            return "string";
        case Token::Kind::NAME:
            return "name";
        case Token::Kind::BINARY_NUMERIC_VALUE:
            return "binary number";
        case Token::Kind::ARRAYBLOB:
            return "array-blob";
        case Token::Kind::NUMERIC_VALUE:
            return "number";
        default:
            assert( false );
    }

    return "";
}

int Token::binaryTokenSizeInBytes( Kind kind )
{
    switch ( kind )
    {
        case Token::Kind::BOOL:
            return 1;
        case Token::Kind::BYTE:
            return 1;
        case Token::Kind::INT:
            return 4;
        case Token::Kind::FLOAT:
            return 4;
        case Token::Kind::DOUBLE:
            return 8;
        default:
            assert( false );
    }

    return -1;
}
