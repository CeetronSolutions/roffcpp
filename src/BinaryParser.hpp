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

#include "Parser.hpp"
#include "Token.hpp"

#include "RoffScalar.hpp"

#include <istream>
#include <map>
#include <variant>
#include <vector>

namespace roff
{
class BinaryParser : public Parser
{
public:
    BinaryParser();

    std::vector<std::string> parseStringArray( const std::vector<Token>& tokens,
                                               std::istream&             stream,
                                               long                      startIndex,
                                               long                      arrayLength ) const override;

    std::vector<int> parseIntArray( const std::vector<Token>& tokens,
                                    std::istream&             stream,
                                    long                      startIndex,
                                    long                      arrayLength ) const override;

    std::vector<double> parseDoubleArray( const std::vector<Token>& tokens,
                                          std::istream&             stream,
                                          long                      startIndex,
                                          long                      arrayLength ) const override;

    std::vector<float> parseFloatArray( const std::vector<Token>& tokens,
                                        std::istream&             stream,
                                        long                      startIndex,
                                        long                      arrayLength ) const override;

    std::vector<char> parseByteArray( const std::vector<Token>& tokens,
                                      std::istream&             stream,
                                      long                      startIndex,
                                      long                      arrayLength ) const override;

    std::string   parseString( const Token& token, std::istream& stream ) const override;
    int           parseInt( const Token& token, std::istream& stream ) const override;
    double        parseDouble( const Token& token, std::istream& stream ) const override;
    float         parseFloat( const Token& token, std::istream& stream ) const override;
    bool          parseBool( const Token& token, std::istream& stream ) const override;
    unsigned char parseByte( const Token& token, std::istream& stream ) const override;

private:
    template <typename T, Token::Kind K>
    T parseScalar( const Token& token, std::istream& stream ) const
    {
        stream.clear();
        int  length = Token::binaryTokenSizeInBytes( K );
        auto start  = token.start();
        T    value;
        stream.seekg( start );
        stream.read( reinterpret_cast<char*>( &value ), length );
        return value;
    }

    template <typename T, Token::Kind K>
    std::vector<T> parseArray( const std::vector<Token>& tokens, std::istream& stream, long startIndex, long arrayLength ) const
    {
        stream.clear();
        std::vector<T> values( arrayLength );
        int            length = Token::binaryTokenSizeInBytes( K );
        auto           start  = tokens[startIndex].start();
        stream.seekg( start );
        stream.read( reinterpret_cast<char*>( values.data() ), static_cast<std::streamsize>( arrayLength ) * length );
        return values;
    }
};
} // namespace roff
