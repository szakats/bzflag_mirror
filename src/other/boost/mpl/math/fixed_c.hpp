
#ifndef BOOST_MPL_FIXED_C_HPP_INCLUDED
#define BOOST_MPL_FIXED_C_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date: 2004-09-02 17:41:37 +0200 (Thu, 02 Sep 2004) $
// $Revision: 24874 $

#include <boost/mpl/aux_/config/static_constant.hpp>

namespace boost { namespace mpl {

template<
      long IntegerPart
    , unsigned long FractionPart
    >
struct fixed_c
{
    BOOST_STATIC_CONSTANT(long, integer_part = IntegerPart);
    BOOST_STATIC_CONSTANT(unsigned long, fraction_part = FractionPart);
    typedef fixed_c<IntegerPart, FractionPart> type;
    
    fixed_c() {}
};

}}

#endif // BOOST_MPL_FIXED_C_HPP_INCLUDED
