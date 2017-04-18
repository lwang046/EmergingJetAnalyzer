"""Generates code for EmergingJetAnalyzer"""
import cog

TYPESTRLENGTH = 30

def pad_with_whitespace(input_string, length=20):
    '''Pads input string with spaces to given length'''
    format_string = '{: <%d}' % width
    output_string = format_string.format(input_string)
    return output_string

def vector(type_string):
    '''Take a string defining a C++ type and outputs a string defining a vector of that type.
    Also pads the output string to the default TYPESTRLENGTH defined in file'''
    stripped_string = type_string.strip()
    vector_type_string = 'vector < %s >' % stripped_string
    output_string = pad_with_whitespace(input_string, TYPESTRLENGTH)
    return output_string

non_vector_default = '-1' # Default value for non vector variables
type_int         = 'int                     '
type_vint        = 'vector<int>             '
type_vvint       = 'vector< vector<int> >   '
type_float       = 'float                   '
type_vfloat      = 'vector<float>           '
type_vvfloat     = 'vector< vector<float> > '
typestr_int   = [type_int   , type_vint   , type_vvint   ]
typestr_float = [type_float , type_vfloat , type_vvfloat ]
event_vars = [
    ( "run                           " , typestr_int    [0] ) ,
    ( "lumi                          " , typestr_int    [0] ) ,
    ( "event                         " , typestr_int    [0] ) ,
    ( "bx                            " , typestr_int    [0] ) ,
    ( "nVtx                          " , typestr_int    [0] ) ,
    ( "nGoodVtx                      " , typestr_int    [0] ) ,
    ( "nTrueInt                      " , typestr_int    [0] ) ,
    ( "met_pt                        " , typestr_float  [0] ) ,
    ( "met_phi                       " , typestr_float  [0] ) ,
]
jet_vars = [
    ( "jets_pt                    " , type_vfloat  ) ,
    ( "jets_eta                   " , type_vfloat  ) ,
    ( "jets_phi                   " , type_vfloat  ) ,
    ( "jets_cef                   " , type_vfloat  ) ,
    ( "jets_nef                   " , type_vfloat  ) ,
    ( "jets_chf                   " , type_vfloat  ) ,
    ( "jets_nhf                   " , type_vfloat  ) ,
    ( "jets_phf                   " , type_vfloat  ) ,
    ( "jets_nPromptTracks         " , type_vint    ) ,
    ( "jets_nDispTracks           " , type_vint    ) ,
    ( "jets_nSV                   " , type_vint    ) ,
    ( "jets_medianLogIpSig        " , type_vfloat  ) ,
    ( "jets_missHits              " , type_vint    ) ,
    ( "jets_muonHits              " , type_vint    ) ,
    ( "jets_alphaMax              " , type_vfloat  ) ,
    ( "jets_nDarkPions            " , type_vint    ) ,
    ( "jets_minDRDarkPion         " , type_vfloat  ) ,
]
jet_track_vars = [
    ( "tracks_source              " , type_vvint   ) ,
    ( "tracks_pt                  " , type_vvfloat ) ,
    ( "tracks_eta                 " , type_vvfloat ) ,
    ( "tracks_phi                 " , type_vvfloat ) ,
    ( "tracks_pca_r               " , type_vvfloat ) ,
    ( "tracks_pca_eta             " , type_vvfloat ) ,
    ( "tracks_pca_phi             " , type_vvfloat ) ,
    ( "tracks_algo                " , type_vvint   ) ,
    ( "tracks_originalAlgo        " , type_vvint   ) ,
    ( "tracks_nHits               " , type_vvint   ) ,
    ( "tracks_nMissInnerHits      " , type_vvint   ) ,
    ( "tracks_nTrkLayers          " , type_vvint   ) ,
    ( "tracks_nMissInnerTrkLayers " , type_vvint   ) ,
    ( "tracks_nMissOuterTrkLayers " , type_vvint   ) ,
    ( "tracks_nMissTrkLayers      " , type_vvint   ) ,
    ( "tracks_nPxlLayers          " , type_vvint   ) ,
    ( "tracks_nMissInnerPxlLayers " , type_vvint   ) ,
    ( "tracks_nMissOuterPxlLayers " , type_vvint   ) ,
    ( "tracks_nMissPxlLayers      " , type_vvint   ) ,
    ( "tracks_ipXY                " , type_vvfloat ) ,
    ( "tracks_ipZ                 " , type_vvfloat ) , # Empty for now
    ( "tracks_ipXYSig             " , type_vvfloat ) ,
    ( "tracks_dRToJetAxis         " , type_vvfloat ) ,
    ( "tracks_distanceToJet       " , type_vvfloat ) ,
    ( "tracks_vertexLxy           " , type_vvfloat ) ,
]
jet_trackref_vars = [
    ( "trackrefs_pt                  " , type_vvfloat ) ,
    ( "trackrefs_eta                 " , type_vvfloat ) ,
    ( "trackrefs_phi                 " , type_vvfloat ) ,
]
jet_vertex_vars = [
    ( "jet_vertex_source             " , typestr_int   [2] ) ,
    ( "jet_vertex_x                  " , typestr_float [2] ) ,
    ( "jet_vertex_y                  " , typestr_float [2] ) ,
    ( "jet_vertex_z                  " , typestr_float [2] ) ,
    ( "jet_vertex_xError             " , typestr_float [2] ) ,
    ( "jet_vertex_yError             " , typestr_float [2] ) ,
    ( "jet_vertex_zError             " , typestr_float [2] ) ,
    ( "jet_vertex_deltaR             " , typestr_float [2] ) ,
    ( "jet_vertex_Lxy                " , typestr_float [2] ) ,
    ( "jet_vertex_mass               " , typestr_float [2] ) ,
    ( "jet_vertex_chi2               " , typestr_float [2] ) ,
    ( "jet_vertex_ndof               " , typestr_float [2] ) ,
    ( "jet_vertex_pt2sum             " , typestr_float [2] ) ,
]
vertex_vars = [
    ( "vertex_source                 " , typestr_int   [1] ) ,
    ( "vertex_x                      " , typestr_float [1] ) ,
    ( "vertex_y                      " , typestr_float [1] ) ,
    ( "vertex_z                      " , typestr_float [1] ) ,
    ( "vertex_xError                 " , typestr_float [1] ) ,
    ( "vertex_yError                 " , typestr_float [1] ) ,
    ( "vertex_zError                 " , typestr_float [1] ) ,
    ( "vertex_Lxy                    " , typestr_float [1] ) ,
    ( "vertex_mass                   " , typestr_float [1] ) ,
    ( "vertex_chi2                   " , typestr_float [1] ) ,
    ( "vertex_ndof                   " , typestr_float [1] ) ,
    ( "vertex_pt2sum                 " , typestr_float [1] ) ,
]
all_vars = event_vars + jet_vars + jet_track_vars + jet_trackref_vars+ jet_vertex_vars + vertex_vars

def printLine(*args):
    line = "".join(args)
    # print line
    cog.outl(line)

def genOutputTree():
    # Output <typename> <varname>;
    for var in all_vars:
        varname = var[0]
        typename = var[1]
        printLine("%s %s;" % (typename, varname))

def genInit():
    # Output <varname>.clear();
    for var in all_vars:
        varname = var[0]
        typename = var[1]
        # Clear vectors
        if 'vector' in typename:
            printLine("%s.clear();" % varname)
        else:
            printLine("%s= %s;" % (varname, non_vector_default))

def genBranch():
    # printLine("#define BRANCH(tree, branch) (tree)->Branch(#branch, &branch);")
    # Output BRANCH(tree, <varname>);
    for var in all_vars:
        varname = var[0]
        typename = var[1]
        printLine("BRANCH(tree, %s);" % varname)
