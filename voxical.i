 %module voxical
%include "std_vector.i"
%include "std_string.i"

%{
/* Includes the header in the wrapper code */
#include "voxical.h"
#include "geotypes.h"
%}

/* Parse the header file to generate wrappers */
%include "voxical.h"
%include "geotypes.h"

%template(VSeg2) std::vector<Segment2>;
%template(VVSeg2) std::vector<std::vector<Segment2> >;

%extend Segment2 {
    Point2 __getitem__(unsigned int i) {
        return (*($self))[i];
    }
}

