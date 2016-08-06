/*******************************************************************************
 * Author: Mingxu Hu
 * Dependency:
 * Test:
 * Execution:
 * Description:
 *
 * Manual:
 * ****************************************************************************/

#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "Complex.h"
#include "Error.h"

#include "Euler.h"

#include "Image.h"
#include "Volume.h"

#include "Coordinate5D.h"

#include "ImageFunctions.h"

class Projector
{
    MAKE_DEFAULT_MOVE(Projector)

    private:

        /**
         * Only the signal beyond the max radius in frequnecy will be processed.
         * Max radius must be smaller than half of the shortest dimension of the
         * projectee. When projectee is set, max radius will be properly set as
         * well. However, it can be overwrited.
         */
        int _maxRadius = -1;

        /**
         * the interpolation type (SINC_INTERP, LINEAR_INTERP, NEAREST_INTERP)
         */
        int _interp = LINEAR_INTERP;

        /**
         * padding factor
         */
        int _pf = 2;

        /**
         * the volume to be projected
         */
        Volume _projectee;

    public:

        /**
         * default constructor
         */
        Projector();

        /**
         * default deconstructor
         */
        ~Projector();

        /**
         * If there is a volume to be projected, return false, otherwise return
         * true.
         */
        bool isEmpty() const;

        /**
         * This function returns the max radius for processing signal in Fourier
         * transform (in pixel).
         */
        int maxRadius() const;

        /**
         * This function sets the max radius for processing signal in Fourier
         * transform (in pixel).
         *
         * @param maxRadius the max radius
         */
        void setMaxRadius(const int maxRadius);

        /**
         * This function returns the interpolation type for this projection.
         */
        int interp() const;

        void setInterp(const int interp);

        int pf() const;

        void setPf(const int pf);

        const Volume& projectee() const;

        void setProjectee(Volume src);

        void project(Image& dst,
                     const mat33& mat) const;

        void project(Image& dst,
                     const double phi,
                     const double theta,
                     const double psi) const;

        void project(Image& dst,
                     const double phi,
                     const double theta,
                     const double psi,
                     const double x,
                     const double y) const;

        void project(Image& dst,
                     const Coordinate5D& coordinate5D) const;

        void project(Image& dst,
                     const mat33& rot,
                     const vec2& t) const;

    private:

        void gridCorrection();
        /* perform gridding correction on _projectee */
};

#endif // PROJECTOR_H
