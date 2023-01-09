#include "Reader.hpp"

#include "AsciiParser.hpp"
#include "AsciiTokenizer.hpp"
#include "BinaryParser.hpp"
#include "BinaryTokenizer.hpp"
#include "Parser.hpp"
#include "Tokenizer.hpp"

#include <cassert>
#include <cctype>
#include <stdexcept>
#include <variant>
#include <vector>

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
Reader::Reader( std::istream& stream )
    : m_stream( &stream )
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void Reader::parse()
{
    bool isBinary = detectFileTypeFromFirstToken( *m_stream );
    if ( isBinary )
    {
        parseBinary();
    }
    else
    {
        parseAscii();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool Reader::detectFileTypeFromFirstToken( std::istream& stream )
{
    try
    {
        // Detect file type from first token
        BinaryTokenizer tokenizer;
        Token           token = tokenizer.tokenizeFileType( stream );
        assert( token.kind() == Token::Kind::ROFF_ASC || token.kind() == Token::Kind::ROFF_BIN );
        stream.clear();
        stream.seekg( 0 );
        return token.kind() == Token::Kind::ROFF_BIN;
    }
    catch ( std::runtime_error& )
    {
        throw std::runtime_error( "Unexpected file type." );
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void Reader::parseAscii()
{
    AsciiTokenizer tokenizer;
    m_tokens = tokenizer.tokenizeStream( *m_stream );
    m_parser = std::make_unique<AsciiParser>();
    m_parser->parse( *m_stream, m_tokens, m_scalarValues, m_arrayTypes, m_arrayInfo );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void Reader::parseBinary()
{
    BinaryTokenizer tokenizer;
    m_tokens = tokenizer.tokenizeStream( *m_stream );
    m_parser = std::make_unique<BinaryParser>();
    m_parser->parse( *m_stream, m_tokens, m_scalarValues, m_arrayTypes, m_arrayInfo );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<std::pair<std::string, RoffScalar>> Reader::scalarNamedValues() const
{
    return m_scalarValues;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<std::pair<std::string, Token::Kind>> Reader::getNamedArrayTypes() const
{
    return m_arrayTypes;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
size_t Reader::getArrayLength( const std::string& keyword ) const
{
    auto it = m_arrayInfo.find( keyword );
    if ( it != m_arrayInfo.end() )
        return it->second.second;
    else
        return 0;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<std::string> Reader::getStringArray( const std::string& keyword )
{
    auto [startIndex, arrayLength] = m_arrayInfo[keyword];
    return m_parser->parseStringArray( m_tokens, *m_stream, startIndex, arrayLength );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<int> Reader::getIntArray( const std::string& keyword )
{
    auto [startIndex, arrayLength] = m_arrayInfo[keyword];
    return m_parser->parseIntArray( m_tokens, *m_stream, startIndex, arrayLength );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<char> Reader::getByteArray( const std::string& keyword )
{
    auto [startIndex, arrayLength] = m_arrayInfo[keyword];
    return m_parser->parseByteArray( m_tokens, *m_stream, startIndex, arrayLength );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<float> Reader::getFloatArray( const std::string& keyword )
{
    auto [startIndex, arrayLength] = m_arrayInfo[keyword];
    return m_parser->parseFloatArray( m_tokens, *m_stream, startIndex, arrayLength );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<double> Reader::getDoubleArray( const std::string& keyword )
{
    auto [startIndex, arrayLength] = m_arrayInfo[keyword];
    return m_parser->parseDoubleArray( m_tokens, *m_stream, startIndex, arrayLength );
}
