/*******************************************************************************
 * Author: Mingxu Hu
 * Dependency:
 * Test:
 * Execution:
 * Description:
 *
 * Manual:
 * ****************************************************************************/

#ifndef IMAGE_BASE
#define IMAGE_BASE

#include <functional>
#include <cstring>
#include <cstdio>

#include <gsl/gsl_math.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>

#include "Complex.h"
#include "Typedef.h"
#include "Functions.h"
#include "Utils.h"
#include "Logging.h"

using namespace std;

#define RL_SPACE 0

#define FT_SPACE 1

/**
 * This macro loops over each pixel of an Image / Volume in real space.
 * @param base an Image / Volume
 */
#define FOR_EACH_PIXEL_RL(base) \
    for (size_t i = 0; i < base.sizeRL(); i++)

/**
 * This macro loops over each pixel of an Image / Volume in Fourier space.
 * @param base an Image / Volume
 */
#define FOR_EACH_PIXEL_FT(base) \
    for (size_t i = 0; i < base.sizeFT(); i++)

/**
 * This macro sets each pixel of an Image / Volume to 0 in real space.
 * @param base an Image / Volume
 */
#define SET_0_RL(base) \
    memset(&base(0), 0, sizeof(double) * base.sizeRL());

/**
 * This macro sets each pixel of an Image / Volume to 0 in Fourier space.
 * @param base an Image / Volume
 */
#define SET_0_FT(base) \
    memset(&base[0], 0, sizeof(Complex) * base.sizeFT());

/**
 * This macro sets each pixel of an Image / Volume to 1 in real space.
 * @param base an Image / Volume
 */
#define SET_1_RL(base) \
    FOR_EACH_PIXEL_RL(base) \
        base(i) = 1

/**
 * This macro sets each pixel of an Image / Volume to 1 in Fourier space.
 * @param base an Image / Volume
 */
#define SET_1_FT(base) \
    FOR_EACH_PIXEL_FT(base) \
        base[i] = COMPLEX(1, 0)

/**
 * This macro negatives each pixel of an Image / Volume in real space.
 * @param base an Image / Volume
 */
#define NEG_RL(base) \
    SCALE_RL(base, -1)

/**
 * This macro negatives each pixel of an Image / Volume in Fourier space.
 * @param base an Image / Volume
 */
#define NEG_FT(base) \
    SCALE_FT(base, -1)

/**
 * This macro scales each pixel of an Image / Volume with a certain factor in
 * real space.
 * @param base an Image / Volume
 * @param a the scale factor
 */
#define SCALE_RL(base, a) \
    FOR_EACH_PIXEL_RL(base) \
        (base)(i) *= a

/**
 * This macro scales each pixel of an Image / Volume with a certain factor in
 * Fourier space.
 * @param base an Image / Volume
 * @param a the scale factor
 */
#define SCALE_FT(base, a) \
    FOR_EACH_PIXEL_FT(base) \
        (base)[i] *= a

/**
 * This macro adds each pixel from B to A respectively in real space.
 * @param a Image / Volume A
 * @param b Image / Volume B
 */
#define ADD_RL(a, b) \
    FOR_EACH_PIXEL_RL(a) \
        (a)(i) += (b)(i)

/**
 * This macro adds each pixel from B to A respectively in Fourier space.
 * @param a Image / Volume A
 * @param b Image / Volume B
 */
#define ADD_FT(a, b) \
    FOR_EACH_PIXEL_FT(a) \
        (a)[i] += (b)[i]

/**
 * This macro substracts each pixel from B to A respectively in real space.
 * @param a Image / Volume A
 * @param b Image / Volume B
 */
#define SUB_RL(a, b) \
    FOR_EACH_PIXEL_RL(a) \
        (a)(i) -= (b)(i)

/**
 * This macro substracts each pixel from B to A respectively in Fourier space.
 * @param a Image / Volume A
 * @param b Image / Volume B
 */
#define SUB_FT(a, b) \
    FOR_EACH_PIXEL_FT(a) \
        (a)[i] -= (b)[i]

/**
 * This macro multiplys each pixel of A with the pixel of B respectively in Fourier space.
 * @param a Image / Volume A
 * @param b Image / Volume B
 */
#define MUL_FT(a, b) \
    FOR_EACH_PIXEL_FT(a) \
        (a)[i] *= (b)[i]

/**
 * This macro divides each pixel of A with the pixel of B respectively in Fourier space.
 * @param a Image / Volume A
 * @param b Image / Volume B
 */
#define DIV_FT(a, b) \
    FOR_EACH_PIXEL_FT(a) \
        (a)[i] /= (b)[i]

class ImageBase
{
    MAKE_DEFAULT_MOVE(ImageBase)

    protected:

        unique_ptr<double[]> _dataRL;
        unique_ptr<Complex[]> _dataFT;

        size_t _sizeRL = 0;

        size_t _sizeFT = 0;

        ImageBase();

        ~ImageBase();

    public:

        /**
         * return a const pointer which points to the i-th element in real space
         * @param i index of the element
         */
        const double& iGetRL(const size_t i = 0) const;

        /**
         * return a const pointer which points to the i-th element in Fourier
         * space
         * @param i index of the element
         */
        const Complex& iGetFT(const size_t i = 0) const;

        /**
         * return the i-th element in real space
         * @param i index of the element
         */
        double& operator()(const size_t i);

        /**
         * return the i-th element in Fourier space
         * @param i index of the element
         */
        Complex& operator[](const size_t i);

        /**
         * check whether _dataRL is NULL or not
         */
        bool isEmptyRL() const;

        /**
         * check whether _dataFT is nULL or not
         */
        bool isEmptyFT() const;

        /**
         * return the number of pixels in real space
         */
        size_t sizeRL() const;

        /**
         * return the number of pixels in Fourier space
         */
        size_t sizeFT() const;
        
        /**
         * free the allocated space both in real space and Fouier space
         */
        void clear();

        /**
         * free the allocated space in real space
         */
        void clearRL();

        /**
         * free the allocated space in Fourier space
         */
        void clearFT();

        void copyBase(ImageBase&) const;

        ImageBase copyBase() const;
};

double norm(ImageBase& base);

void normalise(ImageBase& base);

#endif // IMAGE_BASE
