/*===========================================================================
 *
 *  Sensor controller
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File kalmanf.c. Kalman filter.
 *
 *===========================================================================*/

#include "sensor.h"

/* Initialize Kalman filter (float) */
void InitializeKalmanF(KalmanF *kf, float q, float  r, float  f, float  h)
{
    kf->X0     = 0.0F;
    kf->P0     = 0.0F;
    kf->output = 0.0F;
    kf->covar  = 0.0F;

    kf->Q = q;
    kf->R = r;
    kf->F = f;
    kf->H = h;
}



/* Apply Kalman filter (float) */
float ApplyKalmanF(KalmanF *kf, float inval)
{
    float K;

    kf->X0 = kf->F * kf->output;
    kf->P0 = kf->F * kf->covar * kf->F + kf->Q;

    K = kf->H * kf->P0 / (kf->H * kf->P0 * kf->H + kf->R);

    kf->output = kf->X0 + K * (inval - kf->H * kf->X0);

    kf->covar = (1.0F - K * kf->H) * kf->P0;

    return kf->output;
}

/*===========================================================================
 * End of file kalmanf.c
 *===========================================================================*/
