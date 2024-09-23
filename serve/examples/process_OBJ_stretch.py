#!/usr/bin/env python

from numpy import *

stretch = True

def print_uvs( xys ):
    xys = [ map( float, xy ) for xy in xys ]
    
    xys = array( xys )
    ## Lower-left corner is now at the origin.
    xys -= xys.min(axis=0)
    ## Scale such that the width is 1.
    xys /= xys.max(0)
    
    if stretch:
        ## Apply a non-uniform stretch to the x coordinates.
        xys[:,0] = log( 1.+10*xys[:,0] )
        ## Re-scale x to [0,1].
        xys[:,0] -= xys[:,0].min()
        xys[:,0] /= xys[:,0].max()
    
    ## Print vertex lines.
    for x, y in xys:
        print 'v', 2*x-1, 2*y-1, 0.
    
    ## Print a blank line
    print
    
    ## Print uv lines.
    for x, y in xys:
        print 'vt', x, y
    
    ## Print a blank line
    print

def main():
    import sys
    
    if len( sys.argv ) == 1:
        print >> sys.stderr, 'Usage:', sys.argv[0], 'path/to/mesh.obj'
        print >> sys.stderr, 'WARNING: The result is printed to standard out.'
        print >> sys.stderr, "EXAMPLE: parallel ./process_OBJ_stretch.py '{}' '>' '{.}-stretch.obj' ::: *regular*.obj"
        sys.exit(-1)
    
    first_face = True
    
    vlines = []
    for line in open( sys.argv[1] ):
        sline = line.strip().split()
        
        ## Skip blank lines
        if len( sline ) == 0:
            continue
        
        ## Save vertices
        elif sline[0] == 'v':
            vlines.append( sline[1:3] )
        
        ## Print faces
        elif sline[0] == 'f':
            if first_face:
                print_uvs( vlines )
                first_face = False
            
            ## Grab the first number before the slash for each of the three face vertex bundles.
            a,b,c = [ bundle.split('/')[0] for bundle in sline ][1:4]
            print 'f %s/%s %s/%s %s/%s' % ( a,a, b,b, c,c )

if __name__ == '__main__': main()
