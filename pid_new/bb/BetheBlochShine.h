//
// Created by eugene on 25/11/2019.
//

#ifndef PID_BB_SHINE_H_
#define PID_BB_SHINE_H_

/**
 * @brief a Bethe-Block implementation adjusted by Antoni Marcinek
 * used in calibration and it is preferred to be used in the analysis
 * @param betaGamma
 * @return dEdx (MIP)
 */
double
BetheBlochAntoniMod(double betaGamma);

/**
 * @brief ??
 * @param betaGamma
 * @return
 */
double
BetheBlochAntoni(double betaGamma);

/**
 * @brief a Bethe-Bloch implementation used in ShineOffline
 * @param betaGamma
 * @return
 */
double
BetheBlochShine(double betaGamma);

#endif//PID_BB_SHINE_H_
