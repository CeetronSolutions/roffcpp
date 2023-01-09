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

#include <istream>
#include <optional>
#include <vector>

class Tokenizer
{
public:
    virtual ~Tokenizer();

    virtual std::vector<Token>   tokenizeStream( std::istream& stream )                                             = 0;
    virtual bool                 tokenizeSpace( std::istream& stream )                                              = 0;
    virtual std::optional<Token> tokenizeString( std::istream& stream )                                             = 0;
    virtual Token                tokenizeName( std::istream& stream )                                               = 0;
    virtual std::vector<Token>   tokenizeArrayTagKey( std::istream& stream )                                        = 0;
    virtual std::optional<Token> tokenizeWord( std::istream& stream, const std::string& keywork, Token::Kind kind ) = 0;

    virtual std::vector<Token> tokenizeTagKey( std::istream& stream );
    std::vector<Token>         tokenizeStream( std::istream& stream, Token::Kind expectedKind );
    virtual std::vector<Token> tokenizeTagGroup( std::istream& stream );
    virtual bool               tokenizeComment( std::istream& stream );
    virtual void               tokenizeDelimiter( std::istream& stream );
    virtual Token              tokenizeSimpleType( std::istream& stream );
    virtual Token              tokenizeKeyword( std::istream& stream );
    virtual Token              tokenizeFileType( std::istream& stream );
    virtual Token tokenizeKeyword( std::istream& stream, const std::vector<std::pair<Token::Kind, std::string>>& keywords );

protected:
    virtual std::vector<Token> tokenizeTagKeyInternal( std::istream& stream ) = 0;
};
