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
