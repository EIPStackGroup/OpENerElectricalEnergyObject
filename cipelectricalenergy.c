/*******************************************************************************
 * Copyright (c) 2019, Rockwell Automation, Inc.
 * All rights reserved.
 *
 ******************************************************************************/
#include "cipbaseenergy.h"
#include "cipelectricalenergy.h"
#include "opener_api.h"
#include "trace.h"
#include "endianconv.h"
#include <string.h>
#include <math.h>
#include <cipcommon.h>

CipReal line_frequency = 0.0; /**< #9 Specifies the Line Frequency in Hertz */
CipReal l1_current = 0.0; /**< #10 Specifies the RMS line current in L1 */
CipReal l2_current = 0.0; /**< #11 Specifies the RMS line current in L2 */
CipReal l3_current = 0.0; /**< #12 Specifies the RMS line current in L3 */
CipReal average_current = 0.0; /**< #13 Specifies the RMS line current of three-phase average */
CipReal percent_current_unbalance = 0.0; /**< #14 Specifies the percentage current deviation between phases */
CipReal l1_n_voltage = 0.0; /**< #15 Specifies the RMS line to neutral voltage of L1 */
CipReal l2_n_voltage = 0.0; /**< #16 Specifies the RMS line to neutral voltage of L2 */
CipReal l3_n_voltage = 0.0; /**< #17 Specifies the RMS line to neutral voltage of L3 */
CipReal average_l_n_voltage = 0.0; /**< #18 Specifies the RMS line to neutral voltage of three-phase average */
CipReal l1_l2_voltage = 0.0; /**< #19 Specifies the RMS L1 to L2 voltage */
CipReal l2_l3_voltage = 0.0; /**< #20 Specifies the RMS L2 to L3 voltage */
CipReal l3_l1_voltage = 0.0; /**< #21 Specifies the RMS L3 to L1 voltage */
CipReal average_l_l_voltage = 0.0; /**< #22 Specifies the RMS line to line voltage three-phase average */
CipReal percent_voltage_unbalance = 0.0; /**< #23 Specifies the percentage voltage deviation between phases */
CipReal l1_real_power = 0.0; /**< #24 Specifies the L1 real power, signed to show direction */
CipReal l2_real_power = 0.0; /**< #25 Specifies the L2 real power, signed to show direction */
CipReal l3_real_power = 0.0; /**< #26 Specifies the L3 real power, signed to show direction */
CipReal total_real_power = 0.0; /**< #27 Specifies the total real power, signed to show direction */
CipReal l1_reactive_power = 0.0; /**< #28 Specifies the L1 reactive power, signed to show direction */
CipReal l2_reactive_power = 0.0; /**< #29 Specifies the L2 reactive power, signed to show direction */
CipReal l3_reactive_power = 0.0; /**< #30 Specifies the L3 reactive power, signed to show direction */
CipReal total_reactive_power = 0.0; /**< #31 Specifies the total reactive power, signed to show direction */
CipReal l1_apparent_power = 0.0; /**< #32 Specifies the L1 apparent power */
CipReal l2_apparent_power = 0.0; /**< #33 Specifies the L2 apparent power */
CipReal l3_apparent_power = 0.0; /**< #34 Specifies the L3 apparent power */
CipReal total_apparent_power = 0.0; /**< #35 Specifies the total apparent power */
CipReal l1_true_power_factor = 0.0; /**< #36 Specifies the L1 Ratio between power and apparent power, + = Lead, - = Lag */
CipReal l2_true_power_factor = 0.0; /**< #37 Specifies the L2 Ratio between power and apparent power, + = Lead, - = Lag */
CipReal l3_true_power_factor = 0.0; /**< #38 Specifies the L3 Ratio between power and apparent power, + = Lead, - = Lag */
CipReal three_phase_true_power_factor = 0.0; /**< #39 Specifies the ratio between power and apparent power, + = Lead, - = Lag */
CipUint phase_rotation = 0; /**< #40 Specifies the phase rotation of a three-phase system */
CipEpath associated_base_energy_object_path = { 3, 0x4e, 1, 0 }; /**< #41 Specifies the path to associated Base Energy Object instance */
CipUsint dummy_odometer = 0;


/** @brief Special initializer function for the Electrical Energy object
 *
 */
void InitializeCipElectricalEnergy(CipClass *class) {

  CipClass *meta_class = class->class_instance.cip_class;

  InsertAttribute((CipInstance *) class, 1, kCipUint, EncodeCipUint, (void *) &class->revision,
                  kGetableSingleAndAll); /* revision */

  InsertService(meta_class, kGetAttributeAll,
                  &GetAttributeAll, "GetAttributeAll");
  InsertService(meta_class, kGetAttributeSingle,
                &GetAttributeSingle, "GetAttributeSingle");

}

EipStatus CipElectricalEnergyInit(void) {
  CipClass *electrical_energy_class = NULL;
  CipInstance *instance;

  electrical_energy_class = CreateCipClass(kCipElectricalEnergyClassCode,
		                                   1, /* # of non-default class attributes*/
                                           1, /* # highest class attribute number*/
                                           2, /* # class services*/
                                           41, /* # instance attributes*/
                                           41, /* # highest instance attribute number*/
                                           2, /* # instance services*/
                                           1, /* # instances*/
                                           "Electrical Energy", /* class name */
										   2, /* # class revision*/
                                           &InitializeCipElectricalEnergy); /* # function pointer for initialization*/

  if (NULL == electrical_energy_class) {
    return kEipStatusError;
  }

  instance = GetCipInstance(electrical_energy_class, 1);

  InsertAttribute(instance, 1, kCipAny, EncodeUINTOdometer, &consumed_energy_value,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 2, kCipAny, EncodeUINTOdometer, &produced_energy_value,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 3, kCipAny, EncodeINTOdometer, &total_energy_value,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 4, kCipAny, EncodeUINTOdometer, &dummy_odometer, kGetableSingleAndAll);
  InsertAttribute(instance, 5, kCipAny, EncodeUINTOdometer, &dummy_odometer, kGetableSingleAndAll);
  InsertAttribute(instance, 6, kCipAny, EncodeINTOdometer, &dummy_odometer, kGetableSingleAndAll);
  InsertAttribute(instance, 7, kCipAny, EncodeUINTOdometer, &dummy_odometer, kGetableSingleAndAll);
  InsertAttribute(instance, 8, kCipAny, EncodeUINTOdometer, &dummy_odometer, kGetableSingleAndAll);
  InsertAttribute(instance, 9, kCipReal, EncodeCipReal, &line_frequency, kGetableSingleAndAll);
  InsertAttribute(instance, 10, kCipReal, EncodeCipReal, &l1_current, kGetableSingleAndAll);
  InsertAttribute(instance, 11, kCipReal, EncodeCipReal, &l2_current, kGetableSingleAndAll);
  InsertAttribute(instance, 12, kCipReal, EncodeCipReal, &l3_current, kGetableSingleAndAll);
  InsertAttribute(instance, 13, kCipReal, EncodeCipReal, &average_current,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 14, kCipReal, EncodeCipReal, &percent_current_unbalance,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 15, kCipReal, EncodeCipReal, &l1_n_voltage, kGetableSingleAndAll);
  InsertAttribute(instance, 16, kCipReal, EncodeCipReal, &l2_n_voltage, kGetableSingleAndAll);
  InsertAttribute(instance, 17, kCipReal, EncodeCipReal, &l3_n_voltage, kGetableSingleAndAll);
  InsertAttribute(instance, 18, kCipReal, EncodeCipReal, &average_l_n_voltage,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 19, kCipReal, EncodeCipReal, &l1_l2_voltage, kGetableSingleAndAll);
  InsertAttribute(instance, 20, kCipReal, EncodeCipReal, &l2_l3_voltage, kGetableSingleAndAll);
  InsertAttribute(instance, 21, kCipReal, EncodeCipReal, &l3_l1_voltage, kGetableSingleAndAll);
  InsertAttribute(instance, 22, kCipReal, EncodeCipReal, &average_l_l_voltage,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 23, kCipReal, EncodeCipReal, &percent_voltage_unbalance,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 24, kCipReal, EncodeCipReal, &l1_real_power, kGetableSingleAndAll);
  InsertAttribute(instance, 25, kCipReal, EncodeCipReal, &l2_real_power, kGetableSingleAndAll);
  InsertAttribute(instance, 26, kCipReal, EncodeCipReal, &l3_real_power, kGetableSingleAndAll);
  InsertAttribute(instance, 27, kCipReal, EncodeCipReal, &total_real_power,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 28, kCipReal, EncodeCipReal, &l1_reactive_power,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 29, kCipReal, EncodeCipReal, &l2_reactive_power,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 30, kCipReal, EncodeCipReal, &l3_reactive_power,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 31, kCipReal, EncodeCipReal, &total_reactive_power,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 32, kCipReal, EncodeCipReal, &l1_apparent_power,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 33, kCipReal, EncodeCipReal, &l2_apparent_power,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 34, kCipReal, EncodeCipReal, &l3_apparent_power,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 35, kCipReal, EncodeCipReal, &total_apparent_power,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 36, kCipReal, EncodeCipReal, &l1_true_power_factor,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 37, kCipReal, EncodeCipReal, &l2_true_power_factor,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 38, kCipReal, EncodeCipReal, &l3_true_power_factor,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 39, kCipReal, EncodeCipReal, &three_phase_true_power_factor,
                  kGetableSingleAndAll);
  InsertAttribute(instance, 40, kCipUint, EncodeCipUint, &phase_rotation,
                  kGetableSingleAndAll);

  //Required Instance Attributes: Energy Type Specific Object Path (ID=12): Struct of:
  //  Path Size (UINT)
  //  Path (EPATH)
  InsertAttribute(instance, 41, kCipEpath, EncodeCipEPath, &associated_base_energy_object_path,
                  kGetableSingle);

  /* class services */
  /* none implemented because no class attributes are implemented so far */

  /* use Class-specific GetAttributeSingle method to allow special handling of odo-meter-values */
  InsertService(electrical_energy_class, kGetAttributeSingle,
                &GetAttributeSingle, "GetAttributeSingle");

  InsertService(electrical_energy_class, kGetAttributeAll,
                &GetAttributeAll, "GetAttributeAll");

  /* startup the measurement and metering functionality */
  /* enterStateInitializing(); */
  return kEipStatusOk;
}


/** @brief Updates Energy Object values based on measured data
 *
 * @param time_interval_in_seconds Measurement interval in seconds
 * @param voltage Measured voltage
 * @param current Measured current
 *
 */
void UpdateElectricalMeasuringAndMeteringData(double time_interval_in_seconds,
                                              double voltage, double current) {

  //TODO: check if in metering State

  static CipLreal temp_energy_value_in_ws;
  temp_energy_value_in_ws = temp_energy_value_in_ws + voltage * current * time_interval_in_seconds;

  l1_current = current;
  l1_n_voltage = voltage;
  l1_real_power = voltage * current;
  total_real_power = voltage * current;
  CipInt w_hours = 0;

  if (3600.0 < (temp_energy_value_in_ws)) {
    w_hours = floor(temp_energy_value_in_ws / 3600);
    temp_energy_value_in_ws = fmod(temp_energy_value_in_ws, 3600);
    UpdateOdometers(w_hours);
  } else if (-3600.0 > (temp_energy_value_in_ws)) {
    w_hours = ceil(temp_energy_value_in_ws / 3600);
    temp_energy_value_in_ws = fmod(temp_energy_value_in_ws, 3600);
    UpdateOdometers(w_hours);
  }

}
