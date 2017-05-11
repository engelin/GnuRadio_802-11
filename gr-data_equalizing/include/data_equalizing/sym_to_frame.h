/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef INCLUDED_DATA_EQUALIZING_SYM_TO_FRAME_H
#define INCLUDED_DATA_EQUALIZING_SYM_TO_FRAME_H

#include <data_equalizing/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace data_equalizing {

    /*!
     * \brief <+description of block+>
     * \ingroup data_equalizing
     *
     */
    class DATA_EQUALIZING_API sym_to_frame : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<sym_to_frame> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of data_equalizing::sym_to_frame.
       *
       * To avoid accidental use of raw pointers, data_equalizing::sym_to_frame's
       * constructor is in a private implementation
       * class. data_equalizing::sym_to_frame::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace data_equalizing
} // namespace gr

#endif /* INCLUDED_DATA_EQUALIZING_SYM_TO_FRAME_H */

