/*******************************************************************************
 * Author: Hongkun Yu, Mingxu Hu, Kunpeng Wang
 * Dependecy:
 * Test:
 * Execution:
 * Description:
 * ****************************************************************************/

#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <numeric>

#include <gsl/gsl_math.h>
#include <gsl/gsl_statistics.h>
#include <glog/logging.h>
#include <armadillo>
#include <bingham.h>

#include "Typedef.h"
#include "Macro.h"

#include "Coordinate5D.h"
#include "Random.h"
#include "Euler.h"
#include "Functions.h"
#include "Symmetry.h"

using namespace arma;
using namespace google;
using namespace std;

static double e0[4] = {0, 1, 0, 0};
static double e1[4] = {0, 0, 1, 0};
static double e2[4] = {0, 0, 0, 1};

class Particle
{
    private:

        int _n;

        double _maxX;
        double _maxY;

        double** _r = NULL;
        /* rotation, quaternion */

        mat _t; // translation

        vec _w; // weight
        
        const Symmetry* _sym = NULL;

        double _k0 = 0;
        double _k1 = 0;
        double _k2 = 0;

        double _s0 = 0; // sigma0
        double _s1 = 0; // sgima1
        double _rho = 0;

    public:

        /** @brief default constructor of Particle
         */
        Particle();

        /** @brief constructor of Particle
         *  @param n number of particles in this particle filter
         *  @param maxX maximum X-axis translation in pixel
         *  @param maxY maximum Y-axis translation in pixel
         *  @param sym symmetry of resampling space
         */
        Particle(const int n,
                 const double maxX,
                 const double maxY,
                 const Symmetry* sym = NULL);

        /** @brief deconstructor of Particle
         */
        ~Particle();

        /** @brief This function initialises Particle
         *  @param n number of particles in this particle filter
         *  @param maxX maximum X-axis translation in pixel
         *  @param maxY maximum Y-axis translation in pixel
         *  @param sym symmetry of resampling space
         */
        void init(const int n,
                  const double maxX,
                  const double maxY,
                  const Symmetry* sym = NULL);

        void reset();

        int n() const;

        void vari(double& k0,
                  double& k1,
                  double& k2,
                  double& s0,
                  double& s1,
                  double& rho) const;

        double w(const int i) const;

        void setW(const double w,
                  const int i);

        void mulW(const double w,
                  const int i);

        void normW();

        void coord(Coordinate5D& dst,
                   const int i) const;
        /* return the coordinate of the ith particle */

        void rot(mat33& dst,
                 const int i) const;
        /* return the rotation matrix of the ith particle */

        void t(vec2& dst,
               const int i) const;
        /* return the translate coordinate of the ith particle */

        void quaternion(vec4& dst,
                        const int i) const;

        void setSymmetry(const Symmetry* sym);

        void calVari();

        void perturb();

        void resample();

        /* resample to number of particles of n */
        void resample(const int n);

        double neff() const;

        uvec iSort() const;
        /* return the index of sorting in descending order */
    
    private:

        void symmetrise();

        void clear();
};

void display(const Particle& particle);

void save(const char filename[],
          const Particle& particle);

#endif  //PARTICLE_H
