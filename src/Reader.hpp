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
#include "RoffScalar.hpp"
#include "Token.hpp"

#include <istream>
#include <map>
#include <memory>
#include <vector>

class Reader
{
public:
    Reader( std::istream& stream );

    void parse();

    std::vector<std::pair<std::string, RoffScalar>>  scalarNamedValues() const;
    std::vector<std::pair<std::string, Token::Kind>> getNamedArrayTypes() const;
    size_t                                           getArrayLength( const std::string& keyword ) const;

    std::vector<std::string> getStringArray( const std::string& keyword );
    std::vector<int>         getIntArray( const std::string& keyword );
    std::vector<double>      getDoubleArray( const std::string& keyword );
    std::vector<float>       getFloatArray( const std::string& keyword );
    std::vector<char>        getByteArray( const std::string& keyword );

private:
    void parseAscii();
    void parseBinary();
    bool detectFileTypeFromFirstToken( std::istream& stream );

    std::vector<Token>                               m_tokens;
    std::istream*                                    m_stream;
    std::vector<std::pair<std::string, RoffScalar>>  m_scalarValues;
    std::vector<std::pair<std::string, Token::Kind>> m_arrayTypes;
    std::map<std::string, std::pair<long, long>>     m_arrayInfo;
    std::unique_ptr<Parser>                          m_parser;
};
