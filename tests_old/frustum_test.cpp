#include "frustum_test.hpp"

#include <vmmlib/frustum.hpp>

namespace vmml
{

bool frustum_test::run()
{
    bool ok = true;

    const frustum< float > frustum( -1.f, 1., -1.f, 1., 1.f, 100.f );
    const matrix< 4, 4, float > mvp = frustum.compute_matrix();

    frustum_culler< float > fc;
    fc.setup( mvp );
    if( !_test( fc ))
        ok = false;

    //   e_____f
    //  /     /|
    // | a b | |
    // | c d |/h
    //  -----
    const vector< 3, float > a( -1.f,  1.f, -1.f );
    const vector< 3, float > b(  1.f,  1.f, -1.f );
    const vector< 3, float > c( -1.f, -1.f, -1.f );
    const vector< 3, float > d(  1.f, -1.f, -1.f );
    const vector< 3, float > e( -100.f,  100.f, -100.f );
    const vector< 3, float > f(  100.f,  100.f, -100.f );
    const vector< 3, float > g( -100.f, -100.f, -100.f );
    const vector< 3, float > h(  100.f, -100.f, -100.f );

    fc.setup( a, b, c, d, e, f, g, h );
    if( !_test( fc ))
        ok = false;

    return ok;
}

bool frustum_test::_test( const frustum_culler< float > fc )
{
    bool ok = true;
    bool global_ok = true;

    const vector< 4, float > sphereIn( 0.f, 0.f, -10.f, 1.f );
    const vector< 4, float > sphereOut( 0.f, 0.f, 0.f, .5f );
    const vector< 4, float > sphereBorder( 0.f, 0.f, -1.f, 1.f );

    TESTINFO( fc.test_sphere( sphereIn ) == VISIBILITY_FULL,
              fc.test_sphere( sphereIn ));
    TESTINFO( fc.test_sphere( sphereOut ) == VISIBILITY_NONE,
              fc.test_sphere( sphereOut ));
    TESTINFO( fc.test_sphere( sphereBorder ) == VISIBILITY_PARTIAL,
              fc.test_sphere( sphereBorder ));

    const vector< 2, float > xy( -1.f, 1.f );
    const vector< 2, float > zIn( -2.f, -4.f );
    const vector< 2, float > zOut( 0.f, -.5f );
    const vector< 2, float > zBorder( -.5f, -1.5f );

    TESTINFO( fc.test_aabb( xy, xy, zIn ) == VISIBILITY_FULL,
              fc.test_aabb( xy, xy, zIn ));
    TESTINFO( fc.test_aabb( xy, xy, zOut ) == VISIBILITY_NONE,
              fc.test_aabb( xy, xy, zOut ));
    TESTINFO( fc.test_aabb( xy, xy, zBorder ) == VISIBILITY_PARTIAL,
              fc.test_aabb( xy, xy, zBorder ));

    return global_ok && ok;
}

} // namespace vmml
