#pragma once

#include "Token.hpp"
#include "Tokenizer.hpp"

#include <istream>
#include <optional>
#include <vector>

class AsciiTokenizer : public Tokenizer
{
public:
    AsciiTokenizer();
    virtual ~AsciiTokenizer();

    std::vector<Token>   tokenizeStream( std::istream& stream ) override;
    bool                 tokenizeSpace( std::istream& stream ) override;
    std::optional<Token> tokenizeString( std::istream& stream ) override;
    Token                tokenizeName( std::istream& stream ) override;
    std::vector<Token>   tokenizeArrayTagKey( std::istream& stream ) override;
    std::optional<Token> tokenizeWord( std::istream& stream, const std::string& keywork, Token::Kind kind ) override;

    std::optional<Token> tokenizeNumber( std::istream& stream );
    std::optional<Token> tokenizeValue( std::istream& stream );
    std::vector<Token>   tokenizeArrayData( std::istream& stream );

protected:
    std::vector<Token> tokenizeTagKeyInternal( std::istream& stream ) override;
};
