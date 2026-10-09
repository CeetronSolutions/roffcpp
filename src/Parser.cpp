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

#include "Parser.hpp"

#include "RoffScalar.hpp"

#include <cassert>
#include <cctype>
#include <stdexcept>
#include <variant>
#include <vector>

using namespace roff;

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
Parser::Parser()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
Parser::~Parser()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void Parser::parse( std::istream&                                     stream,
                    const std::vector<Token>&                         tokens,
                    std::vector<std::pair<std::string, RoffScalar>>&  scalarValues,
                    std::vector<std::pair<std::string, Token::Kind>>& arrayTypes,
                    std::map<std::string, std::pair<long, long>>&     arrayInfo ) const
{
    auto        it           = tokens.begin();
    std::string tagGroupName = "";
    std::string lastName     = "";

    // Step to the next token, failing instead of moving past the end of the token list
    auto next = [&it, &tokens]()
    {
        if ( std::distance( it, tokens.end() ) <= 1 ) throw std::runtime_error( "Unexpected end of file." );
        std::advance( it, 1 );
    };

    while ( it != tokens.end() )
    {
        if ( it->kind() == Token::Kind::TAG )
        {
            next();
            assert( it->kind() == Token::Kind::NAME );
            tagGroupName = parseString( *it, stream );
        }
        else if ( isSimpleType( it->kind() ) )
        {
            auto simpleType = parseSimpleType( it, tokens.end(), tagGroupName, stream );
            if ( simpleType.first == "parameter.name" )
            {
                lastName = std::get<std::string>( simpleType.second );
            }

            // Only little-endian is supported.
            if ( simpleType.first == "filedata.byteswaptest" && std::get<int>( simpleType.second ) != 1 )
            {
                throw std::runtime_error( "Unexpected endianness." );
            }

            scalarValues.push_back( simpleType );
        }
        else if ( it->kind() == Token::Kind::ARRAY )
        {
            // Extract the type
            next();
            Token typeToken = *it;
            assert( isSimpleType( typeToken.kind() ) );

            // Extract the name
            next();
            std::string name = tagGroupName + "." + parseString( *it, stream );

            // Special handling for parameter tags.
            // Take the name of parameter as name of the group to avoid
            // name-clashes (multiple properties named parameter.data/codeNames/codeValues).
            if ( name == ( "parameter" + postFixData() ) )
            {
                name     = lastName;
                lastName = "";
            }
            else if ( name == ( "parameter" + postFixCodeNames() ) )
            {
                name = lastName + postFixCodeNames();
            }
            else if ( name == ( "parameter" + postFixCodeValues() ) )
            {
                name = lastName + postFixCodeValues();
            }

            // Extract number of array items
            next();
            long arrayLength = parseInt( *it, stream );
            if ( arrayLength < 0 ) throw std::runtime_error( "Invalid array length." );

            // The index of the first item
            long startIndex = static_cast<long>( std::distance( tokens.begin(), it ) ) + 1;

            // The array length is read from the file and must be backed by actual tokens
            const long tokensAfterLength = static_cast<long>( std::distance( it, tokens.end() ) ) - 1;
            const bool isBlob            = tokensAfterLength > 0 && ( it + 1 )->kind() == Token::Kind::ARRAYBLOB;

            if ( isBlob )
            {
                // The blob is always only one token
                std::advance( it, 1 );

                // The data of the blob must be present in the file
                stream.clear();
                const auto currentPos = stream.tellg();
                stream.seekg( 0, std::ios::end );
                const auto streamSize = stream.tellg();
                stream.seekg( currentPos );
                if ( it->end() > static_cast<size_t>( streamSize ) )
                    throw std::runtime_error( "Array data is incomplete." );
            }
            else
            {
                if ( arrayLength > tokensAfterLength )
                    throw std::runtime_error( "Array length exceeds the number of items in the file." );

                // Skip rest of the array
                std::advance( it, arrayLength );
            }

            arrayInfo[name] = std::make_pair( startIndex, arrayLength );

            arrayTypes.push_back( std::make_pair( name, typeToken.kind() ) );
        }
        else if ( it->kind() == Token::Kind::ENDTAG )
        {
            tagGroupName = "";
        }
        it++;
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool Parser::isSimpleType( Token::Kind kind )
{
    return kind == Token::Kind::INT || kind == Token::Kind::BOOL || kind == Token::Kind::BYTE ||
           kind == Token::Kind::FLOAT || kind == Token::Kind::DOUBLE || kind == Token::Kind::CHAR;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::pair<std::string, RoffScalar> Parser::parseSimpleType( std::vector<Token>::const_iterator& it,
                                                            std::vector<Token>::const_iterator  end,
                                                            const std::string&                  tagGroupName,
                                                            std::istream&                       stream ) const
{
    assert( isSimpleType( it->kind() ) );
    Token::Kind kind = it->kind();

    if ( std::distance( it, end ) <= 2 ) throw std::runtime_error( "Unexpected end of file." );

    std::advance( it, 1 );
    assert( it->kind() == Token::Kind::NAME );
    std::string name = parseString( *it, stream );

    std::advance( it, 1 );

    auto extractValue = [this]( auto it, Token::Kind kind, std::istream& stream )
    {
        if ( kind == Token::Kind::INT ) return RoffScalar( parseInt( *it, stream ) );
        if ( kind == Token::Kind::BOOL ) return RoffScalar( parseBool( *it, stream ) );
        if ( kind == Token::Kind::BYTE ) return RoffScalar( parseByte( *it, stream ) );
        if ( kind == Token::Kind::CHAR ) return RoffScalar( parseString( *it, stream ) );
        if ( kind == Token::Kind::DOUBLE ) return RoffScalar( parseDouble( *it, stream ) );
        if ( kind == Token::Kind::FLOAT ) return RoffScalar( parseFloat( *it, stream ) );
        return RoffScalar( 1 );
    };

    RoffScalar val = extractValue( it, kind, stream );

    return std::make_pair( tagGroupName + "." + name, val );
}
