/*
 * Copyright (c) 2018, Sunanda Bose (Neel Basu) (neel.basu.z@gmail.com) 
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met: 
 * 
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY 
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
 * DAMAGE. 
 */

#ifndef MATHEMATICAPP_DEFS_H
#define MATHEMATICAPP_DEFS_H

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
#define UNPACK(...) __VA_ARGS__
#define GET_MACRO(_1,_2,_3, NAME,...) NAME
#define MATHEMATICA_ASSOCIATE(T, ...)                                   \
    template <>                                                         \
    struct mathematica::association<T>: mathematica::dictionary<association<T>, T, __VA_ARGS__>
    
#define MATHEMATICA_SEQUENCE(T, ...)                                   \
    template <>                                                        \
    struct mathematica::association<T>: mathematica::sequence<association<T>, T, __VA_ARGS__>
    
#define MATHEMATICA_TYPEMAP(T, U)                                      \
    template <>                                                        \
    struct mathematica::association<T>: mathematica::typemap<association<T>, T, UNPACK U>
    
#define MATHEMATICA_PROPERTY_NAMED(I, N, P) static auto detail(property<I>){return std::make_pair(N,  &class_type::P);}
#define MATHEMATICA_PROPERTY_AUTO(I, P) static auto detail(property<I>){return std::make_pair(STR_EXPAND(P), &class_type::P);}

#define MATHEMATICA_PROPERTY(...) GET_MACRO(__VA_ARGS__, MATHEMATICA_PROPERTY_NAMED, MATHEMATICA_PROPERTY_AUTO)(__VA_ARGS__)
#define MATHEMATICA_ELEMENT(I, P) static auto detail(property<I>){return &class_type::P;}

namespace mathematica{

enum packet_type{
    packet_ILLEGAL=0,

    packet_CALL=7,
    packet_EVALUATE=13,
    packet_RETURN=3,

    packet_INPUTNAME=8,
    packet_ENTERTEXT=14,
    packet_ENTEREXPR=15,
    packet_OUTPUTNAME=9,
    packet_RETURNTEXT=4,
    packet_RETURNEXPR=16,

    packet_DISPLAY=11,
    packet_DISPLAYEND=12,

    packet_MESSAGE=5,
    packet_TEXT=2,

    packet_INPUT=1,
    packet_INPUTSTR=21,
    packet_MENU=6,
    packet_SYNTAX=10,

    packet_SUSPEND=17,
    packet_RESUME=18,

    packet_BEGINDLG=19,
    packet_ENDDLG=20,

    packet_FIRSTUSER=128,
    packet_LASTUSER=255
};

enum features{
    bulk_io             = 1 << 0,
    sanity_check        = 1 << 1,
    graphics_autosave   = 1 << 2
};

}

#endif
