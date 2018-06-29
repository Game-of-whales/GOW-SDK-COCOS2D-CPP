/*
 * Game Of Whales SDK
 *
 * https://www.gameofwhales.com/
 *
 * Copyright © 2018 GameOfWhales. All rights reserved.
 *
 * Licence: https://github.com/Game-of-whales/GOW-SDK-COCOS2D-CPP/blob/master/LICENSE
 *
 */

#include "GameOfWhalesJSON.h"



namespace gameofwhales {
    
    namespace json {
      

        namespace patch
        {
            template < typename T > std::string to_string( const T& n )
            {
                std::ostringstream stm ;
                stm << n ;
                return stm.str() ;
            }
        }
        
        
        string json_escape( const string &str ) {
            string output;
            for( unsigned i = 0; i < str.length(); ++i )
                switch( str[i] ) {
                    case '\"': output += "\\\""; break;
                    case '\\': output += "\\\\"; break;
                    case '\b': output += "\\b";  break;
                    case '\f': output += "\\f";  break;
                    case '\n': output += "\\n";  break;
                    case '\r': output += "\\r";  break;
                    case '\t': output += "\\t";  break;
                    default  : output += str[i]; break;
                }
            return std::move( output );
        }
        
        
        /////JSON CLASS
        JSON::JSON(): Internal(), Type( Class::Null )
        {
            
        }
        
        JSON::~JSON() {
            switch( Type ) {
                case Class::Array:
                    delete Internal.List;
                    break;
                case Class::Object:
                    delete Internal.Map;
                    break;
                case Class::String:
                    delete Internal.String;
                    break;
                default:;
            }
        }
        
        
        JSON::JSON( initializer_list<JSON> list ): JSON()
        {
            SetType( Class::Object );
            for( auto i = list.begin(), e = list.end(); i != e; ++i, ++i )
                operator[]( i->ToString() ) = *std::next( i );
        }
        
        JSON::JSON(JSON&& other): Internal( other.Internal ), Type( other.Type )
        {
            other.Type = Class::Null; other.Internal.Map = nullptr;
        }
        
        JSON::JSON( const JSON &other ) {
            switch( other.Type ) {
                case Class::Object:
                    Internal.Map =
                    new map<string,JSON>( other.Internal.Map->begin(),
                                         other.Internal.Map->end() );
                    break;
                case Class::Array:
                    Internal.List =
                    new deque<JSON>( other.Internal.List->begin(),
                                    other.Internal.List->end() );
                    break;
                case Class::String:
                    Internal.String =
                    new string( *other.Internal.String );
                    break;
                default:
                    Internal = other.Internal;
            }
            Type = other.Type;
        }
        
        JSON& JSON::operator=( JSON&& other ) {
            ClearInternal();
            Internal = other.Internal;
            Type = other.Type;
            other.Internal.Map = nullptr;
            other.Type = Class::Null;
            return *this;
        }
        
        JSON& JSON::operator=( const JSON &other ) {
            ClearInternal();
            switch( other.Type ) {
                case Class::Object:
                    Internal.Map =
                    new map<string,JSON>( other.Internal.Map->begin(),
                                         other.Internal.Map->end() );
                    break;
                case Class::Array:
                    Internal.List =
                    new deque<JSON>( other.Internal.List->begin(),
                                    other.Internal.List->end() );
                    break;
                case Class::String:
                    Internal.String =
                    new string( *other.Internal.String );
                    break;
                default:
                    Internal = other.Internal;
            }
            Type = other.Type;
            return *this;
        }
        
        JSON& JSON::operator[]( const string &key ) {
            SetType( Class::Object ); return Internal.Map->operator[]( key );
        }
        
        JSON& JSON::operator[]( unsigned index ) {
            SetType( Class::Array );
            if( index >= Internal.List->size() ) Internal.List->resize( index + 1 );
            return Internal.List->operator[]( index );
        }
        
        JSON& JSON::at( const string &key ) {
            return operator[]( key );
        }
        
        const JSON& JSON::at( const string &key ) const {
            return Internal.Map->at( key );
        }
        
        JSON& JSON::at( unsigned index ) {
            return operator[]( index );
        }
        
        const JSON& JSON::at( unsigned index ) const {
            return Internal.List->at( index );
        }
        
        int JSON::length() const {
            if( Type == Class::Array )
                return Internal.List->size();
            else
                return -1;
        }
        
        bool JSON::hasKey( const string &key ) const {
            if( Type == Class::Object )
                return Internal.Map->find( key ) != Internal.Map->end();
            return false;
        }
        
        int JSON::size() const {
            if( Type == Class::Object )
                return Internal.Map->size();
            else if( Type == Class::Array )
                return Internal.List->size();
            else
                return -1;
        }
        
        string JSON::dump( int depth, string tab) const {
            string pad = "";
            for( int i = 0; i < depth; ++i, pad += tab );
            
            switch( Type ) {
                case Class::Null:
                    return "null";
                case Class::Object: {
                    string s = "{\n";
                    bool skip = true;
                    for( auto &p : *Internal.Map ) {
                        if( !skip ) s += ",\n";
                        s += ( pad + "\"" + p.first + "\" : " + p.second.dump( depth + 1, tab ) );
                        skip = false;
                    }
                    s += ( "\n" + pad.erase( 0, 2 ) + "}" ) ;
                    return s;
                }
                case Class::Array: {
                    string s = "[";
                    bool skip = true;
                    for( auto &p : *Internal.List ) {
                        if( !skip ) s += ", ";
                        s += p.dump( depth + 1, tab );
                        skip = false;
                    }
                    s += "]";
                    return s;
                }
                case Class::String:
                    return "\"" + json_escape( *Internal.String ) + "\"";
                case Class::Floating:
                    return patch::to_string( Internal.Float );
                case Class::Integral:
                    return patch::to_string( Internal.Int );
                case Class::Boolean:
                    return Internal.Bool ? "true" : "false";
                default:
                    return "";
            }
            return "";
        }
        
        void JSON::SetType( Class type ) {
            if( type == Type )
                return;
            
            ClearInternal();
            
            switch( type ) {
                case Class::Null:      Internal.Map    = nullptr;                break;
                case Class::Object:    Internal.Map    = new map<string,JSON>(); break;
                case Class::Array:     Internal.List   = new deque<JSON>();     break;
                case Class::String:    Internal.String = new string();           break;
                case Class::Floating:  Internal.Float  = 0.0;                    break;
                case Class::Integral:  Internal.Int    = 0;                      break;
                case Class::Boolean:   Internal.Bool   = false;                  break;
            }
            
            Type = type;
        }
        
        void JSON::ClearInternal() {
            switch( Type ) {
                case Class::Object: delete Internal.Map;    break;
                case Class::Array:  delete Internal.List;   break;
                case Class::String: delete Internal.String; break;
                default:;
            }
        }
        /////--JSON CLASS
        
        
        
        JSON Array() {
            return std::move( JSON::Make( JSON::Class::Array ) );
        }
        
        JSON Object() {
            return std::move( JSON::Make( JSON::Class::Object ) );
        }
        

        
        namespace {
            JSON parse_next( const string &, size_t & );
            
            void consume_ws( const string &str, size_t &offset ) {
                while( isspace( str[offset] ) ) ++offset;
            }
            
            JSON parse_object( const string &str, size_t &offset ) {
                JSON Object = JSON::Make( JSON::Class::Object );
                
                ++offset;
                consume_ws( str, offset );
                if( str[offset] == '}' ) {
                    ++offset; return std::move( Object );
                }
                
                while( true ) {
                    JSON Key = parse_next( str, offset );
                    consume_ws( str, offset );
                    if( str[offset] != ':' ) {
                        std::cerr << "Error: Object: Expected colon, found '" << str[offset] << "'\n";
                        break;
                    }
                    consume_ws( str, ++offset );
                    JSON Value = parse_next( str, offset );
                    Object[Key.ToString()] = Value;
                    
                    consume_ws( str, offset );
                    if( str[offset] == ',' ) {
                        ++offset; continue;
                    }
                    else if( str[offset] == '}' ) {
                        ++offset; break;
                    }
                    else {
                        std::cerr << "ERROR: Object: Expected comma, found '" << str[offset] << "'\n";
                        break;
                    }
                }
                
                return std::move( Object );
            }
            
            JSON parse_array( const string &str, size_t &offset ) {
                JSON Array = JSON::Make( JSON::Class::Array );
                unsigned index = 0;
                
                ++offset;
                consume_ws( str, offset );
                if( str[offset] == ']' ) {
                    ++offset; return std::move( Array );
                }
                
                while( true ) {
                    Array[index++] = parse_next( str, offset );
                    consume_ws( str, offset );
                    
                    if( str[offset] == ',' ) {
                        ++offset; continue;
                    }
                    else if( str[offset] == ']' ) {
                        ++offset; break;
                    }
                    else {
                        std::cerr << "ERROR: Array: Expected ',' or ']', found '" << str[offset] << "'\n";
                        return std::move( JSON::Make( JSON::Class::Array ) );
                    }
                }
                
                return std::move( Array );
            }
            
            JSON parse_string( const string &str, size_t &offset ) {
                JSON String;
                string val;
                for( char c = str[++offset]; c != '\"' ; c = str[++offset] ) {
                    if( c == '\\' ) {
                        switch( str[ ++offset ] ) {
                            case '\"': val += '\"'; break;
                            case '\\': val += '\\'; break;
                            case '/' : val += '/' ; break;
                            case 'b' : val += '\b'; break;
                            case 'f' : val += '\f'; break;
                            case 'n' : val += '\n'; break;
                            case 'r' : val += '\r'; break;
                            case 't' : val += '\t'; break;
                            case 'u' : {
                                val += "\\u" ;
                                for( unsigned i = 1; i <= 4; ++i ) {
                                    c = str[offset+i];
                                    if( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') )
                                        val += c;
                                    else {
                                        std::cerr << "ERROR: String: Expected hex character in unicode escape, found '" << c << "'\n";
                                        return std::move( JSON::Make( JSON::Class::String ) );
                                    }
                                }
                                offset += 4;
                            } break;
                            default  : val += '\\'; break;
                        }
                    }
                    else
                        val += c;
                }
                ++offset;
                String = val;
                return std::move( String );
            }
            
            JSON parse_number( const string &str, size_t &offset ) {
                JSON Number;
                string val, exp_str;
                char c;
                bool isDouble = false;
                long exp = 0;
                while( true ) {
                    c = str[offset++];
                    if( (c == '-') || (c >= '0' && c <= '9') )
                        val += c;
                    else if( c == '.' ) {
                        val += c; 
                        isDouble = true;
                    }
                    else
                        break;
                }
                if( c == 'E' || c == 'e' ) {
                    c = str[ offset++ ];
                    if( c == '-' ){ ++offset; exp_str += '-';}
                    while( true ) {
                        c = str[ offset++ ];
                        if( c >= '0' && c <= '9' )
                            exp_str += c;
                        else if( !isspace( c ) && c != ',' && c != ']' && c != '}' ) {
                            std::cerr << "ERROR: Number: Expected a number for exponent, found '" << c << "'\n";
                            return std::move( JSON::Make( JSON::Class::Null ) );
                        }
                        else
                            break;
                    }
                    exp = std::atoi( exp_str.c_str() );
                }
                else if( !isspace( c ) && c != ',' && c != ']' && c != '}' ) {
                    std::cerr << "ERROR: Number: unexpected character '" << c << "'\n";
                    return std::move( JSON::Make( JSON::Class::Null ) );
                }
                --offset;
                
                if( isDouble )
                    Number = std::atof( val.c_str() ) * std::pow( 10, exp );
                else {
                    if( !exp_str.empty() )
                        Number = std::atoi( val.c_str() ) * std::pow( 10, exp );
                    else
                        Number = std::atoi( val.c_str() );
                }
                return std::move( Number );
            }
            
            JSON parse_bool( const string &str, size_t &offset ) {
                JSON Bool;
                if( str.substr( offset, 4 ) == "true" )
                    Bool = true;
                else if( str.substr( offset, 5 ) == "false" )
                    Bool = false;
                else {
                    std::cerr << "ERROR: Bool: Expected 'true' or 'false', found '" << str.substr( offset, 5 ) << "'\n";
                    return std::move( JSON::Make( JSON::Class::Null ) );
                }
                offset += (Bool.ToBool() ? 4 : 5);
                return std::move( Bool );
            }
            
            JSON parse_null( const string &str, size_t &offset ) {
                JSON Null;
                if( str.substr( offset, 4 ) != "null" ) {
                    std::cerr << "ERROR: Null: Expected 'null', found '" << str.substr( offset, 4 ) << "'\n";
                    return std::move( JSON::Make( JSON::Class::Null ) );
                }
                offset += 4;
                return std::move( Null );
            }
            
            JSON parse_next( const string &str, size_t &offset ) {
                char value;
                consume_ws( str, offset );
                value = str[offset];
                switch( value ) {
                    case '[' : return std::move( parse_array( str, offset ) );
                    case '{' : return std::move( parse_object( str, offset ) );
                    case '\"': return std::move( parse_string( str, offset ) );
                    case 't' :
                    case 'f' : return std::move( parse_bool( str, offset ) );
                    case 'n' : return std::move( parse_null( str, offset ) );
                    default  : if( ( value <= '9' && value >= '0' ) || value == '-' )
                        return std::move( parse_number( str, offset ) );
                }
                std::cerr << "ERROR: Parse: Unknown starting character '" << value << "'\n";
                return JSON();
            }
        }
        
        JSON JSON::Load( const string &str ) {
            size_t offset = 0;
            return std::move( parse_next( str, offset ) );
        }
        
    }
    
}

