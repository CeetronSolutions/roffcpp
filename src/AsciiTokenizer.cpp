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

#include "AsciiTokenizer.hpp"

#include <cctype>
#include <optional>
#include <stdexcept>
#include <vector>

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
AsciiTokenizer::AsciiTokenizer()
    : Tokenizer()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
AsciiTokenizer::~AsciiTokenizer()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool AsciiTokenizer::tokenizeSpace( std::istream& stream )
{
    auto start    = stream.tellg();
    char readChar = static_cast<char>( stream.get() );
    if ( !std::isspace( readChar ) )
    {
        stream.seekg( start );
        return false;
    }

    auto end = stream.tellg();
    readChar = static_cast<char>( stream.get() );
    while ( stream.good() && std::isspace( readChar ) )
    {
        end      = stream.tellg();
        readChar = static_cast<char>( stream.get() );
    }

    stream.seekg( end );
    return true;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::optional<Token> AsciiTokenizer::tokenizeString( std::istream& stream )
{
    tokenizeDelimiter( stream );

    // First part of string should be double-quote
    auto start    = stream.tellg();
    char readChar = static_cast<char>( stream.get() );
    if ( readChar != '"' )
    {
        // Expected string, but no opening quote found
        stream.seekg( start );
        return {};
    }

    // Read until closing double-quote.
    start    = stream.tellg();
    auto end = start;
    readChar = static_cast<char>( stream.get() );
    while ( stream.good() && readChar != '"' )
    {
        end      = stream.tellg();
        readChar = static_cast<char>( stream.get() );
    }

    if ( !stream.good() )
    {
        // Reached unexpected end of file.
        stream.seekg( start );
        return {};
    }

    return Token( Token::Kind::STRING_LITERAL, start, end );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
Token AsciiTokenizer::tokenizeName( std::istream& stream )
{
    tokenizeDelimiter( stream );

    auto start  = stream.tellg();
    int  length = 0;

    auto readChar = static_cast<char>( stream.get() );
    while ( stream.good() && !std::isspace( readChar ) )
    {
        length++;
        readChar = static_cast<char>( stream.get() );
    }

    if ( length < 1 )
    {
        stream.seekg( start );
        throw std::runtime_error( "Could not tokenize name." );
    }

    return Token( Token::Kind::NAME, start, static_cast<size_t>( start ) + length );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::optional<Token> AsciiTokenizer::tokenizeNumber( std::istream& stream )
{
    tokenizeDelimiter( stream );

    auto isCharValidInDigit = []( char c ) {
        return std::isdigit( c ) || c == '.' || c == 'e' || c == 'E' || c == '-' || c == '+';
    };

    auto start = stream.tellg();
    auto end   = start;

    char readChar = static_cast<char>( stream.get() );
    if ( stream.good() && ( std::isdigit( readChar ) || readChar == '-' ) )
    {
        while ( stream.good() && isCharValidInDigit( readChar ) )
        {
            end      = stream.tellg();
            readChar = static_cast<char>( stream.get() );
        }
    }

    if ( end - start < 1 )
    {
        // Expected numeric value, but could not find one.
        stream.seekg( start );
        return {};
    }

    stream.seekg( end );
    return Token( Token::Kind::NUMERIC_VALUE, start, end );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::optional<Token> AsciiTokenizer::tokenizeValue( std::istream& stream )
{
    auto numberToken = tokenizeNumber( stream );
    if ( numberToken )
        return numberToken;
    else
        return tokenizeString( stream );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::optional<Token> AsciiTokenizer::tokenizeWord( std::istream& stream, const std::string& keyword, Token::Kind kind )
{
    tokenizeDelimiter( stream );
    auto        start = stream.tellg();
    std::string word;
    if ( stream >> word && word == keyword )
    {
        auto end = static_cast<size_t>( start ) + keyword.length();
        return Token( kind, start, end );
    }
    else
    {
        stream.seekg( start );
        return {};
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<Token> AsciiTokenizer::tokenizeTagKeyInternal( std::istream& stream )
{
    std::vector<Token> tokens;
    tokens.push_back( tokenizeSimpleType( stream ) );
    tokens.push_back( tokenizeName( stream ) );
    auto value = tokenizeValue( stream );
    if ( value )
        tokens.push_back( value.value() );
    else
        throw std::runtime_error( "Invalid value." );
    return tokens;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<Token> AsciiTokenizer::tokenizeArrayTagKey( std::istream& stream )
{
    std::vector<Token> tokens;
    tokens.push_back( tokenizeKeyword( stream, { std::make_pair( Token::Kind::ARRAY, "array" ) } ) );
    tokens.push_back( tokenizeSimpleType( stream ) );
    tokens.push_back( tokenizeName( stream ) );
    auto numberOfElements = tokenizeNumber( stream );
    if ( !numberOfElements ) throw std::runtime_error( "Expected numeric value" );

    tokens.push_back( numberOfElements.value() );

    std::vector<Token> arrayTokens = tokenizeArrayData( stream );
    tokens.insert( tokens.end(), arrayTokens.begin(), arrayTokens.end() );

    return tokens;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<Token> AsciiTokenizer::tokenizeArrayData( std::istream& stream )
{
    std::vector<Token> tokens;

    bool gotNewToken = true;
    while ( gotNewToken )
    {
        auto token = tokenizeValue( stream );
        if ( token )
            tokens.push_back( token.value() );
        else
            gotNewToken = false;
    }

    return tokens;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<Token> AsciiTokenizer::tokenizeStream( std::istream& stream )
{
    return Tokenizer::tokenizeStream( stream, Token::Kind::ROFF_ASC );
}
