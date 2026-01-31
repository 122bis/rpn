
# rpn

    noitaton..........tpirstsop..........esrever
    a simple postscript inspired notation system
    for basic description of 2d geometry objects
    easy to read, parse, transcript to PS or svg 
    ................. syntax ...................
    a : arc (CW)............... an cx cy r a1 a2
    an : arc (CCW).............. a cx cy r a1 a2
    B : Bezier.......... B x y x1 y1 x2 y2 x3 y3
    l : line........................ l x y x1 y1
    p : point............................. p x y
    p3 : point & altitude (or depth)... p3 x y z
    t : turn (heading, length) ........... t a l
    _ : custom (keyword, value) ... _something x
    inc : include ... inc x y rot scale filename
    single line comment begin with character : #

# example rpn file

    l 1.500000, 0.000000, 1.500057, 250.000000
    a -286.478821, 250.000137, 287.978882, 0.000000, 20.000000 
    an 126.297775, 400.238342, 151.288742, 200.000000, 170.000000 
    an 210.935760, 385.314392, 237.232407, 170.000000, 110.000000 
    l 129.797470, 608.239929, 364.720642, 693.744934
    a 341.153137, 758.496155, 68.906799, -70.000000, 15.000000 
    an 475.442444, 794.478882, 70.119720, 195.000000, 135.000000 
    an 492.323395, 777.597900, 93.992966, 135.000000, 105.000000 
    a 439.803192, 973.606018, 108.929588, -75.000000, -35.000000 


# TODO

    implement :
    b bend to be included, bend as road bend is an arc,
    but described by a road length and the angle of change of previous direction 
    it is intended to describe a path made of lines and arc

    implement :
    v vertex are stored for later use "stack like" for bsplines
    ex : 
        v x y z
        v x y z
        v x y z
        v x y z
        B

    a angles are stored for later use ?
    r ray : vertex + direction

    O origin --> ps 'moveto'
    S scale --> ps 'scale'
    m matrix
    M matrix 3d
    >< push pop matrix (2d) --
    >> << push pop matrix (3d) --
    o observer (3d) x y z (position)  x y z (look at)
    o observer (3d)  v x y z (position) v u v w (euler angles)
 
