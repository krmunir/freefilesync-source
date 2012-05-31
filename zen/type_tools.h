// **************************************************************************
// * This file is part of the zenXML project. It is distributed under the   *
// * Boost Software License, Version 1.0. See accompanying file             *
// * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.       *
// * Copyright (C) ZenJu (zhnmju123 AT gmx DOT de) - All Rights Reserved    *
// **************************************************************************

#ifndef TYPE_TOOLS_HEADER_45237590734254545
#define TYPE_TOOLS_HEADER_45237590734254545

#include "type_traits.h"

namespace zen
{
//########## Strawman Classes ##########################
struct NullType {}; //:= no type here

//########## Type Mapping ##############################
template <int n>
struct Int2Type {};
//------------------------------------------------------
template <class T>
struct Type2Type {};

//########## Control Structures ########################
template <bool flag, class T, class U>
struct SelectIf : ResultType<T> {};

template <class T, class U>
struct SelectIf<false, T, U> : ResultType<U> {};
//------------------------------------------------------
template <class T, class U>
struct IsSameType : StaticBool<false> {};

template <class T>
struct IsSameType<T, T> : StaticBool<true> {};

//------------------------------------------------------
template <bool, class T = void>
struct EnableIf {};

template <class T>
struct EnableIf<true, T> : ResultType<T> {};
//########## Type Cleanup ##############################
template <class T>
struct RemoveRef : ResultType<T> {};

template <class T>
struct RemoveRef<T&> : ResultType<T> {};
//------------------------------------------------------
template <class T>
struct RemoveConst : ResultType<T> {};

template <class T>
struct RemoveConst<const T> : ResultType<T> {};
//------------------------------------------------------
template <class T>
struct RemovePointer : ResultType<T> {};

template <class T>
struct RemovePointer<T*> : ResultType<T> {};
//------------------------------------------------------
template <class T>
struct RemoveArray : ResultType<T> {};

template <class T, int N>
struct RemoveArray<T[N]> : ResultType<T> {};

//########## Sorting ##############################
/*
Generate a descending binary predicate at compile time!

Usage:
    static const bool ascending = ...
    makeSortDirection(old binary predicate, Int2Type<ascending>()) -> new binary predicate

or directly;
    makeDescending(old binary predicate) -> new binary predicate
*/

template <class Predicate>
struct LessDescending
{
    LessDescending(Predicate lessThan) : lessThan_(lessThan) {}
    template <class T> bool operator()(const T& lhs, const T& rhs) const { return lessThan_(rhs, lhs); }
private:
    Predicate lessThan_;
};

template <class Predicate> inline
/**/           Predicate  makeSortDirection(Predicate pred, Int2Type<true>) { return pred; }

template <class Predicate> inline
LessDescending<Predicate> makeSortDirection(Predicate pred, Int2Type<false>) { return pred; }

template <class Predicate> inline
LessDescending<Predicate> makeDescending(Predicate pred) { return pred; }
}

#endif //TYPE_TOOLS_HEADER_45237590734254545