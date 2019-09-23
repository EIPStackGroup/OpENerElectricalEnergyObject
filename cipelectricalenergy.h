/*******************************************************************************
 * Copyright (c) 2019, Rockwell Automation, Inc.
 * All rights reserved.
 *
 ******************************************************************************/

#ifndef OPENER_CIP_ELECTRICAL_ENERGY_H_
#define OPENER_CIP_ELECTRICAL_ENERGY_H_

#include "opener_api.h"
#include "typedefs.h"

static const EipUint16 kCipElectricalEnergyClassCode = 0x4f; /**< Electrical Energy Object class code */

/** @brief Energy Object specific getAttributeSingle implementation for special handling of attributes (odo-meters)
 *
 * @param instance The requested instance
 * @param message_router_request The request message
 * @param message_router_response The generated response message
 * @param originator_address The originators sockaddr information
 * @param encapsulation_session The associated encapsulation session
 *
 * @return kEipStatusOkSend on success
 */
EipStatus GetAttributeSingleElectricalEnergy(CipInstance *RESTRICT const instance,
        CipMessageRouterRequest *const message_router_request,
        CipMessageRouterResponse *const message_router_response,
        const struct sockaddr *originator_address,
        const int encapsulation_session);


/** @brief Initializes the CipElectricalEnergy object
 *
 * @return kEipStatusOk on success, else kEipStatusError
 */
EipStatus
CipElectricalEnergyInit(void);

/* \defgroup CIP_ENERGY API of OpENer's CIP Energy implementation
 *
 */

/* \ingroup CIP_ENERGY_API
 * \brief function that will handle the application processing of the base energy object
 *
 * Will be invoked right before the void IApp_HandleApplication(void) callback.
 */
void HandleBaseEnergyObject();

/**  \defgroup CIP_ENERGY_API Callback Functions Demanded by CIP Energy
 * \ingroup CIP_ENERGY_API
 *
 * \brief These functions have to implemented in order to give the CIP Energy
 * implementation of OpENer a method to inform the application on certain state changes.
 */

/** \ingroup CIP_ENERGY_CALLBACK_API
 * \brief Callback informing that the cip energy object is entering the state
 * Metering
 */
void
BaseEnergyOnEnterStateMetering();

/** \ingroup CIP_ENERGY_CALLBACK_API
 * \brief Callback informing that the cip energy object is entering the state
 * NotMetering
 */
void
BaseEnergyOnEnterStateNotMetering();

/** \ingroup CIP_ENERGY_CALLBACK_API
 * \brief Callback informing that the cip energy object is entering the state
 * Starting
 */
void
BaseEnergyOnEnterStateStarting();

/** \ingroup CIP_ENERGY_CALLBACK_API
 * \brief Callback informing that the cip energy object is entering the state
 * Stopping
 */
void
BaseEnergyOnEnterStateStopping();

/** \ingroup CIP_ENERGY_CALLBACK_API
 * \brief Application specific processing during the energy object state
 * Metering.
 *
 * State processing is triggered cyclically synchronously with the manage
 * connections cycle.
 *
 * Transitional state: Application informs with true return value that the process
 * in this state has finished.
 */
CipBool
BaseEnergyProcessStateMetering();

/** \ingroup CIP_ENERGY_CALLBACK_API
 * \brief Application specific processing during the energy object state
 * NotMetering.
 *
 * State processing is triggered cyclically synchronously with the manage
 * connections cycle.
 *
 * Transitional state: Application informs with true return value that the process
 * in this state has finished.
 */
CipBool
BaseEnergyProcessStateNotMetering();

/** \ingroup CIP_ENERGY_CALLBACK_API
 * \brief Application specific processing during the energy object state
 * Starting.
 *
 * State processing is triggered cyclically synchronously with the manage
 * connections cycle.
 *
 * Transitional state: Application informs with true return value that the process
 * in this state has finished.
 */
CipBool
BaseEnergyProcessStateStarting();

/** \ingroup CIP_ENERGY_CALLBACK_API
 * \brief Application specific processing during the energy object state
 * Stopping.
 *
 * State processing is triggered cyclically synchronously with the manage
 * connections cycle.
 *
 * Transitional state: Application informs with true return value that the process
 * in this state has finished.
 */
CipBool
BaseEnergyProcessStateStopping();

void UpdateElectricalMeasuringAndMeteringData(double time_interval_in_seconds,
                                              double voltage, double current);

#endif /* OPENER_CIP_ELECTRICAL_ENERGY_H_ */
