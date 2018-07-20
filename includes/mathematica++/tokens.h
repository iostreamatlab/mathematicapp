/*
 * Copyright 2017 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#ifndef WSTP_TOKENS_H
#define WSTP_TOKENS_H

#include <vector>
#include <string>
#include <stack>
#include <complex>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include "mathematica++/variant.h"

namespace mathematica{

class accessor;

class token{
	int _token;
	public:
		enum token_type{
			token_null,
			token_integer,
			token_real,
			token_function,
			token_str,
			token_symbol
		};
		typedef boost::shared_ptr<token> ptr;
	protected:
		token_type _type; 
		mathematica::accessor* _accessor;
		token(mathematica::accessor* accessor, int t, token_type type);
	public:
		int mathematica_type() const;
        token_type type() const;
		virtual void fetch() = 0;
		virtual std::string stringify() const = 0;
		virtual mathematica::variant serialize() const = 0;
    public:
        operator int() const;
        operator double() const;
        operator std::string() const;
};

typedef token::ptr value;
std::ostream& operator<<(std::ostream& stream, const token& tkn);
std::ostream& operator<<(std::ostream& stream, const value& tkn);

template <typename T>
bool operator==(const token& tkn, const T& other){
    return other == boost::lexical_cast<T>(tkn);
}
template <typename T>
bool operator==(const T& other, const token& tkn){
    return operator==(tkn, other);
}
template <typename T>
bool operator!=(const token& tkn, const T& other){
    return !(operator==(tkn, other));
}
template <typename T>
bool operator!=(const T& other, const token& tkn){
    return !(operator==(tkn, other));
}
template <typename T>
bool operator==(token::ptr tkn, const T& other){
    return operator==(*tkn, other);
}
template <typename T>
bool operator!=(token::ptr tkn, const T& other){
    return operator!=(*tkn, other);
}

/**
 * coerce's a value to the given type
 * \note may use static_cast or lexical_cast or no cast at all
 * \warning returns default constructed T for composite types
 */
template <typename T>
T coerce(mathematica::value var){
    return coerce<T>(var->serialize());
}

/**
 * creates a container of type T having values of type T::value_type while var of children of var are coerced to T::value_type through coerce()
 * \note creates one element container for scalers
 * \warning nesting of composites not supported (cannot be supported in vector)
 */
template <typename T>
T vectorify(mathematica::value var){
   return vectorify<T>(var->serialize());
}

template <typename T>
T tuplify(mathematica::value var){
    return tuplify<T>(var->serialize());
}

template <typename T>
T cast(mathematica::value var){
    return cast<T>(var->serialize());
}

namespace tokens{
class integer: public token{
	int _data;
	public:
        typedef boost::shared_ptr<integer> ptr;
		integer(mathematica::accessor* accessor);
	public:
		virtual void fetch();
		virtual std::string stringify() const;
        virtual mathematica::variant serialize() const;
		int value() const;
};
class real: public token{
	double _data;
	public:
        typedef boost::shared_ptr<real> ptr;
		real(mathematica::accessor* accessor);
	public:
		virtual void fetch();
		virtual std::string stringify() const;
        virtual mathematica::variant serialize() const;
		double value() const;
};
struct function: public token{
	int _nargs;
	std::string _name;
	std::vector<mathematica::value> _args;
	public:
        typedef boost::shared_ptr<function> ptr;
		function(mathematica::accessor* accessor);
	public:
		virtual void fetch();
		virtual std::string stringify() const;
        virtual mathematica::variant serialize() const;
		int nargs() const;
		std::string name() const;
        bool is_graphics() const;
	protected:
		virtual void populate();
};
class str: public token{
  std::string _message;
	public:
        typedef boost::shared_ptr<str> ptr;
		str(mathematica::accessor* accessor);
	public:
		virtual void fetch();
		virtual std::string stringify() const;
        virtual mathematica::variant serialize() const;
};
class symbol: public token{
	std::string _name;
	public:
        typedef boost::shared_ptr<symbol> ptr;
		symbol(mathematica::accessor* accessor);
	public:
		virtual void fetch();
		virtual std::string stringify() const;
        virtual mathematica::variant serialize() const;
		std::string name() const;
};
boost::shared_ptr<token> factory(accessor* accessor, int type);
}

}
#endif // WSTP_TOKENS_H