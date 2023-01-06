# roffcpp

C++ library for parsing Roxar Open File Format (ROFF) (binary and ascii) files.

This library is inspired by [XTGeo](https://github.com/equinor/xtgeo) and [Roffio](https://github.com/equinor/roffio).

Some test data is taken from [XTGeo test data](https://github.com/equinor/xtgeo-testdata).

## Building on Linux

```
mkdir build
cd build
cmake ..
```

## Run tests

```
tests/roffcpp-tests
```

## Usage

```cpp
std::ifstream stream( filename, std::ios::binary );
Reader reader( stream );
reader.parse();

std::vector<std::pair<std::string, RoffScalar>>  values     = reader.scalarNamedValues();
std::vector<std::pair<std::string, Token::Kind>> arrayTypes = reader.getNamedArrayTypes();

auto getInt = []( auto values, const std::string& name ) {
    auto v = std::find_if( values.begin(), values.end(), [&name]( const auto& arg ) { return arg.first == name; } );
    if ( v != values.end() )
        return std::get<int>( v->second );
    else
        throw std::runtime_error( "Missing parameter (integer): " + name );
};


int nx = getInt( values, "dimensions.nX" );
int ny = getInt( values, "dimensions.nY" );
int nz = getInt( values, "dimensions.nZ" );

std::vector<int> layers = reader.getIntArray( "subgrids.nLayers" );
```

## Licensing

Licensed under GNU GPL version 3.
