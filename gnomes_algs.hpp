///////////////////////////////////////////////////////////////////////////////
// gnomes_algs.hpp
//
// Algorithms that solve the greedy gnomes problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on gnomes_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>

#include "gnomes_types.hpp"
#include <vector>

namespace gnomes {

// Solve the greedy gnomes problem for the given grid, using an exhaustive
// search algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
path greedy_gnomes_exhaustive(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  // Compute maximum path length, and check that it is legal.
  const size_t max_steps = setting.rows() + setting.columns() - 2;
  assert(max_steps < 64);
  path best(setting);
  for(size_t i = 0; i <= max_steps; i++)
  {
    for(size_t bit = 0; bit <= (pow(2, i)-1); bit++)
    {
      path candidate(setting);
      for(size_t k = 0; k <= (i); k++)
      {
        int tempbit = (bit >> k) & 1;
        if(tempbit == 1 and candidate.is_step_valid(STEP_DIRECTION_RIGHT))
          candidate.add_step(STEP_DIRECTION_RIGHT);
        else if(candidate.is_step_valid(STEP_DIRECTION_DOWN))
          candidate.add_step(STEP_DIRECTION_DOWN);
      }
      if(best.total_gold() < candidate.total_gold())
        best = candidate;
    }
  }
  return best;
}


path greedy_gnomes_dyn_prog(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  path *from_above = 0;
  path *from_left = 0;
  path * a[setting.rows()][setting.columns()];
  a[0][0] = new path(setting);
  for(size_t i = 0; i < setting.rows(); i++)
  {
    for(size_t j = 0; j < setting.columns(); j++)
    {
      from_above = 0;
      from_left = 0;
      if(setting.get(i, j) == CELL_ROCK)
      {
        a[i][j] = 0;
      }
      else
      {
        if(i == 0 && j == 0)
        {/*do nothing for (0, 0)*/}
        else
        {
          if(i > 0 && a[i-1][j] != 0)
          {
            from_above = new path(*a[i-1][j]);
            from_above->add_step(STEP_DIRECTION_DOWN);
          }
          if(j > 0 && a[i][j-1] != 0)
          {
            from_left = new path(*a[i][j-1]);
            from_left->add_step(STEP_DIRECTION_RIGHT);
          }

          //decide what path to assign to grid point(i, j)
          if(from_above != 0 && from_left != 0)
          {
            if(from_above->total_gold() >= from_left->total_gold())
              a[i][j] = from_above;
            else
              a[i][j] = from_left;
          }
          else if(from_above != 0)
            a[i][j] = from_above;
          else if(from_left != 0)
            a[i][j] = from_left;
          else
            a[i][j] = 0;
        }
      }
    }
  }

  //post processing
  path best = *a[0][0];
  for(size_t ind = 0; ind < setting.rows(); ind++)
  {
    for(size_t j = 0; j < setting.columns(); j++)
    {
      if(a[ind][j] != 0 && a[ind][j]->total_gold() > best.total_gold())
        best = *a[ind][j];
    }
  }
  return best;
}
}
