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

#ifndef __GAME_OF_WHALES__JSON__HEADER__
#define __GAME_OF_WHALES__JSON__HEADER__

#include <cstdint>
#include <cmath>
#include <cctype>
#include <string>
#include <deque>
#include <map>
#include <type_traits>
#include <initializer_list>
#include <ostream>
#include <iostream>
#include <sstream>
#include <cstdlib>





namespace gameofwhales {
    namespace json {
        
        using std::map;
        using std::deque;
        using std::string;
        using std::enable_if;
        using std::initializer_list;
        using std::is_same;
        using std::is_convertible;
        using std::is_integral;
        using std::is_floating_point;
        
        string json_escape( const string &str );
        
        class JSON
        {
            union BackingData {
                BackingData( double d ) : Float( d ){}
                BackingData( long   l ) : Int( l ){}
                BackingData( bool   b ) : Bool( b ){}
                BackingData( string s ) : String( new string( s ) ){}
                BackingData()           : Int( 0 ){}
                
                deque<JSON>        *List;
                map<string,JSON>   *Map;
                string             *String;
                double              Float;
                long                Int;
                bool                Bool;
            } Internal;
            
        public:
            enum class Class {
                Null,
                Object,
                Array,
                String,
                Floating,
                Integral,
                Boolean
            };
            
            template <typename Container>
            class JSONWrapper {
                Container *object;
                
                public:
                    JSONWrapper( Container *val ) : object( val ) {}
                    JSONWrapper( std::nullptr_t )  : object( nullptr ) {}
                
                    typename Container::iterator begin() { return object ? object->begin() : typename Container::iterator(); }
                    typename Container::iterator end() { return object ? object->end() : typename Container::iterator(); }
                    typename Container::const_iterator begin() const { return object ? object->begin() : typename Container::iterator(); }
                    typename Container::const_iterator end() const { return object ? object->end() : typename Container::iterator(); }
                };
            
                template <typename Container>
                class JSONConstWrapper {
                    const Container *object;
                    
                public:
                    JSONConstWrapper( const Container *val ) : object( val ) {}
                    JSONConstWrapper( std::nullptr_t )  : object( nullptr ) {}
                    
                    typename Container::const_iterator begin() const { return object ? object->begin() : typename Container::const_iterator(); }
                    typename Container::const_iterator end() const { return object ? object->end() : typename Container::const_iterator(); }
            };//CLASS JSONWrapper
            
            JSON();
            ~JSON();
            JSON( initializer_list<JSON> list );
            JSON( JSON&& other );
            
            JSON& operator=( JSON&& other );
            JSON( const JSON &other );
            JSON& operator=( const JSON &other );
            
            
            
            template <typename T>
            JSON( T b, typename enable_if<is_same<T,bool>::value>::type* = 0 ) : Internal( b ), Type( Class::Boolean ){}
            
            template <typename T>
            JSON( T i, typename enable_if<is_integral<T>::value && !is_same<T,bool>::value>::type* = 0 ) : Internal( (long)i ), Type( Class::Integral ){}
            
            template <typename T>
            JSON( T f, typename enable_if<is_floating_point<T>::value>::type* = 0 ) : Internal( (double)f ), Type( Class::Floating ){}
            
            template <typename T>
            JSON( T s, typename enable_if<is_convertible<T,string>::value>::type* = 0 ) : Internal( string( s ) ), Type( Class::String ){}
            
            JSON( std::nullptr_t ) : Internal(), Type( Class::Null ){}
            
            static JSON Make( Class type ) {
                JSON ret; ret.SetType( type );
                return ret;
            }
            
            static JSON Load( const string & );
            
            template <typename T>
            void append( T arg ) {
                SetType( Class::Array ); Internal.List->emplace_back( arg );
            }
            
            template <typename T, typename... U>
            void append( T arg, U... args ) {
                append( arg ); append( args... );
            }
            
            template <typename T>
            typename enable_if<is_same<T,bool>::value, JSON&>::type operator=( T b ) {
                SetType( Class::Boolean ); Internal.Bool = b; return *this;
            }
            
            template <typename T>
            typename enable_if<is_integral<T>::value && !is_same<T,bool>::value, JSON&>::type operator=( T i ) {
                SetType( Class::Integral ); Internal.Int = i; return *this;
            }
            
            template <typename T>
            typename enable_if<is_floating_point<T>::value, JSON&>::type operator=( T f ) {
                SetType( Class::Floating ); Internal.Float = f; return *this;
            }
            
            template <typename T>
            typename enable_if<is_convertible<T,string>::value, JSON&>::type operator=( T s ) {
                SetType( Class::String ); *Internal.String = string( s ); return *this;
            }
            
            JSON& operator[]( const string &key );
            JSON& operator[]( unsigned index );
            JSON &at( const string &key );
            const JSON &at( const string &key ) const;
            JSON &at( unsigned index );
            const JSON &at( unsigned index ) const;
            int length() const;
            bool hasKey( const string &key ) const;
            int size() const;
            Class JSONType() const { return Type; }
            
            /// Functions for getting primitives from the JSON object.
            bool IsNull() const { return Type == Class::Null; }
            
            string ToString() const { bool b; return std::move( ToString( b ) ); }
            string ToString( bool &ok ) const {
                ok = (Type == Class::String);
                return ok ? std::move( json_escape( *Internal.String ) ): string("");
            }
            
            double ToFloat() const { bool b; return ToFloat( b ); }
            double ToFloat( bool &ok ) const {
                ok = (Type == Class::Floating);
                return ok ? Internal.Float : 0.0;
            }
            
            long ToInt() const { bool b; return ToInt( b ); }
            long ToInt( bool &ok ) const {
                ok = (Type == Class::Integral);
                return ok ? Internal.Int : 0;
            }
            
            bool ToBool() const { bool b; return ToBool( b ); }
            bool ToBool( bool &ok ) const {
                ok = (Type == Class::Boolean);
                return ok ? Internal.Bool : false;
            }
            
            JSONWrapper<map<string,JSON>> ObjectRange() {
                if( Type == Class::Object )
                    return JSONWrapper<map<string,JSON>>( Internal.Map );
                return JSONWrapper<map<string,JSON>>( nullptr );
            }
            
            JSONWrapper<deque<JSON>> ArrayRange() {
                if( Type == Class::Array )
                    return JSONWrapper<deque<JSON>>( Internal.List );
                return JSONWrapper<deque<JSON>>( nullptr );
            }
            
            JSONConstWrapper<map<string,JSON>> ObjectRange() const {
                if( Type == Class::Object )
                    return JSONConstWrapper<map<string,JSON>>( Internal.Map );
                return JSONConstWrapper<map<string,JSON>>( nullptr );
            }
            
            
            JSONConstWrapper<deque<JSON>> ArrayRange() const {
                if( Type == Class::Array )
                    return JSONConstWrapper<deque<JSON>>( Internal.List );
                return JSONConstWrapper<deque<JSON>>( nullptr );
            }
            
            string dump( int depth = 1, string tab = "  ") const;
            friend std::ostream& operator<<( std::ostream &os, const JSON &json ) {
                os << json.dump();
                return os;
            }
            
        private:
            void SetType( Class type );
            
        private:
            /* beware: only call if YOU know that Internal is allocated. No checks performed here.
             This function should be called in a constructed JSON just before you are going to
             overwrite Internal...
             */
            void ClearInternal();
        private:
            Class Type = Class::Null;
        };//CLASS JSON
        
        JSON Array();
        JSON Object();
       
        
        template <typename... T>
        JSON Array( T... args ) {
            JSON arr = JSON::Make( JSON::Class::Array );
            arr.append( args... );
            return std::move( arr );
        }
        
    }//NAMESPACE JSON
    
}//NAMESPACE GAMEOFWHALES


#endif//__GAME_OF_WHALES__JSON__HEADER__
