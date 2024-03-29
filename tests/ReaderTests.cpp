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

#include <algorithm>
#include <fstream>
#include <string>
#include <variant>

#include "Parser.hpp"
#include "Reader.hpp"
#include "RoffTestDataDirectory.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"

using namespace roff;

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
TEST( ReaderTests, testParseScalarNamedValuesFromExampleFile )
{
    std::ifstream stream( std::string( TEST_DATA_DIR ) + "/facies_info.roff", std::ios::binary );
    ASSERT_TRUE( stream.good() );

    Reader reader( stream );
    reader.parse();

    std::vector<std::pair<std::string, RoffScalar>> values = reader.scalarNamedValues();
    ASSERT_FALSE( values.empty() );

    auto hasValue = []( auto values, const std::string& keyword )
    {
        return std::any_of( values.cbegin(),
                            values.cend(),
                            [&keyword]( const auto& arg ) { return arg.first == keyword; } );
    };

    auto getValue = []( const std::vector<std::pair<std::string, RoffScalar>>& values, const std::string& keyword )
    {
        auto a =
            std::find_if( values.begin(), values.end(), [&keyword]( const auto& arg ) { return arg.first == keyword; } );
        if ( a != values.end() )
            return a->second;
        else
            return RoffScalar( std::numeric_limits<float>::infinity() );
    };

    ASSERT_TRUE( hasValue( values, "filedata.byteswaptest" ) );
    ASSERT_EQ( 1, std::get<int>( getValue( values, "filedata.byteswaptest" ) ) );

    ASSERT_TRUE( hasValue( values, "version.major" ) );
    ASSERT_EQ( 2, std::get<int>( getValue( values, "version.major" ) ) );

    ASSERT_TRUE( hasValue( values, "dimensions.nX" ) );
    ASSERT_EQ( 46, std::get<int>( getValue( values, "dimensions.nX" ) ) );

    ASSERT_TRUE( hasValue( values, "parameter.name" ) );
    ASSERT_EQ( std::string( "composite" ), std::get<std::string>( getValue( values, "parameter.name" ) ) );

    ASSERT_TRUE( hasValue( values, "parameter.fl" ) );
    ASSERT_DOUBLE_EQ( 1.23f, std::get<float>( getValue( values, "parameter.fl" ) ) );

    ASSERT_TRUE( hasValue( values, "parameter.db" ) );
    ASSERT_DOUBLE_EQ( 9000.9, std::get<double>( getValue( values, "parameter.db" ) ) );

    ASSERT_TRUE( hasValue( values, "parameter.bt" ) );
    ASSERT_EQ( 42, std::get<unsigned char>( getValue( values, "parameter.bt" ) ) );

    ASSERT_TRUE( hasValue( values, "parameter.bo" ) );
    ASSERT_EQ( true, std::get<bool>( getValue( values, "parameter.bo" ) ) );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
TEST( ReaderTests, testParseArrayValuesFromExampleFile )
{
    std::ifstream stream( std::string( TEST_DATA_DIR ) + "/facies_info.roff", std::ios::binary );
    ASSERT_TRUE( stream.good() );

    Reader reader( stream );
    reader.parse();

    std::vector<std::pair<std::string, Token::Kind>> arrayTypes = reader.getNamedArrayTypes();
    ASSERT_FALSE( arrayTypes.empty() );

    auto hasValue = []( auto values, const std::string& keyword, Token::Kind kind )
    {
        auto x =
            std::find_if( values.begin(), values.end(), [&keyword]( const auto& arg ) { return arg.first == keyword; } );
        return x != values.end() && x->second == kind;
    };

    ASSERT_TRUE( hasValue( arrayTypes, "composite" + roff::Parser::postFixCodeNames(), Token::Kind::CHAR ) );
    ASSERT_TRUE( hasValue( arrayTypes, "composite" + roff::Parser::postFixCodeValues(), Token::Kind::INT ) );
    ASSERT_TRUE( hasValue( arrayTypes, "parameter.floatData", Token::Kind::FLOAT ) );
    ASSERT_TRUE( hasValue( arrayTypes, "parameter.doubleData", Token::Kind::DOUBLE ) );
    ASSERT_TRUE( hasValue( arrayTypes, "parameter.intData", Token::Kind::INT ) );

    std::vector<std::string> codeNames = reader.getStringArray( "composite" + roff::Parser::postFixCodeNames() );
    ASSERT_EQ( 6u, codeNames.size() );
    ASSERT_EQ( "code name 1", codeNames[0] );
    ASSERT_EQ( "code name 6", codeNames[5] );

    std::vector<int> codeValues = reader.getIntArray( "composite" + roff::Parser::postFixCodeValues() );
    ASSERT_EQ( 6u, codeValues.size() );
    for ( int i = 0; i < 6; i++ )
    {
        ASSERT_EQ( i, codeValues[i] );
    }

    std::vector<float> floatData = reader.getFloatArray( "parameter.floatData" );
    ASSERT_EQ( 5u, floatData.size() );

    std::vector<double> doubleData = reader.getDoubleArray( "parameter.doubleData" );
    ASSERT_EQ( 6u, doubleData.size() );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
TEST( ReaderTests, testParseScalarNamedValuesFromBinaryExampleFile )
{
    std::ifstream stream( std::string( TEST_DATA_DIR ) + "/facies_info.roffbin", std::ios::binary );
    ASSERT_TRUE( stream.good() );

    Reader reader( stream );
    reader.parse();

    std::vector<std::pair<std::string, RoffScalar>> values = reader.scalarNamedValues();
    ASSERT_FALSE( values.empty() );

    auto hasValue = []( auto values, const std::string& keyword )
    {
        return std::any_of( values.cbegin(),
                            values.cend(),
                            [&keyword]( const auto& arg ) { return arg.first == keyword; } );
    };

    auto getValue = []( const std::vector<std::pair<std::string, RoffScalar>>& values, const std::string& keyword )
    {
        auto a =
            std::find_if( values.begin(), values.end(), [&keyword]( const auto& arg ) { return arg.first == keyword; } );
        if ( a != values.end() )
            return a->second;
        else
            return RoffScalar( std::numeric_limits<float>::infinity() );
    };

    ASSERT_TRUE( hasValue( values, "filedata.byteswaptest" ) );
    ASSERT_EQ( 1, std::get<int>( getValue( values, "filedata.byteswaptest" ) ) );

    ASSERT_TRUE( hasValue( values, "version.major" ) );
    ASSERT_EQ( 2, std::get<int>( getValue( values, "version.major" ) ) );

    ASSERT_TRUE( hasValue( values, "dimensions.nX" ) );
    ASSERT_EQ( 46, std::get<int>( getValue( values, "dimensions.nX" ) ) );

    ASSERT_TRUE( hasValue( values, "parameter.name" ) );
    ASSERT_EQ( std::string( "composite" ), std::get<std::string>( getValue( values, "parameter.name" ) ) );

    ASSERT_TRUE( hasValue( values, "parameter.fl" ) );
    ASSERT_DOUBLE_EQ( 1.23f, std::get<float>( getValue( values, "parameter.fl" ) ) );

    ASSERT_TRUE( hasValue( values, "parameter.db" ) );
    ASSERT_DOUBLE_EQ( 9000.9, std::get<double>( getValue( values, "parameter.db" ) ) );

    ASSERT_TRUE( hasValue( values, "parameter.bt" ) );
    ASSERT_EQ( 42, std::get<unsigned char>( getValue( values, "parameter.bt" ) ) );

    ASSERT_TRUE( hasValue( values, "parameter.bo" ) );
    ASSERT_EQ( true, std::get<bool>( getValue( values, "parameter.bo" ) ) );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
TEST( ReaderTests, testParseArrayValuesFromBinaryExampleFile )
{
    std::ifstream stream( std::string( TEST_DATA_DIR ) + "/facies_info.roffbin", std::ios::binary );
    ASSERT_TRUE( stream.good() );

    Reader reader( stream );
    reader.parse();

    std::vector<std::pair<std::string, Token::Kind>> arrayTypes = reader.getNamedArrayTypes();
    ASSERT_FALSE( arrayTypes.empty() );

    auto hasValue = []( auto values, const std::string& keyword, Token::Kind kind )
    {
        auto x =
            std::find_if( values.begin(), values.end(), [&keyword]( const auto& arg ) { return arg.first == keyword; } );
        return x != values.end() && x->second == kind;
    };

    ASSERT_TRUE( hasValue( arrayTypes, "composite" + roff::Parser::postFixCodeValues(), Token::Kind::INT ) );
    ASSERT_TRUE( hasValue( arrayTypes, "parameter.floatData", Token::Kind::FLOAT ) );
    ASSERT_TRUE( hasValue( arrayTypes, "parameter.doubleData", Token::Kind::DOUBLE ) );
    ASSERT_TRUE( hasValue( arrayTypes, "parameter.intData", Token::Kind::INT ) );

    std::vector<int> codeValues = reader.getIntArray( "composite" + roff::Parser::postFixCodeValues() );
    ASSERT_EQ( 6u, codeValues.size() );
    for ( int i = 0; i < 6; i++ )
    {
        ASSERT_EQ( i, codeValues[i] );
    }

    std::vector<float> floatData = reader.getFloatArray( "parameter.floatData" );
    ASSERT_EQ( 5u, floatData.size() );

    std::vector<double> doubleData = reader.getDoubleArray( "parameter.doubleData" );
    ASSERT_EQ( 6u, doubleData.size() );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
TEST( ReaderTests, testParseGridFiles )
{
    std::vector<std::string> fileNames = { "reek_box_grid_w_props.roff", "reek_box_grid_w_props.roffasc" };

    for ( auto fileName : fileNames )
    {
        std::ifstream stream( std::string( TEST_DATA_DIR ) + "/" + fileName, std::ios::binary );
        ASSERT_TRUE( stream.good() );

        Reader reader( stream );
        reader.parse();

        std::vector<std::pair<std::string, Token::Kind>> arrayTypes = reader.getNamedArrayTypes();
        ASSERT_FALSE( arrayTypes.empty() );

        auto hasValue = []( auto values, const std::string& keyword, Token::Kind kind )
        {
            auto x = std::find_if( values.begin(),
                                   values.end(),
                                   [&keyword]( const auto& arg ) { return arg.first == keyword; } );
            return x != values.end() && x->second == kind;
        };

        ASSERT_TRUE( hasValue( arrayTypes, "zvalues" + roff::Parser::postFixData(), Token::Kind::FLOAT ) );
        ASSERT_TRUE( hasValue( arrayTypes, "zvalues.splitEnz", Token::Kind::BYTE ) );
        ASSERT_TRUE( hasValue( arrayTypes, "PORO", Token::Kind::FLOAT ) );

        ASSERT_TRUE( hasValue( arrayTypes, "EQLNUM" + roff::Parser::postFixCodeNames(), Token::Kind::CHAR ) );
        ASSERT_TRUE( hasValue( arrayTypes, "EQLNUM" + roff::Parser::postFixCodeValues(), Token::Kind::INT ) );

        ASSERT_TRUE( hasValue( arrayTypes, "EQLNUM" + roff::Parser::postFixCodeNames(), Token::Kind::CHAR ) );
        ASSERT_TRUE( hasValue( arrayTypes, "EQLNUM" + roff::Parser::postFixCodeValues(), Token::Kind::INT ) );

        std::vector<int> codeValues = reader.getIntArray( "EQLNUM" + roff::Parser::postFixCodeValues() );
        ASSERT_EQ( 2u, codeValues.size() );
        ASSERT_EQ( 1, codeValues[0] );
        ASSERT_EQ( 2, codeValues[1] );

        std::vector<std::string> codeNames = reader.getStringArray( "EQLNUM" + roff::Parser::postFixCodeNames() );
        ASSERT_EQ( 2u, codeNames.size() );
        ASSERT_EQ( "", codeNames[0] );
        ASSERT_EQ( "", codeNames[1] );
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
TEST( ReaderTests, testParseUnsupportedEndianness )
{
    std::ifstream stream( std::string( TEST_DATA_DIR ) + "/unsupported_endianness.roff", std::ios::binary );
    ASSERT_TRUE( stream.good() );

    Reader reader( stream );
    ASSERT_THROW( reader.parse(), std::runtime_error );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
TEST( ReaderTests, testReadWrongFileType )
{
    std::ifstream stream( std::string( TEST_DATA_DIR ) + "/unexpected_filetype.roff", std::ios::binary );
    ASSERT_TRUE( stream.good() );

    Reader reader( stream );

    std::string errMsg;
    try
    {
        reader.parse();
    }
    catch ( std::runtime_error& ex )
    {
        errMsg = ex.what();
    }
    ASSERT_EQ( errMsg, std::string( "Unexpected file type." ) );
}
