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

#include "RoffScalar.hpp"

#include <istream>
#include <map>
#include <variant>
#include <vector>

class Parser
{
public:
    Parser();
    virtual ~Parser();

    void parse( std::istream&                                     stream,
                const std::vector<Token>&                         tokens,
                std::vector<std::pair<std::string, RoffScalar>>&  scalarValues,
                std::vector<std::pair<std::string, Token::Kind>>& arrayTypes,
                std::map<std::string, std::pair<long, long>>&     arrayInfo ) const;

    std::pair<std::string, RoffScalar> parseSimpleType( std::vector<Token>::const_iterator& it,
                                                        const std::string&                  tagGroupName,
                                                        std::istream&                       stream ) const;

    virtual std::vector<std::string> parseStringArray( const std::vector<Token>& tokens,
                                                       std::istream&             stream,
                                                       long                      startIndex,
                                                       long                      arrayLength ) const = 0;

    virtual std::vector<int>
        parseIntArray( const std::vector<Token>& tokens, std::istream& stream, long startIndex, long arrayLength ) const = 0;

    virtual std::vector<double> parseDoubleArray( const std::vector<Token>& tokens,
                                                  std::istream&             stream,
                                                  long                      startIndex,
                                                  long                      arrayLength ) const = 0;

    virtual std::vector<float> parseFloatArray( const std::vector<Token>& tokens,
                                                std::istream&             stream,
                                                long                      startIndex,
                                                long                      arrayLength ) const = 0;

    virtual std::vector<char> parseByteArray( const std::vector<Token>& tokens,
                                              std::istream&             stream,
                                              long                      startIndex,
                                              long                      arrayLength ) const = 0;

    virtual std::string   parseString( const Token& token, std::istream& stream ) const = 0;
    virtual int           parseInt( const Token& token, std::istream& stream ) const    = 0;
    virtual double        parseDouble( const Token& token, std::istream& stream ) const = 0;
    virtual float         parseFloat( const Token& token, std::istream& stream ) const  = 0;
    virtual bool          parseBool( const Token& token, std::istream& stream ) const   = 0;
    virtual unsigned char parseByte( const Token& token, std::istream& stream ) const   = 0;

    static bool isSimpleType( Token::Kind kind );
};
