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

#include "Token.hpp"
#include "Tokenizer.hpp"

#include <istream>
#include <optional>
#include <vector>

namespace roff
{
class BinaryTokenizer : public Tokenizer
{
public:
    BinaryTokenizer();
    virtual ~BinaryTokenizer();

    std::vector<Token>   tokenizeStream( std::istream& stream ) override;
    bool                 tokenizeSpace( std::istream& stream ) override;
    std::optional<Token> tokenizeString( std::istream& stream ) override;
    Token                tokenizeName( std::istream& stream ) override;
    std::vector<Token>   tokenizeArrayTagKey( std::istream& stream ) override;

    std::vector<Token>   tokenizeArrayData( std::istream& stream, size_t numElements, Token::Kind kind );
    std::optional<Token> tokenizeNumber( std::istream& stream, Token::Kind kind );
    std::optional<Token> tokenizeValue( std::istream& stream, Token::Kind kind );

    std::optional<Token> tokenizeWord( std::istream& stream, const std::string& keywork, Token::Kind kind ) override;

protected:
    std::vector<Token>   tokenizeTagKeyInternal( std::istream& stream ) override;
    std::optional<Token> tokenizeStringInternal( std::istream& stream, bool skipDelimiter );
};
} // namespace roff
