#!/usr/bin/env python

def print_uvs( xys ):
    xys = [ map( float, xy ) for xy in xys ]
    from numpy import array
    xys = array( xys )
    xys0 = xys - xys.min(axis=0)
    xys0.min(0)
    xys0/=xys0.max(0)
    xys0.max(0)
    
    for x, y in xys0:
        print 'vt', x, y
    print

def main():
    import sys
    
    if len( sys.argv ) == 1:
        print >> sys.stderr, 'Usage:', sys.argv[0], 'path/to/mesh.obj'
        print >> sys.stderr, 'WARNING: The result is printed to standard out.'
        sys.exit(-1)
    
    first_face = True
    
    vlines = []
    for line in open( sys.argv[1] ):
        sline = line.strip().split()
        will_print = True
        
        if len( sline ) == 0:
            pass
        elif sline[0] == 'v':
            vlines.append( sline[1:3] )
        elif sline[0] == 'vt':
            ## Skip uv lines, because we are going to write our own.
            will_print = False
        
        elif sline[0] == 'f':
            if first_face:
                print_uvs( vlines )
                first_face = False
            
            ## Grab the first number before the slash for each of the three face vertex bundles.
            a,b,c = [ bundle.split('/')[0] for bundle in sline ][1:4]
            print 'f %s/%s %s/%s %s/%s' % ( a,a, b,b, c,c )
            will_print = False
        
        if will_print:
            print line.strip()

if __name__ == '__main__': main()
