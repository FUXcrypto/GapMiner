/**
 * Header file for a prime gap candidate used in the ChineseSieve
 *
 * Copyright (C)  2014  Jonny Frey  <j0nn9.fr39@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __GAP_CANDIDATE_H__
#define __GAP_CANDIDATE_H__

#include <gmp.h>
#include <inttypes.h>
#include <vector>

using namespace std;

class GapCandidate {

  public: 

    /* the nonce of this */
    uint32_t nonce;

    /* the target */
    uint64_t target;
 
    /* the gap start */
    mpz_t mpz_gap_start;
 
    /* the number of prime candidates */
    uint32_t n_candidates;
 
    /* the candidate offsets */
    vector<uint32_t> candidates;

    /* creat a new GapCandidate */
    GapCandidate(uint32_t nonce,
                 uint64_t target,
                 mpz_t mpz_gap_start, 
                 vector<uint32_t> candidates);
 
    ~GapCandidate();
};

#endif
