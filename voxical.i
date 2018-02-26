 %module voxical
%include "std_vector.i"
%include "std_string.i"

%{
/* Includes the header in the wrapper code */
#include "voxical.h"
%}

/* Parse the header file to generate wrappers */
%include "voxical.h"

%template(VSeg2) std::vector<Segment2>;
%template(VVSeg2) std::vector<std::vector<Segment2> >;
