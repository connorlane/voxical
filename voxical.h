#ifndef VOXICAL_H
#define VOXICAL_H

#include <geotypes.h>
#include <stlparser.h>
#include <slice.h>
#include <cassert>
#include <string>

std::vector< std::vector<Segment2> > makeslice(std::string filepath);

#endif
