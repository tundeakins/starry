//! The Map class
template <class S=Default<double>, bool EMISSION=true>
class Map 
{

public:

    // Types
    using Scalar = typename S::Scalar;                                         /**< The scalar type of the map */
    using YType = typename S::YType;                                           /**< The type of the spherical harmonic coefficient object (vector/matrix) */
    using YCoeffType = typename S::YCoeffType;                                 /**< The type of the spherical harmonic coefficients (scalar/row vector) */
    using UType = typename S::UType;                                           /**< The type of the limb darkening coefficient object (vector/matrix) */
    using UCoeffType = typename S::UCoeffType;                                 /**< The type of the limb darkening coefficients (scalar/row vector) */
    using FluxType = typename S::FluxType;                                     /**< The type of the output flux (scalar/row vector) */

    // Public variables
    const int lmax;                                                            /**< Maximum spherical harmonic degree of the map */
    const int N;                                                               /**< Number of spherical harmonic `(l, m)` coefficients */
    const int ncoly;                                                           /**< Number of columns in the `y` matrix */
    const int ncolu;                                                           /**< Number of columns in the `u` matrix */
    const int nflx;                                                            /**< Number of columns in the output flux */
    Cache<S> cache;                                                            /**< Internal cache & storage class */

protected:

    // Internal methods
    #include "protected/intensity.h"
    #include "protected/flux.h"
    #include "protected/reflectance.h"
    #include "protected/maxlike.h"
    #include "protected/deriv.h"
    #include "protected/oper.h"
    #include "protected/python_interface.h"

    // Internal variables
    YType y;                                                                   /**< Vector/matrix of spherical harmonic coefficients */
    UType u;                                                                   /**< Vector/matrix of limb darkening coefficients */
    UnitVector<Scalar> axis;                                                   /**< The axis of rotation for the map */
    basis::Basis<Scalar> B;                                                    /**< Basis transform stuff */
    rotation::Wigner<YType> W;                                                 /**< Ylm rotation stuff */
    solver::Greens<Scalar> G;                                                  /**< The occultation integral solver class */
    limbdark::GreensLimbDark<Scalar> L;                                        /**< The occultation integral solver class (optimized for limb darkening) */
    Vector<Scalar> taylor;                                                     /**< Vector of Taylor expansion coefficients for a given `t` */
    int u_deg;                                                                 /**< Highest degree set by the user in the limb darkening vector */
    int y_deg;                                                                 /**< Highest degree set by the user in the spherical harmonic vector */
    Scalar radian;                                                             /**< Conversion factor from degrees to radians */
     
    //! Constructor for all map types
    explicit Map (
        int lmax,
        int ncoly,
        int ncolu,
        int nflx
    ) :
        lmax(lmax), 
        N((lmax + 1) * (lmax + 1)), 
        ncoly(ncoly), 
        ncolu(ncolu),
        nflx(nflx),
        cache(lmax, ncoly, ncolu, nflx),
        B(lmax),
        W(lmax, ncoly, nflx, (*this).y, (*this).axis),
        G(lmax),
        L(lmax),
        taylor(ncoly)
    {
        if ((lmax < 0) || (lmax > STARRY_MAX_LMAX))
            throw errors::ValueError(
                "Spherical harmonic degree out of range.");
        if ((ncoly < 1) || (ncolu < 1))
            throw errors::ValueError(
                "The number of map columns must be positive.");
        radian = pi<Scalar>() / 180.;
        taylor(0) = 1.0;
        reset();
    };

public:

    //! Constructor for single-column maps
    template <typename U=S, typename=IsDefault<U>>
    explicit Map (
        int lmax
    ) : Map(lmax, 1, 1, 1) {}

    //! Constructor for multi-column maps
    template <typename U=S, typename=IsSpectralOrTemporal<U>>
    explicit Map (
        int lmax,
        int ncol
    ) : Map(lmax, ncol, 
            std::is_same<U, Spectral<Scalar>>::value ? ncol : 1,
            std::is_same<U, Spectral<Scalar>>::value ? ncol : 1) {}

    // Public methods
    #include "public/io.h"
    #include "public/oper.h"
    #include "public/intensity.h"
    #include "public/flux.h"
    #include "public/reflectance.h"
    #include "public/maxlike.h"
    #include "public/python_interface.h"

}; // class Map




